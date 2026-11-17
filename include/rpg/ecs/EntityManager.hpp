#pragma once

#include <cstdint>
#include <vector>

#include "rpg/ecs/Entity.hpp"

namespace RPG::ECS
{
    /**
     * @brief Gère le cycle de vie des entités ECS.
     *
     * Responsable de la création, de la destruction
     * et de la validation des entités.
     */
    class EntityManager
    {
    public:
        /**
         * @brief Crée une nouvelle entité.
         */
        Entity createEntity();

        /**
         * @brief Détruit une entité existante.
         *
         * @return true si l'entité a été détruite.
         */
        bool destroyEntity(Entity entity);

        /**
         * @brief Vérifie si une entité est toujours valide.
         */
        bool isValid(Entity entity) const;

    private:
        /**
         * @brief Informations internes d'un emplacement.
         */
        struct Slot
        {
            std::uint32_t generation{0};
            bool alive{false};
        };

        // Emplacements utilisés pour stocker les entités.
        std::vector<Slot> m_slots;

        // Indices des emplacements disponibles pour réutilisation.
        std::vector<std::uint32_t> m_availableIndices;
    };
}