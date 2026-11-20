# --------------------------------------------------
# TEST EXECUTABLE
# --------------------------------------------------

add_executable(rpg-tests
    domain/ecs/EntityManagerTests.cpp
    domain/ecs/ComponentStorageTests.cpp
    domain/ecs/RegistryTests.cpp
    domain/rendering/IsometricProjectionTests.cpp
    domain/world/IsometricGridTests.cpp
    application/systems/MovementSystemTests.cpp
    ../src/domain/ecs/EntityManager.cpp
    ../src/domain/rendering/IsometricProjection.cpp
    ../src/domain/world/IsometricGrid.cpp
    ../src/application/systems/MovementSystem.cpp
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
