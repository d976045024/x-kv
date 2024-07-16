#include <iostream>
#include <string>
#include <atomic>
#include "RWLock.h"
#include "common.h"

class store_manager {
// private:
public:
    std::string file_name;  // 索引文件名
    int64_t file_size;  // 索引文件大小

    int64_t total_block_num;  // 将这块空间分为多少个block（桶）
    int64_t hash_bucket_num;  // 多少个用作哈希桶，其余的作为备用桶
    RWLock* block_locks;   // 各个桶上的读写锁

    int64_t backup_next_block_idx;  // 下一个可分配的备用桶idx
    std::atomic_flag backup_block_idx_spin_lock;   // 备用桶idx变量的自旋锁

    size_t record_size;

public:
    store_manager() {}
    void init( std::string file_name_, int64_t file_size_, size_t record_size_) 
    {
        file_name = file_name_;
        file_size = file_size_; 
        record_size = record_size_;
        total_block_num = file_size / per_block_size;
        hash_bucket_num = total_block_num * (1 - backup_rate);
        block_locks = new RWLock[total_block_num];
        backup_next_block_idx = hash_bucket_num; 
    }
    store_manager( std::string file_name_, int64_t file_size_, size_t record_size_) 
    {
       init(file_name_,file_size_,record_size_);
    }
    ~store_manager() {
        delete[] block_locks;
    }

};