#include "cpprinter.hpp"
#include <thread>

static thread_local int key = 0;
void testFunctionProfiler2() {
    PROFILE_FUNCTION_WITH_DESC("_add_decs");
    PROFILE_RECORD("in func 2");
    // 模拟工作
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
}

void testFunctionProfiler() {
    PROFILE_FUNCTION();
    key += 1;
    PROFILE_RECORD("key num is %d",key);
    // 模拟工作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    testFunctionProfiler2();
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