cmake_minimum_required(VERSION 3.10)
project(MyCProject C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Collect all .c files in root directory
file(GLOB SOURCES "*.c")

# Create the executable
add_executable(MyCProject ${SOURCES})
