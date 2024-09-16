#include "cpprinter.hpp"

#include "utils/fstream_wrapper.hpp"
#include "utils/time.hpp"

#include <string>
#include <thread>
#include <sstream> // for ostringstream
#include <iostream>

namespace cpprinter{

FunctionProfiler::FunctionProfiler(const std::string& funcName)
        : functionName_(funcName), startTime_(std::chrono::high_resolution_clock::now()) {
    callCount_++;
    logCallStack();
}

FunctionProfiler::~FunctionProfiler() {
    logStats();
    std::cout << "FunctionProfiler to /tmp/cpp_" << std::to_string(getpid()) << std::endl;
}

std::string FunctionProfiler::getThreadFileName(const std::string& funcName, const std::string& suffix) {
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid); /* 获取系统级 TID */ 
    return "/tmp/cpp_" + std::to_string(pid) + "/" + std::to_string(tid) + "/" + funcName + "_" + suffix;
}

std::string FunctionProfiler::getResultPath(){
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid); /* 获取系统级 TID */ 
    return "/tmp/cpp_" + std::to_string(pid) + "/" + std::to_string(tid) + "/" + functionName_ + "_funStack.log";
}

void FunctionProfiler::logCallStack() {
    auto stackLog = getOfStream(getThreadFileName(functionName_, "funStack.log"));
    
    stackLog << "Time: " << getHumanReadableTime(startTime_) << ", Call " << callCount_ << std::endl;

    cpptrace::stacktrace trace = cpptrace::generate_trace();
    std::ostringstream oss;
    trace.print_with_snippets(oss);  

    stackLog << oss.str() << std::endl;
}


void FunctionProfiler::logStats() {
    auto statLog = getOfStream(getThreadFileName(functionName_, "stat.txt"));
    statLog << "Time: " << getHumanReadableTime(startTime_) 
            << ", Call " << callCount_ 
            << ": Duration " << getDurationInMicroseconds(startTime_) 
            << " microseconds." << std::endl;
}

}