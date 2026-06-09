cmake_minimum_required(VERSION 3.10)
project(rtlsdr_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain")
# Find the rtlsdr package installed via vcpkg
find_package(rtlsdr CONFIG REQUIRED)

# Point to the source file in the src directory
add_executable(rtlsdr_test src/main.cpp)

# Link the library using the exact target string provided by vcpkg
target_link_libraries(rtlsdr_test PRIVATE $<IF:$<TARGET_EXISTS:rtlsdr::rtlsdr>,rtlsdr::rtlsdr,rtlsdr::rtlsdr_static>)