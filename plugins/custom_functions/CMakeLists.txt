cmake_minimum_required(VERSION 2.6)
project(DmxCustomFunctions)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(HexFunctions)
add_subdirectory(StringFunctions)

#set(CMAKE_SUPPRESS_REGENERATION true)
#set(CMAKE_DEFAULT_STARTUP_PROJECT HexFunctions)

