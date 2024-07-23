#pragma once

#include <stdint.h>
#include <vector>
#include <string>

namespace xkv {

struct DBOptions {
  DBOptions();

  explicit DBOptions(const DBOptions &options);

  uint64_t max_total_wal_size = 128 * 1024* 1024;

  // 存放SST files的路径
  std::vector<std::string> db_paths;

  // 存放wal log的路径，如果没有指定，将和db_paths相同
  std::string wal_dir = "";

  int table_cache_numshardbits = 7;
};

}