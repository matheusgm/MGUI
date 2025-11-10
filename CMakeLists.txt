cmake_minimum_required(VERSION 3.16)
project(MGUI)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source files (all .cpp files in src/ and subfolders)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Include SFML headers
include_directories("${PROJECT_SOURCE_DIR}/include/SFML-3.0.2/include")

# Find SFML libraries manually
link_directories("${PROJECT_SOURCE_DIR}/include/SFML-3.0.2/lib")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

#cmake -B build -G "MinGW Makefiles"
#cmake --build build


#cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
#cmake --build build --config Debug