#pragma once

#include <memory>
#include <cstddef>
#include <utility>
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

#include "rpg/domain/ecs/View.hpp"
#include "rpg/domain/ecs/Entity.hpp"
#include "rpg/domain/ecs/EntityManager.hpp"
#include "rpg/domain/ecs/ComponentStorage.hpp"
#include "rpg/application/interfaces/IComponentStorage.hpp"

using namespace RPG::Application::Interfaces;

namespace RPG::Domain::ECS
{
    /**
     * @brief Point central de gestion des entités et des composants.
     *
     * Registry centralise la création et la destruction des entités,
     * ainsi que la gestion des différents stockages de composants.
     *
     * Chaque type de composant possède son propre ComponentStorage.
     * Les stockages sont identifiés dynamiquement à l'aide de std::type_index.
     *
     * Registry permet notamment de :
     * - Créer et détruire des entités.
     * - Ajouter des composants aux entités.
     * - Vérifier la présence d'un composant.
     * - Accéder aux composants d'une entité.
     * - Consulter le nombre de composants d'un type donné.
     */
    class Registry
    {
    public:
        /**
         * @brief Crée une nouvelle entité.
         *
         * Délègue la création à l'EntityManager.
         *
         * @return L'entité créée.
         */
        Entity createEntity()
        {
            return m_entityManager.createEntity();
        }

        /**
         * @brief Vérifie si une entité est valide.
         *
         * Une entité est valide si son index existe, si son emplacement
         * est occupé et si sa génération correspond à celle enregistrée.
         *
         * @param entity Entité à vérifier.
         * @return true si l'entité est valide, false sinon.
         */
        bool isEntityValid(Entity entity) const
        {
            return m_entityManager.isValid(entity);
        }

        /**
         * @brief Détruit une entité et supprime ses composants.
         *
         * Parcourt tous les stockages de composants afin de retirer
         * les composants appartenant à l'entité avant de la détruire
         * dans l'EntityManager.
         *
         * Une entité invalide ne peut pas être détruite.
         *
         * @param entity Entité à détruire.
         * @return true si l'entité a été détruite, false sinon.
         */
        bool destroyEntity(Entity entity)
        {
            if (!m_entityManager.isValid(entity))
                return false;

            for (auto &[type, storage] : m_componentStorages)
            {
                storage->remove(entity);
            }

            return m_entityManager.destroyEntity(entity);
        }

        /**
         * @brief Ajoute un composant à une entité.
         *
         * Récupère le stockage correspondant au type T ou le crée
         * s'il n'existe pas encore, puis y ajoute le composant.
         *
         * Une entité ne peut pas posséder deux composants du même type.
         *
         * @tparam T Type du composant.
         * @param entity Entité à laquelle ajouter le composant.
         * @param component Composant à ajouter.
         *
         * @throws std::invalid_argument Si l'entité n'est pas valide.
         * @throws std::runtime_error Si l'entité possède déjà ce composant.
         */
        template <typename T>
        void add(Entity entity, const T &component)
        {
            // Vérifier que l'entité existe.
            if (!isEntityValid(entity))
            {
                throw std::invalid_argument("Entity is not valid.");
            }

            // Récupérer ou créer le stockage correspondant au type T.
            ComponentStorage<T> &storage = getOrCreateStorage<T>();

            // Ajouter le composant dans ce stockage.
            storage.add(entity, component);
        }

        /**
         * @brief Vérifie si une entité possède un composant d'un type donné.
         *
         * Recherche le stockage correspondant au type T.
         * Si ce stockage n'existe pas, l'entité ne possède pas ce composant.
         *
         * @tparam T Type du composant recherché.
         * @param entity Entité à vérifier.
         * @return true si l'entité possède le composant, false sinon.
         */
        template <typename T>
        bool has(Entity entity) const
        {
            if (!isEntityValid(entity))
                return false;

            const std::type_index typeIndex(typeid(T));

            auto it = m_componentStorages.find(typeIndex);

            if (it == m_componentStorages.end())
                return false;

            const auto &storage =
                static_cast<const ComponentStorage<T> &>(*it->second);

            return storage.contains(entity);
        }

        /**
         * @brief Récupère une référence vers le composant d'une entité.
         *
         * Recherche le stockage correspondant au type T, puis récupère
         * le composant associé à l'entité.
         *
         * La référence retournée permet de consulter et de modifier
         * directement le composant stocké.
         *
         * @tparam T Type du composant recherché.
         * @param entity Entité dont on souhaite récupérer le composant.
         * @return Référence modifiable vers le composant.
         *
         * @throws std::invalid_argument Si l'entité n'est pas valide.
         * @throws std::out_of_range Si le stockage ou le composant est absent.
         */
        template <typename T>
        T &get(Entity entity)
        {
            if (!isEntityValid(entity))
                throw std::invalid_argument("Entity is not valid.");

            const std::type_index typeIndex(typeid(T));

            auto it = m_componentStorages.find(typeIndex);

            if (it == m_componentStorages.end())
                throw std::out_of_range("Component storage does not exist.");

            auto &storage =
                static_cast<ComponentStorage<T> &>(*it->second);

            return storage.get(entity);
        }

