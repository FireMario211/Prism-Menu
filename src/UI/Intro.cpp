#include "Intro.hpp"
#include "Dropdown.h"
#include "PrismUI.hpp"

IntroUI* IntroUI::create() {
    auto pRet = new IntroUI();
    if (pRet) {
        pRet->showCloseBtn = false;
        if (pRet->init(IntroUI::s_defWidth, IntroUI::s_defHeight)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};

void IntroUI::keyDown(cocos2d::enumKeyCodes key) {
    //keyPressed(key);
    if (key == cocos2d::enumKeyCodes::KEY_Escape)
        return;
    if (key == cocos2d::enumKeyCodes::KEY_Tab)
        return;
    if (key == cocos2d::enumKeyCodes::KEY_Space)
        return;
    return FLAlertLayer::keyDown(key);
}

void IntroUI::setup() {
    this->setID("prism-intro");
    m_items = CCMenu::create();
    auto title = CCLabelBMFont::create("Setup", "bigFont.fnt");
    auto desc = CCLabelBMFont::create("Would you like to skip the intro?", "chatFont.fnt");
    title->setPositionY((s_defHeight / 3));

    auto skipBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Skip", 0, 0, "bigFont.fnt", "GJ_button_01.png", 0, .6f),
        this,
        menu_selector(IntroUI::onSkip)
    );
    auto continueBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Continue", 0, 0, "bigFont.fnt", "GJ_button_01.png", 0, .6f),
        this,
        menu_selector(IntroUI::onContinue)
    );

    skipBtn->setPosition({-(s_defWidth / 4), -(s_defHeight / 3)});

    continueBtn->setPosition({(s_defWidth / 4.5), -(s_defHeight / 3)});
    this->m_buttonMenu->addChild(title);
    this->m_buttonMenu->addChild(desc);
    this->m_buttonMenu->addChild(skipBtn);
    this->m_buttonMenu->addChild(continueBtn);
}

void IntroUI::onContinue(CCObject*) {
    m_buttonMenu->removeAllChildrenWithCleanup(true);
    auto title = CCLabelBMFont::create("Setup", "bigFont.fnt");
    title->setPositionY((s_defHeight / 3));
    this->m_buttonMenu->addChild(title);

    auto lang = Hacks::getHack("Language");
    auto langLabel = CCLabelBMFont::create("Language", "chatFont.fnt");
    langLabel->setPositionY(25);
    auto values = lang->data.get<matjson::Array>("values");
    auto langBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create(
            values[lang->value.intValue].as_string().c_str(),
            100.0f,
            true,
            "PrismMenu.fnt"_spr,
            "GJ_button_04.png",
            30.0f,
            0.4f
        ),
        this,
        nullptr
    );
    langBtn->setPositionY(-2);
    this->m_buttonMenu->addChild(langBtn);
    this->m_buttonMenu->addChild(langLabel);

    auto posXLabel = CCLabelBMFont::create("Button Position X", "chatFont.fnt");
    auto posYLabel = CCLabelBMFont::create("Button Position Y", "chatFont.fnt");
    posXLabel->setScale(.8F);
    posYLabel->setScale(.8F);
    posXLabel->setPosition({s_defWidth / 4,-35});
    posYLabel->setPosition({s_defWidth / 4,-55});
    this->m_buttonMenu->addChild(posXLabel);
    this->m_buttonMenu->addChild(posYLabel);

    auto posX = Hacks::getHack("Button Position X");
    if (posX != nullptr) {
        auto posX_slider = Slider::create(this, nullptr, .6f);
        auto posX_value = posX->value.intValue;

        posX_slider->setPosition({(-(s_defWidth / 4)) + 10,-35});
        posX_slider->setValue(Utils::getSliderValue(posX_value, posX->data.get<int>("min"), posX->data.get<int>("max"), false));
        this->m_buttonMenu->addChild(posX_slider);
    }

    auto posY = Hacks::getHack("Button Position Y");
    if (posY != nullptr) {
        auto posY_slider = Slider::create(this, nullptr, .6f);
        auto posY_value = posY->value.intValue;
        posY_slider->setPosition({(-(s_defWidth / 4)) + 10,-55});
        posY_slider->setValue(Utils::getSliderValue(posY_value, posY->data.get<int>("min"), posY->data.get<int>("max"), false));
        this->m_buttonMenu->addChild(posY_slider);
    }
}

void IntroUI::onLang(CCObject*) {
    auto lang = Hacks::getHack("Language");
    auto values = lang->data.get<matjson::Array>("values");
    DropdownPopup::create(values, lang)->show();
}

void IntroUI::onSkip(CCObject*) {
    onClose(nullptr);
    PrismUI::create()->show();
    Mod::get()->setSettingValue("skip-intro", true);
}
