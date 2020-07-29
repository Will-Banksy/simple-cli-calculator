# Minimum required version of cmake to build this project
cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(Calculator VERSION 1.0)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CPP_FILES
	src/main.cpp

	src/helpers/Helper.cpp

	src/structs/Element.cpp
	src/structs/Function.cpp
	src/structs/Expression.cpp

	src/handlers/ParseHandler.cpp
	src/handlers/CalculationHandler.cpp
	src/handlers/EvaluateHandler.cpp
	src/handlers/FunctionHandler.cpp
)

set(HEADERS
	src/helpers/Helper.h

	src/structs/Element.h
	src/structs/Function.h
	src/structs/Expression.h

	src/handlers/ParseHandler.h
	src/handlers/CalculationHandler.h
	src/handlers/EvaluateHandler.h
	src/handlers/FunctionHandler.h
)

set(SRC ${CPP_FILES} ${HEADERS})

# Add all source files needed to build executable, with the first argument the name for the executable
add_executable(Calculator ${SRC})

# Add the include path for header files (in this case just the source directory). Can now include files like so: #include src/<dir>/<header_file>
target_include_directories(Calculator PRIVATE src)

# See https://ncona.com/2019/03/building-a-cpp-project-with-cmake/ for info on cmake
# Also see https://cmake.org/cmake/help/latest/guide/tutorial/index.html for cmake tutorial