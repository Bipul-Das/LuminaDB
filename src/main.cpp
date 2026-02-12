#include <iostream>
#include <thread>
#include <chrono>
#include "../include/HashTable.h"

void printHeader(const std::string &title)
{
    std::cout << "\n==========================================" << std::endl;
    std::cout << "   " << title << std::endl;
    std::cout << "==========================================" << std::endl;
}

int main()
{
    const std::string DB_FILE = "database.bin";

    // --- SCOPE 1: The "Day 1" Session ---
    {
        printHeader("SESSION 1: INGESTION & SERIALIZATION");

        HashTable db;
        std::cout << "[SYSTEM] Database initialized. RAM is empty." << std::endl;

        std::cout << "[ACTION] Inserting critical system data..." << std::endl;
        db.set("sys_config", "v2.5.0-stable");
        db.set("admin_user", "root_access");
        db.set("max_connections", "5000");
        db.set("cache_policy", "LRU-Eviction");

        std::cout << "[STATUS] Current Database Size: " << db.getSize() << " items." << std::endl;

        std::cout << "[ACTION] Saving data to disk (" << DB_FILE << ")..." << std::endl;
        if (db.saveToFile(DB_FILE))
        {
            std::cout << "[SUCCESS] binary snapshot written to storage." << std::endl;
        }
        else
        {
            std::cerr << "[ERROR] Critical failure: Could not write to disk." << std::endl;
            return 1;
        }

        std::cout << "[SYSTEM] Simulating sudden server shutdown..." << std::endl;
    }
    // <--- DESTRUCTOR CALLED HERE. MEMORY IS WIPED.

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause for dramatic effect

    // --- SCOPE 2: The "Day 2" Session ---
    {
        printHeader("SESSION 2: RECOVERY & VALIDATION");

        HashTable db;
        std::cout << "[SYSTEM] Server restarting... (RAM: 0 items)" << std::endl;
        std::cout << "[STATUS] Initial Size: " << db.getSize() << std::endl;

        std::cout << "[ACTION] Loading snapshot from " << DB_FILE << "..." << std::endl;
        if (db.loadFromFile(DB_FILE))
        {
            std::cout << "[SUCCESS] Binary data deserialized into RAM." << std::endl;
        }
        else
        {
            std::cerr << "[ERROR] Failed to find or read database file." << std::endl;
            return 1;
        }

        std::cout << "[STATUS] Restored Database Size: " << db.getSize() << " items." << std::endl;

        // The Final Proof
        printHeader("DATA INTEGRITY CHECK");
        std::string val1 = db.get("sys_config");
        std::string val2 = db.get("admin_user");
        std::string val3 = db.get("max_connections");

        std::cout << "Key: 'sys_config'      -> Value: " << val1 << " " << (val1 == "v2.5.0-stable" ? "✅" : "❌") << std::endl;
        std::cout << "Key: 'admin_user'      -> Value: " << val2 << " " << (val2 == "root_access" ? "✅" : "❌") << std::endl;
        std::cout << "Key: 'max_connections' -> Value: " << val3 << " " << (val3 == "5000" ? "✅" : "❌") << std::endl;
    }

    return 0;
}