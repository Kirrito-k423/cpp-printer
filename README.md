# cpp-printer

A lightweight utility for monitoring **Multithread/Multiprocess** function behavior, including call stacks, timing, frequency, and call counts.

## Quick Usage

### Installation

CMake FetchContent Usage:

```m
include(FetchContent)
FetchContent_Declare(
  cpp_printer # match project name
  GIT_REPOSITORY https://github.com/Kirrito-k423/cpp-printer.git
  GIT_TAG        v1.2.9
)
FetchContent_MakeAvailable(cpp_printer) # match project name

target_link_libraries(your_project PRIVATE cpprinter)
```

### API Usage
 
Simply insert `PROFILE_FUNCTION()` at the start of the function you wish to monitor. This will automatically log the function’s call stack, invocation count, and frequency.

```cpp
#include "cpprinter.hpp"
void exampleFunction() {
    PROFILE_FUNCTION(); // Automatically records function info
}
```

Or using icecream-plus with tid and color info

```cpp
#include "icecream_wrapper.hpp"
void exampleFunction() {
    // It seems will core dumped under some multithread situation.
    tIC(xxx); // printer the simple valiable xxx
}
```

## Features

* [x] Integrated with [icecream-cpp](https://github.com/renatoGarcia/icecream-cpp) and [cpptrace](https://github.com/jeremy-rifkin/cpptrace).
  * Note: [icecream-cpp](https://github.com/renatoGarcia/icecream-cpp) is still under active development. You may need to update headers for new features or bug fixes.
* [x] Supports **multi-process/thread logging** to `/tmp/{pid}/{tid}/{fun_name}.log`.
* [ ] Planned support for Google Test integration.
* [ ] virtualization with graphviz (not necessary due to callstack is not closely related)

## Limitations

* Linux-only.
  * Uses `mkdir -p` for directory creation.

## Motivation

* **Understanding Project Logic**: Running test cases provides clear insights into project flow and behavior.
* **Handling Multi-Process/Thread Environments**:
    * GDB is often cumbersome in these scenarios.
    * Logging directly with `cout` can clutter the terminal output, making it difficult to trace.

## Known Issues / To Do

* [x] Compiling with `-g -O0` caused segmentation faults with `backward-cpp`. This issue has been resolved by replacing [backward-cpp](https://github.com/bombela/backward-cpp/tree/master) with [cpptrace](https://github.com/jeremy-rifkin/cpptrace).
* [ ] When integrating the [v1 code](https://github.com/Kirrito-k423/cpp-printer/releases/tag/v1.0) into a [more complex project](https://gitee.com/shaojiemike/pytorch/tree/v2.1.0/) (involving more threads and deeper function call stacks), `cpptrace` occasionally caused segmentation faults. Instead of fixing `cpptrace`, I decided to develop a simple stack trace printing feature from scratch.
