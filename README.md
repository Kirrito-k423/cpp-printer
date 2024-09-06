# cpp-printer

## Motivation

* **Understanding Project Logic**: Running test cases is an effective way to grasp the project's structure and behavior.
* **Multi-Process/Thread Handling**:
    * GDB can be cumbersome to use in these environments.
    * Outputting via `cout` to the terminal often leads to cluttered, hard-to-read logs.

## Features

* [x] Integrated with [icecream-cpp](https://github.com/renatoGarcia/icecream-cpp) and ~~[backward-cpp](https://github.com/bombela/backward-cpp/tree/master)~~ [cpptrace](https://github.com/jeremy-rifkin/cpptrace)
  * Please note: [icecream-cpp](https://github.com/renatoGarcia/icecream-cpp) is still under development, so you may need to update headers for new features or bug fixes.
* [x] Supports multi-process/thread logging to `/tmp/{pid}/{tid}/{fun_name}.log`.
* [x] Replaced [backward-cpp](https://github.com/bombela/backward-cpp/tree/master) with [cpptrace](https://github.com/jeremy-rifkin/cpptrace).
* [ ] Planned support for Google Test.

## Limitations

* Linux-only
  * Uses `mkdir -p` for directory creation.

## Known Issues

* [x] Compiling with `-g -O0` may cause segmentation faults when using backward-cpp.

