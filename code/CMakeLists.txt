# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Engine-BRAVO VERSION 0.1)

# Specify the C++ standard (e.g., C++11, C++14, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add debug options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode enabled")
    add_compile_options(-g -O0)
endif()

# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Specify the source files
set(SOURCES
    src/main.cpp
    # Add more source files here
)

# Add the executable target
add_executable(engine_BRAVO ${SOURCES})

# =====================
# Google Test Integration
# =====================

# FetchContent module to download GTest
include(FetchContent)

# Download and configure GTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip  # You can use the latest release
)

# Download GTest during configuration step
FetchContent_MakeAvailable(googletest)


# Enable testing in the project
enable_testing()

# Add the test source files
set(TEST_SOURCES
    test/test_main.cpp    # Your test source files
    # Add more test files here
)

# Create an executable for tests
add_executable(tests ${TEST_SOURCES})

# Link GTest and GTest Main to the test executable
target_link_libraries(tests gtest gtest_main)


# Fetch TinyXML2
FetchContent_Declare(
  tinyxml2
  URL https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.zip
)
FetchContent_MakeAvailable(tinyxml2)

# Fetch Zstd
FetchContent_Declare(
  zstd
  URL https://github.com/facebook/zstd/archive/refs/tags/v1.5.2.zip
)
FetchContent_MakeAvailable(zstd)


# Fetch TinyTMX
FetchContent_Declare(
  tinytmx
  URL https://github.com/KaseyJenkins/tinytmx/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(tinytmx)


# Fetch Zlib with Z_PREFIX option to avoid macro conflicts
FetchContent_Declare(
  zlib
  URL https://github.com/madler/zlib/archive/refs/tags/v1.2.13.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(zlib)

# Pass Zlib information to TinyTMX before FetchContent_MakeAvailable(tinytmx)

set(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR})
set(ZLIB_LIBRARY ${zlib_BINARY_DIR}/libz.a)
set(ZLIB_LIBRARIES ${zlib_BINARY_DIR}/libz.a)
set(ZLIB_INCLUDE_DIRS ${zlib_SOURCE_DIR})

# Add the Z_PREFIX option when building Zlib to prevent macro conflicts
add_compile_definitions(Z_PREFIX)
target_include_directories(engine_BRAVO PRIVATE ${zlib_INCLUDE_DIR})
target_include_directories(engine_BRAVO PRIVATE ${zlib_SOURCE_DIR})
# Include the Zlib and TinyXML2 headers and link the libraries



# Link libraries
target_link_libraries(engine_BRAVO 
    ${zlib_LIBRARY} 
    tinyxml2   # Link TinyXML2
    zstd       # Link Zstd
    tinytmx    # Link TinyTMX
)
# Add test execution
add_test(NAME engine_BRAVO_tests COMMAND tests)

# Optionally, set up test include directories
#target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/inc)
