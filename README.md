# LuminaDB - High-Performance In-Memory Key-Value Store

**LuminaDB** is a high-throughput, thread-safe key-value storage engine designed for low-latency systems. Built from the ground up in C++17, it utilizes a custom implementation of a chained hash table with dynamic resizing, ensuring amortized O(1) time complexity for all CRUD operations. Unlike standard containers, LuminaDB implements a Multiple-Readers/Single-Writer concurrency model using **std::shared_mutex**, allowing it to handle heavy concurrent read loads without blocking. The engine also features a robust binary serialization protocol, ensuring data persistence and state recovery across system restarts.

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

## ✅ Verification

To verify system integrity, the project includes a test suite that validates collision handling, resizing logic, and memory persistence.

**Run the verification suite:**
```bash
./run_tests.exe
```

**Expected Output:**
```bash
[PASS] Basic Set/Get
[PASS] Update Existing Key
[PASS] Get Non-Existent Key
[PASS] Collision Retrieval 1
[PASS] Collision Retrieval 2
[PASS] Collision Retrieval 3
[PASS] Pre-Delete Check
[PASS] Post-Delete Check

[SUMMARY] All internal logic verified successfully.
```
## 🧪 Performance Benchmarks
- Concurrency: Successfully tested with 10 concurrent threads performing 10,000 mixed operations.

- Resizing: Verified dynamic expansion from 100 to 25,000+ slots without data loss.

---
### 👨‍💻 Author
**Bipul Das** *Architected and engineered as an advanced Systems Programming initiative, focusing on memory optimization and concurrency.*
