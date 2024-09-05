#include "printer/function.h"

void exampleFunction() {
    PROFILE_FUNCTION();
    // 模拟工作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    exampleFunction();
    return 0;
}