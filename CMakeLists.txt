cmake_minimum_required(VERSION 3.9)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 17)

set(SFML_ROOT "F:/SFML_Debug")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED COMPONENTS network graphics window system)

find_package(OpenMP REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

include_directories(${SFML_INCLUDE_DIR})

add_executable(Mandelbrot main.cpp src/Mandelbrot.h)
target_link_libraries(Mandelbrot ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
