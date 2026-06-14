# cyberpawn
A chess engine personal project

## Building with CMake

CMake is the cross-platform build system. It works on macOS (Apple Clang or LLVM Clang) and Windows (MSVC).

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

Build options:

- `CYBERPAWN_BUILD_CLI` (default ON) — build the interactive CLI
- `CYBERPAWN_BUILD_PROFILER` (default ON) — build the profiler utility
- `CYBERPAWN_BUILD_TESTS` (default ON) — build and register unit tests

Example: build only the core library and CLI:

```bash
cmake -S . -B build -DCYBERPAWN_BUILD_TESTS=OFF -DCYBERPAWN_BUILD_PROFILER=OFF
cmake --build build
```

Visual Studio solution files are still present for Windows development.

## Architecture thoughts
There is a VS project called cyberpawn. This is the core engine itself. It is just a statically linkable library.

There is a VS solution called cyberpawnService. This will be a windows service of some sort that will take requests and return results (e.g. here's a game position, give me the best 10 moves, etc.)

Maybe I'll make a node server that serves a webpage and lets you play chess against the engine. It will run on a machine with the cyberpawn service and make calls to it for moves.


