#include "movement.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    // Reads linearVelocity, angularVelocity, and boundaries from the given json object
    void MovementComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        // Deserialize linear and angular velocity
        linearVelocity = data.value("linearVelocity", linearVelocity);
        angularVelocity = glm::radians(data.value("angularVelocity", angularVelocity));

        // Deserialize boundaries if they exist
        if (data.contains("boundaries"))
        {
            auto boundaries = data["boundaries"];
            minBoundaries.x = boundaries["x"][0];
            maxBoundaries.x = boundaries["x"][1];
            minBoundaries.y = boundaries["y"][0];
            maxBoundaries.y = boundaries["y"][1];
            minBoundaries.z = boundaries["z"][0];
            maxBoundaries.z = boundaries["z"][1];
        }
    }
    
    void MovementComponent::setMovementBoundary(glm::vec3 minBoundary, glm::vec3 maxBoundary)
    {
        minBoundaries = minBoundary;
        maxBoundaries = maxBoundary;
    }
}