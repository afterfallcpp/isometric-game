#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "rpg/ecs/Registry.hpp"
#include "rpg/systems/MovementSystem.hpp"

namespace RPG::Core
{
    class Game
    {
    private:
        // Fenêtre principale du jeu.
        sf::RenderWindow m_window;

        // Monde ECS : entités et composants.
        RPG::ECS::Registry m_registry;

        // Systèmes qui exécutent la logique du jeu.
        RPG::Systems::MovementSystem m_movementSystem;

        // Gestion des événements.
        void processEvents();

        // Mise à jour de la logique du jeu.
        void update(float deltaTime);

        // Affichage du jeu.
        void render();

    public:
        Game();

        ~Game() = default;

        // Démarre la boucle principale.
        void run();
    };
}
