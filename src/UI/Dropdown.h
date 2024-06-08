// code from https://github.com/TheSillyDoggo/GD-Discord-Webhook-Integration
// sorta fixed up because why do you need reinterpret_cast!? its unsafe!
// maybe i should move it to both hpp and cpp
#pragma once
#include "PrismUI.hpp"
#include "../hacks.hpp"
using namespace geode::prelude;

class Dropdown : public CCMenu {
    public:
        int selected = 0;
        CCMenu* menu;
        std::vector<matjson::Value> strs;
        CCLabelBMFont* lbl;

        void onToggle(CCObject* sender);
        static Dropdown* create(std::vector<matjson::Value> strs, HackItem* item, cocos2d::SEL_MenuHandler callback, cocos2d::SEL_MenuHandler callback2);
};

class DropdownPopup : public FLAlertLayer {
    protected:
        //CCScrollLayerExt* m_scrollLayer;
        ScrollLayer* m_list;
        CCMenu* m_content;
        HackItem* m_hack;
        std::vector<matjson::Value> m_values;
        virtual bool init(std::vector<matjson::Value> strs, HackItem* item);
        virtual void keyDown(cocos2d::enumKeyCodes key) override;
        virtual void keybackClicked() {
            onClose(nullptr);
        };
    public:
        virtual void onClose(cocos2d::CCObject*) {
            if (auto prismMenu = typeinfo_cast<PrismUI*>(CCScene::get()->getChildByID("prism-menu"))) {
                prismMenu->toggleVisibility();
            }
            this->removeFromParentAndCleanup(true);
        };
        static DropdownPopup* create(std::vector<matjson::Value> strs, HackItem* item) {
            auto pRet = new DropdownPopup();
            if (pRet) {
                if (pRet->init(strs, item)) {
                    pRet->autorelease();
                    return pRet;
                }
            }
            CC_SAFE_DELETE(pRet);
            return nullptr;
        };
};
