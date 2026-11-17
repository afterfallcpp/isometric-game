#include <gtest/gtest.h>

#include <vector>

#include "rpg/ecs/Entity.hpp"
#include "rpg/ecs/Registry.hpp"
#include "rpg/components/Position.hpp"
#include "rpg/components/Velocity.hpp"
#include "rpg/components/Health.hpp"

using RPG::Components::Health;
using RPG::Components::Position;
using RPG::Components::Velocity;

namespace RPG::ECS
{
    TEST(RegistryTests, CreateEntityReturnsValidEntity)
    {
        // Arrange : créer un Registry.
        Registry registry;

        // Act : créer une entité.
        Entity entity = registry.createEntity();

        // Assert : vérifier que l'entité est valide.
        EXPECT_TRUE(registry.isEntityValid(entity));
    }

    TEST(RegistryTests, DestroyEntityMakesItInvalid)
    {
        // Arrange : créer un Registry et une entité.
        Registry registry;
        Entity entity = registry.createEntity();

        // Act : détruire l'entité.
        bool destroyed = registry.destroyEntity(entity);

        // Assert : vérifier sa destruction.
        EXPECT_TRUE(destroyed);
        EXPECT_FALSE(registry.isEntityValid(entity));
    }

    TEST(RegistryTests, DestroyingEntityTwiceReturnsFalse)
    {
        // Arrange : créer un Registry et une entité.
        Registry registry;
        Entity entity = registry.createEntity();

        // Act : détruire deux fois la même entité.
        bool firstAttempt = registry.destroyEntity(entity);
        bool secondAttempt = registry.destroyEntity(entity);

        // Assert : seule la première destruction doit réussir.
        EXPECT_TRUE(firstAttempt);
        EXPECT_FALSE(secondAttempt);
    }

    TEST(RegistryTests, AddComponentAllowsEntityToHaveComponent)
    {
        // Arrange : créer un Registry et une entité.
        Registry registry;
        Entity entity = registry.createEntity();

        // Act : ajouter un composant Position.
        registry.add<Position>(entity, Position{10.0f, 20.0f});

        // Assert : vérifier que l'entité possède ce composant.
        EXPECT_TRUE(registry.has<Position>(entity));
    }

    TEST(RegistryTests, AddComponentStoresCorrectData)
    {
        // Arrange : créer un Registry et une entité.
        Registry registry;
        Entity entity = registry.createEntity();

        // Act : ajouter un composant Position.
        registry.add<Position>(entity, Position{10.0f, 20.0f});

        // Assert : vérifier que les données sont conservées.
        Position &position = registry.get<Position>(entity);

        EXPECT_FLOAT_EQ(position.x, 10.0f);
        EXPECT_FLOAT_EQ(position.y, 20.0f);
    }

    TEST(RegistryTests, DestroyEntityRemovesItsComponents)
    {
        // Arrange
        Registry registry;

        Entity entity = registry.createEntity();
        registry.add<Position>(entity, Position{10.0f, 20.0f});

        EXPECT_EQ(registry.componentCount<Position>(), 1);

        // Act
        bool destroyed = registry.destroyEntity(entity);

        // Assert
        EXPECT_TRUE(destroyed);
        EXPECT_FALSE(registry.isEntityValid(entity));
        EXPECT_EQ(registry.componentCount<Position>(), 0);
    }

    TEST(RegistryTests, DestroyEntityRemovesAllComponents)
    {
        // Arrange : créer une entité avec plusieurs composants.
        Registry registry;
        Entity entity = registry.createEntity();

        registry.add<Position>(entity, Position{10.0f, 20.0f});
        registry.add<Health>(entity, Health{100});
        registry.add<Velocity>(entity, Velocity{5.0f, 2.0f});

        EXPECT_EQ(registry.componentCount<Position>(), 1);
        EXPECT_EQ(registry.componentCount<Health>(), 1);
        EXPECT_EQ(registry.componentCount<Velocity>(), 1);

        // Act : détruire l'entité.
        bool destroyed = registry.destroyEntity(entity);

        // Assert : vérifier que tous ses composants ont été supprimés.
        EXPECT_TRUE(destroyed);
        EXPECT_FALSE(registry.isEntityValid(entity));

        EXPECT_EQ(registry.componentCount<Position>(), 0);
        EXPECT_EQ(registry.componentCount<Health>(), 0);
        EXPECT_EQ(registry.componentCount<Velocity>(), 0);
    }

