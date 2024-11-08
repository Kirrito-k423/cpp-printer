#ifndef CLI_HPP
#define CLI_HPP

#include <string>

class CLI {
public:
    static std::string executeCommand(const std::string& command);
};

#endif // CLI_HPP