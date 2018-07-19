cmake_minimum_required(VERSION 2.6)
project(StringFunctions)

set(StringFunctions_src src/StringFunctions.cpp src/StringUtil.cpp)
include_directories(${DmxCustomFunctions_SOURCE_DIR}/include)
include_directories(${StringFunctions_SOURCE_DIR}/include)

add_library(StringFunctions SHARED ${StringFunctions_src})
