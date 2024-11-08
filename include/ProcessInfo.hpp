#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <string>
#include <map>
#include <thread>
#include <chrono>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace cpprinter {
    namespace process_info {

        class ProcessInfo {
        public:
            static std::string getProcessInfo();

        private:
            static pid_t getCurrentPID();
            static std::string readFile(const std::string& path);
            static bool isNumber(const std::string& str);
            static std::map<pid_t, std::string> getSubprocesses(pid_t pid);
            static std::map<pid_t, std::string> getThreads(pid_t pid);
            static std::string formatInfo(pid_t pid, const std::map<pid_t, std::string>& subprocesses, const std::map<pid_t, std::string>& threads);
        };

    } // namespace process_info
} // namespace cpprinter

#endif // PROCESS_INFO_H