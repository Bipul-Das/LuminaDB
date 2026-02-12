#include "../include/HashTable.h"
#include <mutex> // For std::unique_lock and std::shared_lock

HashTable::HashTable()
{
    capacity = 100;
    currentSize = 0;
    table = new HashNode *[capacity]();
}

HashTable::~HashTable()
{
    for (int i = 0; i < capacity; ++i)
    {
        HashNode *entry = table[i];
        while (entry != nullptr)
        {
            HashNode *prev = entry;
            entry = entry->next;
            delete prev;
        }
    }
    delete[] table;
}

int HashTable::hashFunction(const std::string &key)
{
    int hash = 0;
    for (char c : key)
    {
        hash = (hash + c) % capacity;
    }
    return hash;
}

// Private method: No lock needed here. It relies on set() to lock the database first.
void HashTable::rehash()
{
    int oldCapacity = capacity;
    capacity *= 2;
    HashNode **newTable = new HashNode *[capacity]();

    for (int i = 0; i < oldCapacity; ++i)
    {
        HashNode *entry = table[i];
        while (entry != nullptr)
        {
            HashNode *nextNode = entry->next;
            int newIndex = hashFunction(entry->key);
            entry->next = newTable[newIndex];
            newTable[newIndex] = entry;
            entry = nextNode;
        }
    }
    delete[] table;
    table = newTable;
}

void HashTable::set(const std::string &key, const std::string &value)
{
    // EXCLUSIVE LOCK: Only one thread can write at a time. All readers are blocked.
    std::unique_lock<std::shared_mutex> lock(rw_lock);

    if ((float)currentSize / capacity >= 0.75f)
    {
        rehash(); // Safe to call because we already hold the unique_lock
    }

    int hashValue = hashFunction(key);
    HashNode *entry = table[hashValue];
    HashNode *prev = nullptr;

    while (entry != nullptr)
    {
        if (entry->key == key)
        {
            entry->value = value;
            return;
        }
        prev = entry;
        entry = entry->next;
    }

    HashNode *newNode = new HashNode(key, value);
    if (prev == nullptr)
    {
        table[hashValue] = newNode;
    }
    else
    {
        prev->next = newNode;
    }
    currentSize++;
}

std::string HashTable::get(const std::string &key)
{
    // SHARED LOCK: Multiple threads can read simultaneously. Blocked only if a write is happening.
    std::shared_lock<std::shared_mutex> lock(rw_lock);

    int hashValue = hashFunction(key);
    HashNode *entry = table[hashValue];

    while (entry != nullptr)
    {
        if (entry->key == key)
        {
            return entry->value;
        }
        entry = entry->next;
    }
    return "";
}

void HashTable::remove(const std::string &key)
{
    // EXCLUSIVE LOCK: Memory modification requires isolation.
    std::unique_lock<std::shared_mutex> lock(rw_lock);

    int hashValue = hashFunction(key);
    HashNode *entry = table[hashValue];
    HashNode *prev = nullptr;

    while (entry != nullptr)
    {
        if (entry->key == key)
        {
            if (prev == nullptr)
            {
                table[hashValue] = entry->next;
            }
            else
            {
                prev->next = entry->next;
            }
            delete entry;
            currentSize--;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

int HashTable::getSize() const
{
    std::shared_lock<std::shared_mutex> lock(rw_lock);
    return currentSize;
}

int HashTable::getCapacity() const
{
    std::shared_lock<std::shared_mutex> lock(rw_lock);
    return capacity;
}

// ... existing code ...

bool HashTable::saveToFile(const std::string &filename)
{
    std::shared_lock<std::shared_mutex> lock(rw_lock); // Read-only lock while saving

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open())
        return false;

    // 1. Save Header: Store the current size so we know how many items to load later
    // We cast the address of 'currentSize' to a char pointer to write raw bytes
    // size_t is the safest type for counting size
    size_t count = currentSize;
    outFile.write((char *)&count, sizeof(count));

    // 2. Serialize Data
    for (int i = 0; i < capacity; ++i)
    {
        HashNode *entry = table[i];
        while (entry != nullptr)
        {
            // Write Key Length
            size_t keyLen = entry->key.size();
            outFile.write((char *)&keyLen, sizeof(keyLen));
            // Write Key Bytes
            outFile.write(entry->key.c_str(), keyLen);

            // Write Value Length
            size_t valLen = entry->value.size();
            outFile.write((char *)&valLen, sizeof(valLen));
            // Write Value Bytes
            outFile.write(entry->value.c_str(), valLen);

            entry = entry->next;
        }
    }

    outFile.close();
    return true;
}

bool HashTable::loadFromFile(const std::string &filename)
{
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open())
        return false;

    size_t count = 0;
    inFile.read((char *)&count, sizeof(count));

    for (size_t i = 0; i < count; ++i)
    {
        size_t keyLen = 0;
        inFile.read((char *)&keyLen, sizeof(keyLen));
        std::string key(keyLen, '\0');
        inFile.read(&key[0], keyLen);

        size_t valLen = 0;
        inFile.read((char *)&valLen, sizeof(valLen));
        std::string value(valLen, '\0');
        inFile.read(&value[0], valLen);

        // We use the public set() method so it handles rehashing/locking automatically
        set(key, value);
    }

    inFile.close();
    return true;
}