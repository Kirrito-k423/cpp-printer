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

thread_local const std::string savePrefix="/tmp/cpp_";

// for stacked
thread_local std::stack<std::string> FunctionProfiler::functionName_;
thread_local std::stack<std::chrono::high_resolution_clock::time_point> FunctionProfiler::startTime_;
thread_local std::shared_ptr<CallTrace> FunctionProfiler::calltrace_ = std::make_shared<CallTrace>();

std::string Format2String(pid_t pid){
    if (isCPPrinterDEBUG()){
        std::cerr << "pid is " << pid << std::endl;
    }
    return std::to_string(pid);
}

void FunctionProfiler::CheckChildProcessInit(){
    // 检查 /tmp/{ppid()} 路径是否存在 并且 /tmp/{pid()} 不存在
    if (isCPPrinterDEBUG()){
        std::cerr << "stack functionName_ size is " << functionName_.size() << std::endl;
        std::cerr << "stack startTime_ size is " << startTime_.size() << std::endl;
        std::cerr << "savePrefix is " << savePrefix << std::endl;
    }
    std::string ppid = Format2String(getppid());
    std::string ppath = savePrefix + ppid;
    std::string path = savePrefix + Format2String(getpid());
    if (std::filesystem::exists(ppath) && !std::filesystem::exists(path)) {
        printed_ = false;
    }
}

void FunctionProfiler::initialize(const std::string& fullName) {
    functionName_.push(fullName);
    startTime_.push(std::chrono::high_resolution_clock::now());
    callCount_++;
    CheckChildProcessInit();
    logCallStack(fullName.c_str());

}

FunctionProfiler::FunctionProfiler(const char* funcName, const char* description = ""){
    if (isCPPrinterOff()) {
        return;
    }
    initialize(std::string(funcName)+std::string(description));
}

FunctionProfiler::FunctionProfiler(const char* funcName){
    if (isCPPrinterOff()) {
        return;
    }
    initialize(std::string(funcName));
}

FunctionProfiler::~FunctionProfiler() {
    if (isCPPrinterOff()) {
        return;
    }
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
    if (isCPPrinterOff()) {
        return;
    }
    CheckChildProcessInit();

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
    if (isFuncStackLoggingEnabled()) {
        record("FuncStack begin. %s", funcName);
        calltrace_->logCallStack(stackLog);
        record("FuncStack end. %s", funcName);
    }
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