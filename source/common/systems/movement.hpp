#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    class MovementSystem
    {
    public:
        void update(World *world, float deltaTime)
        {
            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // Get the MovementComponent if it exists
                MovementComponent *movement = entity->getComponent<MovementComponent>();

                // If the MovementComponent exists
                if (movement)
                {
                    // Calculate the final displacement for each axis
                    glm::vec3 displacement = deltaTime * movement->linearVelocity;
                    glm::vec3 newPosition = entity->localTransform.position + displacement;

                    // Check boundaries for the X-axis
                    if (newPosition.x > movement->maxBoundaries.x || newPosition.x < movement->minBoundaries.x)
                    {
                        movement->linearVelocity.x = -movement->linearVelocity.x; // Reverse X direction
                        newPosition.x = glm::clamp(newPosition.x, movement->minBoundaries.x, movement->maxBoundaries.x);
                    }

                    // Check boundaries for the Y-axis
                    if (newPosition.y > movement->maxBoundaries.y || newPosition.y < movement->minBoundaries.y)
                    {
                        movement->linearVelocity.y = -movement->linearVelocity.y; // Reverse Y direction
                        newPosition.y = glm::clamp(newPosition.y, movement->minBoundaries.y, movement->maxBoundaries.y);
                    }

                    // Check boundaries for the Z-axis
                    if (newPosition.z > movement->maxBoundaries.z || newPosition.z < movement->minBoundaries.z)
                    {
                        movement->linearVelocity.z = -movement->linearVelocity.z; // Reverse Z direction
                        newPosition.z = glm::clamp(newPosition.z, movement->minBoundaries.z, movement->maxBoundaries.z);
                    }

                    // Update the position
                    entity->localTransform.position = newPosition;

                    // Update the rotation based on angular velocity and delta time
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }
        }
    };

}