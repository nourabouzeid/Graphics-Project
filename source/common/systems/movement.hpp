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

        void update(World* world, float deltaTime)
        {

            std::vector<Entity*> groundPlanes;
            std::vector<Entity*> movingTraps;
            
            for(auto entity : world->getEntities()){
                MovementComponent *movement = entity->getComponent<MovementComponent>();

                if(movement)
                    movingTraps.push_back(entity);

                if(entity->name == "groundEarth")
                    groundPlanes.push_back(entity);
            }


            for(auto trap : movingTraps){

                float minDistance = FLT_MAX;
                int ind = 0, minInd = 0;
                for(auto ground: groundPlanes){
                    glm::vec3 diff = ground->localTransform.position - trap->localTransform.position;
                    float distance = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
                    
                    if(distance < minDistance){
                        minDistance = distance;
                        minInd = ind;
                    }

                    ind++;
                }

                Transform& groundLocation = groundPlanes[minInd]->localTransform;
                Transform& trapLocation = trap->localTransform;
                glm::vec3 minGroundBoundary = groundLocation.position - (groundLocation.scale/glm::vec3(2.0)) + trapLocation.scale;
                glm::vec3 maxGroundBoundary = groundLocation.position + (groundLocation.scale/glm::vec3(2.0)) - trapLocation.scale;

                trap->getComponent<MovementComponent>()->setMovementBoundary(minGroundBoundary, maxGroundBoundary);
            }

            // For each entity in the world
            for(auto trap : movingTraps){    
                
                std::vector<Entity*> groundPlanes;
                std::vector<Entity*> movingTraps;
                // Get the MovementComponent if it exists
                // Calculate the final displacement for each axis

                MovementComponent *movement = trap->getComponent<MovementComponent>();

                glm::vec3 displacement = deltaTime * movement->linearVelocity;
                glm::vec3 newPosition = trap->localTransform.position + displacement;

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
                trap->localTransform.position = newPosition;

                // Update the rotation based on angular velocity and delta time
                trap->localTransform.rotation += deltaTime * movement->angularVelocity;
                
        }
    }

    };
}