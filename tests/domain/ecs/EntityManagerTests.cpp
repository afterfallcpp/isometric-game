#include <gtest/gtest.h>

#include "rpg/domain/ecs/EntityManager.hpp"

namespace RPG::Domain::ECS
{
    TEST(EntityManagerTests, CreateEntityReturnsValidEntity)
    {
        EntityManager manager;

        Entity entity = manager.createEntity();

        EXPECT_TRUE(manager.isValid(entity));
    }

    TEST(EntityManagerTests, CreateTwoEntitiesReturnsDifferentEntities)
    {
        EntityManager manager;

        Entity first = manager.createEntity();
        Entity second = manager.createEntity();

        EXPECT_NE(first, second);
    }

    TEST(EntityManagerTests, DestroyEntityMakesItInvalid)
    {
        EntityManager manager;

        Entity entity = manager.createEntity();

        bool destroyed = manager.destroyEntity(entity);

        EXPECT_TRUE(destroyed);
        EXPECT_FALSE(manager.isValid(entity));
    }

    TEST(EntityManagerTests, DestroyingEntityTwiceReturnsFalse)
    {
        EntityManager manager;

        Entity entity = manager.createEntity();

        EXPECT_TRUE(manager.destroyEntity(entity));
        EXPECT_FALSE(manager.destroyEntity(entity));
    }

    TEST(EntityManagerTests, ReusedEntityHasNewGeneration)
    {
        EntityManager manager;

        Entity oldEntity = manager.createEntity();

        manager.destroyEntity(oldEntity);

        Entity newEntity = manager.createEntity();

        EXPECT_EQ(oldEntity.index, newEntity.index);
        EXPECT_NE(oldEntity.generation, newEntity.generation);

        EXPECT_FALSE(manager.isValid(oldEntity));
        EXPECT_TRUE(manager.isValid(newEntity));
    }
}