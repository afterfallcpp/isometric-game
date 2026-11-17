#include <gtest/gtest.h>

#include "rpg/ecs/Registry.hpp"
#include "rpg/components/Position.hpp"
#include "rpg/components/Velocity.hpp"

#include "rpg/systems/MovementSystem.hpp"

using RPG::Components::Position;
using RPG::Components::Velocity;
using RPG::ECS::Registry;
using RPG::Systems::MovementSystem;

namespace RPG::Systems
{
    TEST(MovementSystemTests, UpdateMovesEntityAccordingToVelocity)
    {
        // Arrange : préparer les données
        Registry registry;
        MovementSystem movementSystem;

        const auto entity = registry.createEntity();

        registry.add<Position>(entity, Position{10.0f, 20.0f});
        registry.add<Velocity>(entity, Velocity{5.0f, 3.0f});

        // Act : mettre à jour le système pendant une seconde
        movementSystem.update(registry, 1.0f);

        // Assert : vérifier la nouvelle position
        const auto &position = registry.get<Position>(entity);

        EXPECT_FLOAT_EQ(position.x, 15.0f);
        EXPECT_FLOAT_EQ(position.y, 23.0f);
    }

    TEST(MovementSystemTests, UpdateDoesNotMoveEntityWithoutVelocity)
    {
        Registry registry;
        MovementSystem movementSystem;

        const auto entity = registry.createEntity();

        registry.add<Position>(entity, Position{10.0f, 20.0f});

        movementSystem.update(registry, 1.0f);

        const auto &position = registry.get<Position>(entity);

        EXPECT_FLOAT_EQ(position.x, 10.0f);
        EXPECT_FLOAT_EQ(position.y, 20.0f);
    }

    TEST(MovementSystemTests, UpdateMovesAllMatchingEntities)
    {
        Registry registry;
        MovementSystem movementSystem;

        const auto entity1 = registry.createEntity();
        const auto entity2 = registry.createEntity();
        const auto entity3 = registry.createEntity();

        registry.add<Position>(entity1, Position{10.0f, 20.0f});
        registry.add<Velocity>(entity1, Velocity{4.0f, 2.0f});

        registry.add<Position>(entity2, Position{50.0f, 50.0f});

        registry.add<Position>(entity3, Position{0.0f, 0.0f});
        registry.add<Velocity>(entity3, Velocity{-2.0f, 6.0f});

        movementSystem.update(registry, 0.5f);

        const auto &position1 = registry.get<Position>(entity1);
        const auto &position2 = registry.get<Position>(entity2);
        const auto &position3 = registry.get<Position>(entity3);

        EXPECT_FLOAT_EQ(position1.x, 12.0f);
        EXPECT_FLOAT_EQ(position1.y, 21.0f);

        EXPECT_FLOAT_EQ(position2.x, 50.0f);
        EXPECT_FLOAT_EQ(position2.y, 50.0f);

        EXPECT_FLOAT_EQ(position3.x, -1.0f);
        EXPECT_FLOAT_EQ(position3.y, 3.0f);
    }

}
