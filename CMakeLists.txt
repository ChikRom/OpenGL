cmake_minimum_required(VERSION 4.2.1)

project(OpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# all cpp and h files

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE  HEADER_FILES "src/*.h" "src/*.hpp")

#create executable file
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# all libraries
file (GLOB LIB_FILES "lib/*.lib" "lib/*.dll")
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_FILES} opengl32)

# header files
target_include_directories(OpenGL PRIVATE include src)

# include support for Debug/Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# directory with executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

# Separate Debug and Release
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(_DEBUG)
	add_compile_options(/Od)
else()
	add_compile_definitions(NDEBUG)
	add_compile_options(/O2)
endif() 

#Create a constant path for our shaders
target_compile_definitions(${PROJECT_NAME} PUBLIC SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")
target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")