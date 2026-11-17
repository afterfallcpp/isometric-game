# --------------------------------------------------
# TEST EXECUTABLE
# --------------------------------------------------

add_executable(rpg-tests
    ecs/EntityManagerTests.cpp
    ecs/ComponentStorageTests.cpp
    ecs/RegistryTests.cpp
    systems/MovementSystemTests.cpp
    rendering/IsometricProjectionTests.cpp
    ../src/ecs/EntityManager.cpp
    ../src/systems/MovementSystem.cpp
    ../src/rendering/IsometricProjection.cpp
)

# --------------------------------------------------
# INCLUDE DIRECTORIES
# --------------------------------------------------

target_include_directories(rpg-tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# --------------------------------------------------
# GOOGLE TEST
# --------------------------------------------------

target_link_libraries(rpg-tests
    PRIVATE
        GTest::gtest_main
)

# --------------------------------------------------
# CTEST
# --------------------------------------------------

include(GoogleTest)

gtest_discover_tests(
    rpg-tests
    DISCOVERY_MODE PRE_TEST
)
