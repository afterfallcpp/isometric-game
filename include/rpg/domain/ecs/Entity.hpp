#pragma once

#include <cstdint>

namespace RPG::Domain::ECS
{
    /**
     * @brief Identifie une entité dans notre ECS.
     *
     * Une entité est identifiée par :
     * - Un index dans le gestionnaire.
     * - Une génération permettant de distinguer
     *   les différentes utilisations de cet index.
     */
    struct Entity
    {
        std::uint32_t index;
        std::uint32_t generation;

        // permet de comparer deux entités
        bool operator==(const Entity &other) const
        {
            return index == other.index && generation == other.generation;
        }
    };

} // namespace RPG::ECS
