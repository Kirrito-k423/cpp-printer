// 为了解决这个需求，你可以设计一个辅助类来实现函数包装，通过 RAII（资源获取即初始化）或直接包装函数调用，统计信息并输出到对应的文件。以下是一些步骤和要点来设计这个辅助类：

// ### 1. 统计信息
// 你需要跟踪以下数据：
// - **调用次数**：可以通过一个计数器统计每次函数调用。
// - **执行时间**：使用高精度计时器（如 `std::chrono`）记录每次调用的持续时间。
// - **调用频率**：通过时间差计算函数调用频率。
// - **函数栈信息**：可以通过 `backtrace` 获取函数调用栈信息（可能需要依赖外部库，如 `libunwind`）。

// ### 2. 多线程支持
// 通过线程本地存储（Thread Local Storage, TLS）来管理每个线程独立的文件和统计信息。使用 `std::this_thread::get_id()` 获取当前线程的 ID。

// ### 3. 日志文件路径
// 日志文件路径按进程 ID 和线程 ID 来命名。你可以使用 `getpid()` 和 `std::this_thread::get_id()` 动态构建文件路径。

// ### 4. 包装函数的设计
// 通过模板类或宏包装函数，实现函数调用时自动记录统计信息。可以采用 RAII 的方式，进入函数时记录调用次数和时间，退出函数时(FunctionProfiler被销毁)写入数据。

// ### 关键点：
// 1. **多线程支持**：每个线程有自己独立的日志文件（通过线程 ID 和进程 ID 区分）。
// 2. **函数栈追踪**：通过 `backtrace()` 获取函数栈，并输出到专门的日志文件中。
// 3. **时间记录**：使用高精度计时器 `std::chrono` 来统计函数的执行时间。
// 4. **可扩展性**：可以通过 RAII 模式将这个类扩展到任意函数中，做到低侵入性。

// #### 示例代码框架：

// ```cpp
#include "backward.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <execinfo.h>

class FunctionProfiler {
public:
    FunctionProfiler(const std::string& funcName)
        : functionName(funcName), startTime(std::chrono::high_resolution_clock::now()) {
        // 统计调用次数
        callCount++;
        // 记录调用栈
        logCallStack();
    }

    ~FunctionProfiler() {
        // 统计执行时间
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        // 输出统计信息
        logStats(duration);
    }

    static void logCallStack() {
        void* buffer[256];
        int nptrs = backtrace(buffer, 256);
        char** symbols = backtrace_symbols(buffer, nptrs);

        std::ofstream stackLog(getThreadFileName(functionName, "funStack.log"), std::ios_base::app);
        for (int i = 0; i < nptrs; i++) {
            stackLog << symbols[i] << std::endl;
        }
        free(symbols);
    }

    static void logStats(long duration) {
        std::ofstream statLog(getThreadFileName(functionName, "stat.txt"), std::ios_base::app);
        statLog << "Call " << callCount << ": Duration " << duration << " microseconds." << std::endl;
    }

private:
    std::string functionName;
    std::chrono::high_resolution_clock::time_point startTime;
    // 线程独立的计数器
    static inline thread_local int callCount = 0;
    // tid 写不同线程，不需要锁
    // static inline thread_local std::mutex logMutex;

    static std::string getThreadFileName(const std::string& funcName, const std::string& suffix) {
        pid_t pid = getpid();
        std::thread::id tid = std::this_thread::get_id();
        return "/tmp/cpp_" + std::to_string(pid) + "/" + std::to_string(tid) + "/" + funcName + "_" + suffix;
    }
};

// 用 RAII 包装函数调用
#define PROFILE_FUNCTION() FunctionProfiler profiler(__FUNCTION__);

// 示例函数
// void exampleFunction() {
//     PROFILE_FUNCTION();
//     // 模拟工作
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));
// }

// int main() {
//     exampleFunction();
//     return 0;
// }

