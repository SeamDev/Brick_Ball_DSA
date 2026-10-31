# High-Density 2D Brick Breaker Engine (C++ & Raylib)

![C++](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B)
![Raylib](https://img.shields.io/badge/Library-Raylib_5.0-white?style=for-the-badge&logo=raylib)
![DSA](https://img.shields.io/badge/Focus-Data_Structures_%26_Algorithms-success?style=for-the-badge)
![FPS](https://img.shields.io/badge/Performance-60_FPS_%40_4000_Balls-brightgreen?style=for-the-badge)

An algorithmically optimized **"Many-Bricks / Ball-Swarm" Brick Breaker** game engine written in C++ using the **Raylib** multimedia library. Built as a Data Structures and Algorithms (DSA) course project, this engine demonstrates how replacing brute-force $O(N \times M)$ collision loops with **$O(1)$ Spatial Grid Hashing** and **$O(1)$ Swap-and-Pop Dynamic Arrays** allows real-time simulation of **4,000+ simultaneous physics balls** and **2,000+ pixel bricks** at a locked 60 FPS.

---

## Key Features & DSA Highlights

* **$O(1)$ Spatial Hash Grid Collision:** Converts continuous 2D ball coordinates `(x, y)` directly into 2D matrix indices `(row, col)` using integer quantization, eliminating $O(M)$ brick iteration.
* **$O(1)$ Swap-and-Pop Entity Management:** Removes out-of-bounds balls and collected power-up capsules from contiguous `std::vector` containers without triggering $O(N)$ memory shifts.
* **Sub-Stepped Continuous Physics:** Splits frame delta-time ($\Delta t$) into discrete sub-steps to prevent high-velocity balls from tunneling through thin $12\times12$ pixel barriers.
* **Exponential Multiplier Capsules:** Breaking bricks dynamically spawns **`+3` (Additive)** and **`x3` (Multiplicative)** capsules that rapidly scale active entities into the thousands.
* **Tunnel-Trap Level Architecture:** Features indestructible gray containment walls, yellow inner borders, and a dense purple pixel core designed to trap and multiply balls inside the chamber.

---

## Repository Structure

```text
high-density-brick-breaker/
├── src/
│   └── main.cpp                     # Complete C++ & Raylib game engine source code
├── docs/
│   ├── PROJECT_REPORT.md            # Full 7-Section Academic DSA Project Report
│   └── printable_project_report.pdf # Exported PDF Report for Academic Submission
├── .gitignore                       # Ignores build artifacts (.exe, .o)
└── README.md                        # Repository documentation
```

---

## Complexity Comparison

| Operation | Naive Brute-Force Approach | Proposed Engine (Spatial Grid + Swap-Pop) | Speedup Factor (4,000 Balls, 2,025 Bricks) |
| :--- | :---: | :---: | :---: |
| **Single Ball Collision** | $O(M)$ | $O(1)$ | $2,025\times$ fewer checks |
| **Full Frame Collision** | $O(N \times M)$ | $O(N)$ | $8.1 \times 10^6 \rightarrow 8.0 \times 10^3$ checks |
| **Entity Deletion** | $O(N)$ shift | $O(1)$ swap-pop | Instantaneous (No memmove) |
| **Memory Layout** | Pointer-chased | Contiguous L1/L2 Cache | Zero heap fragmentation |

*(Where $N$ = active balls up to $4,000$, and $M$ = total grid cells $45 \times 45 = 2,025$)*

---

## How to Compile and Run

### Prerequisites
* A C++17 compatible compiler (`g++`, `clang++`, or MSVC)
* [Raylib](https://www.raylib.com/) installed on your system

### 1. Linux / macOS
```bash
git clone https://github.com/yourusername/high-density-brick-breaker.git
cd high-density-brick-breaker
g++ src/main.cpp -o brick_breaker -O3 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./brick_breaker
```

### 2. Windows (MinGW / w64devkit)
```bash
g++ src/main.cpp -o brick_breaker.exe -O3 -lraylib -lopengl32 -lgdi32 -lwinmm
./brick_breaker.exe
```

---

## Controls

| Input | Action |
| :--- | :--- |
| `Left Arrow` / `A` | Move Paddle Left |
| `Right Arrow` / `D` | Move Paddle Right |
| `Left Mouse Hold` | Direct Paddle Tracking |
| `SPACEBAR` | **Sandbox Stress-Test:** Instantly trigger `x3` Ball Multiplier |
| `ESC` | Exit Application |

---

## Academic Project Report
The complete academic project report covering **Objectives, Data Structure Justification, System Flowchart, Implementation Details, Big-O Result Analysis, and Google Scholar References** can be viewed directly in [`PROJECT_REPORT.md`](./PROJECT_REPORT.md) or [`PROJECT_REPORT.pdf`](./PROJECT_REPORT.pdf).
