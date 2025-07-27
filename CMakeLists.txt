cmake_minimum_required(VERSION 3.15)
project(DIEPIO)

set(CMAKE_C_STANDARD 11)

# Gather source files
file(GLOB SRC_FILES "src/*.c")

# Define the executable
add_executable(game ${SRC_FILES})

# Include paths
target_include_directories(game PRIVATE include)

# Compiler flags
target_compile_options(game PRIVATE -Wall -pedantic)

# SDL_ttf to use vendored FreeType
set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)

# Add SDL libraries from external/
add_subdirectory(external/SDL/)
add_subdirectory(external/SDL_image/)
add_subdirectory(external/SDL_ttf/)

# Link SDL libraries
target_link_libraries(game PRIVATE SDL2 SDL2_image SDL2_ttf m)

# Platform-specific handling (e.g., SDL2main on Windows)
if (WIN32)
  target_link_libraries(game PRIVATE SDL2main)

  add_custom_command(
    TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE:SDL2>"
      "$<TARGET_FILE:SDL2_image>"
      "$<TARGET_FILE:SDL2_ttf>"
      "$<TARGET_FILE_DIR:game>"
  )
endif()

# Post-build: copy assets and savegames directory
add_custom_command(
    TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:game>/assets
    COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:game>/savegames
)
