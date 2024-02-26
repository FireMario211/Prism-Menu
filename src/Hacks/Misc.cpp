#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <Geode/modify/ShaderLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

// no create method?????????????
class $modify(ShaderLayer) {
    // No Shaders
    void performCalculations() {
        if (!Hacks::isHackEnabled("No Shaders")) return ShaderLayer::performCalculations();
    }
};

// modifying PlayLayer::postUpdate twice!
class $modify(PlayLayer) {
    void postUpdate(float dt) {
        if (Hacks::isHackEnabled("Disable Camera Effects")) PlayLayer::resetCamera();
        PlayLayer::postUpdate(dt);
    }
};
