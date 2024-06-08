#pragma once
#include <Geode/Geode.hpp>
#include "UI/Intro.hpp"
#include "hacks.hpp"
#include "UI/PrismUI.hpp"
using namespace geode::prelude;
class PrismButton : public CCMenu {
protected:
    CCTouch* touchs;
    //CCMenuItemSpriteExtra* menuButton;
    bool allowDragging;
    bool isTouchOnButton;
    float initialTouchX;
    float initialTouchY;
    float touchHoldTimer;

    int currentTouchPrio;

    bool init(CCScene* p0) {
        if (!CCMenu::init())
            return false;
        HackItem* posX = Hacks::getHack("Button Position X");
        HackItem* posY = Hacks::getHack("Button Position Y");
        auto menuButton = createButton(this);
        menuButton->setID("menu-button");
        this->addChild(menuButton);
        this->setPositionX(posX->value.intValue);
        this->setPositionY(posY->value.intValue);
        this->registerWithTouchDispatcher();
        this->setTouchEnabled(true);
        this->setZOrder(p0->getHighestChildZ() + 100);
        allowDragging = true;
        isTouchOnButton = false;
        touchHoldTimer = 0.0f;
        //this->scheduleUpdate();
        currentTouchPrio = this->getTouchPriority();
        return true;
    }
public:
    // chat jippity 
#if 0 // fixing later, because chatgpt is givin gme bad results
    void update(float dt) {
        updateTouchHoldTimer(dt);
        // Check if the touch is on the button and has been held for 0.5 seconds
        if (isTouchOnButton && allowDragging && touchHoldTimer >= 0.5) {
            std::cout << "do you" << std::endl;
            // Calculate new position based on touch movement
            float deltaX = touchs->getDelta().x;
            float deltaY = touchs->getDelta().y;
            this->setPositionX(this->getPositionX() + deltaX);
            this->setPositionY(this->getPositionY() + deltaY);

            // Set opacity to 50%
            this->setOpacity(128); // 50% opacity

        } else {
            // If not dragging, set opacity back to 100%
            this->setOpacity(255); // 100% opacity
        }
    }


    bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        this->touchs = touch;
        std::cout << "ccTouchBegan" << std::endl;
        CCRect rect = menuButton->boundingBox();
        if (rect.containsPoint(touch->getLocation())) {
            std::cout << "ccTouchBegan you are TRUE!" << std::endl;
            isTouchOnButton = true;
            initialTouchX = touch->getLocation().x;
            initialTouchY = touch->getLocation().y;
            allowDragging = false; // Reset flag
            touchHoldTimer = 0.0F;
            return true;
        }

        return false;
    }

    void ccTouchMoved(CCTouch* touch, CCEvent* event) {
        // Check if the touch is on the button
        if (isTouchOnButton) {
            float distance = ccpDistance(ccp(initialTouchX, initialTouchY), touch->getLocation());
            std::cout << "ccTouchMoved " << distance << std::endl;
            if (distance > 10) {
                std::cout << "ccTouchBegan ya" << std::endl;
                allowDragging = true; // Enable dragging if the touch has moved by more than 10 pixels
            }
        }
    }

    void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        // Check if the touch is on the button
        if (isTouchOnButton) {
            std::cout << "ccTouchEnded" << std::endl;
            // If not dragging, call onButtonClicked
            if (!allowDragging) {
                onButtonClicked(nullptr);
            }

            // Reset flags and properties
            isTouchOnButton = false;
            allowDragging = false;
            touchHoldTimer = 0.0F;
        }
    }

    void updateTouchHoldTimer(float dt) {
        if (isTouchOnButton) {
            std::cout << "updateTouchHoldTimer " << touchHoldTimer << std::endl;
            touchHoldTimer += dt;
        }
    }
#endif
    bool showImGuiMenu = false;
    static CCMenuItemSpriteExtra* createButton(CCLayer* ret) {
        auto myButtonSpr = CircleButtonSprite::create(CCSprite::create("icon.png"_spr));
        return CCMenuItemSpriteExtra::create(myButtonSpr, ret, menu_selector(PrismButton::onButtonClicked));
    }
    void onButtonClicked(CCObject* p0) {
        HackItem* menuStyle = Hacks::getHack("Menu-Style");
        #ifdef NO_IMGUI
        menuStyle->value.intValue = 1;
        #endif
        if (menuStyle->value.intValue == 0) { // imgui
            showImGuiMenu = !showImGuiMenu;
        } else {
            PrismUI::create()->show();
            /*if (Mod::get()->getSettingValue<bool>("skip-intro")) {
                PrismUI::create()->show();
            } else {
                IntroUI::create()->show();
            }*/
        }
    }
    void resetTouchPriority(bool alsoChild, bool force) {
        cocos::handleTouchPriority(this, force);
        if (alsoChild) {
            if (auto menuItem = typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByID("menu-button"))) {
                cocos::handleTouchPriority(menuItem, force);
            }
        }
    }
    void switchTouchPrio() {
        if (this->getTouchPriority() != currentTouchPrio) {
            this->setTouchPriority(currentTouchPrio);
        } else {
            this->setTouchPriority(-1024);
        }
    }

    static PrismButton* create(CCScene* p0) {
        auto ret = new PrismButton();
        if (ret && ret->init(p0)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
