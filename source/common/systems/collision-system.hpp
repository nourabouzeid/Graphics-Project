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
    enum class CollisionSide {
        NONE,
        LEFT,
        RIGHT,
        FRONT,
        BACK
    };
    class CollisionSystem
    {
    public:


        CollisionSide checkCollision(Entity* player, Entity* entity)
        {
            auto aCollider = player->getComponent<CollisionComponent>();
            auto bCollider = entity->getComponent<CollisionComponent>();
            if (!aCollider || !bCollider)
                return CollisionSide::NONE;

            auto [playerMin, playerMax] = calculateBounds(player);
            auto [entityMin, entityMax] = calculateBounds(entity);

            if (playerMax.x <= entityMin.x || playerMin.x >= entityMax.x ||
                playerMax.y <= entityMin.y || playerMin.y >= entityMax.y ||
                playerMax.z <= entityMin.z || playerMin.z >= entityMax.z)
                return  CollisionSide::NONE;

            float xOverlap = std::min(playerMax.x, entityMax.x) - std::max(playerMin.x, entityMin.x);
            float yOverlap = std::min(playerMax.y, entityMax.y) - std::max(playerMin.y, entityMin.y);
            float zOverlap = std::min(playerMax.z, entityMax.z) - std::max(playerMin.z, entityMin.z);

            // Determine the smallest overlap (the side we're colliding with)
            if (xOverlap < zOverlap)
            {
                // Collision is primarily on X-axis (LEFT or RIGHT)
                if (player->localTransform.position.x < entity->localTransform.position.x)
                {
                    return CollisionSide::LEFT;
                }
                else
                {
                    return CollisionSide::RIGHT;
                }
            }
            else
            {
                // Collision is primarily on Z-axis (FRONT or BACK)
                if (player->localTransform.position.z < entity->localTransform.position.z)
                {
                    return CollisionSide::FRONT;
                }
                else
                {
                    return CollisionSide::BACK;
                }
            }
        }

        std::pair<glm::vec3, glm::vec3> calculateBounds(Entity* entity)
        {
            auto collider = entity->getComponent<CollisionComponent>();
            glm::vec3 pos = getWorldPosition(entity);
            glm::vec3 size = collider->size;

            glm::vec3 minBound, maxBound;

            switch (collider->anchor)
            {
            case CollisionComponent::Anchor::CENTER:
                minBound = pos - size * 0.5f;
                maxBound = pos + size * 0.5f;
                break;
            case CollisionComponent::Anchor::BOTTOM_CENTER:
                minBound = pos - glm::vec3(size.x * 0.5f, 0, size.z * 0.5f);
                maxBound = pos + glm::vec3(size.x * 0.5f, size.y, size.z * 0.5f);
                break;
            case CollisionComponent::Anchor::BOTTOM_LEFT:
                minBound = pos;
                maxBound = pos + size;
                break;
            case CollisionComponent::Anchor::BOTTOM_RIGHT:
                minBound = pos - glm::vec3(size.x, 0, 0);
                maxBound = pos + glm::vec3(0, size.y, size.z);
                break;
            case CollisionComponent::Anchor::TOP_LEFT:
                minBound = pos - glm::vec3(0, 0, size.z);
                maxBound = pos + glm::vec3(size.x, size.y, 0);
                break;
            case CollisionComponent::Anchor::TOP_RIGHT:
                minBound = pos - glm::vec3(size.x, size.y, size.z);
                maxBound = pos;
                break;
            default:
                minBound = pos - size * 0.5f;
                maxBound = pos + size * 0.5f;
                break;
            }

            return { minBound, maxBound };
        }

        glm::vec3 getWorldPosition(Entity* entity)
        {
            glm::vec3 pos = entity->localTransform.position;
            if (entity->parent)
            {
                pos += entity->parent->localTransform.position;
            }
            return pos;
        }
    };
} // namespace our
