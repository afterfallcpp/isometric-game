
#pragma once

#include <vector>

#include "rpg/ecs/Entity.hpp"

namespace RPG::ECS
{
    /**
     * @brief Représente une collection d'entités correspondant à une requête ECS.
     *
     * View permet de parcourir les entités sélectionnées par le Registry.
     *
     * Cette première implémentation utilise une snapshot :
     * les entités sont copiées dans une collection lors de la création
     * de la View. Les modifications ultérieures du Registry ne mettent
     * pas automatiquement cette collection à jour.
     *
     * La View expose uniquement des itérateurs constants afin de ne pas
     * permettre la modification directe de sa collection d'entités.
     */
    class View
    {
    public:
        /**
         * @brief Type d'itérateur constant sur les entités.
         */
        using const_iterator = std::vector<Entity>::const_iterator;

        /**
         * @brief Construit une View vide.
         *
         * Permet de représenter une requête ne retournant aucune entité.
         */
        View() = default;

        /**
         * @brief Construit une View à partir d'une collection d'entités.
         *
         * @param entities Collection des entités correspondant à la requête.
         */
        explicit View(std::vector<Entity> entities)
            : m_entities(std::move(entities))
        {
        }

        /**
         * @brief Retourne un itérateur constant vers la première entité.
         *
         * @return Itérateur constant de début.
         */
        const_iterator begin() const
        {
            return m_entities.begin();
        }

        /**
         * @brief Retourne un itérateur constant situé après la dernière entité.
         *
         * @return Itérateur constant de fin.
         */
        const_iterator end() const
        {
            return m_entities.end();
        }

        /**
         * @brief Retourne le nombre d'entités dans la View.
         *
         * @return Nombre d'entités.
         */
        std::size_t size() const
        {
            return m_entities.size();
        }

        /**
         * @brief Vérifie si la View ne contient aucune entité.
         *
         * @return true si la View est vide, false sinon.
         */
        bool empty() const
        {
            return m_entities.empty();
        }

    private:
        /**
         * @brief Snapshot des entités correspondant à la requête.
         */
        std::vector<Entity> m_entities;
    };

} // namespace RPG::ECS
