#pragma once
#include <string>
#include <shared_mutex>
#include <fstream> // Required for File I/O

struct HashNode
{
    std::string key;
    std::string value;
    HashNode *next;

    HashNode(std::string k, std::string v) : key(k), value(v), next(nullptr) {}
};

class HashTable
{
private:
    HashNode **table;
    int capacity;
    int currentSize;
    mutable std::shared_mutex rw_lock;

    int hashFunction(const std::string &key);
    void rehash();

public:
    HashTable();
    ~HashTable();

    void set(const std::string &key, const std::string &value);
    std::string get(const std::string &key);
    void remove(const std::string &key);

    // --- New Persistence Layer ---
    bool saveToFile(const std::string &filename);
    bool loadFromFile(const std::string &filename);

    int getSize() const;
    int getCapacity() const;
};