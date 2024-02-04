#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/CCTransitionFade.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/CCSprite.hpp>

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
        speedhack->value.floatValue = std::max(speedhack->value.floatValue, 0.01f);
        if (speedhack == nullptr) return CCScheduler::update(dt);
        if (speedhack->value.floatValue == 1.0F) return CCScheduler::update(dt);
        CCScheduler::update(dt * speedhack->value.floatValue);
    }
};
#endif



// Layout Mode
class $modify(GameObject) {
    // Layout Mode, No Glow
    void setVisible(bool v) {
        if (Hacks::isHackEnabled("No Glow")) { // easier than having to check for glow sprite lol
            this->m_hasNoGlow = true;
        }
        if (!Hacks::isHackEnabled("Layout Mode")) return GameObject::setVisible(v);
        GameObject::setVisible(v);
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
    std::string spr_name;
    bool initWithFile(char const* name) {
        if (!CCSprite::initWithFile(name)) return false;
        m_fields->spr_name = name;
        return true;
    }
    void setColor(const ccColor3B& color) {
        CCSprite::setColor(color);
        if (!Hacks::isHackEnabled("Transparent BG")) return;
        if (m_fields->spr_name == "GJ_gradientBG.png") {
            CCSprite::setColor({255,255,255});
        }
        /*
        // taken from devtools because why not
        if (auto texture = this->getTexture()) {
            auto* cachedTextures = CCTextureCache::sharedTextureCache()->m_pTextures;
            // probably a bad idea to loop, might cause some lag idk
            // honestly i should hook the create method instead to give more performance because this is BAD!
            for (auto [key, obj] : CCDictionaryExt<std::string, CCTexture2D*>(cachedTextures)) {
                if (obj == texture) {
                    if (strstr(key.c_str(), "GJ_gradientBG") != NULL) {
                        CCSprite::setColor({255,255,255}); // original color
                    }
                    break;
                }
            }
        }*/
    }
};
