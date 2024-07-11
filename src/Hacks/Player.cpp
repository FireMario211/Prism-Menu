#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCDrawNode.hpp>
#include <Geode/modify/GJEffectManager.hpp>

class $modify(PlayerObject) {
    struct Fields {
    bool isActuallyDart;
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
    };
    /*bool collidedWithObject(float fl, GameObject* obj,  cocos2d::CCRect p0, bool p1) {
        return PlayerObject::collidedWithObject(fl, obj, p0, p1);
    }*/
    // Freeze Player
    void update(float dt) {
        if (!m_fields->was_platformer) {
            m_fields->was_platformer = this->m_isPlatformer;
        }
        #ifndef GEODE_IS_DESKTOP
        if (PlayLayer::get() != nullptr) {
            auto playLayer = PlayLayer::get(); //shut!
            playLayer->m_uiLayer->togglePlatformerMode(Hacks::isHackEnabled("Force Platformer Mode") ? true : m_fields->was_platformer);
            // for some reason i was trying to look for togglePlatformerMode when I realized that i couldve just literally used the func as its android, im so mad
        }
        #endif
        togglePlatformerMode(Hacks::isHackEnabled("Force Platformer Mode") ? true : m_fields->was_platformer);
        auto gravityHack = Hacks::getHack("Gravity Value");
        if (Hacks::isHackEnabled("Change Gravity")) { // assume its enabled 
            m_gravityMod = gravityHack->value.floatValue;
        }
        if (Hacks::isHackEnabled("Instant Complete")) return;
        /*log::info(fmt::format(
            "HardStreak\n---\nx = {}, y = {}, m_waveSize = {}\nm_pulseSize = {}",
            m_waveTrail->m_currentPoint.x,
            m_waveTrail->m_currentPoint.y,
            m_waveTrail->m_waveSize,
            m_waveTrail->m_pulseSize
        ));*/ 
        if (!Hacks::isHackEnabled("Freeze Player")) return PlayerObject::update(dt);
    }

    void playerDestroyed(bool p0) {
        m_fields->was_platformer = this->m_isPlatformer;
        if (!Hacks::isHackEnabled("No Death Effect")) return PlayerObject::playerDestroyed(p0);
        m_isDead = true;
    }
    void pushButton(PlayerButton p0) {
        if (p0 != PlayerButton::Jump) return PlayerObject::pushButton(p0);
        float boost = 10.0F;
        boost = (m_isUpsideDown) ? -boost : boost;
        if (!Hacks::isHackEnabled("Enable Patching") && Hacks::isHackEnabled("Jump Hack")) PlayerObject::boostPlayer(boost); // idk if i should make this customizable
        PlayerObject::pushButton(p0);
    }
    void toggleDartMode(bool p0, bool p1) {
        // this is the fix until someone actually creates pads for android32 and android64, because i cant use m_isDart
        m_fields->isActuallyDart = p0;
        PlayerObject::toggleDartMode(p0, p1);
    }
    void activateStreak() { // i still need this because no one found m_isDart on mac
        /*if (!m_isDart && Hacks::isHackEnabled("No Trail")) return;
        if (m_isDart && Hacks::isHackEnabled("No Wave Trail")) return;*/
        if (!m_fields->isActuallyDart && Hacks::isHackEnabled("No Trail")) return;
        if (m_fields->isActuallyDart && Hacks::isHackEnabled("No Wave Trail")) return;
        PlayerObject::activateStreak();
    }
#ifndef GEODE_IS_ANDROID32
    void setRotation(float p0) {
        if (p0 == 0.F || Hacks::isHackEnabled("No Rotate")) return PlayerObject::setRotation(0);
        PlayerObject::setRotation(p0);
    }
    void setVisible(bool p0) {
        if (!p0) return PlayerObject::setVisible(p0);
        PlayerObject::setVisible(!Hacks::isHackEnabled("Hide Player"));
    }
#endif
};

