#include "address_resolution.hpp"
#include "icecream_wrapper.hpp"

#include <execinfo.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <regex>

namespace cpprinter{
    
// 获取当前进程的共享对象的基地址
std::string getSharedObjectBaseAddress(const std::string& so_name) {
    std::ifstream maps("/proc/self/maps");
    std::string line;

    // 检查so_name是否以".so"结尾
    if (so_name.size() < 3 || so_name.substr(so_name.size() - 3) != ".so") {
        // 如果不是以.so结尾，则直接返回输入地址
        return "0";
    }
    
    // 提取不带路径的 .so 文件名
    std::string so_name_base = so_name.substr(so_name.find_last_of('/') + 1);

    while (std::getline(maps, line)) {
        if (line.find(so_name_base) != std::string::npos) {
            std::stringstream ss(line);
            std::string address_range;
            ss >> address_range;
            // 找到加载地址范围的前半部分
            std::string base_address = address_range.substr(0, address_range.find('-'));
            return base_address;
        }
    }
    return "";
}

std::string get_code_snippet(const std::string& file_name, int line_number, int before = 8, int after = 2) {
    std::ifstream source_file(file_name);
    if (!source_file.is_open()) {
        return "Error opening source file: " + file_name;
    }

    std::string code_snippet;
    std::string line;
    int current_line = 1;
    int start_line = std::max(1, line_number - before);  // 'before' lines before target
    int end_line = line_number + after;  // 'after' lines after target

    while (std::getline(source_file, line)) {
        if (current_line >= start_line && current_line <= end_line) {
            code_snippet += std::to_string(current_line) + ": " + line + "\n";
        }
        if (current_line > end_line) {
            break;
        }
        current_line++;
    }

    source_file.close();
    return code_snippet.empty() ? "No code snippet available" : code_snippet;
}

// 调用 addr2line，使用相对地址解析符号
std::string addr2line(const std::string& so_path, void* addr) {
    // 获取共享对象的基地址
    std::string base_address_str = getSharedObjectBaseAddress(so_path);
    if (base_address_str.empty()) {
        return "Failed to find base address for: " + so_path;
    }

    // 将基地址和绝对地址转换为数值形式
    uintptr_t base_address = std::stoul(base_address_str, nullptr, 16);
    uintptr_t absolute_address = reinterpret_cast<uintptr_t>(addr);
    
    // 计算相对地址
    uintptr_t relative_address = absolute_address - base_address;

    // 使用 addr2line 将地址转换为文件名和行号
    std::ostringstream cmd;
    cmd << "addr2line -e " << so_path << " -fC " << std::hex << relative_address;

    tIC(cmd.str());

    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) {
        return "Error opening pipe";
    }

    char buffer[256];
    std::string result;
    std::string file_name;
    int line_number = -1;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
        // If this line contains a source file and line number (i.e., "/path/to/file:100")
        if (sscanf(buffer, "%[^:]:%d", buffer, &line_number) == 2) {
            file_name = buffer;
        }
    }
    pclose(pipe);

    if (file_name.empty() || line_number == -1) {
        return result.empty() ? "No information available" : result;
    }

    // Use the new get_code_snippet function to get surrounding code
    std::string code_snippet = get_code_snippet(file_name, line_number);
    return result + "\nCode snippet:\n" + code_snippet;
}

// 从 backtrace_symbols 中提取 .so 路径和地址
std::pair<std::string, void*> extractSharedObjectAndAddress(const std::string& symbol) {
    std::regex re(R"((.*)\((.*)(\+0x([0-9a-fA-F]+))?\) \[(0x[0-9a-fA-F]+)\])");
    std::smatch match;

    if (std::regex_search(symbol, match, re)) {
        std::string so_path = match[1];        // .so 文件路径
        void* address = reinterpret_cast<void*>(std::stoul(match[5], nullptr, 16));  // 绝对地址
        return {so_path, address};
    }

    return {"", nullptr};
}

std::string getSourceFromSymbol(const std::string& symbol){
    // 提取 .so 文件路径和地址
    auto [so_path, addr] = extractSharedObjectAndAddress(symbol);

    if (!so_path.empty() && addr) {
        // 使用 addr2line 获取具体代码行号
        return addr2line(so_path, addr);
    } else {
        return "Unable to extract .so path or address";
    }
}
}
