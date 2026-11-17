#pragma once

#include "rpg/ecs/Entity.hpp"

namespace RPG::ECS
{
    /**
     * @brief Interface commune à tous les stockages de composants.
     *
     * Permet de manipuler différents ComponentStorage<T>
     * sans connaître leur type concret.
     *
     * Cette interface utilise le polymorphisme pour permettre
     * à Registry de regrouper plusieurs stockages de composants
     * de types différents dans une même collection.
     */
    class IComponentStorage
    {
    public:
        /**
         * @brief Destructeur virtuel.
         *
         * Garantit que le destructeur de la classe dérivée est appelé
         * correctement lorsqu'un objet est détruit à travers un pointeur
         * vers IComponentStorage.
         */
        virtual ~IComponentStorage() = default;

        /**
         * @brief Supprime le composant associé à une entité.
         *
         * Cette méthode est virtuelle pure : chaque classe dérivée
         * doit fournir sa propre implémentation.
         *
         * @param entity Entité dont le composant doit être supprimé.
         * @return true si le composant a été supprimé,
         *         false si l'entité ne possède pas ce composant.
         */
        virtual bool remove(Entity entity) = 0;
    };

} // namespace RPG::ECS
 