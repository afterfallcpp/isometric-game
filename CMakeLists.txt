
cmake_minimum_required(VERSION 3.28)

project(RPGGame
    VERSION 0.1.0
    DESCRIPTION "A 2D RPG game developed in C++"
    LANGUAGES CXX
)

# --------------------------------------------------
# C++ STANDARD
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------
# CTEST
# --------------------------------------------------

include(CTest)

# --------------------------------------------------
# SFML CONFIGURATION
# --------------------------------------------------

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SFML)

# --------------------------------------------------
# GAME
# --------------------------------------------------

add_executable(rpg-game
    src/main.cpp
    src/core/Game.cpp
    src/ecs/EntityManager.cpp
    src/systems/MovementSystem.cpp
    src/rendering/IsometricProjection.cpp
    src/world/IsometricGrid.cpp
)

target_include_directories(rpg-game
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(rpg-game
    PRIVATE
        SFML::Graphics
)

# --------------------------------------------------
# TESTS
# --------------------------------------------------

if(BUILD_TESTING)

    FetchContent_Declare(
        GoogleTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(GoogleTest)

    add_subdirectory(tests)

endif()
