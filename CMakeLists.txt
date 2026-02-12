cmake_minimum_required(VERSION 3.10)
project(LuminaDB VERSION 1.0)

set(CMAKE_CXX_STANDARD 17) # C++17 is strictly required for std::shared_mutex
set(CMAKE_CXX_STANDARD_REQUIRED True)

# System architecture: Locate the OS threading library
find_package(Threads REQUIRED)

include_directories(include)

add_executable(LuminaDB src/main.cpp src/HashTable.cpp)

# Link the threading library to our executable
target_link_libraries(LuminaDB PRIVATE Threads::Threads)