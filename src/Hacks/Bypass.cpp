#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/modify/EditorUI.hpp>

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

#if 0
class $modify(EditorUI) {
    void onNewCustomItem(CCObject* sender) {
        /*
              if (((*(int *)(param_1 + 0x370) == 0) &&
          (iVar2 = cocos2d::CCArray::count(*(CCArray **)(param_1 + 0x260)), iVar2 == 0)) ||
         ((*(int *)(param_1 + 0x370) != 0 && (*(int *)(*(int *)(param_1 + 0x370) + 0x374) == 0x2ed))
         )) goto LAB_003a0ef6;
      iVar2 = cocos2d::CCArray::count(*(CCArray **)(param_1 + 0x260));
      if (iVar2 == 0) {
        this = (CCArray *)cocos2d::CCArray::create();
        cocos2d::CCArray::addObject(this,*(CCObject **)(param_1 + 0x370));
      }
      else {
        this = *(CCArray **)(param_1 + 0x260);
      }
      copyObjects((CCArray *)&local_24,SUB41(param_1,0),SUB41(this,0));
      if ((local_24[-3] != 0) &&
         (uVar1 = cocos2d::CCArray::count(*(CCArray **)(param_1 + 0x260)), uVar1 < 0x3e9)) {
        bVar6 = GameManager::sharedState();
        FUN_0075c7d0(&puStack_20,&local_24);
        GameManager::addNewCustomObject(bVar6);
        FUN_0075abb8((int *)&puStack_20);
        *(undefined4 *)(param_1 + 0x328) = 0;
        reloadCustomItems((EditorUI *)param_1);
      }
        */
        auto gm = GameManager::sharedState();
        CCDictionary* customObjects = gm->m_customObjectDict;
        //CCArray *local_24;
        //EditorUI::copyObjects(local_24, false, false); // SUB41(this,0)
        gm->addNewCustomObject(m_selectedObject->getSaveString(nullptr));
        EditorUI::reloadCustomItems();
        //EditorUI::onNewCustomItem(sender);
    }
};
#endif
