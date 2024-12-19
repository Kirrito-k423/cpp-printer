#include <iostream>
#include <cstdlib>  // atexit
#include <atomic>
#include <thread>
#include "cpprinter.hpp"
#include "destructor.hpp"



void cleanup() {
    std::cout << "Cleaning up before exit" << std::endl;
}

// 子线程的执行函数
void loopTask(int threadId, MyClass* myClass) {
    PROFILE_FUNCTION();
    while (myClass->keepRunning) {
        PROFILE_RECORD("In loopTask");
        std::cout << "Thread " << threadId << " is running...\n";
        // std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟工作
    }
    std::cout << "Thread " << threadId << " is stopping...\n";
}

MyClass::MyClass() { 
    // 启动多个子线程
    PROFILE_FUNCTION();
    keepRunning = 1;
    std::thread t1(loopTask, 1, this);
    gThread = std::move(t1);
    std::cout << "Constructor called!" << std::endl; 
}

MyClass::~MyClass() { 
    PROFILE_FUNCTION();
    PROFILE_RECORD("In ~MyClass");
    if(gThread.joinable()){
        keepRunning = 0;
        gThread.join();
    }
    std::cout << "Destructor called!" << std::endl; 
}


// 全局变量
MyClass globalVar;

int main() {
    atexit(cleanup);  // 注册清理函数
    std::cout << "Inside main function" << std::endl;
}
