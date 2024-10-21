#include "cpprinter.hpp"
#include <thread>

void testFunctionProfiler() {
    PROFILE_FUNCTION();
    // 模拟工作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    testFunctionProfiler();
}


void testFunctionProfiler2() {
    PROFILE_FUNCTION();
    // 模拟工作
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

int main() {
    for (int i=0; i<3; i++){
        testFunctionProfiler();
    }
    // 创建一个新线程来运行 testFunctionProfiler
    std::thread profilerThread(testFunctionProfiler);
    // 等待新线程完成
    profilerThread.join();
    testFunctionProfiler();
    testFunctionProfiler2();
    return 0;
}