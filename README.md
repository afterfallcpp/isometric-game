# RPG Game — C++ Game Development Lab

A sandbox project dedicated to learning modern C++ and video game development through practical experimentation.

`rpg-game` is an evolving technical playground where concepts, architectures, gameplay systems and development practices are explored, implemented, tested and improved.

The primary goal is not to create a finished commercial game, but to build a solid understanding of how video games work internally and how to design maintainable game systems in C++.

---

## 🎯 Project Goals

This project serves as a learning environment for exploring the fundamentals of video game development.

Main objectives:

- Learn modern C++ through practical game development.
- Understand game architecture and engine fundamentals.
- Explore Entity Component System (ECS) architecture.
- Develop reusable gameplay systems.
- Understand memory management, performance and data-oriented design.
- Learn how game loops, entities, components and systems interact.
- Apply software engineering principles to game development.
- Introduce unit testing and Test-Driven Development (TDD) where relevant.
- Experiment with different technical approaches without the constraints of a production game.

---

## 🏗️ Architecture

The project explores an **Entity Component System (ECS)** architecture.

ECS is an architectural pattern commonly used in game development to separate game data from game logic.

Instead of creating complex inheritance hierarchies for every type of game object, ECS divides the game into three fundamental concepts.

### Entity

An Entity represents an individual object in the game world.

Examples:
- Player
- Zombie
- NPC
- Projectile
- Item
- Environment object

An Entity is essentially an identifier. It does not contain gameplay logic itself.

### Component

A Component represents a specific piece of data associated with an Entity.

Examples:

| Component | Responsibility |
|---|---|
| Transform | Position, rotation and scale |
| Velocity | Movement velocity |
| Health | Current and maximum health |
| Sprite | Visual representation |
| Collider | Collision information |
| Input | Player input data |

Components are designed to remain focused on storing data rather than implementing gameplay behavior.

### System

A System implements gameplay logic by processing Entities that possess the required Components.

Examples:

| System | Responsibility |
|---|---|
| MovementSystem | Updates entity positions |
| RenderSystem | Handles visual representation |
| CollisionSystem | Detects collisions |
| HealthSystem | Manages health and damage |
| InputSystem | Processes player input |
| AISystem | Controls artificial intelligence |

Systems operate on Components rather than relying on complex object inheritance.

### ECS Overview

```text
                 ECS ARCHITECTURE
                       |
          +------------+------------+
          |            |            |
       ENTITY      COMPONENT      SYSTEM
          |            |            |
       Entity ID    Transform    Movement
       Entity ID    Velocity     Rendering
       Entity ID    Health       Collision
       Entity ID    Sprite       Health
          |            |            |
          +------------+------------+
                       |
                 GAME WORLD
```

The objective is to understand how these three elements interact and how an ECS architecture can support scalable and maintainable gameplay systems.

---

## 📁 Project Structure

The project structure evolves as new concepts are introduced.

```text
rpg-game/
│
├── CMakeLists.txt
├── CMakePresets.json
├── .gitignore
├── README.md
│
├── include/
│   └── ...
│
├── src/
│   ├── ecs/
│   │   ├── EntityManager.cpp
│   │   ├── ComponentStorage.cpp
│   │   └── Registry.cpp
│   │
│   ├── systems/
│   │   └── MovementSystem.cpp
│   │
│   └── ...
│
├── tests/
│   ├── ecs/
│   │   ├── EntityManagerTests.cpp
│   │   ├── ComponentStorageTests.cpp
│   │   └── RegistryTests.cpp
│   │
│   └── systems/
│       └── MovementSystemTests.cpp
│
└── build/
    └── ...
```

*Note: This is an illustrative structure. The actual implementation may evolve as the project grows.*

---

## 🧪 Testing

Testing is an important part of the learning process.

Each significant system should have appropriate tests to validate its behavior and prevent regressions.

Areas of focus:

- Entity creation and destruction.
- Component registration and storage.
- Entity-component relationships.
- System execution.
- Movement and gameplay logic.
- Edge cases and invalid states.

Test-Driven Development (TDD) will be introduced when appropriate:

1. Write a test describing the expected behavior.
2. Implement the minimum functionality required.
3. Run the tests.
4. Refactor the implementation.
5. Verify that everything still works.

The objective is to understand not only how to implement a feature, but also how to verify that it works correctly.

---

## 🛠️ Technology Stack

| Technology | Purpose |
|---|---|
| C++ | Main programming language |
| CMake | Build system |
| GCC / MinGW | C++ compiler |
| MSYS2 | Windows development environment |
| Git | Version control |
| CTest | Test execution |

Additional libraries and tools may be introduced as the project explores graphics, audio, input handling, physics and other game development fundamentals.

---

## 🚀 Development Philosophy

This project follows a progressive, learning-oriented approach.

Rather than implementing everything at once, each concept is introduced, understood, implemented and tested before moving forward.

Core principles:

- **Understand before implementing:** learn the underlying concepts rather than blindly copying code.
- **Keep things simple:** avoid unnecessary abstractions and premature optimization.
- **Build incrementally:** introduce new features through small, manageable steps.
- **Test important behavior:** validate systems and identify regressions early.
- **Experiment freely:** compare different approaches and understand their trade-offs.
- **Write maintainable code:** prioritize readability, modularity and clear responsibilities.

DDD, SOLID, clean architecture and data-oriented design may be explored whenever they provide practical value.

---

## 🎮 Long-Term Vision

`rpg-game` is intended to become a technical foundation for understanding and experimenting with increasingly complex game development concepts.

Potential areas of exploration:

- Entity Component System architecture.
- Game loops and time management.
- Input handling.
- 2D and 3D rendering.
- Animation systems.
- Collision detection and physics.
- Artificial intelligence and pathfinding.
- Inventory and equipment systems.
- Combat and gameplay mechanics.
- Resource and asset management.
- Save/load systems.
- Performance optimization.
- Multithreading.

The knowledge and experience gained through this project will also contribute to more ambitious game development projects.

---

## 📌 Project Status

<sub>Active learning project — continuously evolving.</sub>

This repository is a development sandbox. Features, architecture and implementation details may change as new concepts are explored.

**The objective is to learn how to build video games in C++, from the fundamentals to more advanced engine and gameplay architecture.**