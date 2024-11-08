#include "CLI.hpp"
#include <iostream>
#include <cstdio>  // For popen
#include <memory>
#include <array>

std::string CLI::executeCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen failed!");
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    fclose(pipe);
    return result;
}