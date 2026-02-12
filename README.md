# LuminaDB - High-Performance In-Memory Key-Value Store

**LuminaDB** is a lightweight, thread-safe, persistent key-value store built from scratch in C++17. It is designed to mimic the core architecture of systems like Redis, focusing on low-level memory management and high-concurrency performance.

## 🚀 Core Features

- **High Performance:** Utilizes a custom Hash Map with Chaining implementation to ensure **O(1)** average time complexity for Set, Get, and Delete operations.
- **Thread Safety:** Implements **Reader-Writer Locks (`std::shared_mutex`)** to allow unlimited concurrent readers while strictly isolating writers, preventing race conditions.
- **Dynamic Scaling:** Features an automatic **Load Factor Monitoring** system ($\alpha \ge 0.75$) that triggers dynamic resizing (Rehashing) to maintain performance as data grows.
- **Persistence:** Includes a custom **Binary Serialization** engine that snapshots memory to disk (`.bin`), allowing data recovery after system shutdown.
- **Memory Efficient:** Manually managed heap memory using RAII principles to prevent memory leaks.

## 🛠️ Technical Architecture

### Data Structure
- **Index:** Dynamic Array of Pointers (`HashNode** table`).
- **Collision Resolution:** Separate Chaining using Linked Lists.
- **Hashing Algorithm:** Custom modulo-based ASCII hashing.

### Concurrency Model
LuminaDB uses a **Multiple-Readers, Single-Writer** model:
- `GET`: Acquires a **Shared Lock**. Multiple threads can read simultaneously.
- `SET` / `REMOVE`: Acquires an **Exclusive Lock**. Blocks all other operations until completion.


## 💻 Installation & Build

### Prerequisites: ###
- C++17 Compiler (GCC/Clang/MSVC)
- CMake 3.10+

### Build Instructions: ###
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## ⚡ Usage Example
```bash
#include "include/HashTable.h"

int main() {
    HashTable db;

    // 1. Store Data (O(1))
    db.set("user_id", "1024");
    db.set("session_token", "abc-xyz-123");

    // 2. Retrieve Data (Thread-Safe)
    std::string token = db.get("session_token");

    // 3. Persist to Disk
    db.saveToFile("snapshot.bin");

    return 0;
}
```

## 🧪 Performance Benchmarks
- Concurrency: Successfully tested with 10 concurrent threads performing 10,000 mixed operations.

- Resizing: Verified dynamic expansion from 100 to 25,000+ slots without data loss.

---
### 👨‍💻 Author
**Bipul Das** *Architected and engineered as an advanced Systems Programming initiative, focusing on memory optimization and concurrency.*
