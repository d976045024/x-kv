#pragma once

#include <string>

namespace xkv {

class Status {
 public:
  enum Code {
    kOk = 0,
    kNotFound = 1,
    kCorruption = 2,
    kNotSupported = 3,
    kInvalidArgument = 4,
    kIOError = 5,
    kMergeInProgress = 6,
    kIncomplete = 7,
    kShutdownInProgress = 8,
    kTimedOut = 9,
    kAborted = 10,
    kBusy = 11,
    kExpired = 12,
    kTryAgain = 13,
    kMutexTimeout = 14,
    kLockTimeout = 15,
    kLockLimit = 16,
    kNoSpace = 17,
    kDeadlock = 18,
    kStaleFile = 19,
    kMemoryLimit = 20,
    kErrorUnexpected = 21,
    kEntryNotExist = 22,
    kInitTwice = 23,
    kNotInit = 24,
    kIterEnd = 25,
    kCancelTask = 26,
    kInsertCheckFailed = 27,
    kOverLimit = 28,
    kRecoverCommitLater = 29,
    kObjStoreError = 30,
    kNotCompress = 31,
  };

  enum SubCode { kNone = 0, kMaxSubCode };

  
};

}