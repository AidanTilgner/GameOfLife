# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Minimum CMake version and project name
cmake_minimum_required(VERSION 3.14)

project(GameOfLife C)

# Include CPM.cmake
include(cmake/CPM.cmake)

# Fetch SDL2 via CPM.cmake
CPMAddPackage(
  NAME SDL2
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG release-2.28.3
)

# Check if SDL2 was fetched successfully
if(SDL2_ADDED)
  # Create an executable target for our main.c file
  add_executable(GameOfLife main.c)

  # Link SDL2 to our executable target
  target_link_libraries(GameOfLife SDL2::SDL2)
else()
  message(FATAL_ERROR "SDL2 could not be fetched!")
endif()

