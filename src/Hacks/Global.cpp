#include "../hacks.hpp"
#include "Quartz.hpp"
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
#include <Geode/modify/CCApplication.hpp>

// Practice Music
class $modify(GameStatsManager) {
    bool isItemUnlocked(UnlockType p0, int p1) {
        // did i seriously not see this, all i changed was && to || hA!
        if (p0 != UnlockType::GJItem || p1 != 17) return GameStatsManager::isItemUnlocked(p0,p1);
        if (Hacks::isHackEnabled("Practice Music") && p1 == 17) {
            return true;
        } else {
            return GameStatsManager::isItemUnlocked(p0,p1);
        }
    }
};

// *coding sounds* creditos a sillydoggo para esto https://github.com/TheSillyDoggo/GeodeMenu/blob/38ef8f48a3d52b1eefb09ee29dd5b985928edfd2/src/Hacks/Speedhack.cpp
// y xdbot

int syncCooldown = 0;

class $modify(CCScheduler) {
    void update(float dt) {
        HackItem* speedhack = Hacks::getHack("Speedhack");
        if (speedhack == nullptr) return CCScheduler::update(dt);
        speedhack->value.floatValue = std::max(speedhack->value.floatValue, 0.01f);
        if (speedhack->value.floatValue == 1.0F && !current_macro.isEnabled) return CCScheduler::update(dt);
        float speedHackValue = stof(Utils::setPrecision(speedhack->value.floatValue, 3));
        dt *= speedHackValue; // def not copied!
        if (current_macro.isEnabled && PlayLayer::get() != nullptr) { 
            float dt2 = (1.f / current_macro.framerate);
            auto startTime = std::chrono::high_resolution_clock::now();
            int mult = static_cast<int>((dt + current_macro.loaf)/dt2);  
            using namespace std::literals;
            for (int i = 0; i < mult; i++) {
                CCScheduler::update(dt2);
                if (std::chrono::high_resolution_clock::now() - startTime > 33.333ms) {
                    mult = i + 1;
                    break;
                }
            }
            current_macro.loaf += (dt - dt2 * mult); 
            if (Hacks::isHackEnabled("Playback") && !PlayLayer::get()->m_levelSettings->m_platformerMode) {
                syncCooldown++;
                if (syncCooldown >= 20 && current_macro.loaf > 1) {
                    syncCooldown = 0;
                }
            }
            return;
        }
        CCScheduler::update(dt);
    }
};

#if !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS)
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
        if (!(Hacks::isHackEnabled("Safe Mode") || Hacks::isAutoSafeModeActive())) {
            GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
        }
    }
};

// No Transition
#ifndef GEODE_IS_MACOS
class $modify(CCTransitionFade) {
    // I still dont understand why this cant hooked on mac, so i resorted to manually hooking!
    bool initWithDuration(float t, cocos2d::CCScene* scene, cocos2d::ccColor3B const& color)  {
        if (!Hacks::isHackEnabled("No Transition") || Hacks::isHackEnabled("Enable Patching")) {
            return CCTransitionFade::initWithDuration(t, scene, color);
        } else {
            return CCTransitionFade::initWithDuration(0.0F, scene, color);
        }
    }
};
#else // mac
// attempt to fix the hook not working
CCTransitionFade* CCTransitionFade_Create(float t, cocos2d::CCScene* scene) {
    if (!Hacks::isHackEnabled("No Transition") || Hacks::isHackEnabled("Enable Patching")) {
        return CCTransitionFade::create(t, scene);
    } else {
        return CCTransitionFade::create(0.0F, scene);
    }
}

$execute {
    //(void)
    Mod::get()->hook(
        reinterpret_cast<void*>(geode::base::get() + 0xd4dd0), // address
        &CCTransitionFade_Create, // detour
        "CCTransitionFade::create", // display name, shows up on the console
        tulip::hook::TulipConvention::Stdcall // calling convention
    );
}
#endif

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
