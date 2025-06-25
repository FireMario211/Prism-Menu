#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCDrawNode.hpp>
#include <Geode/modify/GJEffectManager.hpp>

class $modify(POPlayerHacks, PlayerObject) {
    /*bool collidedWithObject(float fl, GameObject* obj,  cocos2d::CCRect p0, bool p1) {
        return PlayerObject::collidedWithObject(fl, obj, p0, p1);
    }*/
    // No Solids
    void collidedWithObject(float p0, GameObject* obj, cocos2d::CCRect p2, bool p3) { // what is the point of not having p2, because this doesnt work without it
         if (Hacks::isHackEnabled("Everything Hurts")) {
            if (auto playLayer = PlayLayer::get()) {
                playLayer->destroyPlayer(this, obj);
            }
        }
        if (!Hacks::isHackEnabled("No Solids")) return PlayerObject::collidedWithObject(p0, obj, p2, p3);
    }
    // Freeze Player
    void update(float dt) {
        if (Hacks::isHackEnabled("Change Gravity")) { // assume its enabled 
            auto gravityHack = Hacks::getHack("Gravity Value");
            m_gravityMod = gravityHack->value.floatValue;
        }
        if (Hacks::isHackEnabled("Instant Complete")) return;
        //if (Hacks::isHackEnabled("No Rotate")) PlayerObject::setRotation(0);
        if (!Hacks::isHackEnabled("Freeze Player")) return PlayerObject::update(dt);
    }

    void playerDestroyed(bool p0) {
        if (!Hacks::isHackEnabled("No Death Effect")) return PlayerObject::playerDestroyed(p0);
        m_isDead = true;
    }
#ifndef GEODE_IS_IOS
    bool pushButton(PlayerButton p0) {
        if (p0 != PlayerButton::Jump) return PlayerObject::pushButton(p0);
        float boost = 10.0F;
        boost = (m_isUpsideDown) ? -boost : boost;
        if (Hacks::isHackEnabled("Jump Hack")) PlayerObject::boostPlayer(boost); // idk if i should make this customizable
        return PlayerObject::pushButton(p0);
    }
#endif
    void activateStreak() {
        if (!m_isDart && Hacks::isHackEnabled("No Trail")) return;
        if (m_isDart && Hacks::isHackEnabled("No Wave Trail")) return;
        PlayerObject::activateStreak();
    }
#if !defined(GEODE_IS_ANDROID32) && !defined(GEODE_IS_IOS)
    void runNormalRotation(bool p0, float p1) {
        if (!Hacks::isHackEnabled("No Rotate")) return PlayerObject::runNormalRotation(p0, p1);
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


class $modify(GJBGLPlayerHacks, GJBaseGameLayer) {
    struct Fields {
        float timer = 0;
        bool down = false;
    };
    // No Mirror Transition, Instant Mirror Portal
    void toggleFlipped(bool p0, bool p1) { // i spent a lot of time figuring out why CCActionTween wont hook, only to realize that p1 instantly transitions it
        if (!Hacks::isHackEnabled("No Mirror Transition")) return GJBaseGameLayer::toggleFlipped(p0, (p1) ? p1 : Hacks::isHackEnabled("Instant Mirror Portal"));
    }
    void processCommands(float dt) {
        GJBaseGameLayer::processCommands(dt);
        if (Hacks::isHackEnabled("Auto Clicker")) {
            m_fields->timer += dt;
            int value = ((Hacks::getHack("Clicks per second") != nullptr) ? Hacks::getHack("Clicks per second")->value.intValue : 1) * 2;
            if (m_fields->timer >= (1.0F / (float)value)) {
                m_fields->down = !m_fields->down;
                GJBaseGameLayer::handleButton(m_fields->down, 1, true);
                m_fields->timer = 0.0F;
            }
        }
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
