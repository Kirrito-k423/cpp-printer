#include "cpprinter.hpp"

#include "utils/fstream_wrapper.hpp"
#include "utils/shared_object.hpp"
#include "utils/thread.hpp"
#include "utils/time.hpp"

#include <string>
#include <thread>
#include <sstream> // for ostringstream
#include <iostream>

namespace cpprinter{

FunctionProfiler::FunctionProfiler(const std::string& funcName)
        : functionName_(funcName), startTime_(std::chrono::high_resolution_clock::now()) {
    // IC_CONFIG.enable();
    IC_CONFIG.disable();
    callCount_++;
    logCallStack();
}

FunctionProfiler::~FunctionProfiler() {
    logStats();
    std::cout << "FunctionProfiler to /tmp/cpp_" << std::to_string(getpid()) << std::endl;
}

std::string FunctionProfiler::getThreadFileName(const std::string& funcName, const std::string& suffix) {
    pid_t pid = getpid();
    pid_t tid = get_thread_id();
    return "/tmp/cpp_" + std::to_string(pid) + "/" + std::to_string(tid) + "/" + funcName + "_" + suffix;
}

std::string FunctionProfiler::getResultPath(){
    pid_t pid = getpid();
    pid_t tid = get_thread_id();
    return "/tmp/cpp_" + std::to_string(pid) + "/" + std::to_string(tid) + "/" + functionName_ + "_funStack.log";
}

void FunctionProfiler::logCallStack() {
    std::ofstream stackLog = getOfStream(getThreadFileName(functionName_, "funStack.log"));
    
    stackLog << "Time: " << getHumanReadableTime(startTime_) << ", Call " << callCount_ << std::endl;

    // 获取调用栈
    void* array[10];
    size_t size = backtrace(array, 10);
    char** symbols = backtrace_symbols(array, size);

    for (size_t i = 0; i < size; i++) {
        stackLog << symbols[i] << std::endl;

        // 提取 .so 文件路径和地址
        auto [so_path, addr] = extractSharedObjectAndAddress(symbols[i]);

        if (!so_path.empty() && addr) {
            // 使用 addr2line 获取具体代码行号
            stackLog << addr2line(so_path, addr) << std::endl;
        } else {
            stackLog << "Unable to extract .so path or address" << std::endl;
        }
    }

}


void FunctionProfiler::logStats() {
    auto statLog = getOfStream(getThreadFileName(functionName_, "stat.txt"));
    statLog << "Time: " << getHumanReadableTime(startTime_) 
            << ", Call " << callCount_ 
            << ": Duration " << getDurationInMicroseconds(startTime_) 
            << " microseconds." << std::endl;
}

}