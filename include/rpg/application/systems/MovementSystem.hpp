#pragma once

#include "rpg/domain/ecs/Registry.hpp"

using RPG::Domain::ECS::Registry;

namespace RPG::Application::Systems
{
    /**
     * @brief Gère le déplacement des entités.
     *
     * Le système met à jour les positions des entités
     * possédant les composants Position et Velocity.
     */
    class MovementSystem
    {
    public:
        /**
         * @brief Met à jour les positions des entités.
         *
         * @param registry Registry contenant les entités et composants.
         * @param deltaTime Temps écoulé depuis la dernière mise à jour.
         */
        void update(Registry &registry, float deltaTime) const;
    };
}
