#include "collision.hpp"
#include "../deserialize-utils.hpp"
#include <iostream>

namespace our
{

    void CollisionComponent::deserialize(const nlohmann::json &data)
    {
        std::cout << "Deserializing CollisionComponent" << std::endl;
        if (!data.is_object())
            return;

        size = data.value("size", size);

        std::string anchorStr = data.value("anchor", "center");
        if (anchorStr == "bottom_left")
            anchor = Anchor::BOTTOM_LEFT;
        else if (anchorStr == "bottom_right")
            anchor = Anchor::BOTTOM_RIGHT;
        else if (anchorStr == "top_left")
            anchor = Anchor::TOP_LEFT;
        else if (anchorStr == "top_right")
            anchor = Anchor::TOP_RIGHT;
        else
            anchor = Anchor::CENTER;
    }

} // namespace our