cmake_minimum_required(VERSION 3.16)

project(dtmf_detector VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build the DTMF detector as a reusable library
add_library(dtmf
    src/goertzel.cpp
    src/detector.cpp
)

target_include_directories(dtmf PUBLIC include)

# Enable automated testing
enable_testing()

# Goertzel frequency test
add_executable(test_goertzel
    tests/test_goertzel.cpp
)

target_include_directories(test_goertzel PRIVATE tests)
target_link_libraries(test_goertzel PRIVATE dtmf)

add_test(NAME GoertzelTest COMMAND test_goertzel)

# Full DTMF detector test
add_executable(test_detector
    tests/test_detector.cpp
)

target_include_directories(test_detector PRIVATE tests)
target_link_libraries(test_detector PRIVATE dtmf)

add_test(NAME DetectorTest COMMAND test_detector)
