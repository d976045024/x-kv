//
// Created by fanyang on 2022/7/27.
//

#include "example_interface.h"

char* write_pk(char * pk_pmemaddr, char * init_metadata_pmemaddr, const User *user) {
    // 获取hash值
    int64_t hash_bucket_id = get_pk_hash_bucket_id(user->id);

    while(1)
    {
        // 上写锁
        WriteGuard autoSync(managers[0].block_locks[hash_bucket_id]);
        // 获取该桶已写入的tuple数量
        int64_t used_tuple_num = *((int64_t*)((char *)pk_pmemaddr + hash_bucket_id * per_block_size));
//        memcpy(&used_tuple_num, (char *)pk_pmemaddr + hash_bucket_id*per_block_size,sizeof(int64_t));

        // 判断该hash桶是否已满
        if( used_tuple_num >= (per_block_size - block_mata_offset) / (managers[0].record_size + 1) )
        {
            // 若已满，切换下一个hash桶

            // 根据该桶metadata，获取下一个桶的id
            int64_t next_hash_bucket_id = *((int64_t*)((char *)pk_pmemaddr+ hash_bucket_id*per_block_size + 8));
            // 如果下一个桶还未分配
            if(next_hash_bucket_id == 0 || next_hash_bucket_id == hash_bucket_id)
            {
                // 将下一个备用桶分配给该hash值
                while (managers[0].backup_block_idx_spin_lock.test_and_set(std::memory_order_acquire))  // acquire lock
                    ; // spin
                    // 将下一个备用桶idx自增1，并持久化
                next_hash_bucket_id = managers[0].backup_next_block_idx++;
                pmem_memcpy_persist(init_metadata_pmemaddr+1+0*8, &managers[0].backup_next_block_idx, 8);
                managers[0].backup_block_idx_spin_lock.clear(std::memory_order_release);               // release lock
                // 将分配的新桶信息持久化到旧桶的matadata中
                pmem_memcpy_persist((char *)pk_pmemaddr + hash_bucket_id*per_block_size + 8, &next_hash_bucket_id, sizeof(int64_t));
            }
            // 将桶idx更改，进入下一层循环，查看下一个桶
            hash_bucket_id = next_hash_bucket_id;
        }
        else
        {
            // 标志位处理，将其置为0，全部写入完成后，再置为1
            // 也可以在初始化文件的时候就做好该操作
            char * tag = (char *)pk_pmemaddr + hash_bucket_id*per_block_size + block_mata_offset + used_block_num*(managers[0].record_size+1);
            char c = 0;
            pmem_memcpy_persist(tag ,&c, 1);

            // 将该桶已用的tuple数量自增1，并持久化
            ++used_tuple_num;
            pmem_memcpy_persist((char *)pk_pmemaddr + hash_bucket_id*per_block_size ,&used_block_num,sizeof(int64_t));

            // 将数据持久化
            // 文件地址 + 哈希桶偏移 + 该block的元数据偏移 + 前面的记录偏移 + 该条记录元数据偏移
            pmem_memcpy_persist((char *)pk_pmemaddr + hash_bucket_id*per_block_size + block_mata_offset + used_block_num*(managers[0].record_size+1) + 1, user, managers[0].record_size);

            return tag;
        }
    }
    return nullptr;
}