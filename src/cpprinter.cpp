#include "cpprinter.hpp"

#include "trace/address_resolution.hpp"
#include "trace/calltrace.hpp"
#include "utils/fstream_wrapper.hpp"
#include "utils/thread.hpp"
#include "utils/time.hpp"
#include "utils/env.hpp"

#include <string>
#include <thread>
#include <sstream> // for ostringstream
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <fstream>
#include <filesystem>

namespace cpprinter{

const std::string savePrefix="/tmp/cpp_";

// for stacked
thread_local std::stack<std::string> FunctionProfiler::functionName_;
thread_local std::stack<std::chrono::high_resolution_clock::time_point> FunctionProfiler::startTime_;
thread_local std::shared_ptr<CallTrace> FunctionProfiler::calltrace_ = std::make_shared<CallTrace>();

void FunctionProfiler::childProcessInit(){
    // 检查 /tmp/{ppid()} 路径是否存在 并且 /tmp/{pid()} 不存在
    std::string ppath = savePrefix + std::to_string(getppid());
    std::string path = savePrefix + std::to_string(getpid());
    if (std::filesystem::exists(ppath) && !std::filesystem::exists(path)) {
        printed_ = false;
    }
}

void FunctionProfiler::initialize(const std::string& fullName) {
    functionName_.push(fullName);
    startTime_.push(std::chrono::high_resolution_clock::now());
    callCount_++;
    childProcessInit();
    logCallStack(fullName.c_str());

}

FunctionProfiler::FunctionProfiler(const char* funcName, const char* description = ""){
    initialize(std::string(funcName)+std::string(description));
}

FunctionProfiler::FunctionProfiler(const char* funcName){
    initialize(std::string(funcName));
}

FunctionProfiler::~FunctionProfiler() {
    logStats();
    printInfoOnce();
}

void FunctionProfiler::printInfoOnce() {
    if (!printed_) {
        std::cout << std::endl  // 前置换行
                  << "FunctionProfiler to " << savePrefix << std::to_string(getpid())
                  << "/" << std::to_string(get_thread_id())
                  << std::endl << std::endl;  // 后置换行
        std::cout.flush();  // 刷新输出缓冲区
        printed_ = true;  // 标记为已输出
    }
}

std::string FunctionProfiler::getThreadFileName(const std::string& funcName, const std::string& suffix) {
    pid_t pid = getpid();
    pid_t tid = get_thread_id();
    return savePrefix + std::to_string(pid) + "/" + std::to_string(tid) + "/" + funcName + "_" + suffix;
}

void FunctionProfiler::record(const char* format, ...) {
    childProcessInit();

    // 计算需要的缓冲区大小
    va_list args;
    va_start(args, format);
    int buffer_size = std::vsnprintf(nullptr, 0, format, args) + 1;  // +1 for null terminator
    va_end(args);

    // 动态分配缓冲区
    std::unique_ptr<char[]> buffer(new char[buffer_size]);

    va_start(args, format);
    std::vsnprintf(buffer.get(), buffer_size, format, args);
    va_end(args);

    auto record_time = std::chrono::high_resolution_clock::now();
    auto recordLog = getOfStream(getThreadFileName(functionName_.top(), "record.txt"));

    // 写入格式化后的信息
    recordLog << "Time: " << getHumanReadableTime(record_time)
              << ", SinceStart " << calculateMicrosecondsSinceStart(record_time)
              << ", Call " << callCount_
              << ": Duration " << getDurationInMicroseconds(record_time)
              << " microseconds.(1e-6 s)"
              << ", record: " << buffer.get() << std::endl;
}


void FunctionProfiler::logCallStack(const char* funcName) {
    std::ofstream stackLog = getOfStream(getThreadFileName(std::string(funcName), "funStack.log"));
    stackLog << "--------------------------------------" << std::endl;
    stackLog << "Time: " << getHumanReadableTime(std::chrono::high_resolution_clock::now()) << ", Call " << callCount_ << std::endl;
    if (isFuncStackLoggingEnabled())
        calltrace_->logCallStack(stackLog);
    stackLog << "--------------------------------------" << std::endl << std::endl;

}

void FunctionProfiler::logStats() {
    auto statLog = getOfStream(getThreadFileName(functionName_.top(), "stat.txt"));
    functionName_.pop();
    auto start_time = startTime_.top();
    startTime_.pop();
    statLog << "Time: " << getHumanReadableTime(start_time) 
            << ", SinceStart " << calculateMicrosecondsSinceStart(start_time)
            << ", Call " << callCount_ 
            << ": Duration " << getDurationInMicroseconds(start_time) 
            << " microseconds.(1e-6 s)" << std::endl;
}

}