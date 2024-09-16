#include <iostream>
#include <unistd.h>
#ifdef __APPLE__
    #include <pthread.h>
#elif defined(__linux__)
    #include <sys/syscall.h>  // For syscall and SYS_gettid
#endif

pid_t get_thread_id() {
#ifdef __APPLE__
    // macOS: Use pthread_threadid_np to get the thread ID
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    return static_cast<pid_t>(tid);
#elif defined(__linux__)
    // Linux: Use syscall to get the system thread ID (TID)
    return syscall(SYS_gettid);
#else
    // Fallback for other platforms if needed
    return -1;
#endif
}