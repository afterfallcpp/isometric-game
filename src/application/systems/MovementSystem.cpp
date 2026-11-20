#include "rpg/application/systems/MovementSystem.hpp"

#include "rpg/application/components/Position.hpp"
#include "rpg/application/components/Velocity.hpp"

using RPG::Application::Components::Position;
using RPG::Application::Components::Velocity;
using RPG::Application::Systems::MovementSystem;
using RPG::Domain::ECS::Registry;

namespace RPG::Application::Systems
{
    void MovementSystem::update(Registry &registry, float deltaTime) const
    {
        // Récupère toutes les entités possédant Position et Velocity.
        const auto entities = registry.view<Position, Velocity>();

        // Parcourt chaque entité sélectionnée.
        for (const auto entity : entities)
        {
            auto &position = registry.get<Position>(entity);

            const auto &velocity = registry.get<Velocity>(entity);

            // Applique le déplacement en fonction du temps écoulé.
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
        }
    }
}
