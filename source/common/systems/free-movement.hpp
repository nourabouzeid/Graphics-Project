#pragma once

#include "../ecs/world.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/free-movement.hpp"
#include "../components/collision.hpp"
#include "../application.hpp"
#include "collision-system.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../components/mesh-renderer.hpp"
#include <iostream>
#include <vector>
namespace our {

    class FreeMovementSystem {
        Application* app; // The application in which the state runs
        CollisionSystem* collisionSystem; // The application in which the state runs
        float currPitch = -1;
        std::unordered_map<Entity*, bool> boxLanded;
        std::vector<Entity*> grounds;
        std::vector<Entity*> boxes;

    public:
        void enter(Application* app, CollisionSystem* collisionSystem) {
            this->app = app;
            this->collisionSystem = collisionSystem;
        }

        void update(World* world, float deltaTime) {
            FreeCameraControllerComponent* controller = nullptr;
            FreeMovementComponent* character = nullptr;
            for (auto entity : world->getEntities()) {
                if (!controller) controller = entity->getComponent<FreeCameraControllerComponent>();
                if (!character) character = entity->getComponent<FreeMovementComponent>();
                if (controller && character) break;
            }
            if (!(controller && character)) return;

            Entity* entity = controller->getOwner();
            Entity* characterEntity = character->getOwner();
            glm::vec3& cameraPosition = entity->localTransform.position;
            glm::vec3& characterPos = characterEntity->localTransform.position;

            // Calculate movement direction (camera-relative)
            glm::vec3 movementDirection = glm::normalize(characterPos - cameraPosition);
            if (currPitch == -1)
                currPitch = atan2(cameraPosition.y, glm::sqrt(cameraPosition.x * cameraPosition.x + cameraPosition.z * cameraPosition.z));
            movementDirection.y *= sin(glm::radians(currPitch));

            // Get camera axes for movement
            glm::mat4 matrix = entity->localTransform.toMat4();
            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));
            glm::vec3 current_sensitivity = character->positionSensitivity;

            // Calculate intended movement
            glm::vec3 movement(0.0f);
            if (app->getKeyboard().isPressed(GLFW_KEY_W)) movement += movementDirection;
            if (app->getKeyboard().isPressed(GLFW_KEY_S)) movement -= movementDirection;
            if (app->getKeyboard().isPressed(GLFW_KEY_D)) movement += glm::cross(movementDirection, up);
            if (app->getKeyboard().isPressed(GLFW_KEY_A)) movement -= glm::cross(movementDirection, up);

            // Normalize and apply speedob
            if (glm::length(movement) > 0.0f) {
                movement = glm::normalize(movement) * (deltaTime * current_sensitivity.z);
            }

            // Store old position for collision rollback
            glm::vec3 oldCharacterPos = characterPos;
            glm::vec3 oldCameraPos = cameraPosition;

            // Apply movement to character and camera
            characterPos += movement;
            cameraPosition += movement;

            // Check for collisions after movement
            

            for(auto entity: world->getEntities()){
                if(entity->name == "groundEarth")
                    grounds.push_back(entity);

                if(entity->name == "box")
                    boxes.push_back(entity);
            }


            bool characterGroundCollision = false;
            for(auto ground:grounds){
                characterGroundCollision |= collisionSystem->checkCollision(characterEntity, ground) != CollisionSide::NONE;
            }


            // Update character rotation if moving
            if (glm::length(movement) > 0.0f) {
                float characterRotation = atan2(movement.x, movement.z);
                characterEntity->localTransform.rotation = glm::vec3(0.0f, characterRotation, 0.0f);
            }

            bool characterBoxCollision = false;
            CollisionSide boxCollisionSide = CollisionSide::NONE;

            // Box handling
            for (auto entity : world->getEntities())
            {
                if (entity != characterEntity)
                {
                    CollisionSide side = collisionSystem->checkCollision(characterEntity, entity);
                    if (side != CollisionSide::NONE)
                    {
                        handleCollision(characterEntity, entity, side, deltaTime);
                        if(entity->name == "box"){
                            characterBoxCollision = true;
                            boxCollisionSide = side;
                        }
                    }
                }
            }
            
            if(!characterGroundCollision && !characterBoxCollision){
                characterPos = oldCharacterPos;
                cameraPosition = oldCameraPos;
            }
            
            handleBoxMovement(world, boxCollisionSide);
            cleanupBoxStates(world);

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


            grounds.clear();
            boxes.clear();
        }


        void moveBox(Entity* other, CollisionSide side, float deltaTime) {
            // std::cout << "Player collided with box on side: ";
            // switch (side)
            // {
            // case CollisionSide::LEFT: std::cout << "LEFT"; break;
            // case CollisionSide::RIGHT: std::cout << "RIGHT"; break;
            // case CollisionSide::FRONT: std::cout << "FRONT"; break;
            // case CollisionSide::BACK: std::cout << "BACK"; break;
            // }
            // std::cout << std::endl;

            // Push the box in the opposite direction of collision
            glm::vec3 pushDirection(0.0f);
            float pushSpeed = 4.0f; // Adjust as needed

            switch (side)
            {
            case CollisionSide::LEFT:  pushDirection.x = 1.0f; break; // Push left
            case CollisionSide::RIGHT: pushDirection.x = -1.0f;  break; // Push right
            case CollisionSide::FRONT:  pushDirection.z = 1.0f; break; // Push forward
            case CollisionSide::BACK:   pushDirection.z = -1.0f;  break; // Push backward
            }

            other->localTransform.position += pushDirection * pushSpeed * deltaTime;
        }

        void handleCollision(Entity* player, Entity* other, CollisionSide side, float deltaTime)
        {

            if (other->name == "box" && !boxLanded[other])
            {
                moveBox(other, side, deltaTime);
            }
            else if (other->name == "trap")
            {
                // Trap logic here
                std::cout << "Player collided with trap!" << std::endl;
            }
            else if (other->name == "groundEarth")
            {
                // Enemy logic here
                // std::cout << "Player collided with groundEarth!" << std::endl;
            }
        }

        void handleBoxMovement(World* world, CollisionSide side)
        {

            if(side != CollisionSide::NONE){
                std::cout << "Player collided with box on side: ";
                switch (side)
                {
                    case CollisionSide::LEFT: std::cout << "LEFT"; break;
                    case CollisionSide::RIGHT: std::cout << "RIGHT"; break;
                    case CollisionSide::FRONT: std::cout << "FRONT"; break;
                    case CollisionSide::BACK: std::cout << "BACK"; break;
                }
                std::cout << std::endl;
            }

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
                    if (collisionSystem->checkCollision(box, ground) != CollisionSide::NONE)
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
                
                    switch(side){
                        case CollisionSide::FRONT:
                            box->localTransform.position.z +=0.8f;
                        case CollisionSide::BACK:
                            box->localTransform.position.z -=0.5f;
                        case CollisionSide::RIGHT:
                            box->localTransform.position.x -=0.8f;
                        case CollisionSide::LEFT:
                            box->localTransform.position.x +=0.4f;
                        default: 
                            break;
                    }

                    boxLanded[box] = true;
                }
            }
        }

        void exit() {
            // Cleanup if needed
        }
    };
}