// No Wave Pulse
class $modify(HardStreak) {
    void updateStroke(float dt) {
        //log::info("HardStreak\n---\nm_waveSize = {}\nm_pulseSize = {}\nm_isSolid = {}", m_waveSize, m_pulseSize, m_isSolid);
        if (Hacks::isHackEnabled("No Wave Pulse")) {
            m_pulseSize = 1.24F;
        }
        auto wavePulseSize = Hacks::getHack("Wave Pulse Size");
        if (wavePulseSize != nullptr) {
            m_pulseSize *= wavePulseSize->value.floatValue;
        }
        HardStreak::updateStroke(dt);
    }
};

// yeah no this will make the game lag
class $modify(MenuGameLayer) {
    struct Fields {
        int frame = 0;
    };
    void update(float dt) {
        if (Hacks::isHackEnabled("Suicide")) {
            m_fields->frame++;
            if (m_fields->frame % 250 == 0) { // uhh idk
                m_fields->frame = 1;
                MenuGameLayer::destroyPlayer();
            }
        }
        // if i dont have the "frame" modulus if condition, this will crash the game because its running EVERY FRAME
        MenuGameLayer::update(dt);
    }
};


class $modify(GJBaseGameLayer) {
    // No Mirror Transition, Instant Mirror Portal
    void toggleFlipped(bool p0, bool p1) { // i spent a lot of time figuring out why CCActionTween wont hook, only to realize that p1 instantly transitions it
        if (!Hacks::isHackEnabled("No Mirror Transition")) return GJBaseGameLayer::toggleFlipped(p0, (p1) ? p1 : Hacks::isHackEnabled("Instant Mirror Portal"));
    }
#if 0
    void update(float dt) {
        /*
            fVar30 = (float)((double)dt + *(double *)&this[4].GJBaseGameLayer_data.m_gameState.field_0xf0);
    fVar25 = *(float *)&(this->GJBaseGameLayer_data).m_gameState.field_0x188;
    if (1.0 < fVar25) {
      fVar25 = 1.0;
    }
    fVar24 = (float10)roundf(fVar30 / (fVar25 * 0.004166667));
    fStack_64 = (float)fVar24;
    dVar28 = (double)(int)fVar24 * (double)(fVar25 * 0.004166667);
    dVar26 = (double)fVar30 - dVar28;
    dt = (float)dVar28;
        */
        // we love ghidra
        /*float fVar30 = (float)((double)dt + 0.0);
        float fVar25 = m_gameState.m_unk188;
        if (1.0 < fVar25) {
            fVar25 = 1.0;
        }
        float physicsTPS = (1.F / 360.F);
        float fVar24 = (float)roundf(fVar30 / (fVar25 * physicsTPS));
        double dVar28 = (int)fVar24 * (double)(fVar25 * physicsTPS);
        double dVar26 = (double)fVar30 - dVar28;
        auto beforeDt = dt;
        dt = (float)dVar28;
        std::cout << dt << "," << beforeDt << std::endl;*/
        //this->m_gameState.m_unk188 = 0.7F;
        //std::cout << (float)((double)dt + (double)m_gameState.m_unkf0) << "," << dt << std::endl;
        //std::cout << m_gameState.m_unkf0 << std::endl;
        //m_gameState.m_unkf0 = 0.00001;
        GJBaseGameLayer::update(dt);
    }
#endif
};

#ifndef GEODE_IS_MACOS
// Solid Wave Trail
class $modify(CCDrawNode) {
    bool drawPolygon(CCPoint *p0, unsigned int p1, const ccColor4F &p2, float p3, const ccColor4F &p4) {
        if (!Hacks::isHackEnabled("Solid Wave Trail")) return CCDrawNode::drawPolygon(p0,p1,p2,p3,p4);
        if (p2.r == 1.F && p2.g == 1.F && p2.b == 1.F && p2.a != 1.F) return true; // tried doing just p2.a != 1.F but uh
        this->setBlendFunc(CCSprite::create()->getBlendFunc());
        this->setZOrder(-1); // ok but why
        return CCDrawNode::drawPolygon(p0,p1,p2,p3,p4);
    }
};
#endif
