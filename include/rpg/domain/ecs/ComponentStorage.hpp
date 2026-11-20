#pragma once

#include <vector>
#include <limits>
#include <utility>
#include <cstddef>
#include <stdexcept>

#include "rpg/domain/ecs/Entity.hpp"
#include "rpg/application/interfaces/IComponentStorage.hpp"

using namespace RPG::Application::Interfaces;

namespace RPG::Domain::ECS
{
    /**
     * @brief Stocke les composants d'un type donné.
     *
     * @tparam T Type du composant stocké.
     */
    template <typename T>
    class ComponentStorage : public IComponentStorage
    {
    public:
        /**
         * @brief Retourne le nombre de composants stockés.
         */
        std::size_t size() const
        {
            return m_dense.size();
        }

        /**
         * @brief Retourne les entités présentes dans le stockage.
         *
         * Les entités sont conservées dans le même ordre que les composants
         * du stockage dense.
         *
         * Cette méthode permet notamment aux Views de parcourir les entités
         * possédant un composant de type T.
         *
         * @return Référence constante vers le tableau des entités.
         */
        const std::vector<Entity> &entities() const
        {
            return m_entities;
        }

        /**
         * @brief Vérifie si une entité possède ce composant.
         */
        bool contains(Entity entity) const
        {
            // Vérifier que l'index existe dans sparse.
            if (entity.index >= m_sparse.size())
            {
                return false;
            }

            const std::size_t denseIndex = m_sparse[entity.index];

            // Vérifier qu'un composant est associé à cet index.
            if (denseIndex == INVALID_INDEX)
            {
                return false;
            }

            // Vérifier l'identité complète, génération comprise.
            return m_entities[denseIndex] == entity;
        }

        /**
         * @brief Ajoute un composant à une entité.
         */
        void add(Entity entity, const T &component)
        {
            // Agrandir sparse si l'index est hors limites.
            if (entity.index >= m_sparse.size())
            {
                m_sparse.resize(entity.index + 1, INVALID_INDEX);
            }

            // Empêcher l'ajout d'un doublon.
            if (contains(entity))
            {
                throw std::runtime_error(
                    "Entity already has this component.");
            }

            // Déterminer la prochaine position disponible.
            const std::size_t denseIndex = m_dense.size();

            // Enregistrer la correspondance dans sparse.
            m_sparse[entity.index] = denseIndex;

            // Stocker les données et l'identité.
            m_dense.push_back(component);
            m_entities.push_back(entity);
        }

        /**
         * @brief Récupère un composant associé à une entité.
         *
         * @param entity Entité dont on veut récupérer le composant.
         * @return Référence modifiable vers le composant.
         *
         * @throws std::out_of_range Si l'entité ne possède pas ce composant.
         */
        T &get(Entity entity)
        {
            // Vérifier que l'entité possède ce composant.
            if (!contains(entity))
            {
                throw std::out_of_range("Entity does not have this component.");
            }

            // Récupérer la position du composant dans dense.
            const std::size_t denseIndex = m_sparse[entity.index];

            // Retourner une référence vers le composant stocké.
            return m_dense[denseIndex];
        }

        /**
         * @brief Supprime le composant associé à une entité.
         *
         * @param entity Entité dont on veut supprimer le composant.
         * @return true si le composant a été supprimé, false sinon.
         */
        bool remove(Entity entity) override
        {
            // Vérifier que l'entité possède ce composant.
            if (!contains(entity))
            {
                return false;
            }

            // Position du composant à supprimer.
            const std::size_t denseIndex = m_sparse[entity.index];

            // Position du dernier composant.
            const std::size_t lastIndex = m_dense.size() - 1;

            // Si le composant à supprimer n'est pas le dernier.
            if (denseIndex != lastIndex)
            {
                // Déplacer le dernier composant à sa place.
                m_dense[denseIndex] = std::move(m_dense[lastIndex]);

                // Déplacer également l'identité de son entité.
                m_entities[denseIndex] = m_entities[lastIndex];

                // Mettre à jour la correspondance sparse.
                m_sparse[m_entities[denseIndex].index] = denseIndex;
            }

            // Supprimer le dernier composant et son identité.
            m_dense.pop_back();
            m_entities.pop_back();

            // Invalider la correspondance de l'entité supprimée.
            m_sparse[entity.index] = INVALID_INDEX;

            return true;
        }

    private:
        // Associe l'index d'une entité à sa position dense.
        std::vector<std::size_t> m_sparse;

        // Stocke les composants de manière compacte.
        std::vector<T> m_dense;

        // Conserve les identités complètes des entités.
        std::vector<Entity> m_entities;

        // Valeur indiquant qu'aucun composant n'est associé.
        static constexpr std::size_t INVALID_INDEX =
            std::numeric_limits<std::size_t>::max();
    };

} // namespace RPG::Domain::ECS
