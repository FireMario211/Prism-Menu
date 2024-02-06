// Prism Menu by Firee
// i am looking at you. make sure you credit me and this mod, i worked hard on it ;-;
// please do not judge my coding, ok thank you!!!
#include "CustomSettings.hpp"
#include <Geode/utils/file.hpp>
#include "hacks.hpp"
#include "Languages.hpp"
#include "Themes.hpp"
#include "PrismUI.hpp"
#include "Utils.hpp"
#include <Geode/Geode.hpp>
//#include <geode.custom-keybinds/include/Keybinds.hpp>
//using namespace keybinds;

#include <iomanip>
#include <string>
#include <locale>
#include <codecvt>
#include <algorithm>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/AchievementNotifier.hpp>


std::string determineName(const char* name, float width) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; // what is this nonsense chat jippity, i just wanna handle unicode!

    std::string nameStr(name);
    if (width <= 125) {
        if (width <= 80) {
            std::wstring wideName = converter.from_bytes(name);
            return converter.to_bytes(wideName.substr(0, 1));
        } else {
            return nameStr.substr(2);
        }
    } else {
        return nameStr;
    }
}

/*
for i in range(char2nr('A'), char2nr('Z'))
  call append(line("."), printf("{'%c', cocos2d::KEY_%c},", i, i))
endfor
*/

class PrismButton : public CCMenu {
protected:
    CCTouch* touchs;
    //CCMenuItemSpriteExtra* menuButton;
    bool allowDragging;
    bool isTouchOnButton;
    float initialTouchX;
    float initialTouchY;
    float touchHoldTimer;

    bool init(CCScene* p0) {
        if (!CCMenu::init())
            return false;
        HackItem* posX = Hacks::getHack("Button Position X");
        HackItem* posY = Hacks::getHack("Button Position Y");
        auto menuButton = createButton(this);
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
    static CCMenuItemSpriteExtra* createButton(CCLayer* ret) {
        auto myButtonSpr = CircleButtonSprite::create(CCSprite::create("icon.png"_spr));
        return CCMenuItemSpriteExtra::create(myButtonSpr, ret, menu_selector(PrismButton::onButtonClicked));
    }
    void onButtonClicked(CCObject* p0) {
        PrismUI::create()->show();
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

PrismButton* prismButton;


bool firstLoad = false;
bool changedOpacity = false;
bool changedMenuScale = false;
// early load is amazing!

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        /*auto username = static_cast<CCLabelBMFont*>(this->getChildByID("player-username"));
        if (username != nullptr) {
            auto normal = CCSprite::createWithSpriteFrameName("difficulty_02_btn_001.png");
            normal->setPosition({44,91});
            normal->setScale(2.0F);
            this->addChild(normal);
            username->setString("FIRE IN THE HOLE");
        }*/
        // lmao
        HackItem* posX = Hacks::getHack("Button Position X");
        HackItem* posY = Hacks::getHack("Button Position Y");
        auto mainMenu = static_cast<CCMenu*>(this->getChildByID("bottom-menu"));
        if (firstLoad) return true;
        firstLoad = true;
        log::info("Prism Menu loaded! Enjoy the mod.");
#ifndef GEODE_IS_MACOS
        prismButton = PrismButton::create(CCScene::get());
        prismButton->setVisible(Hacks::isHackEnabled("Show Button"));
        prismButton->setID("prism-icon");
        SceneManager::get()->keepAcrossScenes(prismButton);
#endif
        return true;
    }
};

// maybe this will fix the issue
$execute {
    SettingHackStruct val { matjson::Array() };
    Mod::get()->addCustomSetting<SettingHackValue>("values", val);
}
$on_mod(Loaded) {
    Hacks::processJSON(false);
    Themes::addToCurrentThemes();
    //auto getKeybindHack = Hacks::getHack("Open Menu Keybind");
    //char inputKeybind = 'C';
    //if (getKeybindHack != nullptr) inputKeybind = *(getKeybindHack->value.charValue);
    /*¶ Settings
    BindManager::get()->registerBindable({
        "prism-menu"_spr,
        "Toggle Prism Menu",
        "Keybind for toggling when to show Prism Menu's Mod Menu.",
        { Keybind::create(KEY_C, Modifier::None) },
        "Prism Menu/ModMenu"
    });

    new EventListener([=](InvokeBindEvent* event) {
        if (event->isDown()) {
            // do a backflip!
            showMenu = !showMenu;
        }
        return ListenerResult::Propagate;
    }, InvokeBindFilter(nullptr, "prism-menu"_spr));
    */
}

// remove when custom keybinds is fixed
#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool arr) {
        if (down && (key == KEY_Tab)) {
            auto prismUIExists = CCScene::get()->getChildByID("prism-menu");
            if (prismUIExists == nullptr) {
                PrismUI::create()->show();
            } else {
                static_cast<PrismUI*>(prismUIExists)->onClose(CCNode::create());
            }
            return true;
        }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, arr);
    }
};

