#include "rpg/domain/ecs/EntityManager.hpp"

namespace RPG::Domain::ECS
{

    Entity EntityManager::createEntity()
    {
        // Si aucun emplacement disponlible,
        // créer un nouveau
        if (m_availableIndices.empty())
        {
            const std::uint32_t index = static_cast<std::uint32_t>(m_slots.size());

            m_slots.emplace_back();

            m_slots[index].alive = true;

            return Entity{
                .index = index,
                .generation = m_slots[index].generation};
        }

        // Sinon, réutiliser un emplacement libéré
        const std::uint32_t index = m_availableIndices.back();

        m_availableIndices.pop_back();

        Slot &slot = m_slots[index];

        slot.alive = true;

        return Entity{
            .index = index,
            .generation = slot.generation};
    }

    bool EntityManager::destroyEntity(Entity entity)
    {
        // Une entity déjà détruite ou inexiste
        // ne peut pas être détruite une seconde fois

        if (!isValid(entity))
        {
            return false;
        }

        Slot &slot = m_slots[entity.index];

        // l'emplacement devient disponible
        slot.alive = false;

        // changer la génération pour invalider
        // toutes les anciennes références à cette entité
        ++slot.generation;

        // conserver l'index pour le réutiliser
        m_availableIndices.push_back(entity.index);

        return true;
    }

    bool EntityManager::isValid(Entity entity) const
    {
        // l'index doit exister
        if (entity.index >= m_slots.size())
        {
            return false;
        }

        const Slot &slot = m_slots[entity.index];

        // L'emplacement doit être occupé et sa génération
        // doit correspondre à celle de l'entité.
        return slot.alive && slot.generation == entity.generation;
    }
}