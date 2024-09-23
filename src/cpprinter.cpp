#include "cpprinter.hpp"
#include "icecream_wrapper.hpp"

#include "trace/address_resolution.hpp"
#include "trace/calltrace.hpp"
#include "utils/fstream_wrapper.hpp"
#include "utils/thread.hpp"
#include "utils/time.hpp"

#include <string>
#include <thread>
#include <sstream> // for ostringstream
#include <iostream>

namespace cpprinter{

FunctionProfiler::FunctionProfiler(const char* funcName)
        : functionName_(std::string(funcName)), 
          startTime_(std::chrono::high_resolution_clock::now()),
          calltrace_(std::make_unique<CallTrace>()) {
    IC_CONFIG.disable();
    // IC_CONFIG.enable();
    callCount_++;
    logCallStack();
}

FunctionProfiler::~FunctionProfiler() {
    logStats();
    printInfoOnce();
}

void FunctionProfiler::printInfoOnce() {
    if (!printed_) {
        std::cout << "FunctionProfiler to /tmp/cpp_" << std::to_string(getpid()) 
                  << "/" + std::to_string(get_thread_id())  << std::endl;
        printed_ = true;  // Mark it as printed
    }
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
    stackLog << "--------------------------------------" << std::endl;
    stackLog << "Time: " << getHumanReadableTime(startTime_) << ", Call " << callCount_ << std::endl;
    calltrace_->logCallStack(stackLog);
    stackLog << "--------------------------------------" << std::endl << std::endl;

}


void FunctionProfiler::logStats() {
    auto statLog = getOfStream(getThreadFileName(functionName_, "stat.txt"));
    statLog << "Time: " << getHumanReadableTime(startTime_) 
            << ", Call " << callCount_ 
            << ": Duration " << getDurationInMicroseconds(startTime_) 
            << " microseconds." << std::endl;
}

}