# AIGame_2025_Abdelbaki_Lang

This project implements an AI-based game engine using C++14. It utilizes Iterative Deepening Depth-First Search for its bot logic.

## Prerequisites

Before building the project, ensure you have the following installed:

* **CMake**: Version 3.20 or newer.
* **C++ Compiler**: Must support **C++14** (e.g., GCC, Clang, MSVC).
* **Make** or another build tool (Ninja, Visual Studio, etc.).

## Project Structure

* `src/`: Source files (Game logic, Bot implementations).
* `src/Evaluation/`: Evaluation Logic and Metaheuristic
* `include/`: Header files.
* `main.cpp`: Application entry point.

## Build Instructions

It is recommended to build the project in **Release** mode to enable optimizations (like `-O3` and `-march=native`), which are critical for the AI's performance.

1.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```
    
3.  **Compile the executable:**
    ```bash
    cmake --build .
    ```

## Running the Game

After a successful build, the executable will be located in the `build` directory.

**On Linux / macOS:**
```bash
./AIGame_2025_Abdelbaki_Lang
```

**On Windows**
```bash
AIGame_2025_Abdelbaki_Lang.exe
```

**IDE Setup**
This project uses standard CMake configurations and can be opened directly in:
- CLion
- Visual Studio Code (CMake Tools extension)
- Visual Studio
