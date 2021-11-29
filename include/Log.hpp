#pragma once

#include <assert.h>
#include <stdio.h>

#include <string>

#ifdef _DEBUG

#define LOG(message)                                                       \
  {                                                                        \
    std::string file_name = std::string(__FILE__);                         \
    file_name = file_name.substr(file_name.find_last_of('\\') + 1);        \
    printf("[LOG:%s:%d:%s] - %s\n", file_name.c_str(), __LINE__, __func__, \
           message);                                                       \
  }

#define LOG_ASSERT(expr, message)                                          \
  {                                                                        \
    if (expr == false) {                                                   \
      std::string file_name = std::string(__FILE__);                       \
      file_name = file_name.substr(file_name.find_last_of('\\') + 1);      \
      printf("[ERROR:%s:%d:%s] - (%s) is false - %s\n", file_name.c_str(), \
             __LINE__, __func__, #expr, message);                          \
      assert(false);                                                       \
    }                                                                      \
  }
#else
#define LOG(X)
#define LOG_ERROR(value, message)
#endif