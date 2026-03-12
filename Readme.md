# Terminal Snake Game (C++)

A console-based implementation of the classic "Snake" game, written in C++ using multithreading and ANSI escape codes for optimized rendering.


https://github.com/user-attachments/assets/804fdd86-81e0-4dad-a98e-4316f439dea2


## Technical Description

The program implements game logic through direct terminal cursor control. Instead of redrawing the entire screen (which causes flickering), the application only updates the modified coordinates: the head, the tail, and the food.

### Key Features:
* **Concurrency:** Utilizes `std::thread` to decouple the game loop from input processing.
* **Synchronization:** Protects the game state using `std::mutex` to prevent potential race conditions.
* **Rendering:** Optimized output via ANSI escape sequences.
* **Input Handling:** Non-blocking key reading (Raw mode) for instantaneous response.

---

## System Requirements

* **OS:** Windows (utilizes `windows.h` and `conio.h`).
* **Terminal:** Windows Terminal, PowerShell, or CMD with VT100 (ANSI) support.
* **Compiler:** Support for C++17 standard or higher.

---

## Controls

Commands are entered in real-time without the need to press the Enter key.

| Key | Action |
| :--- | :--- |
| **W** | Move Up |
| **S** | Move Down |
| **A** | Move Left |
| **D** | Move Right |
| **ESC** | Exit Game |

---

## Setup and Launch Instructions

### 1. File Preparation
Ensure the following files are located in the same directory:
* `main.cpp` — The primary source code.
* `game_settings.h` — Configuration file (board dimensions, symbols, colors).

### 2. Compilation
Run the following command in your terminal (for GCC/MinGW):
```bash
g++ main.cpp -o snake_game.exe -O3
