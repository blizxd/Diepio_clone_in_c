cmake_minimum_required(VERSION 3.15)
project(DIEPIO)

set(CMAKE_C_STANDARD 11)

# Gather source files
file(GLOB SRC_FILES "src/*.c")

# Set SDL2_ttf to use vendored FreeType
set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)

# Fetch SDL2 and its extensions
include(FetchContent)

# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.2
)
# SDL2_image
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.2
)
# SDL2_ttf
FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.22.0
)

# Download and make them available
FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf)

# Define the executable
add_executable(game ${SRC_FILES})

# Include paths
target_include_directories(game PRIVATE include)

# Compiler flags
target_compile_options(game PRIVATE -Wall -pedantic)

#Link to sdl2main only on windows
if (WIN32)
  target_link_libraries( game PRIVATE SDL2main )

add_custom_command(
  TARGET Sandbox POST_BUILD COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:SDL2>"
    "$<TARGET_FILE:SDL2_image>" 
    "$<TARGET_FILE:SDL2_ttf>" 
    "$<TARGET_FILE_DIR:game>"
)
endif()

# Link libraries
target_link_libraries(game PRIVATE SDL2 SDL2_image SDL2_ttf m)

# Post-build: copy assets
add_custom_command(
    TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:game>/assets
    COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:game>/savegames

)