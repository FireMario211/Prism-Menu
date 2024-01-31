#include "GatoSim.hpp"
// username8282_8 on Discord

// TODO: add a popup asking for name, and what sprite

// definitely not copied off a mod that i was working on with someone *cough cough* 
bool GatoSim::init() {
    auto size = cocos2d::CCDirector::sharedDirector()->getWinSize();
    auto background = CCSprite::create("GJ_gradientBG.png");
    auto backgroundSize = background->getContentSize();

    background->setScaleX(size.width / backgroundSize.width);
    background->setScaleY(size.height / backgroundSize.height);
    background->setAnchorPoint({0, 0});
    background->setColor({70, 40, 90});
    background->setZOrder(-1);
    this->addChild(background);

    // Buttons
    auto closeBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        (cocos2d::SEL_MenuHandler)&GatoSim::onClose
    );
    CCMenu* menu = CCMenu::create();
    menu->addChild(closeBtn);
    menu->setPosition({ 25, size.height - 25 });
    this->addChild(menu);

    auto name = CCLabelBMFont::create("GATO", "bigFont.fnt");
    name->limitLabelWidth(80, 1.0F, 0.25F);
    name->setPosition({ size.width / 2, size.height - 40 });
    this->addChild(name);


    auto otherMenu = CCMenu::create();
    auto gato = CCSprite::create("gato.jpg"_spr);
    
    float rotateBy = 15;
    float moveBy = 15;
    float speed = 0.25;

    auto bouncy = CCSequence::create(
        CCSpawn::create(
            CCMoveBy::create(speed, {moveBy, moveBy}),
            CCRotateBy::create(speed, rotateBy),
            nullptr
        ),
        CCSpawn::create(
            CCMoveBy::create(speed, {-moveBy, -moveBy}),
            CCRotateBy::create(speed, -rotateBy),
            nullptr
        ),
        CCSpawn::create(
            CCMoveBy::create(speed, {-moveBy, moveBy}),
            CCRotateBy::create(speed, -rotateBy),
            nullptr
        ),
        CCSpawn::create(
            CCMoveBy::create(speed, {moveBy, -moveBy}),
            CCRotateBy::create(speed, rotateBy),
            nullptr
        ),
        nullptr
    );
    gato->runAction(CCRepeatForever::create(bouncy));

    auto thecenterofattention = CCMenuItemSpriteExtra::create(gato, this, nullptr);
    otherMenu->setPosition({ size.width / 2, size.height / 2});
    otherMenu->addChild(thecenterofattention);
    this->addChild(otherMenu);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    return true;
};

void GatoSim::keybackClicked() {
    cocos2d::CCDirector::sharedDirector()->popSceneWithTransition(0.5F, cocos2d::PopTransition::kPopTransitionFade);
};

void GatoSim::onClose(cocos2d::CCObject*) {
    GatoSim::keybackClicked();
};



void GatoSim::onButton() {
    auto scene = CCScene::create();
    auto layer = GatoSim::create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
}

GatoSim* GatoSim::create() {
    auto pRet = new GatoSim();
    if (pRet) {
        if (pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};
