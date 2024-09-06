#include "printer/function.h"
#include "printer/icecream-wrap.hpp"

void testFunctionProfiler() {
    PROFILE_FUNCTION();
    tIC(profiler.getResultPath());
    // 模拟工作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    testFunctionProfiler();
    return 0;
}