    TEST(RegistryTests, DestroyEntityDoesNotAffectOtherEntities)
    {
        // Arrange : créer deux entités.
        Registry registry;

        Entity firstEntity = registry.createEntity();
        Entity secondEntity = registry.createEntity();

        // Ajouter les mêmes types de composants aux deux entités.
        registry.add<Position>(firstEntity, Position{10.0f, 20.0f});
        registry.add<Position>(secondEntity, Position{30.0f, 40.0f});

        registry.add<Health>(firstEntity, Health{100});
        registry.add<Health>(secondEntity, Health{80});

        // Act : détruire uniquement la première entité.
        bool destroyed = registry.destroyEntity(firstEntity);

        // Assert : vérifier que la première entité est détruite.
        EXPECT_TRUE(destroyed);
        EXPECT_FALSE(registry.isEntityValid(firstEntity));

        // Vérifier que la deuxième entité est toujours valide.
        EXPECT_TRUE(registry.isEntityValid(secondEntity));

        // Vérifier que ses composants sont toujours présents.
        EXPECT_TRUE(registry.has<Position>(secondEntity));
        EXPECT_TRUE(registry.has<Health>(secondEntity));

        // Vérifier que les données sont intactes.
        EXPECT_FLOAT_EQ(registry.get<Position>(secondEntity).x, 30.0f);
        EXPECT_FLOAT_EQ(registry.get<Position>(secondEntity).y, 40.0f);
        EXPECT_EQ(registry.get<Health>(secondEntity).current, 80);

        // Vérifier le nombre de composants restants.
        EXPECT_EQ(registry.componentCount<Position>(), 1);
        EXPECT_EQ(registry.componentCount<Health>(), 1);
    }

    TEST(RegistryTests, ReusedEntityIndexDoesNotKeepOldComponents)
    {
        // Arrange : créer une première entité.
        Registry registry;

        Entity oldEntity = registry.createEntity();

        registry.add<Position>(
            oldEntity,
            Position{10.0f, 20.0f});

        // Act : détruire l'ancienne entité.
        bool destroyed = registry.destroyEntity(oldEntity);

        ASSERT_TRUE(destroyed);

        // Créer une nouvelle entité.
        Entity newEntity = registry.createEntity();

        // Assert : vérifier la réutilisation de l'index.
        EXPECT_EQ(oldEntity.index, newEntity.index);

        // Vérifier que la génération a changé.
        EXPECT_NE(oldEntity.generation, newEntity.generation);

        // Vérifier la validité des entités.
        EXPECT_FALSE(registry.isEntityValid(oldEntity));
        EXPECT_TRUE(registry.isEntityValid(newEntity));

        // La nouvelle entité ne doit pas récupérer l'ancien composant.
        EXPECT_FALSE(registry.has<Position>(newEntity));
        EXPECT_EQ(registry.componentCount<Position>(), 0);

        // Ajouter un nouveau composant.
        registry.add<Position>(
            newEntity,
            Position{50.0f, 60.0f});

        // Vérifier les nouvelles données.
        ASSERT_TRUE(registry.has<Position>(newEntity));

        EXPECT_FLOAT_EQ(registry.get<Position>(newEntity).x, 50.0f);
        EXPECT_FLOAT_EQ(registry.get<Position>(newEntity).y, 60.0f);

        EXPECT_EQ(registry.componentCount<Position>(), 1);
    }

    TEST(RegistryTests, ViewReturnsEntitiesWithAllRequestedComponents)
    {
        // Arrange : créer quatre entités.
        Registry registry;

        Entity firstEntity = registry.createEntity();
        Entity secondEntity = registry.createEntity();
        Entity thirdEntity = registry.createEntity();
        Entity fourthEntity = registry.createEntity();

        // Première entité : Position + Velocity.
        registry.add<Position>(firstEntity, Position{10.0f, 20.0f});
        registry.add<Velocity>(firstEntity, Velocity{1.0f, 2.0f});

        // Deuxième entité : Position uniquement.
        registry.add<Position>(secondEntity, Position{30.0f, 40.0f});

        // Troisième entité : Velocity uniquement.
        registry.add<Velocity>(thirdEntity, Velocity{3.0f, 4.0f});

        // Quatrième entité : Position + Velocity.
        registry.add<Position>(fourthEntity, Position{50.0f, 60.0f});
        registry.add<Velocity>(fourthEntity, Velocity{5.0f, 6.0f});

        // Act : récupérer les entités possédant Position et Velocity.
        View view = registry.view<Position, Velocity>();

        // Assert : seules les première et quatrième entités sont présentes.
        EXPECT_EQ(view.size(), 2);

        std::vector<Entity> result(view.begin(), view.end());

        ASSERT_EQ(result.size(), 2);
        EXPECT_EQ(result[0], firstEntity);
        EXPECT_EQ(result[1], fourthEntity);
    }

    TEST(RegistryTests, ViewReturnsEmptyWhenStorageDoesNotExist)
    {
        // Arrange
        Registry registry;

        // Act
        View view = registry.view<Health>();

        // Assert
        EXPECT_TRUE(view.empty());
        EXPECT_EQ(view.size(), 0);
    }

    TEST(RegistryTests, ViewReturnsEntitiesWithSingleComponent)
    {
        RPG::ECS::Registry registry;

        const auto entity1 = registry.createEntity();
        const auto entity2 = registry.createEntity();
        const auto entity3 = registry.createEntity();

        registry.add<Position>(entity1, Position{10.0f, 20.0f});
        registry.add<Position>(entity2, Position{30.0f, 40.0f});

        const auto view = registry.view<Position>();

        EXPECT_EQ(view.size(), 2);
        EXPECT_FALSE(view.empty());

        for (const auto entity : view)
        {
            EXPECT_TRUE(registry.has<Position>(entity));
        }

        EXPECT_FALSE(registry.has<Position>(entity3));
    }
}
