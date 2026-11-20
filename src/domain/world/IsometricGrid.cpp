#include "rpg/domain/world/IsometricGrid.hpp"

#include <stdexcept>

namespace RPG::Domain::World
{
    IsometricGrid::IsometricGrid(std::size_t width, std::size_t height)
        : m_width(width),
          m_height(height),
          m_tiles(width * height)
    {
    }

    std::size_t IsometricGrid::getWidth() const
    {
        return m_width;
    }

    std::size_t IsometricGrid::getHeight() const
    {
        return m_height;
    }

    Tile &IsometricGrid::getTile(std::size_t x, std::size_t y)
    {
        if (x >= m_width || y >= m_height)
        {
            throw std::out_of_range("Tile coordinates are out of bounds.");
        }

        return m_tiles[y * m_width + x];
    }

    const Tile &IsometricGrid::getTile(std::size_t x, std::size_t y) const
    {
        if (x >= m_width || y >= m_height)
        {
            throw std::out_of_range("Tile coordinates are out of bounds.");
        }

        return m_tiles[y * m_width + x];
    }
}
