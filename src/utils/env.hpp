#ifndef ENV_H
#define ENV_H

#include <string>
#include <cstdlib>  // 用于 std::getenv

namespace cpprinter{

// 检查环境变量 CPPRINTER_RECORD_FUNCSTACK 是否开启函数调用栈记录
bool isFuncStackLoggingEnabled() {
    // 获取环境变量 CPPRINTER_RECORD_FUNCSTACK 的值
    const char* env_p = std::getenv("CPPRINTER_RECORD_FUNCSTACK");

    // 如果环境变量存在且其值为 "1" 或 "true"，返回 true，否则返回 false
    return (env_p && (std::string(env_p) == "1" || std::string(env_p) == "true"));
}

// 检查环境变量 CPPRINTER_OFF 
bool isCPPrinterOff() {
    // 获取环境变量 CPPRINTER_OFF 的值
    const char* env_p = std::getenv("CPPRINTER_OFF");

    // 如果环境变量存在且其值为 "1" 或 "true"，返回 true，否则返回 false
    return (env_p && (std::string(env_p) == "1" || std::string(env_p) == "true"));
}

}


#endif // ENV_H