// sorry, not sorry. for some reason the allHacks vector isnt being initialized in the other .cpp file
// i completely wasted my time writing this whole patch script, and i kinda want android + mac support soooo

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
// TODO later: move these
class $modify(GJBaseGameLayer) {
#ifndef GEODE_IS_MACOS
    // No Mirror Transition, Instant Mirror Portal
    void toggleFlipped(bool p0, bool p1) { // i spent a lot of time figuring out why CCActionTween wont hook, only to realize that p1 instantly transitions it
        if (Hacks::isHackEnabled("Enable Patching")) return GJBaseGameLayer::toggleFlipped(p0, p1);
        /*
pCVar2 = (CCActionInterval *)cocos2d::CCActionTween::create((float)uVar8,(char *)0x3f000000,(float)((ulonglong)uVar8 >> 0x20),fVar7);
        */
        // MOV DWORD PTR [ESP], 3F
        // ??* esp = 0x3F;
        if (!Hacks::isHackEnabled("No Mirror Transition")) GJBaseGameLayer::toggleFlipped(p0, Hacks::isHackEnabled("Instant Mirror Portal"));
    }
#endif
    // Speedhack fix
#ifdef GEODE_IS_WINDOWS // sorry its weird on android
    void applyTimeWarp(float speed) {
        HackItem* speedhack = Hacks::getHack("Speedhack");
        if (speedhack == nullptr) return GJBaseGameLayer::applyTimeWarp(speed);
        if (speedhack->value.floatValue == 1.0F) return GJBaseGameLayer::applyTimeWarp(speed);
        GJBaseGameLayer::applyTimeWarp(speed * speedhack->value.floatValue);
    }
#endif
};
// showing the icon for android users lol
class $modify(PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        for (size_t i = 0; i < this->getChildrenCount(); i++) {
            auto obj = this->getChildren()->objectAtIndex(i);
            if (Utils::getNodeName(obj) == "cocos2d::CCMenu") {
                auto menu = static_cast<CCMenu*>(obj);
                auto button = PrismButton::createButton(this);
                button->setPositionX(-240);
                menu->addChild(button);
                break;
            }
        }
        
    }
};

CircleButtonSprite* createCheatIndicator(bool isHacking) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto cheatIndicator = CircleButtonSprite::create(CCNode::create(), (isHacking) ? CircleBaseColor::Pink : CircleBaseColor::Green, CircleBaseSize::Tiny); //, CircleBaseColor::Geode
    cheatIndicator->setAnchorPoint({1,1});
    cheatIndicator->setPosition({winSize.width, winSize.height});
    return cheatIndicator;
}
class $modify(PlayLayer) {
    float previousPositionX = 0.0F;
    GameObject* antiCheatObject; // removing after lol
    CircleButtonSprite* cheatIndicator;
    bool previousTestMode;

    CCSprite* progressBar;
    CCLabelBMFont* percentLabel;
    CCLabelBMFont* attemptLabel;

