#include <iostream>
#include <cassert>
#include <vector>
#include "../include/HashTable.h"

// --- Micro-Test Framework ---
// This allows us to print green/red messages for pass/fail
void log_pass(const std::string &test_name)
{
    std::cout << "[PASS] " << test_name << std::endl;
}

void log_fail(const std::string &test_name)
{
    std::cerr << "[FAIL] " << test_name << std::endl;
    exit(1); // Stop immediately if a test fails
}

#define ASSERT_EQ(actual, expected, test_name)                                                \
    if (actual != expected)                                                                   \
    {                                                                                         \
        std::cerr << "   Expected: " << expected << "\n   Actual:   " << actual << std::endl; \
        log_fail(test_name);                                                                  \
    }                                                                                         \
    else                                                                                      \
    {                                                                                         \
        log_pass(test_name);                                                                  \
    }

// --- The Actual Tests ---

void test_basic_operations()
{
    HashTable db;
    db.set("username", "admin");

    // Test 1: Can we retrieve what we set?
    ASSERT_EQ(db.get("username"), "admin", "Basic Set/Get");

    // Test 2: Does updating a key work?
    db.set("username", "superuser");
    ASSERT_EQ(db.get("username"), "superuser", "Update Existing Key");

    // Test 3: Does getting a non-existent key return empty string?
    ASSERT_EQ(db.get("ghost_user"), "", "Get Non-Existent Key");
}

void test_collision_handling()
{
    HashTable db;
    // We intentionally add enough keys to force collisions or resizing
    // (Assuming small initial capacity or just standard logic)
    db.set("key1", "value1");
    db.set("key2", "value2");
    db.set("key3", "value3");

    ASSERT_EQ(db.get("key1"), "value1", "Collision Retrieval 1");
    ASSERT_EQ(db.get("key2"), "value2", "Collision Retrieval 2");
    ASSERT_EQ(db.get("key3"), "value3", "Collision Retrieval 3");
}

void test_deletion()
{
    HashTable db;
    db.set("temp_file", "junk_data");

    // Verify it exists first
    ASSERT_EQ(db.get("temp_file"), "junk_data", "Pre-Delete Check");

    // Delete it
    db.remove("temp_file");

    // Verify it's gone
    ASSERT_EQ(db.get("temp_file"), "", "Post-Delete Check");
}

int main()
{
    std::cout << "======================================" << std::endl;
    std::cout << "    LuminaDB Core Logic Unit Tests    " << std::endl;
    std::cout << "======================================" << std::endl;

    test_basic_operations();
    test_collision_handling();
    test_deletion();

    std::cout << "\n[SUMMARY] All internal logic verified successfully." << std::endl;
    return 0;
}