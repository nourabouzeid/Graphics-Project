#pragma once

#include "../application.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/free-movement.hpp"
#include <glm/glm.hpp>
#include <iostream>

namespace our
{
    class CollisionSystem
    {
    public:
        void update(World *world, float deltaTime)
        {
            Entity *player = findPlayer(world);
            if (!player)
                return;

            for (auto entity : world->getEntities())
            {
                if (entity != player && checkCollision(player, entity))
                {
                    handleCollision(player, entity);
                }
            }
        }

    private:
        Entity *findPlayer(World *world)
        {
            for (auto entity : world->getEntities())
            {
                if (entity->getComponent<FreeMovementComponent>())
                {
                    return entity;
                }
            }
            return nullptr;
        }

        bool checkCollision(Entity *a, Entity *b)
        {
            auto aCollider = a->getComponent<CollisionComponent>();
            auto bCollider = b->getComponent<CollisionComponent>();
            if (!aCollider || !bCollider)
                return false;

            auto [aMin, aMax] = calculateBounds(a);
            auto [bMin, bMax] = calculateBounds(b);

            // Check collision in XZ plane (ignoring Y axis)
            return aMax.x > bMin.x && aMin.x < bMax.x &&
                   aMax.z > bMin.z && aMin.z < bMax.z;
        }

        std::pair<glm::vec3, glm::vec3> calculateBounds(Entity *entity)
        {
            auto collider = entity->getComponent<CollisionComponent>();
            glm::vec3 position = getWorldPosition(entity);
            glm::vec3 size = {collider->size.x, 0.0f, collider->size.y}; // Using x/z for 2D plane

            switch (collider->anchor)
            {
            case CollisionComponent::Anchor::CENTER:
                return {
                    position - size / 2.0f,
                    position + size / 2.0f};
            case CollisionComponent::Anchor::BOTTOM_LEFT:
                return {
                    position,
                    position + size};
            case CollisionComponent::Anchor::BOTTOM_RIGHT:
                return {
                    {position.x - size.x, position.y, position.z},
                    {position.x, position.y, position.z + size.z}};
            case CollisionComponent::Anchor::TOP_LEFT:
                return {
                    {position.x, position.y, position.z - size.z},
                    {position.x + size.x, position.y, position.z}};
            case CollisionComponent::Anchor::TOP_RIGHT:
                return {
                    position - size,
                    position};
            default:
                return {position, position};
            }
        }

        glm::vec3 getWorldPosition(Entity *entity)
        {
            glm::vec3 pos = entity->localTransform.position;
            if (entity->parent)
            {
                pos += entity->parent->localTransform.position;
            }
            return pos; // Return full 3D position
        }

        void handleCollision(Entity *player, Entity *other)
        {
            auto [min, max] = calculateBounds(other);
            glm::vec3 center = (min + max) / 2.0f;

            std::cout << "XZ Plane collision with " << other->name
                      << " at (" << center.x << ", " << center.z << ")"
                      << " - Bounds X:(" << min.x << "," << max.x << ")"
                      << " Z:(" << min.z << "," << max.z << ")" << std::endl;

            if (other->name == "box")
            {
                // Push logic in XZ plane
            }
            else if (other->name == "trap")
            {
                // Trap activation in XZ plane
            }
        }
    };
} // namespace our