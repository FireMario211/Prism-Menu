#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <Geode/modify/ShaderLayer.hpp>

// no create method?????????????
class $modify(ShaderLayer) {
    // No Shaders
    void performCalculations() {
        if (!Hacks::isHackEnabled("No Shaders")) return ShaderLayer::performCalculations();
    }
};
