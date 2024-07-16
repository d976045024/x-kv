//
// Created by fanyang on 2022/7/27.
//

#ifndef LIVE_EXAMPLE_INTERFACE_H
#define LIVE_EXAMPLE_INTERFACE_H

//
// Created by fanyang on 2022/7/18.
//

#ifndef TIANCHI_POLARDBX_EVALUATION_FY_FINAL_INTERFACE_H
#define TIANCHI_POLARDBX_EVALUATION_FY_FINAL_INTERFACE_H

#include "../inc/interface.h"
#include <iostream>
#include <stdlib.h>
#include <libpmem.h>
#include <string.h>
#include <string>
#include <memory>
#include <mutex>
#include <iostream>
#include <atomic>
#include <vector>
#include <functional>
#include <cstddef>
#include <unistd.h>
#include <signal.h>
#include <chrono>
#include <fstream>

#include "RWLock.h"
#include "store_manager.h"


std::string init_metadata_file_name;
size_t init_metadata_file_size = 1 + 8 * 3; // 第一字节标志是第一次初始化还是恢复，后面跟着三个int64_t，表示backup_next_block_idxs

store_manager managers[3]; //管理三个索引的存储
std::vector<char *> pmemaddrs;


// 对外提供的四个标准接口
// void engine_write( void *ctx, const void *data, size_t len){}
// size_t engine_read( void *ctx, int32_t select_column, int32_t where_column, const void *column_key, size_t column_key_len, void *res){return 0;}
// void* engine_init(char* host_info, char** peer_host_info, size_t peer_host_info_num){return nullptr;}
// void engine_deinit(void *ctx){}

// init相关函数
bool judge_first_init();
void set_init_tag();
void take_first_init();
void recover();

// 根据传入值进行哈希分区
int64_t get_pk_hash_bucket_id(int64_t id);
int64_t get_uk_hash_bucket_id(const char* user_id);
int64_t get_ck0_hash_bucket_id(int64_t salary);

// write相关函数
char* write_pk(char * pk_pmemaddr, char * init_metadata_pmemaddr, const User *user);
void write_uk(char* uk_pmemaddr, char * init_metadata_pmemaddr, const User *user);
void write_sk(char* sk_pmemaddr, char * init_metadata_pmemaddr, const User *user);
void write_complete_tag(char *tag);

// read相关函数
void* read_pk(char* pk_pmemaddr, int64_t id);
void* read_uk(char* uk_pmemaddr, char* user_id);
std::vector<int64_t> read_sk(char* sk_pmemaddr, int64_t salary);
bool check_tag(char* pk_pmemaddr, int64_t id);

size_t user2column(User* user,int32_t select_column, void *res);
size_t users2columns(std::vector<User*> users, int32_t select_column, void *res);


#endif //TIANCHI_POLARDBX_EVALUATION_FY_FINAL_INTERFACE_H


















#endif //LIVE_EXAMPLE_INTERFACE_H
