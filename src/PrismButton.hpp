#pragma once
#include <Geode/Geode.hpp>
#include "UI/Intro.hpp"
#include "hacks.hpp"
#include "UI/PrismUI.hpp"
using namespace geode::prelude;

// definitely not copied from eclipse!
class PrismButton : public CCMenu {
protected:
#ifndef GEODE_IS_IOS
    // how many units required to begin dragging the button
    constexpr static float MIN_MOVE_DISTANCE = 30.f;
    // move progress per second
    constexpr static float MOVE_SPEED = 9.2f;
    // how many units until the button snaps to touch pos
    constexpr static float SNAP_MARGIN = 0.1f;
#else
    // i do not know why but the numbers are vastly different on ios
    constexpr static float MIN_MOVE_DISTANCE = 2.5f;
    constexpr static float MOVE_SPEED = 9.2f;
    constexpr static float SNAP_MARGIN = 40.f;
#endif
    cocos2d::CCPoint m_holdPosition{};
    bool m_shouldMove = false; // whether currently in move animation
    bool m_haveMoved = false; // used to determine whether we should trigger callback after release
    CCSprite* m_button;
    int currentTouchPrio = 0;

    bool init(CCScene* p0) {
        if (!CCMenu::init())
            return false;
        if (!p0)
            return false;
        HackItem* posX = Hacks::getHack("Button Position X");
        HackItem* posY = Hacks::getHack("Button Position Y");
        m_button = CircleButtonSprite::create(CCSprite::create("icon.png"_spr));
        m_button->setID("menu-button");
        this->addChild(m_button);
        m_button->setPositionX(posX->value.intValue);
        m_button->setPositionY(posY->value.intValue);
        this->registerWithTouchDispatcher();
        this->setTouchEnabled(true);
        this->setID("prism-icon");
        currentTouchPrio = this->getTouchPriority();
        this->scheduleUpdate();
        return true;
    }
    void update(float) override {
        this->setPosition({0, 0});
        auto dt = CCDirector::get()->getActualDeltaTime();
        if (!isEnabled() && m_button->getOpacity() == 255) {
            m_button->setOpacity(100);
        }
        if (isEnabled() && m_button->getOpacity() != 255) {
            m_button->setOpacity(255);
        }
        if (!m_shouldMove) return;
        auto currentPos = m_button->getPosition();

        // if very close to the end, speed up
        if (ccpDistance(currentPos, m_holdPosition) < SNAP_MARGIN * 10.f) {
            dt *= 2.f;
        }

        auto newPoint = ccpLerp(currentPos, m_holdPosition, MOVE_SPEED * dt);
        if (ccpDistance(newPoint, m_holdPosition) < SNAP_MARGIN) {
            newPoint = m_holdPosition;
            m_shouldMove = false; // finished moving
        }

        m_button->setPosition(newPoint);
        if (!m_shouldMove) {
            log::debug("Set new button position: {}", m_button->getPosition());
            HackItem* posX = Hacks::getHack("Button Position X");
            HackItem* posY = Hacks::getHack("Button Position Y");
            posX->value.intValue = m_button->getPositionX();
            posY->value.intValue = m_button->getPositionY();
            auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
            Hacks::Settings::setSettingValue(&settings, *posX, posX->value.intValue);
            Hacks::Settings::setSettingValue(&settings, *posY, posY->value.intValue);
        }
        // where you set the pos stuff
    }
public:
    float getRadius() {
        return m_button->getScaledContentSize().width / 2.f;
    }
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
        if (!m_button->isVisible() || !this->isVisible() || !isEnabled()) return false;
        auto touchPos = this->convertToNodeSpace(touch->getLocation());
        auto spritePos = m_button->getPosition();
        if (ccpDistance(touchPos, spritePos) > getRadius())
            return false;

        m_haveMoved = false;
        m_holdPosition = touchPos;
        m_button->stopAllActions();
        m_button->runAction(CCScaleTo::create(0.2f, 0.8F));
        return true;
    }
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
        m_button->runAction(CCScaleTo::create(0.2f, 1.0F));
        if (m_haveMoved) return;
        onButtonClicked(nullptr);
    }
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
        auto touchPos = this->convertToNodeSpace(touch->getLocation());
        if (!m_shouldMove && ccpDistance(touchPos, m_holdPosition) < MIN_MOVE_DISTANCE)
            return;

        m_shouldMove = true;
        m_haveMoved = true;
        m_holdPosition = touchPos;
    }
    void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
        this->ccTouchEnded(touch, event);
    }
    bool showImGuiMenu = false;
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
    static PrismButton* create(CCScene* p0) {
        auto ret = new PrismButton();
        if (ret && ret->init(p0)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    void setSpritePos(CCPoint pos) {
        m_button->setPosition(pos);
    }
    void setSpritePosX(float x) {
        m_button->setPositionX(x);
    }
    void setSpritePosY(float y) {
        m_button->setPositionY(y);
    }
    static CCMenuItemSpriteExtra* createButton(CCLayer* ret) {
        auto myButtonSpr = CircleButtonSprite::create(CCSprite::create("icon.png"_spr));
        return CCMenuItemSpriteExtra::create(myButtonSpr, ret, menu_selector(PrismButton::onButtonClicked));
    }
};
