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

                    
                    // Update the position
                    entity->localTransform.position = newPosition;

                    // Update the rotation based on angular velocity and delta time
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }
        }
    };

}