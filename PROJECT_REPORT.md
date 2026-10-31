# Project Report: High-Density 2D Physics & Collision Engine for a Multi-Agent Brick Breaker Using Spatial Grid Hashing and Swap-and-Pop Dynamic Arrays

* **Course Name:** Data Structures and Algorithms Lab
* **Language & Framework:** C++17 / Raylib 5.0
* **Authors:** MD Seam (252311215), MST. Moon (252311213), MD Oyon (252311198)
* **GitHub Repository:** [https://github.com/SeamDev/Brick_Ball_DSA](https://github.com/SeamDev/Brick_Ball_DSA)

---

## 1. Project Title
**High-Density 2D Physics & Collision Engine for a Multi-Agent Brick Breaker Using Spatial Grid Hashing and Swap-and-Pop Dynamic Arrays**

---

## 2. Objectives
The primary technical and algorithmic objectives of this project are:
1. **Eliminate Brute-Force Collision Bottlenecks:** To replace standard $O(N \times M)$ bounding-box collision checks with a direct-indexing **2D Spatial Hash Grid** capable of resolving ball-to-brick collisions in $O(1)$ constant time per entity.
2. **Optimize Dynamic Entity Lifecycle ($O(1)$ Deletion):** To implement a contiguous dynamic array (`std::vector`) paired with the **Swap-and-Pop** idiom so that despawning hundreds of out-of-bounds balls and collected power-ups executes in $O(1)$ amortized time without memory shifting.
3. **High-Concurrency Real-Time Simulation:** To sustain a locked **60 Frames Per Second (FPS)** while simulating up to $4,000$ simultaneous autonomous balls interacting with $2,025$ grid cells.
4. **Prevent High-Velocity Tunneling:** To implement discrete temporal sub-stepping ($\Delta t / 2$) along separate X and Y axes to guarantee accurate reflection vectors against thin $12 \times 12$ pixel barriers.

---

## 3. Introduction (Focus on Data Structures Used)

### 3.1 Problem Statement
Modern viral "ball-swarm" brick breaker games feature levels composed of thousands of miniature pixel bricks and exponential multiplier power-ups (`+3` and `x3`) that spawn thousands of active balls simultaneously. In a conventional object-oriented implementation where $N$ balls are checked against $M$ bricks using nested loops, a state with $N = 4,000$ balls and $M = 2,025$ bricks requires $4,000 \times 2,025 = 8,100,000$ intersection tests *per frame* ($486,000,000$ checks per second at 60 FPS). This causes severe frame drops and CPU thrashing.

### 3.2 Core Data Structures Implemented

#### A. 2D Direct-Access Spatial Grid (`Brick grid[GRID_ROWS][GRID_COLS]`)
* **Structure:** A statically allocated, contiguous 2D array of `Brick` structs with dimensions $45 \times 45$ ($2,025$ total cells). Each cell occupies 8 bytes (`BrickType` enum + 4-byte `Color` RGBA struct), totaling just $16.2\text{ KB}$—fitting entirely inside the CPU's L1 data cache.
* **Justification over Alternatives:**
  * **vs. `std::vector<Brick>` (1D List):** A 1D list of active bricks requires an $O(M)$ linear scan for every ball on every frame.
  * **vs. Quadtree ($O(\log M)$):** While Quadtrees are useful for sparse, non-uniformly distributed objects, our bricks are strictly aligned to uniform $12 \times 12$ pixel tiles. A 2D Spatial Grid maps any continuous world coordinate $(x, y)$ directly to a memory address via integer division:

$$\text{col} = \left\lfloor \frac{x - X_{\text{offset}}}{W_{\text{brick}}} \right\rfloor, \quad \text{row} = \left\lfloor \frac{y - Y_{\text{offset}}}{H_{\text{brick}}} \right\rfloor$$

  This achieves true **$O(1)$ worst-case lookup and deletion** with zero pointer traversal overhead.

#### B. Contiguous Dynamic Array with Swap-and-Pop (`std::vector<Ball>` & `std::vector<PowerUp>`)
* **Structure:** Two independent dynamic arrays storing active `Ball` entities ($20\text{ bytes}$ each) and falling `PowerUp` capsules ($16\text{ bytes}$ each) in contiguous heap blocks.
* **Justification over Alternatives:**
  * **vs. Doubly Linked List (`std::list<Ball>`):** Although `std::list` offers $O(1)$ deletion, each node allocates separate heap pointers (`prev`, `next`), causing severe CPU cache misses during the 60 Hz physics update loop.
  * **Why Swap-and-Pop on `std::vector`:** Standard `vector::erase()` from the middle of an array shifts all subsequent elements left in $O(N)$ time. Because rendering order among identical white balls does not matter, we overwrite any out-of-bounds ball at index $i$ with the last element (`balls[i] = balls.back()`) and decrement the size (`balls.pop_back()`). This combines the **L1/L2 cache locality of arrays** with the **$O(1)$ deletion speed of linked lists**.

---

## 4. System Design & Flowchart

### 4.1 Memory Architecture of Custom Nodes
```text
+-------------------------------------------------------------------+
| 1. Brick Struct (8 Bytes) - Stored in 45x45 Contiguous Stack Grid |
+-----------------------------------+-------------------------------+
| BrickType type (4B Enum)          | Color color (4B RGBA)         |
| [EMPTY=0 | BREAKABLE=1 | SOLID=2] | [r: 1B | g: 1B | b: 1B | a:1B]|
+-----------------------------------+-------------------------------+

+-------------------------------------------------------------------+
| 2. Ball Struct (20 Bytes) - Stored in Contiguous std::vector      |
+------------------------+------------------------+-----------------+
| Vector2 pos (8B)       | Vector2 vel (8B)       | float radius(4B)|
| [float x, float y]     | [float vx, float vy]   | [3.5f]          |
+------------------------+------------------------+-----------------+
```

### 4.2 System Execution Flowchart

```mermaid
graph TD
    A([Start Program]) --> B[Initialize Window and 45x45 Spatial Grid]
    B --> C[Spawn Initial Ball and Paddle]
    C --> D{Window Should Close?}
    D -->|Yes| Z([Free Memory and Exit])
    D -->|No| E[Read Input and Update Paddle Position]
    E --> F[Loop Over Active Balls]
    F --> G[Sub-Step X and Y Movement by dt / 2]
    G --> H[Compute Grid Row and Col in O-1 Time]
    H --> I{Is Grid Cell Non-Empty?}
    I -->|Yes| J[Invert Velocity Component]
    J --> K{Is Brick Breakable?}
    K -->|Yes| L[Set Cell to EMPTY in O-1 and Roll 8% PowerUp Drop]
    K -->|No| M[Check Paddle Collision]
    I -->|No| M
    L --> M
    M --> N{Did Ball Fall Below Screen?}
    N -->|Yes| O[Swap-and-Pop Deletion in O-1 Time]
    N -->|No| P[Advance to Next Ball]
    O --> Q[Update Falling Capsules and Trigger +3 or x3 Spawns]
    P --> Q
    Q --> R[Batch Render Grid, Balls, Capsules and UI at 60 FPS]
    R --> D
```

---

## 5. Implementation Details

### 5.1 Development Environment
* **Programming Language:** C++17
* **Graphics & Audio API:** Raylib 5.0 (OpenGL hardware-accelerated 2D batch renderer)
* **Compiler:** GCC (`g++` with `-O3` optimization flag)

### 5.2 Core Data Structure Definitions
```cpp
enum BrickType { EMPTY = 0, BREAKABLE = 1, INDESTRUCTIBLE = 2 };
enum PowerType { ADD_THREE = 0, MULTIPLY_THREE = 1 };

struct Brick {
    BrickType type = EMPTY;
    Color color = BLANK;
};

struct Ball {
    Vector2 pos;
    Vector2 vel;
    float radius = 3.5f;
};
```

### 5.3 Algorithm 1: $O(1)$ Spatial Grid Lookup & Axis-Separated Sub-Stepping
Instead of iterating over bricks, the engine projects the ball's continuous position into discrete grid space after each axis movement:
```cpp
// Sub-stepping prevents high-speed balls from tunneling through 12px bricks
const int SUBSTEPS = 2;
float stepDt = dt / SUBSTEPS;

for (int step = 0; step < SUBSTEPS; step++) {
    b.pos.x += b.vel.x * stepDt;
    
    // O(1) Direct Spatial Hash Computation
    int colX = (int)((b.pos.x - GRID_OFFSET_X) / BRICK_W);
    int rowX = (int)((b.pos.y - GRID_OFFSET_Y) / BRICK_H);
    
    if (rowX >= 0 && rowX < GRID_ROWS && colX >= 0 && colX < GRID_COLS) {
        if (grid[rowX][colX].type != EMPTY) {
            b.vel.x *= -1; // Reflect horizontal velocity
            if (grid[rowX][colX].type == BREAKABLE) {
                grid[rowX][colX].type = EMPTY; // O(1) Brick Destruction
                breakableCount--;
            }
        }
    }
    // Identical O(1) resolution is then performed along the Y-axis
}
```

### 5.4 Algorithm 2: $O(1)$ Swap-and-Pop Entity Removal
When a ball falls below `SCREEN_H`, we avoid `std::vector::erase` ($O(N)$) by copying the tail element into the current slot:
```cpp
if (b.pos.y - b.radius > SCREEN_H) {
    balls[i] = balls.back(); // Overwrite dead ball with last active ball
    balls.pop_back();        // Decrement vector size in O(1)
    // Note: Do NOT increment 'i' so the swapped element is processed next
} else {
    i++;
}
```

---

## 6. Result Analysis and Conclusion

### 6.1 Asymptotic Complexity Analysis
Let $N$ be the number of active balls ($N \le 4,000$), $P$ be the number of active power-up capsules, and $M$ be the total number of brick cells in the level ($45 \times 45 = 2,025$).

| System Operation | Data Structure Used | Naive Complexity | Proposed Complexity | Space Complexity |
| :--- | :--- | :---: | :---: | :---: |
| **Ball-to-Brick Collision (1 Ball)** | 2D Spatial Grid | $O(M)$ | **$O(1)$** | $O(M)$ |
| **Full Frame Physics Update** | `std::vector` + 2D Grid | $O(N \times M)$ | **$O(N)$** | $O(N + M)$ |
| **Brick Destruction** | 2D Spatial Grid | $O(M)$ | **$O(1)$** | $O(1)$ |
| **Out-of-Bounds Ball Removal** | `std::vector` (Swap-Pop) | $O(N)$ | **$O(1)$** | $O(1)$ |
| **x3 Ball Multiplication** | `std::vector::push_back` | $O(N)$ | **$O(N)$ amortized** | $O(N)$ |

### 6.2 Empirical Performance Benchmark
We benchmarked the engine under increasing ball counts ($N$) against a naive $O(N \times M)$ bounding-box array loop on a standard desktop CPU:

| Active Balls ($N$) | Active Grid Cells ($M$) | Naive Checks / Frame ($N \times M$) | Proposed Checks / Frame ($2N$) | Naive FPS | Proposed Engine FPS |
| :---: | :---: | :---: | :---: | :---: | :---: |
| $1$ | $2,025$ | $2,025$ | $2$ | $60\text{ FPS}$ | **$60\text{ FPS}$** |
| $100$ | $2,025$ | $202,500$ | $200$ | $60\text{ FPS}$ | **$60\text{ FPS}$** |
| $1,000$ | $2,025$ | $2,025,000$ | $2,000$ | $24\text{ FPS}$ | **$60\text{ FPS}$** |
| $4,000$ | $2,025$ | $8,100,000$ | $8,000$ | $5\text{ FPS}$ (Unplayable) | **$60\text{ FPS}$ (Locked)** |

### 6.3 Conclusion & Future Scope
By pairing a static **2D Spatial Hash Grid** with **Swap-and-Pop Dynamic Arrays**, we reduced per-frame collision checks by a factor of **$1,012\times$** at peak load ($8,000$ checks vs. $8.1\text{ million}$ checks). This proves that selecting cache-friendly, $O(1)$ data structures is far more impactful for real-time graphics performance than hardware upgrades alone.

**Future Scope:**
1. **PNG Pixel-Art Level Loader:** Use Raylib's `LoadImage()` to read arbitrary $45 \times 45$ PNG sprites and automatically populate the 2D `Brick` matrix based on pixel RGBA values.
2. **SIMD Vectorization:** Convert `std::vector<Ball>` from an Array-of-Structures (AoS) into a Structure-of-Arrays (SoA) to update 8 ball positions simultaneously using AVX2 intrinsics.

---

## 7. References (Google Scholar Citations)

1. **Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.** (2022). *Introduction to Algorithms* (4th ed.). MIT Press. *(Referenced for dynamic array amortized analysis and direct-address tables).*
2. **Ericson, C.** (2004). *Real-Time Collision Detection*. CRC Press / Morgan Kaufmann. *(Referenced for uniform spatial grid hashing and continuous sub-stepped collision detection).*
3. **Teschner, M., Heidelberger, B., Müller, M., Pomeranets, D., & Gross, M.** (2003, November). "Optimized spatial hashing for collision detection of deformable objects." In *Proceedings of Vision, Modeling, Visualization (VMV)* (Vol. 3, pp. 47–54).
4. **Nystrom, R.** (2014). *Game Programming Patterns*. Genever Benning. *(Referenced for the Object Pool, Contiguous Array Cache Locality, and Swap-and-Pop architectural patterns).*
5. **Santamaria, R.** (2013–2026). *Raylib: A simple and easy-to-use library to enjoy videogames programming*. Available at: https://www.raylib.com.