    bool isCheating = false;
    bool initedDeath = false;
    int updateInit = 0;
    // Anticheat Bypass, Noclip, No Spikes, No Solids
    void destroyPlayer(PlayerObject *p0, GameObject *p1) {
        //bool m_isTestMode = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x413);
        //std::cout << m_isTestMode << std::endl;
        if (Hacks::isHackEnabled("Enable Patching")) return PlayLayer::destroyPlayer(p0, p1);
        bool noclipDisabled = !Hacks::isHackEnabled("No Solids") && !Hacks::isHackEnabled("Noclip");
        if ((noclipDisabled && !Hacks::isHackEnabled("No Spikes"))) return PlayLayer::destroyPlayer(p0, p1);
        if (Hacks::isHackEnabled("Anticheat Bypass")) {
            if (!m_fields->initedDeath) {
            #if !defined(GEODE_IS_ANDROID64) && !defined(GEODE_IS_MACOS)
            if (m_fields->antiCheatObject == nullptr && p1 != nullptr && (
                (p1->m_realXPosition == 0 && p1->m_realYPosition == p0->m_realYPosition) ||
                (p1->m_realXPosition == 0 && p1->m_realYPosition == p0->m_realYPosition) // todo, get player pos during PlayLayer::init
            )) { // thank you whamer100
                m_fields->antiCheatObject = p1;
                m_fields->initedDeath = true;
            } else if (!m_fields->initedDeath && m_fields->antiCheatObject == nullptr) {
                m_fields->antiCheatObject = p1;
                m_fields->initedDeath = true;
            }
            #else //sorry android64 users, someone has to implement fields for GameObject, because I get errors 
                if (!m_fields->initedDeath && m_fields->antiCheatObject == nullptr) {
                    m_fields->antiCheatObject = p1;
                    m_fields->initedDeath = true;
                }
                // bad coding
                /*
                for (int offset = 0x0; ; offset += 0x1) {
                    GameObject* val = reinterpret_cast<GameObject*>(reinterpret_cast<uintptr_t>(this) + offset);
                    if (val == p1) {
                        log::debug("Found Anticheat Object at offset 0x{}", offset);
                        m_fields->antiCheatObject = p1;
                        break;
                    }
                }
                //m_antiCheatObject = p1;
                m_fields->initedDeath = true;*/
            #endif
            }
            if (p1 == m_fields->antiCheatObject) { // nice AC robert
                return PlayLayer::destroyPlayer(p0, p1);
            }
        }
    }
    // Instant Complete, Practice Music, Hide Testmode
    bool init(GJGameLevel *p0, bool p1, bool p2) {
        if (!PlayLayer::init(p0,p1,p2)) return false;
        if (prismButton != nullptr && Hacks::isHackEnabled("Show Button")) prismButton->setVisible(false);
        if (Hacks::isHackEnabled("Instant Complete")) {
            PlayLayer::playEndAnimationToPos({2,2});
        }
        /*
        if (Hacks::isHackEnabled("Show Hitboxes")) {
            PlayLayer::toggleDebugDraw(false);
        }
        */
        
        // 0xaa9
        int targetValue = true;
        /*for (int offset = 0x0; offset < 0xAAAA; offset += 0x1) {
            int val = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offset);
            if (val == 0 || val == 1) {
            //std::cout << "Offset: 0x" << std::hex << offset << std::dec << ", Value: " << val << std::endl;
            }
            /\*if (val == targetValue) {
                std::cout << "Found target " << targetValue << " at offset 0x" << std::hex << offset << std::dec << std::endl;
                break;
            }*\/
        }*/
        #ifndef GEODE_IS_MACOS
        m_fields->previousTestMode = m_isTestMode;
        #endif
        if (Hacks::isHackEnabled("Level Edit")) {
            //m_fields->m_gameLevel->m_levelType = static_cast<GJLevelType>(2);
        }
        if (Hacks::isHackEnabled("Hide Testmode")) {
            for (size_t i = 0; i < this->getChildrenCount(); i++) { // i cant test if this works, bruh
                auto obj = this->getChildren()->objectAtIndex(i);
                if (Utils::getNodeName(obj) == "cocos2d::CCLabelBMFont") {
                    auto testModeLabel = static_cast<CCLabelBMFont*>(obj);
                    if (!strcmp(testModeLabel->getString(), "Testmode")) {
                        testModeLabel->setVisible(false);
                        break;
                    }
                }
            }
        }
        if (Hacks::isHackEnabled("Practice Music")) {
            GameStatsManager::sharedState()->toggleEnableItem(UnlockType::GJItem, 17, true);
        }
        m_fields->cheatIndicator = createCheatIndicator(false);
        m_fields->cheatIndicator->setVisible(Hacks::isHackEnabled("Cheat Indicator"));
        this->addChild(m_fields->cheatIndicator);
        return true;
    }
    void onQuit() {
        if (prismButton != nullptr && Hacks::isHackEnabled("Show Button")) prismButton->setVisible(true); // look at this
        m_fields->initedDeath = false;
        PlayLayer::onQuit();
    }
    void postUpdate(float p0) {
        PlayLayer::postUpdate(p0);
        
#ifndef GEODE_IS_MACOS
        if (Hacks::isHackEnabled("Safe Mode")) {
            m_isTestMode = true;
        } else {
            m_isTestMode = m_fields->previousTestMode;
        }
#endif
        // whats the difference between m_fields and not using? i have no idea!
        if ( // i dont know what are considered "cheats"
            Hacks::isHackEnabled("Noclip") ||
            Hacks::isHackEnabled("No Spikes") ||
            Hacks::isHackEnabled("No Solids") ||
            Hacks::isHackEnabled("Freeze Player") ||
            Hacks::isHackEnabled("No Mirror Transition") ||
            Hacks::isHackEnabled("Instant Mirror Portal") ||
            Hacks::isHackEnabled("Jump Hack") ||
            Hacks::isHackEnabled("Instant Complete") ||
            Hacks::isHackEnabled("Force Platformer Mode") ||
            Hacks::isHackEnabled("Change Gravity") ||
            Hacks::isHackEnabled("Layout Mode")
        ) { // cheating
            if (!m_fields->isCheating) {
                m_fields->isCheating = true;
                if (Hacks::isHackEnabled("Cheat Indicator")) {
                    m_fields->cheatIndicator->removeFromParentAndCleanup(true);
                    m_fields->cheatIndicator = createCheatIndicator(true);
                    this->addChild(m_fields->cheatIndicator);
                }
            }
        } else { // not cheating
            if (m_fields->isCheating) {
                m_fields->isCheating = false;
                m_fields->cheatIndicator->removeFromParentAndCleanup(true);
                m_fields->cheatIndicator = createCheatIndicator(false);
                this->addChild(m_fields->cheatIndicator);
            }
        }
        if (Hacks::isHackEnabled("Instant Complete") && m_fields->updateInit < 5) {
            log::debug("CRIMINAL… criminal… criminal… criminal…");
            // funny message
            FLAlertLayer::create(nullptr, "Cheater!", "Just a warning, you will be <cr>banned off leaderboards</c> if you use this on rated levels. Consider this your <cy>warning</c>.", "OK", nullptr)->show();
        }
        m_fields->updateInit = m_fields->updateInit + 1;
        float attemptOpacity = Hacks::getHack("Attempt Opacity")->value.floatValue;
        //if (!Hacks::isHackEnabled("Hide Attempts") && attemptOpacity == 1.0F) return PlayLayer::postUpdate(p0);
        int currentPosition = Hacks::getHack("Progress Bar Position")->value.intValue;
        // stop dynamic_cast abuse
        auto node = typeinfo_cast<CCNode*>(this->getChildren()->objectAtIndex(0));
        if (node == nullptr) return; // never will happen (or will it)
        m_fields->cheatIndicator->setVisible(Hacks::isHackEnabled("Cheat Indicator"));
        if (m_fields->progressBar == nullptr || m_fields->percentLabel == nullptr || m_fields->attemptLabel == nullptr) {
            for (size_t i = 0; i < this->getChildrenCount(); i++) {
                auto obj = this->getChildren()->objectAtIndex(i);
                if (Utils::getNodeName(obj) == "cocos2d::CCLabelBMFont" && m_fields->percentLabel == nullptr) {
                    auto labelTest = static_cast<CCLabelBMFont*>(obj);
                    if (strlen(labelTest->getString()) < 6) {
                        m_fields->percentLabel = labelTest;
                    }
                } else if (Utils::getNodeName(obj) == "cocos2d::CCSprite" && m_fields->progressBar == nullptr) {
                    m_fields->progressBar = static_cast<CCSprite*>(obj);
                }
            }
            for (size_t i = 0; i < node->getChildrenCount(); i++) {
                auto obj = node->getChildren()->objectAtIndex(i);
                if (Utils::getNodeName(obj) == "cocos2d::CCLayer") {
                    auto layer = static_cast<CCLayer*>(obj);
                    for (size_t y = 0; y < layer->getChildrenCount(); y++) {
                        auto obj2 = layer->getChildren()->objectAtIndex(y);
                        if (Utils::getNodeName(obj2) == "cocos2d::CCLabelBMFont") {
                            m_fields->attemptLabel = static_cast<CCLabelBMFont*>(obj2);
                            break;
                        }
                    }
                }
            }
        } else {
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            m_fields->progressBar->setRotation(0.0F);
            switch (currentPosition) {
                case 0: // Top (312)
                    m_fields->progressBar->setPositionY(winSize.height - 8);
                    m_fields->percentLabel->setPositionY(winSize.height - 8);
                    break;
                case 1: // Bottom (10)
                    m_fields->progressBar->setPositionY(10);
                    m_fields->percentLabel->setPositionY(10);
                    break;
                case 2: // Left // 275
                    m_fields->progressBar->setPosition({ 10, winSize.height / 2 });
                    m_fields->percentLabel->setPosition({ 5, winSize.height - 45 }); // 275
                    m_fields->progressBar->setRotation(-90.0F);
                    break;
                case 3: // Right
                    m_fields->progressBar->setPosition({ winSize.width - 10, winSize.height / 2 });
                    m_fields->percentLabel->setPosition({ winSize.width - 40, winSize.height - 45 });
                    m_fields->progressBar->setRotation(-90.0F);
                    break;
            }
            m_fields->attemptLabel->setOpacity(attemptOpacity * 255);
        }
    }
    
