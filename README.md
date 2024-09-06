# cpp-printer

## Motivation

* Test/Run cases is a good way to understand the project logic.
* Multi-Process/Thread Situation
    * GDB is difficult to used 
    * cout > terminal is in chaos

## Features

* [x] Integrated with [icecream-cpp](https://github.com/renatoGarcia/icecream-cpp) and [backward-cpp](https://github.com/bombela/backward-cpp/tree/master)
  * But these project ([icecream-cpp](https://github.com/renatoGarcia/icecream-cpp)) is under development, you maybe need to update related header for new features and fix bugs.
* [ ] Support Multi-Process/Thread Printer to `/tmp/{pid}/{tid}/{fun_name}.log`
* [ ] Support google test

## Limits

* Only on Linux
  * Using `mkdir -p`