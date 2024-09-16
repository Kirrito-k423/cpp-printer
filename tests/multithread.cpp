#include "cpprinter.hpp"

void testFunctionProfiler() {
    PROFILE_FUNCTION();
    // 模拟工作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    // 创建一个新线程来运行 testFunctionProfiler
    std::thread profilerThread(testFunctionProfiler);
    // 等待新线程完成
    profilerThread.join();
    testFunctionProfiler();
    return 0;
}