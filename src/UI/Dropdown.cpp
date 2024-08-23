#include "Dropdown.h"
#include "../Themes.hpp"
#include "../Utils.hpp"

void Dropdown::onToggle(CCObject* sender) {
    auto obj = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto parent = obj->getParent();
    auto ddmenu = parent->getChildByID("dropdown-menu");
    bool expanded = !ddmenu->isVisible();
    #ifdef GEODE_IS_WINDOWS
    obj->runAction(CCEaseBackOut::create(CCScaleTo::create(0.5f, 0.75f, (!expanded ? -0.75f : 0.75f))));
    #else
    #ifdef GEODE_IS_ANDROID
    obj->runAction(CCScaleTo::create(0.5f, 0.75f, (!expanded ? -0.75f : 0.75f)));
    #endif
    #endif

    auto background = parent->getChildByID("background");

    int h = ddmenu->getChildrenCount() + 1;
    h = expanded ? h : 1;
    ddmenu->setVisible(expanded);
    ddmenu->setZOrder(3);
    background->setContentSize({background->getContentSize().width, (25 / background->getScale()) * h});
    auto menuButtons = static_cast<CCMenu*>(parent->getParent()->getParent()->getParent());
    auto hack = static_cast<HackItem*>(parent->getUserData());
    for (int i = 0; i < menuButtons->getChildrenCount(); i++) {
        auto objN = menuButtons->getChildren()->objectAtIndex(i);
        if (Utils::getNodeName(objN) == "PrismUIButton") {
            auto prismUIButton = static_cast<PrismUIButton*>(objN);
            auto pHack = prismUIButton->getHack();
            if (pHack == hack) {
                prismUIButton->setZOrder((expanded) ? 1 : 0);
                continue;
            };
            auto pMenu = prismUIButton->getMenu();
            if (pHack->type == "bool") {
                static_cast<CCMenuItemToggler*>(pMenu->getChildren()->objectAtIndex(1))->setEnabled(!expanded);
            } else if (pHack->type == "button") {
                static_cast<CCMenuItemSpriteExtra*>(pMenu->getChildren()->objectAtIndex(0))->setEnabled(!expanded);
            } else if (pHack->type == "float" || pHack->name.starts_with("Button Pos")) {
                prismUIButton->getInputNode()->setEnabled(!expanded);
                prismUIButton->getSlider()->getThumb()->setEnabled(!expanded);
                static_cast<CCMenuItemSpriteExtra*>(pMenu->getChildren()->objectAtIndex(0))->setEnabled(!expanded);
            } else if (pHack->type == "dropdown") {
                auto otherDDMenu = static_cast<CCMenu*>(pMenu->getChildren()->objectAtIndex(1));
                static_cast<CCMenuItemSpriteExtra*>(otherDDMenu->getChildByID("flip-btn"))->setEnabled(!expanded);
            }
        }
    }
}
Dropdown* Dropdown::create(std::vector<matjson::Value> strs, HackItem *item, cocos2d::SEL_MenuHandler callback, cocos2d::SEL_MenuHandler callback2) {
    Dropdown* dd = new Dropdown();
    dd->strs = strs;

    CCSize size = {240, 25};

    CCMenu* menu = CCMenu::create();
    menu->ignoreAnchorPointForPosition(false);

    menu->setUserData(reinterpret_cast<void*>(item));
    menu->setContentSize(size);

    //auto background = CCScale9Sprite::create("GJ_square01.png");
    auto background = CCScale9Sprite::create("square02b_001.png");
    background->setScale(0.3f);
    background->setContentSize({size.width / background->getScale(), size.height / background->getScale()});
    background->setAnchorPoint({0, 1});
    background->setPosition(0, size.height);
    background->setID("background");
    //background->setOpacity(100);
    //background->setColor({0, 0, 0});
    Themes::RGBAToCC(Themes::getCurrentTheme()["BG"], background);
    menu->addChild(background);

    auto spr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
    spr->setScale(0.9f);

    auto arrowBtn = CCMenuItemSpriteExtra::create(spr, menu, (callback2 == nullptr) ? menu_selector(Dropdown::onToggle) : callback2);
    arrowBtn->setPosition(size.width - 15, (25.F / 2.F));
    arrowBtn->setID("flip-btn");
    arrowBtn->setScale(0.75f);
    arrowBtn->setScaleY(-0.75f);
    arrowBtn->setUserData(reinterpret_cast<void*>(dd));
    menu->addChild(arrowBtn);
    auto lbl = CCLabelBMFont::create(strs[item->value.intValue].as_string().c_str(), "PrismMenu.fnt"_spr);
    Themes::RGBAToCC(Themes::getCurrentTheme()["Text"], lbl);
    lbl->setScale(0.5f);
    lbl->setPosition({size.width / 2 - (arrowBtn->getScaledContentSize().width / 2) - 5, size.height / 2});
    lbl->limitLabelWidth(145, 0.6f, 0);
    lbl->setID("selected-label");
    dd->lbl = lbl;
    menu->addChild(lbl);

    auto btnMenu = CCMenu::create();
    btnMenu->setAnchorPoint({0, 1});
    //btnMenu->setPosition({0, size.height * strs.size()});
    //btnMenu->setPosition({0, size.height + 25});
    int strAmount = strs.size();
    btnMenu->setPosition({0, static_cast<float>(25 * strAmount - 50)});
    btnMenu->setContentSize({size.width, size.height * strs.size()});
    btnMenu->ignoreAnchorPointForPosition(false);
    btnMenu->setVisible(false);
    btnMenu->setID("dropdown-menu");

    for (size_t i = 0; i < strs.size(); i++)
    {

        auto lbl = CCLabelBMFont::create(strs[i].as_string().c_str(), "PrismMenu.fnt"_spr);
        Themes::RGBAToCC(Themes::getCurrentTheme()["Text"], lbl);
        lbl->limitLabelWidth(size.width - 20, 0.6f, 0);

        auto btn = CCMenuItemSpriteExtra::create(lbl, menu, callback);
        btn->setUserData(reinterpret_cast<void*>(item));
        btn->setID(fmt::format("dropdown-item-{}", i));
        btn->setPosition({size.width / 2, (background->getScaledContentSize().height - (size.height * i)) + size.height / 2});
        btn->setZOrder(3);
        btnMenu->addChild(btn);
    }

    menu->addChild(btnMenu);
    menu->registerWithTouchDispatcher();
    cocos::handleTouchPriority(menu);
    menu->setTouchEnabled(true);

    dd->menu = menu;
    return dd;
}

bool DropdownPopup::init(std::vector<matjson::Value> strs, HackItem* item) {
    m_hack = item;
    m_values = strs;
    if (CCScene::get() != nullptr) {
        if (auto prismMenu = typeinfo_cast<PrismUI*>(CCScene::get()->getChildByID("prism-menu"))) {
            prismMenu->toggleVisibility();
        }
    }
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto title = "Select Language";
    return true;
}

void DropdownPopup::keyDown(cocos2d::enumKeyCodes key) {
    if (key == cocos2d::enumKeyCodes::KEY_Escape)
        return onClose(nullptr);
    return FLAlertLayer::keyDown(key);
};

