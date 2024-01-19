#include "CustomSettings.hpp"
SettingNode* SettingHackValue::createNode(float width) {
    return SettingHackNode::create(this, width);
}

