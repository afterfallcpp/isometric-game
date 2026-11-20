#pragma once

#include <cstddef>
#include <vector>

#include "rpg/domain/world/Tile.hpp"

namespace RPG::Domain::World
{
    /**
     * @brief Représente la grille de tuilles isométriques du monde.
     */
    class IsometricGrid
    {
    public:
        IsometricGrid(std::size_t width, std::size_t height);

        std::size_t getWidth() const;
        std::size_t getHeight() const;

        Tile& getTile(std::size_t x, std::size_t y);
        const Tile& getTile(std::size_t x, std::size_t y) const;

    private:
        std::size_t m_width;
        std::size_t m_height;

        std::vector<Tile> m_tiles;
    };
}
