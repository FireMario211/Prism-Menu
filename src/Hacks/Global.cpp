#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/CCTransitionFade.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/CCSprite.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

// Practice Music
class $modify(GameStatsManager) {
    bool isItemUnlocked(UnlockType p0, int p1) {
        if (p0 != UnlockType::GJItem && p1 != 17) return GameStatsManager::isItemUnlocked(p0,p1);
        if (Hacks::isHackEnabled("Practice Music")) {
            return true;
        } else {
            return GameStatsManager::isItemUnlocked(p0,p1);
        }
    }
};

// *coding sounds* creditos a sillydoggo para esto https://github.com/TheSillyDoggo/GeodeMenu/blob/38ef8f48a3d52b1eefb09ee29dd5b985928edfd2/src/Hacks/Speedhack.cpp
class $modify(CCScheduler) {
    void update(float dt) {
        HackItem* speedhack = Hacks::getHack("Speedhack");
        speedhack->value.floatValue = std::max(speedhack->value.floatValue, 0.01f);
        if (speedhack == nullptr) return CCScheduler::update(dt);
        if (speedhack->value.floatValue == 1.0F) return CCScheduler::update(dt);
        CCScheduler::update(dt * speedhack->value.floatValue);
    }
};

#ifndef GEODE_IS_MACOS
class $modify(PlayLayer) {
    // No Glow, Show Hidden Objects
    void addObject(GameObject* obj) {
        // "Optimization"
        if (Hacks::isHackEnabled("Show Hidden Objects")) {
            obj->m_activeMainColorID = -1;
            obj->m_isHide = false;
            if (obj->m_objectID == 1007) return;
        }
        if (Hacks::isHackEnabled("No Glow")) {
            obj->m_hasNoGlow = true;
        }
        PlayLayer::addObject(obj);
    }
};


// Layout Mode
class $modify(GameObject) {
    void setVisible(bool v) {
        if (!Hacks::isHackEnabled("Layout Mode")) return GameObject::setVisible(v);
        //m_hasGroupParent == 0
        std::vector<int> outerPortal = {};
        //i really dont want to have to check every single object id
        if (m_objectType == GameObjectType::Decoration && m_objectID != 44) { // 44 being practice mode checkpoint, because thats a "decoration"
            GameObject::setVisible(false);
        } else {
            GameObject::setVisible(v);
        }
    }
};
#endif

// Safe Mode (a just incase)
class $modify(GJGameLevel) {
    void savePercentage(int p0, bool p1, int p2, int p3, bool p4) {
        if (!Hacks::isHackEnabled("Safe Mode") || Hacks::isHackEnabled("Enable Patching")) {
            GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
        }
    }
};

// No Transition
class $modify(CCTransitionFade) {
    bool initWithDuration(float t, cocos2d::CCScene* scene, cocos2d::ccColor3B const& color)  {
        if (!Hacks::isHackEnabled("No Transition") || Hacks::isHackEnabled("Enable Patching")) {
            return CCTransitionFade::initWithDuration(t, scene, color);
        } else {
            return CCTransitionFade::initWithDuration(0.0F, scene, color);
        }
    }
};
// Transparent BG
class $modify(CCSprite) {
    bool isGradient;
    bool initWithFile(char const* name) {
        if (!CCSprite::initWithFile(name)) return false;
        if (!Hacks::isHackEnabled("Transparent BG")) return true;
        if (!strcmp(name, "GJ_gradientBG.png")) {
            m_fields->isGradient = true;
        }
        return true;
    }
    void setColor(const ccColor3B& color) {
        CCSprite::setColor(color);
        if (!Hacks::isHackEnabled("Transparent BG")) return;
        if (m_fields->isGradient) {
            CCSprite::setColor({255,255,255});
        }
    }
};

// Physics Bypass (TPS)
// dVar3 = 0.004166666883975267;
// which (dVar3 * 240 = 1) lol, so 1.F / TPS?
/*
class $modify(GJBaseGameLayer) {
    float getModifiedDelta(float p0) {
        std::cout << "yAEY" << std::endl;
        return GJBaseGameLayer::getModifiedDelta(p0);
    }
};*/
