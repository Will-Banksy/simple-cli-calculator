# Minimum required version of cmake to build this project
cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(Calculator VERSION 1.0)

# Add all source files needed to build executable, with the first argument the name for the executable
add_executable(Calculator src/main.cpp src/helpers/CalcHelper.cpp src/structs/Element.cpp src/helpers/Helper.cpp src/helpers/FunctionHandler.cpp src/structs/Function.cpp)

# See https://ncona.com/2019/03/building-a-cpp-project-with-cmake/ for info on cmake
# Also see https://cmake.org/cmake/help/latest/guide/tutorial/index.html for cmake tutorial