#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "../components/free-movement.hpp"
namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data) {
        if (!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if (cameraTypeStr == "orthographic") {
            cameraType = CameraType::ORTHOGRAPHIC;
        }
        else {
            cameraType = CameraType::PERSPECTIVE;
        }
        nearC = data.value("near", 0.01f);
        farC = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix(World* world)const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //TODO: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookA
        const glm::vec4
            eyeCamera = glm::vec4(0, 0, 0, 1),
            centerCamera = glm::vec4(0, 0, -1, 1),
            upCamera = glm::vec4(0, 1, 0, 0);

        FreeMovementComponent* gameCharacter = nullptr;     

        for (auto entity : world->getEntities()) {
            gameCharacter = entity->getComponent<FreeMovementComponent>();
            if (gameCharacter) break;
        }

        const glm::vec3
            eyeWorld = M * eyeCamera,
            centerWorld = gameCharacter->getOwner()->localTransform.position ,
            upWorld = glm::normalize(glm::vec3(M * upCamera));

        glm::mat4 viewMatrix = glm::lookAt(eyeWorld, centerWorld, upWorld);
        return viewMatrix;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TODO: (Req 8) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective
        float aspectRatio = static_cast<float>(viewportSize.x) / static_cast<float>(viewportSize.y);

        if (cameraType == CameraType::ORTHOGRAPHIC) {
            float orthoWidth = orthoHeight * aspectRatio;

            return glm::ortho(
                -orthoWidth / 2.0f,
                orthoWidth / 2.0f,
                -orthoHeight / 2.0f,
                orthoHeight / 2.0f,
                nearC,
                farC
            );
        }
        else {
            return glm::perspective(
                fovY,
                aspectRatio,
                nearC,
                farC
            );
        }
    }
}