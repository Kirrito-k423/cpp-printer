#include "cpprinter.hpp"
#include <thread>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> // for fork()

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
    PROFILE_RECORD("key num is %d", key);
    // 模拟工作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    testFunctionProfiler2();
}

int main() {
    // 创建主进程的测试循环
    for (int i = 0; i < 3; i++) {
        testFunctionProfiler();
    }

    // 使用 fork 创建新进程
    pid_t pid = fork();
    
    if (pid < 0) {
        // fork 失败
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // 子进程中运行 testFunctionProfiler
        printf("In child process with pid %d\n", getpid());
        testFunctionProfiler();
    } else {
        // 父进程中等待子进程完成
        printf("In parent process with pid %d, child pid is %d\n", getpid(), pid);
        waitpid(pid, nullptr, 0);
    }

    // 创建一个新线程来运行 testFunctionProfiler
    std::thread profilerThread(testFunctionProfiler);
    // 等待新线程完成
    profilerThread.join();

    // 在主进程中继续测试
    testFunctionProfiler();
    testFunctionProfiler2();

    return 0;
}