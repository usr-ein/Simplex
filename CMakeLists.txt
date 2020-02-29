cmake_minimum_required(VERSION 3.15)
project(Simplex)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include/eigen3/)
include_directories(/usr/local/include)

# Add all your source files here
# if you forget some, you'll get a linkage error
# i.e. the unfamous "Undefined symbol for architecture x86_64"
# Beware
add_executable(Simplex main.cpp LinalgHelper.cpp LinalgHelper.h Problem.cpp Problem.h SimplexException.h Simplex.cpp Simplex.h)