    // Accurate Percentage
#ifndef GEODE_IS_MACOS
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (Hacks::isHackEnabled("Enable Patching")) return;
        if (Hacks::isHackEnabled("Accurate Percentage")) {
            if (m_fields->percentLabel == nullptr) return;
            std::cout << PlayLayer::getCurrentPercent()  << "," << PlayLayer::getCurrentPercentInt() << std::endl;
            std::string percentStr = std::to_string(PlayLayer::getCurrentPercent()) + "%";
            m_fields->percentLabel->setString(percentStr.c_str());
        }
    }
#endif
    void levelComplete() {
        if (!Hacks::isHackEnabled("Safe Mode") || Hacks::isHackEnabled("Enable Patching")) return PlayLayer::levelComplete();
        PlayLayer::resetLevel(); // haha
    }
};

/*
class $modify(PlayLayer) {
    // Accurate Percentage

    /\*void levelComplete() {
        if (!Hacks::isHackEnabled("Safe Mode") || Hacks::isHackEnabled("Enable Patching")) return PlayLayer::levelComplete();
        PlayLayer::resetLevel(); // haha
    }
    void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5) {
        if (!Hacks::isHackEnabled("Safe Mode")) return PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);
    }*\/ // WHY YOU HAVE DELAY
};
*/





