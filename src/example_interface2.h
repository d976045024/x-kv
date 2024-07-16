//
// Created by fanyang on 2022/7/27.
//

#ifndef LIVE_EXAMPLE_INTERFACE2_H
#define LIVE_EXAMPLE_INTERFACE2_H


#include <libpmemlog.h>
#include <unordered_map>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include "common.h"

// https://pmem.io/pmdk/manpages/linux/v1.3/libpmemlog.3/

#define POOL_SIZE 15*1024*1024*1024  // 15G
PMEMlogpool *plp = nullptr;

std::unordered_map<int64_t, User*> pk;
std::unordered_map<char *, int64_t> uk;
std::multimap<int64_t, int64_t> sk;

int make_indexs(const void *buf, size_t len, void *arg)
{
    // 将该tuple写入到内存索引中
    return 0;
}


/*
 * Writes incoming data to the engine
 */
void engine_write( void *ctx, const void *data, size_t len){
    // 调用pmemlog_append写入数据
    if (pmemlog_append(plp, data, sizeof(User)) < 0) {
        perror("pmemlog_append");
        exit(1);
    }
}

/*
 * Simulate queries to a relational database：
 * SELECT select_column FROM table_name WHERE where_column = column_key ORDER BY select_column ASC .
 * Since there is only one kind of table, the table_name is not provided in the parameters.
 * Column correspondence : Id=0, Userid=1, Name=2, Salary=3.
 * You need to read the data from engine into the res, and return the number of data tuples.
 */
size_t engine_read( void *ctx, int32_t select_column,
                    int32_t where_column, const void *column_key, size_t column_key_len, void *res){
    // 根据内存索引返回相应数据
}

/*
 * Initialization interface, which is called after the engine starts.
 * You need to create or recover db from pmem-file.
 * host_info: Local machine information including ip and port.
 * peer_host_info: Information about other machines in the distributed cluster.
 * peer_host_info_num: The num of other machines in the distributed cluster.
 * aep_dir: AEP file directory.
 * disk_dir: Disk file directory.
 */
void* engine_init(const char* host_info, const char* const* peer_host_info, size_t peer_host_info_num,
                  const char* aep_dir, const char* disk_dir)
{
    std::string path = std::string(aep_dir) + "pmemlog.file";

    /* create the pmemlog pool or open it if it already exists */
    plp = pmemlog_create(path.c_str(), POOL_SIZE, 0666);
    bool is_first_init = true;

    if (plp == NULL)
    {
        plp = pmemlog_open(path);
        is_first_init = false;
    }

    if (plp == NULL)
    {
        // 报错
        exit(1);
    }

    if(!is_first_init)
    {
        /*
         * 做缓冲区预热
         * 将AEP上的数据读取出来，加载到几个索引结构中（hashmap）
         */
        pmemlog_walk(plp, sizeof(User), make_indexs, NULL);
    }
    else
    {
        // 做第一次初始化的一些处理
    }

}

/*
 * Used to release resources when the engine exits normally.
 */
void engine_deinit(void *ctx) {
    pmemlog_close(plp);
}

#endif //LIVE_EXAMPLE_INTERFACE2_H
