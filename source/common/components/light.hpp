#pragma once

#include "../ecs/component.hpp"
#include "../mesh/mesh.hpp"
#include "../material/material.hpp"
#include "../asset-loader.hpp"

namespace our {
    enum LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };
    // This component denotes that the entity is a light source.
    class LightComponent : public Component {
    public:
        LightType type;
        glm::vec3 color;
        float innerAngle;              // spot inner angle in radians
        float outerAngle;              // spot outer angle in radians

        float attenuationConstant;     // The constant of the attenuation
        float attenuationLinear;       // The linear of the attenuation
        float attenuationQuadratic;    // The quadratic of the attenuation


        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Receives the mesh & material from the AssetLoader by the names given in the json object
        void deserialize(const nlohmann::json& data) override;

    };

}