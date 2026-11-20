#include <gtest/gtest.h>

#include "rpg/domain/rendering/IsometricProjection.hpp"

using RPG::Domain::Rendering::IsometricProjection;
using RPG::Domain::Rendering::ScreenPoint;
using RPG::Domain::Rendering::WorldPoint;

namespace RPG::Domain::Rendering
{
    TEST(IsometricProjectionTests, ConvertsWorldOriginToScreenOrigin)
    {
        IsometricProjection projection{64.0f, 32.0f};

        const ScreenPoint screen = projection.toScreen({0.0f, 0.0f});

        EXPECT_FLOAT_EQ(screen.x, 0.0f);
        EXPECT_FLOAT_EQ(screen.y, 0.0f);
    }

    TEST(IsometricProjectionTests, ConvertsWorldXAxisToScreenCoordinates)
    {
        IsometricProjection projection{64.0f, 32.0f};

        const ScreenPoint screen = projection.toScreen({1.0f, 0.0f});

        EXPECT_FLOAT_EQ(screen.x, 32.0f);
        EXPECT_FLOAT_EQ(screen.y, 16.0f);
    }

    TEST(IsometricProjectionTests, ConvertsWorldYAxisToScreenCoordinates)
    {
        IsometricProjection projection{64.0f, 32.0f};

        const ScreenPoint screen = projection.toScreen({0.0f, 1.0f});

        EXPECT_FLOAT_EQ(screen.x, -32.0f);
        EXPECT_FLOAT_EQ(screen.y, 16.0f);
    }

    TEST(IsometricProjectionTests, ConvertsScreenCoordinatesBackToWorld)
    {
        IsometricProjection projection{64.0f, 32.0f};

        const WorldPoint world{2.0f, 3.0f};

        const ScreenPoint screen = projection.toScreen(world);
        const WorldPoint result = projection.toWorld(screen);

        EXPECT_FLOAT_EQ(result.x, world.x);
        EXPECT_FLOAT_EQ(result.y, world.y);
    }

    TEST(IsometricProjectionTests, SupportsFractionalWorldCoordinates)
    {
        IsometricProjection projection{64.0f, 32.0f};

        const ScreenPoint screen = projection.toScreen({1.5f, 2.25f});

        EXPECT_FLOAT_EQ(screen.x, -24.0f);
        EXPECT_FLOAT_EQ(screen.y, 60.0f);
    }

    TEST(IsometricProjectionTests, ThrowsWhenTileSizeXIsZero)
    {
        EXPECT_THROW(
            IsometricProjection(0.0f, 32.0f),
            std::invalid_argument);
    }

    TEST(IsometricProjectionTests, ThrowsWhenTileSizeYIsZero)
    {
        EXPECT_THROW(
            IsometricProjection(64.0f, 0.0f),
            std::invalid_argument);
    }

    TEST(IsometricProjectionTests, ThrowsWhenTileSizeIsNegative)
    {
        EXPECT_THROW(
            IsometricProjection(-64.0f, 32.0f),
            std::invalid_argument);
    }

}