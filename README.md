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

## Referee: Arbitre

The repository includes a referee program `Arbitre.java` that launches and manages two player processes, relays moves between them, enforces timeouts, and prints results. The referee communicates with the player executables via standard input/output.

Key behavior:

- Launches two player processes:
  - `AIGame_2025_Abdelbaki_Lang.exe JoueurA`
  - `AIGame_2025_Abdelbaki_Lang.exe JoueurB`
- Alternates turns between players, sending the last move to the current player.
- Waits up to 3 seconds for a move and disqualifies a player on timeout.
Ends the match when a move contains `RESULT` or after 400 moves.

### How to add our Player to the Arbitre
#### LINUX
- Download the Repo
- Add the Linux excecutable in the following line
```
Process A = Runtime.getRuntime().exec("here the absolute path JoueurA");
```
ou
```
Process B = Runtime.getRuntime().exec("here the absolute path JoueurB");
```

### How to compile the referee

From the `referee` directory:

```powershell
cd referee
javac Arbitre.java
```

This produces `Arbitre.class` in the same directory.

> Note: `Arbitre.java` contains hard-coded paths to the player executable. Adjust them if your build output location differs.

### How to run the referee

From the `referee` directory run:

```powershell
java Arbitre
```

The referee will start both player processes and manage the game until termination.

### Notes

- Adjust the path to the player executable in `Arbitre.java` if your build location differs.
- The referee expects the player executables to communicate via standard input/output.
- The game ends when a player sends a move containing `RESULT` or after 400 moves.


# How to compile the Code

## Build Instructions

1.  **Create a build directory:**
    ```bash
    mkdir -p build
    cd build
    ```
    
2.  **Configure CMake in Release mode:**
    ```bash
    cmake -DCMAKE_BUILD_TYPE=Release ..
    ```

3.  **Compile the executable:**
    ```bash
    cmake --build . --config Release
    ```

## Running the Process without Arbitre

After a successful build, the executable will be located in the `build` directory.

[Options]: JoueurA or JoueurB (JoueurA would be the one who starts)

**On Linux / macOS:**
```bash
./AIGame_2025_Abdelbaki_Lang [Options]
```

**On Windows**
```bash
AIGame_2025_Abdelbaki_Lang.exe [Options]
```

**IDE Setup**
This project uses standard CMake configurations and can be opened directly in:
- CLion
- Visual Studio Code (CMake Tools extension)
- Visual Studio
