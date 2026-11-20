#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "rpg/domain/ecs/Registry.hpp"
#include "rpg/application/systems/MovementSystem.hpp"

using RPG::Domain::ECS::Registry;
using RPG::Application::Systems::MovementSystem;

namespace RPG::Application::Core
{
    class Game
    {
    private:
        // Fenêtre principale du jeu.
        sf::RenderWindow m_window;

        // Monde ECS : entités et composants.
        Registry m_registry;

        // Systèmes qui exécutent la logique du jeu.
        MovementSystem m_movementSystem;

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
