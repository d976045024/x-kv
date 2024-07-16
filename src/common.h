#include <iostream>

typedef struct
{
    int64_t id;
    char user_id[128];
    char name[128];
    int64_t salary;
} User;

int64_t per_block_size = 4 *1024;  // 每个block（桶）的大小
double backup_rate = 0.25;  // 备用block（桶）占总block的比例

int64_t block_mata_offset = 16; //每个block前16字节存储元数据，标记该block中record_num,下一个block的id（该block满时使用）
int64_t record_meta_offset = 1; //每条record在聚簇索引存储时，留1byte作为元数据，标记是否将多个索引都写完















// 读锁开关。读写分离模式可关闭（0），读写混合模式需打开（1）
#define OPEN_READ_GUARD 1
// 故障注入开关。0为无故障，1为超时，2为段错误
#define OPEN_FAULT_INJECTION 0 
// debug模式开关
#define OPEN_DEBUG 1
// 分布式开关
#define OPEN_DISTRIBUTED 1