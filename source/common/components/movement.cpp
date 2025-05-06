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
        }
}