#include "rpg/rendering/IsometricProjection.hpp"

#include <stdexcept>

namespace RPG::Rendering
{

    IsometricProjection::IsometricProjection(float tileWidth, float tileHeight)
        : m_tileWidth(tileWidth),
          m_tileHeight(tileHeight)
    {
        if (m_tileWidth <= 0.0f || m_tileHeight <= 0.0f)
        {
            throw std::invalid_argument("Tile width and height must be positive.");
        }
    }

    ScreenPoint IsometricProjection::toScreen(WorldPoint world) const
    {
        return {
            (world.x - world.y) * (m_tileWidth / 2.0f),
            (world.x + world.y) * (m_tileHeight / 2.0f)};
    }

    WorldPoint IsometricProjection::toWorld(ScreenPoint screen) const
    {

        return {
            screen.x / m_tileWidth + screen.y / m_tileHeight,
            -screen.x / m_tileWidth + screen.y / m_tileHeight};
    }

}
