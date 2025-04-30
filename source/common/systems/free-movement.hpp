#pragma once

#include "../ecs/world.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/free-movement.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../components/mesh-renderer.hpp"
#include <iostream>
namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeMovementSystem {
        Application* app; // The application in which the state runs

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app) {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        void update(World* world, float deltaTime) {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            FreeCameraControllerComponent* controller = nullptr;
            FreeMovementComponent* character = nullptr;
            for (auto entity : world->getEntities()) {
                if (!controller)
                    controller = entity->getComponent<FreeCameraControllerComponent>();
                if (!character)
                    character = entity->getComponent<FreeMovementComponent>();
                if (controller && character) break;
            }
            // If there is no entity with both a controllerComponent and a FreecontrollerControllerComponent, we can do nothing so we return
            if (!(controller && character)) return;
            // Get the entity that we found via getOwner of controller (we could use controller->getOwner())
            Entity* entity = controller->getOwner();
            glm::vec3& position = entity->localTransform.position;

            glm::vec3& characterPos = character->getOwner()->localTransform.position;
            glm::vec3 movementDirection = glm::normalize(characterPos-position);
            float currPitch = glm::asin(glm::clamp(position.y / controller->radius, -1.0f, 1.0f));
            movementDirection.y *= sin(glm::radians(currPitch));

            // We change the controller position based on the keys WASD/QE
            // S & W moves the player back and forth
            glm::mat4 matrix = entity->localTransform.toMat4();
            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));
            glm::vec3 current_sensitivity = character->positionSensitivity;
            if (app->getKeyboard().isPressed(GLFW_KEY_W)) {
                characterPos += movementDirection * (deltaTime * current_sensitivity.z);
                position += movementDirection * (deltaTime * current_sensitivity.z);
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_S)) {
                characterPos -= movementDirection * (deltaTime * current_sensitivity.z);
                position -= movementDirection * (deltaTime * current_sensitivity.z);
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_D)) {
                characterPos += glm::cross(movementDirection, up) * (deltaTime * current_sensitivity.x);
                position += glm::cross(movementDirection, up) * (deltaTime * current_sensitivity.x);
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_A)) {
                characterPos -= glm::cross(movementDirection, up) * (deltaTime * current_sensitivity.x);
                position -= glm::cross(movementDirection, up) * (deltaTime * current_sensitivity.x);
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit() {

        }

    };

}
