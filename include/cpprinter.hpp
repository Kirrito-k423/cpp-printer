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

#include <chrono>
#include <string>


namespace cpprinter{

class CallTrace;

class FunctionProfiler {
public:
    FunctionProfiler(const std::string& funcName);
    ~FunctionProfiler();

    // static 静态函数不依赖于类的实例(也不能使用)，它可以在没有对象的情况下直接通过类名调用。 FunctionProfiler::getThreadFileName("myFunc", "log.txt");
    static std::string getThreadFileName(const std::string& funcName, const std::string& suffix);

private:
    std::string functionName_;
    std::chrono::high_resolution_clock::time_point startTime_;
    // 线程独立的计数器
    static inline thread_local int callCount_ = 0;
    CallTrace* calltrace_;
    static inline thread_local bool printed_ = false; 

private:
    void printInfoOnce();
    std::string getResultPath();
    void logCallStack();
    void logStats();

};

// 用 RAII 包装函数调用
#define PROFILE_FUNCTION() cpprinter::FunctionProfiler profiler(__FUNCTION__);

}//namespace cpprinter