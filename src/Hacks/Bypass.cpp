#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/CCTextInputNode.hpp>

// Icon Bypass
class $modify(GameManager) {
    bool isIconUnlocked(int _id, IconType _type) {
        if (!Hacks::isHackEnabled("Icon Bypass") || Hacks::isHackEnabled("Enable Patching")) return GameManager::isIconUnlocked(_id, _type);
        return true;
    }
    bool isColorUnlocked(int _id, UnlockType _type) {
        if (!Hacks::isHackEnabled("Icon Bypass") || Hacks::isHackEnabled("Enable Patching")) return GameManager::isColorUnlocked(_id, _type);
        return true;
    }
};

// Text Length, Character Filter
class $modify(CCTextInputNode) {
    void updateLabel(gd::string p0) {
        if (Hacks::isHackEnabled("Character Filter") && !Hacks::isHackEnabled("Enable Patching")) this->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,-!?:;)(/\\\"\'`*= +-_%[]<>|@&^#{}%$~");
        if (Hacks::isHackEnabled("Text Length") && !Hacks::isHackEnabled("Enable Patching")) this->m_maxLabelLength = -1;
        CCTextInputNode::updateLabel(p0);
    }
};

/*
class $modify(GameStatsManager) {
    // Almost all bypass
    int getStat(char const* type) { // isnt this supposed to be int
        int ret = GameStatsManager::getStat(type);
        int typeInt = std::stoi(type);
        //std::cout << fmt::format("type = {}, ret = {}", type, ret) << std::endl;
        if (typeInt == 8 && Hacks::isHackEnabled("Main Levels") && !Hacks::isHackEnabled("Enable Patching")) { // main level
            return 30;
        }
        if (typeInt == 12 && Hacks::isHackEnabled("Guard Vault") && !Hacks::isHackEnabled("Enable Patching")) { // guard vault
            return 10;
        }
        if (typeInt == 13 && Hacks::isHackEnabled("Keymaster Vault") && !Hacks::isHackEnabled("Enable Patching")) { // keymaster vault
            return 100;
        }
        if (typeInt == 21 && Hacks::isHackEnabled("Treasure Room") && !Hacks::isHackEnabled("Enable Patching")) { // treasure room
            return 5;
        }
        if (typeInt == 14 && Hacks::isHackEnabled("Free Shop Items") && !Hacks::isHackEnabled("Enable Patching")) { // free shop items
            return 1000000; // lol what
        }
        if (typeInt == 6 && Hacks::isHackEnabled("Backup Stars Limit") && !Hacks::isHackEnabled("Enable Patching")) { // backup stars limit
            return 10;
        }
        return ret;
    }
};
*/


