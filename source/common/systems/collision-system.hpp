#pragma once

#include "../application.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/free-movement.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include "forward-renderer.hpp"

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
    private:
        std::unordered_map<Entity*, bool> boxLanded;
        ForwardRenderer* forwardRenderer;

    public:
        void setup(ForwardRenderer* forwardRenderer) {
            this->forwardRenderer = forwardRenderer;
        }
        void update(World* world, float deltaTime)
        {
            Entity* player = findPlayer(world);
            if (!player)
                return;

            for (auto entity : world->getEntities())
            {
                if (entity != player)
                {
                    CollisionSide side = checkCollision(player, entity);
                    if (side != CollisionSide::NONE)
                    {
                        handleCollision(player, entity, side, deltaTime);
                    }
                }
            }
            cleanupBoxStates(world);

            // handleBoxLandCollision(world);
        }

        void handleBoxLandCollision(World* world)
        {
            std::vector<Entity*> boxes;
            std::vector<Entity*> grounds;

            // Gather all boxes and grounds
            for (auto entity : world->getEntities())
            {
                if (entity->name == "box")
                {
                    boxes.push_back(entity);
                }
                else if (entity->name == "groundEarth")
                {
                    grounds.push_back(entity);
                }
            }

            // For each box
            for (auto box : boxes)
            {
                bool intersectsGround = false;

                // Check if box intersects with any ground
                for (auto ground : grounds)
                {
                    if (checkCollision(box, ground) != CollisionSide::NONE)
                    {
                        intersectsGround = true;
                        break;
                    }
                }

                // If not intersecting and hasn't already dropped
                if (!intersectsGround && !boxLanded[box])
                {
                    // std::cout << "Box is floating — applying gravity effect once!\n";
                    box->localTransform.position.y -= 1.0f; // Decrease Y once
                    boxLanded[box] = true;
                }
            }
        }

        void cleanupBoxStates(World* world)
        {
            for (auto it = boxLanded.begin(); it != boxLanded.end();)
            {
                if (std::find(world->getEntities().begin(), world->getEntities().end(), it->first) == world->getEntities().end())
                {
                    it = boxLanded.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        Entity* findPlayer(World* world)
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


        void handleCollision(Entity* player, Entity* other, CollisionSide side, float deltaTime)
        {


            if (other->name == "trap")
            {
                onHitTrap();
                forwardRenderer->activatePostProcess(0.5);
                // std::cout << "Player collided with trap!" << std::endl;
            }
            else if (other->name == "groundEarth")
            {
                // Enemy logic here
                // std::cout << "Player collided with groundEarth!" << std::endl;
            }
        }
    };
} // namespace our
