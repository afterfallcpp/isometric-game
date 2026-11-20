#pragma once

namespace RPG::Domain::Rendering
{
    /**
     * @brief Représente un point dans les coordonnées du monde.
     */
    struct WorldPoint
    {
        float x;
        float y;
    };

    /**
     * @brief Représente un point en coordonnées écran.
     */
    struct ScreenPoint
    {
        float x;
        float y;
    };

    /**
     * @brief Convertit les coordonnées du monde en coordonnées isométriques.
     */
    class IsometricProjection
    {
    public:
        /**
         * @brief Construit une projection isométrique.
         *
         * @param tileWidth Largeur d'une tuile.
         * @param tileHeight Hauteur d'une tuile.
         */
        IsometricProjection(float tileWidth, float tileHeight);

        /**
         * @brief Convertit un point du monde en point écran.
         *
         * @param world Position dans le monde.
         * @return Position correspondante à l'écran.
         */
        ScreenPoint toScreen(WorldPoint world) const;

        /**
         * @brief Convertit un point écran en point du monde.
         *
         * @param screen Position à l'écran.
         * @return Position correspondante dans le monde.
         */
        WorldPoint toWorld(ScreenPoint screen) const;

    private:
        float m_tileWidth;
        float m_tileHeight;
    };
}
