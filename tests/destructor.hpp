#include <iostream>
#include <cstdlib>  // atexit
#include <atomic>
#include <thread>

class MyClass {
public:
    MyClass();
    ~MyClass();
    std::atomic<bool> keepRunning;
    std::thread gThread;

};