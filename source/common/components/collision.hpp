#pragma once

#include "../ecs/component.hpp"
#include <glm/glm.hpp>

namespace our
{

    class CollisionComponent : public Component
    {
    public:
        enum class Anchor
        {
            CENTER,
            BOTTOM_LEFT,
            BOTTOM_RIGHT,
            TOP_LEFT,
            TOP_RIGHT
        };

        glm::vec2 size = {1.0f, 1.0f};  // Full width and height
        Anchor anchor = Anchor::CENTER; // Where the position is anchored

        static std::string getID() { return "Collision"; }

        void deserialize(const nlohmann::json &data);
    };

} // namespace our