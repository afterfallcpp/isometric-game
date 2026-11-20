#include "rpg/application/core/Game.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace RPG::Application::Core
{
    Game::Game()
        : m_window(sf::VideoMode({1280u, 720u}), "My RPG - C++")
    {

        // Positionne la fenêtre à une position précise.
        m_window.setPosition({100, 100});

        // Limite le nombre d'images par seconde
        m_window.setFramerateLimit(60);
    }

    void Game::run()
    {
        sf::Clock clock;

        while (m_window.isOpen())
        {
            // Calcul du temps écoulé depuis la dernière frame
            const sf::Time elapsed = clock.restart();

            const float deltaTime = elapsed.asSeconds();

            processEvents();

            update(deltaTime);

            render();
        }
    }

    void Game::processEvents()
    {
        while (const std::optional event = m_window.pollEvent())
        {
            // Fermeture de la fenêtre
            if (event->is<sf::Event::Closed>())
            {
                m_window.close();
            }
        }
    }

    void Game::update(float deltaTime)
    {
        // Mise à jour du déplacement des entités.
        m_movementSystem.update(m_registry, deltaTime);
    }

    void Game::render()
    {
        // Nettoyage de l'image précédente
        m_window.clear(sf::Color(24, 28, 40));

        // Les éléments graphiques seront dessinés ici

        // Affichage de l'image
        m_window.display();
    }

} // namespace RPG::Core
