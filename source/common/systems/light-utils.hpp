#pragma once
#include "../shader/shader.hpp"
#include "../components/light.hpp"

namespace our::light_utils {

    struct LightCommand {
        glm::vec3 position, direction;
        LightComponent* lightComponent;
    };

    void setLights(ShaderProgram* program, std::vector<LightCommand>& lightCommands);

}