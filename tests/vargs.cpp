#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <string>

#define RECORD(...) record(__VA_ARGS__)

void record(const char* format, ...) {
    char buffer[1024];
    
    // 使用 va_list 和 vsnprintf 处理变长参数
    va_list args;
    va_start(args, format);
    int result = std::vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (result < 0) {
        std::cerr << "Formatting error occurred!" << std::endl;
    } else if (result >= sizeof(buffer)) {
        std::cerr << "Buffer size is too small for formatted output!" << std::endl;
    } else {
        // 输出格式化结果
        std::cout << "Formatted output: " << buffer << std::endl;
    }
}

int main() {
    RECORD("num is %d", 42);  // 预期输出: "Formatted output: num is 42"
    return 0;
}
