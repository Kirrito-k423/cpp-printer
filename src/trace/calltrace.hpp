#ifndef CALLTRACE_H
#define CALLTRACE_H

#include <string>
#include <map>
#include <fstream>

namespace cpprinter{

class CallTrace {
private:
    static thread_local std::map<std::string, int> call_stack_map;
    static thread_local int current_stack_id;

    std::string symbolsToString(char **symbols, size_t size);
    bool isCallStackExisted(char **symbols, size_t size, int &stack_id);

public:
    void logCallStack(std::ofstream &stackLog);  
};

}

#endif // CALLTRACE_H
