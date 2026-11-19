#pragma once

#include <cstdint>

namespace RPG::World
{
    /**
     * @brief Represents the type of a world tile.
     */
    enum class TileType : std::uint8_t
    {
        Grass,
        Dirt,
        Water,
        Sand,
        Stone
    };

    /**
     * @brief Represents a single tile in the game world.
     */
    struct Tile
    {
        TileType type{TileType::Grass};
    };
}
