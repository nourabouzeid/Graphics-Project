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

namespace our {

    class FreeMovementSystem {
        Application* app; // The application in which the state runs
        CollisionSystem* collisionSystem; // The application in which the state runs
        float currPitch = -1;

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
            glm::vec3 movementDirection = glm::normalize(glm::vec3(
                characterPos.x - cameraPosition.x,
                0.0f, // eliminate vertical component
                characterPos.z - cameraPosition.z
            ));
            // Get camera axes for movement
            glm::mat4 matrix = entity->localTransform.toMat4();
            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));
            glm::vec3 current_sensitivity = character->positionSensitivity;
            // Get camera forward vector from local transform
            // Calculate intended movement
            glm::vec3 movement(0.0f);
            if (app->getKeyboard().isPressed(GLFW_KEY_W)) movement += movementDirection;
            if (app->getKeyboard().isPressed(GLFW_KEY_S)) movement -= movementDirection;
            if (app->getKeyboard().isPressed(GLFW_KEY_D)) movement += glm::cross(movementDirection, up);
            if (app->getKeyboard().isPressed(GLFW_KEY_A)) movement -= glm::cross(movementDirection, up);

            // Normalize and apply speed
            if (glm::length(movement) > 0.0f) {
                movement = glm::normalize(movement) * (deltaTime * current_sensitivity.z);
            }

            // Store old position for collision rollback
            glm::vec3 oldCharacterPos = characterPos;
            glm::vec3 oldCameraPos = cameraPosition;

            // Apply movement to character and camera
            characterPos += movement;
            cameraPosition += movement;

            // // Check for collisions after movement
            // bool collided = false;
            // for (auto other : world->getEntities()) {
            //     if (other == characterEntity) continue; // Skip self

            //     // Get collision system (assumes it exists in the world)
            //     if (!collisionSystem) continue;

            //     CollisionSide side = collisionSystem->checkCollision(characterEntity, other);
            //     if (side != CollisionSide::NONE && other->name == "box") {
            //         collided = true;
            //         break;
            //     }
            // }

            // // If collided, revert movement
            // if (collided) {
            //     characterPos = oldCharacterPos;
            //     cameraPosition = oldCameraPos;
            // }

            // Update character rotation if moving
            if (glm::length(movement) > 0.0f) {
                float characterRotation = atan2(movement.x, movement.z);
                characterEntity->localTransform.rotation = glm::vec3(0.0f, characterRotation, 0.0f);
            }
        }

        void exit() {
            // Cleanup if needed
        }
    };
}