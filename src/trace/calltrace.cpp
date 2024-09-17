#include "calltrace.hpp"
#include "address_resolution.hpp"
#include <execinfo.h>
#include <iostream>

namespace cpprinter{

// 定义 thread_local 静态变量
thread_local std::map<std::string, int> CallTrace::call_stack_map;
thread_local int CallTrace::current_stack_id = 1;

std::string CallTrace::symbolsToString(char **symbols, size_t size) {
    std::string call_stack;
    for (size_t i = 0; i < size; ++i) {
        call_stack += std::string(symbols[i]) + "\n";
    }
    return call_stack;
}

bool CallTrace::isCallStackExisted(char **symbols, size_t size, int &stack_id) {
    std::string call_stack = symbolsToString(symbols, size);

    if (call_stack_map.find(call_stack) != call_stack_map.end()) {
        stack_id = call_stack_map[call_stack];
        return true;
    }

    stack_id = current_stack_id++;
    call_stack_map[call_stack] = stack_id;
    return false;
}

void CallTrace::logCallStack(std::ofstream &stackLog) {
    void *array[10];
    size_t size = backtrace(array, 10);
    char **symbols = backtrace_symbols(array, size);
    
    int stack_id;
    if (isCallStackExisted(symbols, size, stack_id)) {
        stackLog << "重复调用栈，调用栈ID: " << stack_id << std::endl;
    } else {
        stackLog << "调用栈ID: " << stack_id << std::endl;
        for (size_t i = 0; i < size; i++) {
            stackLog << symbols[i] << std::endl;
            stackLog << getSourceFromSymbol(symbols[i]) << std::endl;
        }
    }

    free(symbols);
}

}