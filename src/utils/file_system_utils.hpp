#include <sys/syscall.h>
#include <iostream>

namespace cpprinter{

void createDirectories(const std::string& path) {
    // 调用系统命令 mkdir -p
    std::string command = "mkdir -p " + path;
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Failed to create directory." << std::endl;
    }
}

}