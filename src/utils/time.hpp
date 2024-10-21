#pragma once

#include <chrono>
#include <iomanip>  // std::put_time
#include <ctime>    // std::localtime
#include <sstream> // for ostringstream

using std::chrono::system_clock;
using std::chrono::high_resolution_clock;

namespace cpprinter{

std::string getHumanReadableTime(const high_resolution_clock::time_point& timePoint) {

    // 将 high_resolution_clock::time_point 转换为 system_clock::time_point
    auto systemTime = system_clock::now() + (timePoint - high_resolution_clock::now());

    // 将 system_clock::time_point 转换为 time_t
    auto systemTimeInMicroseconds = std::chrono::time_point_cast<std::chrono::microseconds>(systemTime);
    std::time_t timeT = system_clock::to_time_t(systemTimeInMicroseconds);

    // 将 time_t 转换为本地时间
    std::tm* localTime = std::localtime(&timeT);

    // 使用 std::ostringstream 拼接字符串
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    
    return oss.str();  // 返回拼接后的字符串
}

long long getDurationInMicroseconds(const high_resolution_clock::time_point& startTime) {
    auto endTime = high_resolution_clock::now();  // 获取当前时间
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();  // 计算持续时间
}

// 定义函数，传入时间点B，返回微秒差值
long long calculateMicrosecondsSinceStart(std::chrono::high_resolution_clock::time_point B) {
    // 获取程序启动时间（以high_resolution_clock为基准）
    // auto programStart = std::chrono::high_resolution_clock::now().time_since_epoch();

    // 计算程序启动到时间B的持续时间
    auto duration = B.time_since_epoch();

    // 将持续时间转换为微秒并返回
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}


}