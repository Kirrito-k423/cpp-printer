#include "file_system_utils.hpp"

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <execinfo.h>
#include <cstdlib>   // for system()
#include <sstream> // for ostringstream


namespace cpprinter{

std::ofstream getOfStream(std::string filePath){
    createDirectories(filePath.substr(0, filePath.find_last_of('/')));  // 创建目录
    std::ofstream fileLog(filePath, std::ios_base::app);
    if(!fileLog.is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
    } 
    return fileLog;
}

}