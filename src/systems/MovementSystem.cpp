#include "rpg/systems/MovementSystem.hpp"

#include "rpg/components/Position.hpp"
#include "rpg/components/Velocity.hpp"

using RPG::Components::Position;
using RPG::Components::Velocity;

namespace RPG::Systems
{
    void MovementSystem::update(RPG::ECS::Registry &registry, float deltaTime) const
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
