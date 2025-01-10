#include "CustomSettings.hpp"

SettingNodeV3* SettingHackValue::createNode(float width) {
    return SettingHackNode::create(static_pointer_cast<SettingHackValue>(shared_from_this()), width);
}
