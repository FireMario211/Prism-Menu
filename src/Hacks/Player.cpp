#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>

class $modify(PlayerObject) {
#ifdef GEODE_IS_WINDOWS // for whatever reason, fields arent found!
    // No Solids
    /*
     * +       bool collidedWithObject(float, GameObject*, cocos2d::CCRect, bool) = win 0x2cc450;
+       TodoReturn collidedWithObject(float, GameObject*) = win 0x2cc3d0;
    bool collidedWithObject(float p0, GameObject* obj, cocos2d::CCRect p2, bool p3) { // what is the point of not having p2, because this doesnt work without it
        if (Hacks::isHackEnabled("Enable Patching")) return PlayerObject::collidedWithObject(p0, obj, p2, p3);
        if (!Hacks::isHackEnabled("No Solids")) return PlayerObject::collidedWithObject(p0, obj, p2, p3);
        //return PlayerObject::collidedWithObject(p0, obj, p2, p3);
        return false;
    }*/
    bool was_platformer;
    /*bool collidedWithObject(float fl, GameObject* obj,  cocos2d::CCRect p0, bool p1) {
        return PlayerObject::collidedWithObject(fl, obj, p0, p1);
    }*/
    // Freeze Player
    void update(float dt) {
        if (!m_fields->was_platformer) {
            m_fields->was_platformer = this->m_isPlatformer;
        }
        if (Hacks::isHackEnabled("Force Platformer Mode")) {
            togglePlatformerMode(true);
        } else {
            togglePlatformerMode(m_fields->was_platformer);
        }
        auto gravityHack = Hacks::getHack("Gravity Value");
        if (Hacks::isHackEnabled("Change Gravity")) { // assume its enabled 
            m_gravityMod = gravityHack->value.floatValue;
        }
        if (Hacks::isHackEnabled("Instant Complete")) return;
        if (Hacks::isHackEnabled("Enable Patching") || !Hacks::isHackEnabled("Freeze Player")) return PlayerObject::update(dt);
    }
    bool init(int p0, int p1, GJBaseGameLayer *p2, cocos2d::CCLayer *p3, bool p4) {
        if (!PlayerObject::init(p0,p1,p2,p3,p4)) return false;
        return true;
    }

    void playerDestroyed(bool p0) {
        m_fields->was_platformer = this->m_isPlatformer;
        PlayerObject::playerDestroyed(p0);
    }
    #endif
    void pushButton(PlayerButton p0) {
        if (!Hacks::isHackEnabled("Enable Patching") && Hacks::isHackEnabled("Jump Hack")) PlayerObject::boostPlayer(10.0F); // idk if i should make this customizable
        PlayerObject::pushButton(p0);
    }
};