        /**
         * @brief Retourne le nombre de composants d'un type donné.
         *
         * Consulte le stockage associé au type T.
         * Si aucun stockage n'existe pour ce type, retourne zéro.
         *
         * Cette méthode compte les composants présents dans le stockage,
         * indépendamment de l'identité d'une entité particulière.
         *
         * @tparam T Type du composant à compter.
         * @return Nombre de composants de type T.
         */
        template <typename T>
        std::size_t componentCount() const
        {
            const std::type_index typeIndex(typeid(T));

            auto it = m_componentStorages.find(typeIndex);

            if (it == m_componentStorages.end())
                return 0;

            const auto &storage =
                static_cast<const ComponentStorage<T> &>(*it->second);

            return storage.size();
        }

        /**
         * @brief Crée une View contenant les entités possédant plusieurs composants.
         *
         * Utilise le stockage du premier type de composant comme point de départ,
         * puis vérifie que chaque entité possède également tous les autres
         * composants demandés.
         *
         * La View retournée est une snapshot : elle ne se met pas à jour
         * automatiquement lorsque le Registry est modifié.
         *
         * @tparam First Premier type de composant recherché.
         * @tparam Others Autres types de composants recherchés.
         * @return View contenant les entités correspondant à la requête.
         */
        template <typename First, typename... Others>
        View view() const
        {
            // Identifier le stockage du premier type de composant.
            const std::type_index typeIndex(typeid(First));

            // Rechercher ce stockage.
            auto it = m_componentStorages.find(typeIndex);

            // Aucun stockage : aucune entité ne peut correspondre.
            if (it == m_componentStorages.end())
                return View{};

            // Récupérer le stockage correspondant au premier composant.
            const auto &storage =
                static_cast<const ComponentStorage<First> &>(*it->second);

            // Préparer la collection des entités correspondantes.
            std::vector<Entity> entities;

            // Parcourir uniquement les entités du premier stockage.
            for (Entity entity : storage.entities())
            {
                // Vérifier que l'entité possède tous les autres composants.
                if (has<First>(entity) && (has<Others>(entity) && ...))
                {
                    entities.push_back(entity);
                }
            }

            // Construire et retourner la View.
            return View{std::move(entities)};
        }

    private:
        /**
         * @brief Gestionnaire interne des entités.
         *
         * Responsable de la création, de la destruction et de la validation
         * des entités, notamment grâce à leur index et leur génération.
         */
        EntityManager m_entityManager;

        /**
         * @brief Stockages de composants indexés par leur type.
         *
         * Chaque entrée associe :
         * - Une clé std::type_index identifiant un type de composant.
         * - Un pointeur unique vers son stockage.
         *
         * L'utilisation de IComponentStorage permet de stocker différents
         * types de ComponentStorage dans une même collection grâce
         * au polymorphisme.
         *
         * std::unique_ptr garantit que Registry possède les stockages
         * et gère automatiquement leur durée de vie.
         */
        std::unordered_map<
            std::type_index,
            std::unique_ptr<IComponentStorage>>
            m_componentStorages;

        /**
         * @brief Récupère un stockage existant ou en crée un nouveau.
         *
         * Recherche dans la collection le stockage correspondant au type T.
         * S'il existe, retourne une référence vers celui-ci.
         * Sinon, crée un ComponentStorage<T>, l'enregistre dans la collection
         * et retourne une référence vers le stockage créé.
         *
         * @tparam T Type des composants stockés.
         * @return Référence vers le stockage correspondant au type T.
         */
        template <typename T>
        ComponentStorage<T> &getOrCreateStorage()
        {
            // Identifier le type du composant.
            const std::type_index typeIndex(typeid(T));

            // Rechercher si un stockage existe déjà.
            auto it = m_componentStorages.find(typeIndex);

            // Si le stockage existe, le retourner.
            if (it != m_componentStorages.end())
            {
                return static_cast<ComponentStorage<T> &>(*it->second);
            }

            // Sinon, créer un nouveau stockage.
            auto storage = std::make_unique<ComponentStorage<T>>();

            // Conserver une référence vers le stockage créé.
            ComponentStorage<T> &storageReference = *storage;

            // Enregistrer le stockage dans la collection.
            m_componentStorages.emplace(typeIndex, std::move(storage));

            return storageReference;
        }
    };

} // namespace RPG::Domain::ECS
