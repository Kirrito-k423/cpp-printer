# cpp-printer

A lightweight utility for monitoring function behavior, including call stacks, timing, frequency, and call counts.

## Quick Usage

* **Installation**: Copy the `csrc/printer` directory to your project and include it in your build process.
* **API Usage**: Simply insert `PROFILE_FUNCTION()` at the start of the function you wish to monitor. This will automatically log the function’s call stack, invocation count, and frequency.

```cpp
void exampleFunction() {
    PROFILE_FUNCTION(); // Automatically records function info
    tIC(profiler.getResultPath()); // Outputs the log file path, e.g., /tmp/{pid}/{tid}/{fun_name}.log
}
```

## Features

* [x] Integrated with [icecream-cpp](https://github.com/renatoGarcia/icecream-cpp) and [cpptrace](https://github.com/jeremy-rifkin/cpptrace).
  * Note: [icecream-cpp](https://github.com/renatoGarcia/icecream-cpp) is still under active development. You may need to update headers for new features or bug fixes.
* [x] Supports multi-process/thread logging to `/tmp/{pid}/{tid}/{fun_name}.log`.
* [ ] Planned support for Google Test integration.

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