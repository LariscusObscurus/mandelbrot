cmake_minimum_required(VERSION 3.9)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

add_subdirectory(extern)
add_executable(Mandelbrot main.cpp src/Mandelbrot.h extern/stb_image_write/stb_image_write.h)
target_include_directories(Mandelbrot PRIVATE extern extern/stb_image_write)
