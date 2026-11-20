#include <gtest/gtest.h>

#include <stdexcept>

#include "rpg/domain/ecs/ComponentStorage.hpp"

namespace RPG::Domain::ECS
{
    // Composant temporaire utilisé pour nos tests.
    struct Position
    {
        float x;
        float y;
    };

    TEST(ComponentStorageTests, NewStorageIsEmpty)
    {
        // Arrange : créer un stockage vide.
        ComponentStorage<Position> storage;

        // Assert : vérifier qu'il ne contient aucun composant.
        EXPECT_EQ(storage.size(), 0);
    }

    TEST(ComponentStorageTests, AddComponentIncreasesSize)
    {
        // Arrange
        ComponentStorage<Position> storage;
        Entity entity{0, 0};

        // Act
        storage.add(entity, Position{10.0f, 20.0f});

        // Assert
        EXPECT_EQ(storage.size(), 1);
    }

    TEST(ComponentStorageTests, ContainsReturnsTrueForStoredEntity)
    {
        // Arrange
        ComponentStorage<Position> storage;
        Entity entity{0, 0};

        // Act
        storage.add(entity, Position{10.0f, 20.0f});

        // Assert
        EXPECT_TRUE(storage.contains(entity));
    }

    TEST(ComponentStorageTests, ContainsReturnsFalseForDifferentGeneration)
    {
        // Arrange
        ComponentStorage<Position> storage;

        Entity oldEntity{0, 0};
        Entity newEntity{0, 1};

        // Act
        storage.add(oldEntity, Position{10.0f, 20.0f});

        // Assert
        EXPECT_TRUE(storage.contains(oldEntity));
        EXPECT_FALSE(storage.contains(newEntity));
    }

    TEST(ComponentStorageTests, GetComponentReturnsStoredData)
    {
        // Arrange
        ComponentStorage<Position> storage;
        Entity entity{0, 0};

        storage.add(entity, Position{10.0f, 20.0f});

        // Act
        Position &position = storage.get(entity);

        // Assert
        EXPECT_FLOAT_EQ(position.x, 10.0f);
        EXPECT_FLOAT_EQ(position.y, 20.0f);
    }

    TEST(ComponentStorageTests, GetComponentReturnsModifiableReference)
    {
        // Arrange
        ComponentStorage<Position> storage;
        Entity entity{0, 0};

        storage.add(entity, Position{10.0f, 20.0f});

        // Act
        storage.get(entity).x = 50.0f;

        // Assert
        EXPECT_FLOAT_EQ(storage.get(entity).x, 50.0f);
    }

    TEST(ComponentStorageTests, GetComponentThrowsWhenEntityDoesNotHaveComponent)
    {
        // Arrange
        ComponentStorage<Position> storage;
        Entity entity{0, 0};

        // Assert
        EXPECT_THROW(
            storage.get(entity),
            std::out_of_range);
    }

    TEST(ComponentStorageTests, RemoveComponentDecreasesSize)
    {
        // Arrange
        ComponentStorage<Position> storage;
        Entity entity{0, 0};

        storage.add(entity, Position{10.0f, 20.0f});

        // Act
        bool removed = storage.remove(entity);

        // Assert
        EXPECT_TRUE(removed);
        EXPECT_EQ(storage.size(), 0);
        EXPECT_FALSE(storage.contains(entity));
    }

    TEST(ComponentStorageTests, RemoveMiddleComponentKeepsOtherComponentsAccessible)
    {
        // Arrange : créer trois entités et leurs composants.
        ComponentStorage<Position> storage;

        Entity first{0, 0};
        Entity middle{1, 0};
        Entity last{2, 0};

        storage.add(first, Position{10.0f, 20.0f});
        storage.add(middle, Position{30.0f, 40.0f});
        storage.add(last, Position{50.0f, 60.0f});

        // Act : supprimer le composant du milieu.
        bool removed = storage.remove(middle);

        // Assert : vérifier la suppression.
        EXPECT_TRUE(removed);
        EXPECT_EQ(storage.size(), 2);

        EXPECT_FALSE(storage.contains(middle));

        // Vérifier que les autres composants sont toujours accessibles.
        EXPECT_TRUE(storage.contains(first));
        EXPECT_TRUE(storage.contains(last));

        EXPECT_FLOAT_EQ(storage.get(first).x, 10.0f);
        EXPECT_FLOAT_EQ(storage.get(last).x, 50.0f);
    }

    TEST(ComponentStorageTests, RemoveReturnsFalseWhenEntityDoesNotHaveComponent)
    {
        // Arrange : créer un stockage vide.
        ComponentStorage<Position> storage;
        Entity entity{0, 0};

        // Act : tenter de supprimer un composant inexistant.
        bool removed = storage.remove(entity);

        // Assert : vérifier que la suppression échoue.
        EXPECT_FALSE(removed);
        EXPECT_EQ(storage.size(), 0);
        EXPECT_FALSE(storage.contains(entity));
    }

    TEST(ComponentStorageTests, AddComponentThrowsWhenEntityAlreadyHasComponent)
    {
        // Arrange
        ComponentStorage<Position> storage;
        Entity entity{0, 0};

        storage.add(entity, Position{10.0f, 20.0f});

        // Act & Assert
        EXPECT_THROW(
            storage.add(entity, Position{30.0f, 40.0f}),
            std::runtime_error);

        // Vérifier que les données originales sont conservées.
        EXPECT_EQ(storage.size(), 1);
        EXPECT_FLOAT_EQ(storage.get(entity).x, 10.0f);
        EXPECT_FLOAT_EQ(storage.get(entity).y, 20.0f);
    }

    TEST(ComponentStorageTests, ReusedEntityIndexWithNewGenerationCanStoreComponent)
    {
        // Arrange
        ComponentStorage<Position> storage;

        Entity oldEntity{0, 0};
        Entity newEntity{0, 1};

        storage.add(oldEntity, Position{10.0f, 20.0f});

        // Act
        storage.remove(oldEntity);
        storage.add(newEntity, Position{30.0f, 40.0f});

        // Assert
        EXPECT_FALSE(storage.contains(oldEntity));
        EXPECT_TRUE(storage.contains(newEntity));

        EXPECT_EQ(storage.size(), 1);

        EXPECT_FLOAT_EQ(storage.get(newEntity).x, 30.0f);
        EXPECT_FLOAT_EQ(storage.get(newEntity).y, 40.0f);
    }

    TEST(ComponentStorageTests, EntitiesReturnsStoredEntities)
    {
        // Arrange
        ComponentStorage<Position> storage;

        Entity first{0, 0};
        Entity second{1, 0};

        storage.add(first, Position{10.0f, 20.0f});
        storage.add(second, Position{30.0f, 40.0f});

        // Act
        const std::vector<Entity> &entities = storage.entities();

        // Assert
        ASSERT_EQ(entities.size(), 2);

        EXPECT_EQ(entities[0], first);
        EXPECT_EQ(entities[1], second);
    }

} // namespace RPG::ECS