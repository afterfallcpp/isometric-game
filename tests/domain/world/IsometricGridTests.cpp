#include <gtest/gtest.h>

#include <stdexcept>

#include "rpg/domain/world/IsometricGrid.hpp"

namespace RPG::Domain::World
{
    TEST(IsometricGridTests, CreatesGridWithSpecifiedDimensions)
    {
        IsometricGrid grid{10, 20};

        EXPECT_EQ(grid.getWidth(), 10);
        EXPECT_EQ(grid.getHeight(), 20);
    }

    TEST(IsometricGridTests, InitializesTilesAsGrass)
    {
        IsometricGrid grid{5, 5};

        EXPECT_EQ(grid.getTile(0, 0).type, TileType::Grass);
    }

    TEST(IsometricGridTests, AllowsModifyingTile)
    {
        IsometricGrid grid{5, 5};

        grid.getTile(2, 3).type = TileType::Water;

        EXPECT_EQ(grid.getTile(2, 3).type, TileType::Water);
    }

    TEST(IsometricGridTests, ThrowsWhenAccessingOutOfBoundsTile)
    {
        IsometricGrid grid{5, 5};

        EXPECT_THROW(
            grid.getTile(5, 0),
            std::out_of_range);
    }
}