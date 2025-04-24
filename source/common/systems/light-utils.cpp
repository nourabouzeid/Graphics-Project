#include "light-utils.hpp"

namespace our {
    void light_utils::setLights(ShaderProgram* program, std::vector<light_utils::LightCommand>& lightCommands) {
        int light_index = 0;
        const int MAX_LIGHT_COUNT = 16;
        for (const auto& lightCommand : lightCommands) {
            LightComponent* lightComponent = lightCommand.lightComponent;
            std::string prefix = "lights[" + std::to_string(light_index) + "].";

            program->set(prefix + "type", static_cast<int>(lightComponent->type));
            program->set(prefix + "color", lightComponent->color);

            switch (lightComponent->type) {
            case LightType::DIRECTIONAL:
                program->set(prefix + "direction", glm::normalize(lightCommand.direction));
                break;
            case LightType::POINT:
                program->set(prefix + "position", lightCommand.position);
                program->set(prefix + "attenuation_constant", lightComponent->attenuationConstant);
                program->set(prefix + "attenuation_linear", lightComponent->attenuationLinear);
                program->set(prefix + "attenuation_quadratic", lightComponent->attenuationQuadratic);
                break;
            case LightType::SPOT:
                program->set(prefix + "position", lightCommand.position);
                program->set(prefix + "direction", glm::normalize(lightCommand.direction));
                program->set(prefix + "attenuation_constant", lightComponent->attenuationConstant);
                program->set(prefix + "attenuation_linear", lightComponent->attenuationLinear);
                program->set(prefix + "attenuation_quadratic", lightComponent->attenuationQuadratic);
                program->set(prefix + "inner_angle", lightComponent->innerAngle);
                program->set(prefix + "outer_angle", lightComponent->outerAngle);
                break;
            }
            light_index++;
            if (light_index >= MAX_LIGHT_COUNT) break;
        }

    }
}