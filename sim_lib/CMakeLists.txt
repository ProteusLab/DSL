cmake_minimum_required(VERSION 3.22)

project(sim LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CPM)
# Provide default CXX settings
include(dependencies)


add_executable(sim sim.cc generated/base_exec_engine.cc generated/naive_interpreter.cc generated/memory.cc generated/hart.cc generated/decoder.cc elf_loader.cc)
target_link_libraries(sim elfio::elfio)
target_include_directories(sim PUBLIC generated .)
