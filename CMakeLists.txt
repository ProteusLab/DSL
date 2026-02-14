cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(protea)

include(cmake/Bundler.cmake)
include(cmake/CompilerOptions.cmake)
include(cmake/CPM.cmake)
include(cmake/dependencies.cmake)

add_subdirectory(lib)
add_subdirectory(sim_lib)
add_subdirectory(sim_gen)
