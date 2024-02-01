#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/CCTransitionFade.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <Geode/modify/CCScheduler.hpp>

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

#ifndef GEODE_IS_WINDOWS // for the android users!!! :fire::fire::fire: i dunno maybe ill migrate with windows soon 
    // *coding sounds* creditos a sillydoggo para esto https://github.com/TheSillyDoggo/GeodeMenu/blob/38ef8f48a3d52b1eefb09ee29dd5b985928edfd2/src/Hacks/Speedhack.cpp
class $modify(CCScheduler) {
    void update(float dt) {
        HackItem* speedhack = Hacks::getHack("Speedhack");
        speedhack->value.floatValue = std::max(m_hack->value.floatValue, 0.01f);
        if (speedhack == nullptr) return CCScheduler::update(dt);
        if (speedhack->value.floatValue == 1.0F) return CCScheduler::update(dt);
        CCScheduler::update(dt * speedhack->value.floatValue);
    }
};
#endif

// Layout Mode
#ifdef GEODE_IS_WINDOWS
class $modify(GameObject) {
    void setVisible(bool v) {
        /*int aaa = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x14E); // we do the funny because someone forgor pad for windows
        if (aaa > 1600000000) {
            //int targetValue = 44;
            GameObjectType targetValue = GameObjectType::Decoration;
            for (int offset = 0x0; ; offset += 0x1) {
                GameObjectType val = *reinterpret_cast<GameObjectType*>(reinterpret_cast<uintptr_t>(this) + offset);
                //int val = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offset);
                //std::cout << "Offset: 0x" << std::hex << offset << std::dec << ", Value: " << val << std::endl;
                if (val == targetValue) {
                    //std::cout << "Found target " << targetValue << " at offset 0x" << std::hex << offset << std::dec << std::endl;
                    log::info(fmt::format("Found target at offset 0x{}", offset));;
                    break;
                }
            }
        //}*/
        // yes letes overwrite fields!
        // plesae commit add paddings for android64 and windows kthx
        if (!Hacks::isHackEnabled("Layout Mode")) return GameObject::setVisible(v);
        int objectID = -1;
        GameObjectType objectType;
        int objectTypeInt = -1;
#ifdef GEODE_IS_WINDOWS
        objectID = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x384); // absolutely cursed
        objectType = *reinterpret_cast<GameObjectType*>(reinterpret_cast<uintptr_t>(this) + 0x31c);
#else
        objectID = this->m_objectID;
        objectType = this->m_objectType;
#endif
        GameObject::setVisible(v);
        if (objectType == GameObjectType::Decoration && objectID != 44) { // 44 being practice mode checkpoint, because thats a "decoration"
            GameObject::setVisible(false);
        } else {
            GameObject::setVisible(v);
        }
    }
    /*
    void objectFromVector(gd::vector<gd::string> &p0, gd::vector<void *> &p1, GJBaseGameLayer *p2, bool p3) {
        return GameObject::objectFromVector(p0,p1,p2,p3);
    }*/ // someone got the wrong bindings!
    /*
    

    */
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

class $modify(CCTransitionFade) {
    // No Transition
    bool initWithDuration(float t, cocos2d::CCScene* scene, cocos2d::ccColor3B const& color)  {
        if (!Hacks::isHackEnabled("No Transition") || Hacks::isHackEnabled("Enable Patching")) {
            return CCTransitionFade::initWithDuration(t, scene, color);
        } else {
            return CCTransitionFade::initWithDuration(0.0F, scene, color);
        }
    }
};
