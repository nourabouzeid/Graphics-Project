#include "light-component.hpp"
#include "../asset-loader.hpp"
#include <deserialize-utils.hpp>

#include<glm/glm.hpp>
namespace our {
    // Receives the light configuration from the AssetLoader by the names given in the json object
    void LightComponent::deserialize(const nlohmann::json& data) {
        if (!data.is_object()) return;

        std::string type_string = data.value("lightType", "");

        if (type_string == "directional") type = LightType::DIRECTIONAL;
        else if (type_string == "point") type = LightType::POINT;
        else if (type_string == "spot") type = LightType::SPOT;
        else type = LightType::DIRECTIONAL;

        color = data.value("color", glm::vec3(0));
        inner_angle = data.value("inner_angle", 0.0f);
        outer_angle = data.value("outer_angle", 0.0f);
        attenuationConstant = data.value("attenuationConstant", 1.0f);
        attenuationLinear = data.value("attenuationLinear", 0.007f);
        attenuationQuadratic = data.value("attenuationQuadratic", 0.0014f);
    }
}