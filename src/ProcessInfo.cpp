#include "ProcessInfo.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace cpprinter {
    namespace process_info {

        pid_t ProcessInfo::getCurrentPID() {
            return getpid();
        }

        std::string ProcessInfo::readFile(const std::string& path) {
            std::ifstream file(path);
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        bool ProcessInfo::isNumber(const std::string& str) {
            for (char c : str) {
                if (!isdigit(c)) {
                    return false;
                }
            }
            return true;
        }

        std::map<pid_t, std::string> ProcessInfo::getSubprocesses(pid_t pid) {
            std::map<pid_t, std::string> subprocesses;
            DIR* dir = opendir("/proc");
            if (dir == nullptr) {
                std::cerr << "Failed to open /proc directory" << std::endl;
                return subprocesses;
            }

            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (isNumber(entry->d_name)) {
                    pid_t child_pid = std::stoi(entry->d_name);
                    std::string status_path = "/proc/" + entry->d_name + "/status";
                    std::string status_content = readFile(status_path);

                    std::istringstream iss(status_content);
                    std::string line;
                    while (std::getline(iss, line)) {
                        if (line.find("PPid:") == 0) {
                            std::istringstream line_iss(line);
                            std::string key, value;
                            line_iss >> key >> value;
                            if (std::stoi(value) == pid) {
                                while (std::getline(iss, line)) {
                                    if (line.find("Name:") == 0) {
                                        std::istringstream name_iss(line);
                                        name_iss >> key >> value;
                                        subprocesses[child_pid] = value;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            closedir(dir);
            return subprocesses;
        }

        std::map<pid_t, std::string> ProcessInfo::getThreads(pid_t pid) {
            std::map<pid_t, std::string> threads;
            std::string thread_dir = std::string("/proc/") + std::to_string(pid) + "/task";
            DIR* dir = opendir(thread_dir.c_str());
            if (dir == nullptr) {
                std::cerr << "Failed to open thread directory" << std::endl;
                return threads;
            }

            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (isNumber(entry->d_name)) {
                    pid_t tid = std::stoi(entry->d_name);
                    std::string status_path = thread_dir + "/" + entry->d_name + "/comm";
                    std::string name = readFile(status_path);
                    threads[tid] = name;
                }
            }

            closedir(dir);
            return threads;
        }

        std::string ProcessInfo::formatInfo(pid_t pid, const std::map<pid_t, std::string>& subprocesses, const std::map<pid_t, std::string>& threads) {
            std::ostringstream oss;
            oss << "Current Process ID: " << pid << "\n\n";

            oss << "Subprocesses:\n";
            for (const auto& [child_pid, name] : subprocesses) {
                oss << "PID: " << child_pid << ", Name: " << name << "\n";
            }

            oss << "\nThreads:\n";
            for (const auto& [tid, name] : threads) {
                oss << "TID: " << tid << ", Name: " << name << "\n";
            }

            return oss.str();
        }

        std::string ProcessInfo::getProcessInfo() {
            pid_t current_pid = getCurrentPID();
            std::map<pid_t, std::string> subprocesses = getSubprocesses(current_pid);
            std::map<pid_t, std::string> threads = getThreads(current_pid);
            return formatInfo(current_pid, subprocesses, threads);
        }

    } // namespace process_info
} // namespace cpprinter