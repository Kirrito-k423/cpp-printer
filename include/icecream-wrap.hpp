// icecream-wrap.hpp
#ifndef ICECREAM_WRAP_HPP
#define ICECREAM_WRAP_HPP

#include "icecream.hpp"

#include <unistd.h>
#include <thread>
#include <sstream>
#include <sys/syscall.h>

// 定义一些有颜色的输出宏
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define GPT   "\033[94m"      /* gpt-blue */

#define passBY(format, ...) { \
    pid_t tid = syscall(SYS_gettid); /* 获取系统级 TID */ \
    std::string thread_id_str = std::to_string(tid); \
    std::string prefix = std::string(GREEN) + "\n----------------------------- " + format + " ----------------------------- " \
                        + std::to_string(getpid()) + "/" + thread_id_str + "\n\n" +  std::string(RESET); \
    IC_CONFIG.prefix(prefix.c_str()); \
    IC(__VA_ARGS__); \
    prefix = std::string(YELLOW) + "info " + std::to_string(getpid()) + "/" + thread_id_str + " | " + std::string(RESET); \
    IC_CONFIG.prefix(prefix.c_str()); \
}

#define errorBY(format, ...) { \
    pid_t tid = syscall(SYS_gettid); /* 获取系统级 TID */ \
    std::string thread_id_str = std::to_string(tid); \
    std::string prefix = std::string(RED) + "-------------- " + format + " -------------- " \
                        + std::to_string(getpid()) + "/" + thread_id_str + "\n\n" +  std::string(RESET); \
    IC_CONFIG.prefix(prefix.c_str()); \
    IC(__VA_ARGS__); \
    prefix = std::string(YELLOW) + "info " + std::to_string(getpid()) + "/" + thread_id_str + " | " + std::string(RESET); \
    IC_CONFIG.prefix(prefix.c_str()); \
}

#define colorBY(color, format, ...) { \
    pid_t tid = syscall(SYS_gettid); /* 获取系统级 TID */ \
    std::string thread_id_str = std::to_string(tid); \
    std::string prefix = std::string(color) + format + " " + std::to_string(getpid()) + "/" + thread_id_str  + " | " + std::string(RESET); \
    IC_CONFIG.prefix(prefix.c_str()); \
    IC(__VA_ARGS__); \
    prefix = std::string(YELLOW) + "info " + std::to_string(getpid()) + "/" + thread_id_str + " | " + std::string(RESET); \
    IC_CONFIG.prefix(prefix.c_str()); \
}

#define tIC(...) { \
    pid_t tid = syscall(SYS_gettid); /* 获取系统级 TID */ \
    std::string thread_id_str = std::to_string(tid); \
    std::string prefix = std::string(YELLOW) + "info " + std::to_string(getpid()) + "/" + thread_id_str + " | " + std::string(RESET); \
    IC_CONFIG.prefix(prefix.c_str()); \
    IC(__VA_ARGS__); \
}




#endif // ICECREAM_WRAP_HPP