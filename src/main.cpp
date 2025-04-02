// Prism Menu by Firee
// i am looking at you. make sure you credit me and this mod, i worked hard on it ;-;
// please do not judge my coding, ok thank you!!!
#include "CustomSettings.hpp"
#include <Geode/utils/file.hpp>
#include "PrismButton.hpp"
#include "UI/Intro.hpp"
#include "hacks.hpp"
#include "Themes.hpp"
#include "UI/PrismUI.hpp"
#include "Utils.hpp"
#include <Geode/Geode.hpp>
//#include <geode.custom-keybinds/include/Keybinds.hpp>
//using namespace keybinds;

#include <filesystem>
#include <iomanip>
#include <string>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GameManager.hpp>

/*
for i in range(char2nr('A'), char2nr('Z'))
  call append(line("."), printf("{'%c', cocos2d::KEY_%c},", i, i))
endfor
*/
PrismButton* prismButton;

bool firstLoad = false;
bool removeMe = false;

#if defined(GEODE_IS_WINDOWS) || defined(GEODE_IS_MACOS)
#if 0 // temp disabled until i can figure out why texture loader no work
// for some reason the GEODE_IS_DESKTOP def isnt present???
class $modify(GameManager) {
    void reloadAll(bool switchingModes, bool toFullscreen, bool borderless, bool unused) {
        if (prismButton != nullptr) {
            SceneManager::get()->forget(prismButton);
            // yeah this seems safe, definitely wont be a problem in the future
            delete prismButton;
            //prismButton->release();
        }
        GameManager::reloadAll(switchingModes, toFullscreen, borderless, unused);
        removeMe = true;
    }
};
#endif
// also for some reason this causes texture loader to not work!
#endif

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        if (removeMe) {
            firstLoad = false;
            removeMe = false;
        }

        if (firstLoad) return true;
        HackItem* posX = Hacks::getHack("Button Position X");
        HackItem* posY = Hacks::getHack("Button Position Y");
        firstLoad = true;
        log::info("Prism Menu loaded! Enjoy the mod.");
        #ifndef GEODE_IS_MACOS
        if (auto scene = CCScene::get()) {
            prismButton = PrismButton::create(scene);
            prismButton->setVisible(Hacks::isHackEnabled("Show Button"));
            if (Mod::get()->getSettingValue<bool>("hide-button")) prismButton->setVisible(false);
            prismButton->setID("prism-icon");
            SceneManager::get()->keepAcrossScenes(prismButton);
        }
        #endif
        return true;
    }
};

bool restoreOldFiles() {
    auto currentSave = Mod::get()->getSaveDir();
    auto parentDir = currentSave.parent_path();
    auto oldSave = parentDir / "firee.PrismMenu";
    auto oldoldSave = parentDir / "firee.PrismMenu.old";
    if (std::filesystem::exists(oldoldSave) && std::filesystem::is_directory(oldoldSave)) {
        log::info("Files already restored. Consider deleting the \"firee.PrismMenu.old\" directory in your saves to remove this message.");
        return false;
    }

    if (std::filesystem::exists(oldSave) && std::filesystem::is_directory(oldSave)) {
        log::debug("AIR DETECTED. copying files from firee.PrismMenu to firee.prism");
        std::filesystem::copy(oldSave, currentSave, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);

        std::filesystem::path newBackupDir = parentDir / (oldSave.filename().string() + ".old");

#ifdef GEODE_IS_ANDROID // stupid android and their "OH YOU DONT HAVE PERMISSION TO RENAME"
        std::filesystem::copy(oldSave, newBackupDir, std::filesystem::copy_options::recursive);
        std::filesystem::remove_all(oldSave);
#else 
        std::filesystem::rename(oldSave, newBackupDir);
#endif
        return true;
    } else {
        // assuming new instance
        return false;
    }
}

// maybe this will fix the issue
$execute {
    auto ret = Mod::get()->registerCustomSettingType("hack-values", &SettingHackValue::parse);
    if (!ret) {
        log::error("Unable to register setting type: {}", ret.unwrapErr());
    }
}

$on_mod(Loaded) {
    if (restoreOldFiles()) {
        log::debug("Mod ID save migration complete");
        if (Mod::get()->loadData().isOk()) {
            log::debug("loaded old data");
        }
    }
    Hacks::processJSON(false);
    Themes::addToCurrentThemes();
    Mod::get()->setSettingValue("skip-intro", false); // TODO: remove after intro update
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
#ifndef GEODE_IS_IOS
#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool arr) {
        //if (down && (static_cast<int>(key) == 18)) {//(key == KEY_Tab)) {
        if (down) {
            if (CCScene::get() == nullptr) return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, arr);
            if (auto charUI = typeinfo_cast<CharUI*>(CCScene::get()->getChildByID("prism-charui"))) {
                charUI->keyPressed(key);
            } else {
                HackItem* keybind = Hacks::getHack("Menu Keybind");
                int currentKeyID = (keybind != nullptr) ? keybind->value.intValue : 9;
                if (static_cast<int>(key) == currentKeyID) {
                    auto prismButton = typeinfo_cast<PrismButton*>(CCScene::get()->getChildByID("prism-icon"));
                    HackItem* menuStyle = Hacks::getHack("Menu-Style");
                    #ifdef NO_IMGUI 
                    menuStyle->value.intValue = 1;
                    #else
                    if (prismButton == nullptr) return true;
                    #endif
                    if (menuStyle->value.intValue == 0) { // imgui
                        prismButton->showImGuiMenu = !prismButton->showImGuiMenu;
                    } else {
                        //if (Mod::get()->getSettingValue<bool>("skip-intro")) {
                            auto prismUIExists = CCScene::get()->getChildByID("prism-menu");
                            if (prismUIExists != nullptr && prismUIExists->isVisible() == false) return true;
                            if (prismUIExists == nullptr) {
                                PrismUI::create()->show();
                            } else {
                                static_cast<PrismUI*>(prismUIExists)->onClose(CCNode::create());
                            }
                        /*} else {
                            if (CCScene::get()->getChildByID("prism-intro") == nullptr) {
                                IntroUI::create()->show();
                            }
                        }*/
                    }
                    return true;
                }
            }
        }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, arr);
    }
};
#endif
// sorry, not sorry. for some reason the allHacks vector isnt being initialized in the other .cpp file
// i completely wasted my time writing this whole patch script, and i kinda want android + mac support soooo

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

// showing the icon for android users lol
class $modify(PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        if (!Mod::get()->getSettingValue<bool>("hide-button")) {
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
        
    }
};

// hiding the icon in editor in case for some reason people dont know how to move the button (ITS IN SETTINGS)
#ifndef GEODE_IS_MACOS

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

class $modify(EditorUI) {
    bool init(LevelEditorLayer *editorLayer) {
        if (prismButton != nullptr && Hacks::isHackEnabled("Show Button")) prismButton->setVisible(false);
        return EditorUI::init(editorLayer);
    }
};

class $modify(EditLevelLayer) {
    bool init(GJGameLevel* p0) {
        if (!EditLevelLayer::init(p0)) return false;
        if (prismButton != nullptr && Hacks::isHackEnabled("Show Button")) {
            prismButton->setVisible(true);
        }
        return true;
    }
};
#endif

// TODO: Check Cheat Indicator position
CircleButtonSprite* createCheatIndicator(bool isHacking) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto cheatIndicator = CircleButtonSprite::create(CCNode::create(), (isHacking) ? CircleBaseColor::Pink : CircleBaseColor::Green, CircleBaseSize::Tiny); //, CircleBaseColor::Geode
    cheatIndicator->setAnchorPoint({1,1});
    cheatIndicator->setPosition({28, winSize.height});
    cheatIndicator->setZOrder(1000);
    cheatIndicator->setTag(10420); // prevent PlayLayer from interfering
    return cheatIndicator;
}

class $modify(PrismPlayLayer, PlayLayer) {
    struct Fields {
        float previousPositionX = 0.0F;
        GameObject* antiCheatObject; // removing after lol
        CircleButtonSprite* cheatIndicator;
        CCNode* prismNode;

        CCSprite* progressBar;
        CCLabelBMFont* percentLabel;
        CCLabelBMFont* attemptLabel;

        bool isCheating = false;
        bool hasSetTestMode = false;
        bool previousTestMode;
        
        // Anticheat Bypass
        bool initedDeath = false;

        // Noclip Accuracy
        int death = 0;
        float previousDeathX = 0.0F;
        CCLabelBMFont* accuracyLabel;
        float flashOpacity = 0.0F;
        CCSprite* flashNode;

        bool hasCompletedLevel = false;
    };

    // Anticheat Bypass, Noclip, No Spikes, No Solids
    void destroyPlayer(PlayerObject *p0, GameObject *p1) {
        if (!m_fields->initedDeath) {
            #if !defined(GEODE_IS_ANDROID64) && !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS)
            if (m_fields->antiCheatObject == nullptr && p1 != nullptr && (
                (p1->m_positionX == 0 && p1->m_positionY == p0->m_positionY) ||
                (p1->m_positionX == 0 && p1->m_positionY == p0->m_positionY) // todo, get player pos during PlayLayer::init
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
        //bool m_isTestMode = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x413);
        auto destroyLePlayer = [this,p0,p1]() {
            if (Hacks::isHackEnabled("Safe Mode") || Hacks::isAutoSafeModeActive()) {
                PlayLayer::resetLevel();
            } else {
                PlayLayer::destroyPlayer(p0, p1);
                if (Hacks::isHackEnabled("Instant Respawn")) PlayLayer::delayedResetLevel();
            }
        };
        bool noclipDisabled = !Hacks::isHackEnabled("No Solids") && !Hacks::isHackEnabled("No Spikes") && !Hacks::isHackEnabled("Noclip");
        if (noclipDisabled) {
            destroyLePlayer();
            return;
        }
        if (
            !Hacks::isHackEnabled("Noclip") &&
            Hacks::isHackEnabled("No Solids") &&
            p1 != nullptr &&
            p1->m_objectType != GameObjectType::CollisionObject
        ) {
            destroyLePlayer();
        }
        if (
            !Hacks::isHackEnabled("Noclip") &&
            Hacks::isHackEnabled("No Spikes") &&
            (p1 == nullptr || p1->m_objectType == GameObjectType::CollisionObject)
        ) {
            destroyLePlayer();
        }
        if (p1 != m_fields->antiCheatObject) {
            if (Hacks::isHackEnabled("Suicide")) return PlayLayer::destroyPlayer(p0, p1);
            if (m_player1 != nullptr) {
                if (m_player1->getPositionX() != m_fields->previousDeathX) {
                    m_fields->previousDeathX = m_player1->getPositionX();
                    m_fields->death++;
                }
            }
            if (m_fields->accuracyLabel != nullptr && m_fields->flashNode != nullptr) {
                m_fields->accuracyLabel->setColor({255,0,0});
                if (Hacks::isHackEnabled("Noclip Flash")) {
                    m_fields->flashOpacity = 160.0F;
                }
            }
        }
    }
    // Instant Complete, Hide Testmode
    bool init(GJGameLevel *p0, bool p1, bool p2) {
        if (!PlayLayer::init(p0,p1,p2)) return false;
        if (prismButton != nullptr && Hacks::isHackEnabled("Show Button")) prismButton->setVisible(false);
        if (Hacks::isHackEnabled("Instant Complete")) {
            PlayLayer::playEndAnimationToPos({2,2});
        }
        
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
        m_fields->previousTestMode = m_isTestMode;
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
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        m_fields->prismNode = CCNode::create();
        m_fields->prismNode->setTag(10420);
        m_fields->prismNode->setZOrder(1);
        m_fields->cheatIndicator = createCheatIndicator(false);
        m_fields->cheatIndicator->setVisible(Hacks::isHackEnabled("Cheat Indicator"));
        m_fields->accuracyLabel = CCLabelBMFont::create("100.00%", "bigFont.fnt");
        m_fields->accuracyLabel->setPosition({36, winSize.height - 35});
        m_fields->accuracyLabel->setScale(0.5F);
        m_fields->accuracyLabel->setOpacity(255 / 2);
        m_fields->accuracyLabel->setVisible(Hacks::isHackEnabled("Noclip Accuracy") && (Hacks::isHackEnabled("Noclip") || Hacks::isHackEnabled("No Solids") || Hacks::isHackEnabled("No Spikes")));
        m_fields->accuracyLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
        m_fields->accuracyLabel->setZOrder(1000);
        m_fields->accuracyLabel->setTag(10420); // prevent PlayLayer from interfering
        m_fields->flashNode = CCSprite::create("square02b_001.png");
        m_fields->flashNode->setScale(10);
        m_fields->flashNode->setPosition(winSize / 2);
        m_fields->flashNode->setZOrder(999);
        m_fields->flashNode->setColor({255,0,0});
        m_fields->flashNode->setOpacity(0);
        m_fields->flashNode->setTag(10420);
        m_fields->prismNode->addChild(m_fields->flashNode);
        m_fields->prismNode->addChild(m_fields->accuracyLabel);
        m_fields->prismNode->addChild(m_fields->cheatIndicator);
        this->addChild(m_fields->prismNode);
        return true;
    }
    void onQuit() {
        if (prismButton != nullptr && Hacks::isHackEnabled("Show Button")) prismButton->setVisible(true); // look at this
        m_fields->initedDeath = false;
        m_fields->death = 0;
        PlayLayer::onQuit();
    }
    void resetLevel() {
        m_fields->death = 0;
        m_fields->hasCompletedLevel = false;
        if (m_fields->accuracyLabel != nullptr && m_fields->flashNode != nullptr) {
            m_fields->flashOpacity = 0.0F;
            m_fields->flashNode->setOpacity(m_fields->flashOpacity);
        }
        PlayLayer::resetLevel();
    }
    void postUpdate(float p0) {
        PlayLayer::postUpdate(p0);
        if (m_player1 != nullptr && Hacks::isHackEnabled("Suicide")) return PlayLayer::destroyPlayer(m_player1, nullptr);
        if (m_fields->accuracyLabel != nullptr && !m_fields->hasCompletedLevel) {
            float accuracy = ((static_cast<float>(m_gameState.m_currentProgress - m_fields->death)) / static_cast<float>(m_gameState.m_currentProgress)) * 100; // for some reason this doesnt work on android, like it goes in the negatives
            m_fields->accuracyLabel->setString(fmt::format("{}%", Utils::setPrecision(accuracy, 2)).c_str());
            m_fields->accuracyLabel->setVisible(Hacks::isHackEnabled("Noclip Accuracy") && (Hacks::isHackEnabled("Noclip") || Hacks::isHackEnabled("No Solids") || Hacks::isHackEnabled("No Spikes")));
            if (m_gameState.m_currentProgress % 4 == 0) { // quarter step
                m_fields->accuracyLabel->setColor({255,255,255});
                if (m_fields->flashOpacity > 0.0F) {
                    m_fields->flashOpacity -= 10.0F;
                }
                if (m_fields->flashOpacity <= 0.0F) m_fields->flashOpacity = 0.0F;
                m_fields->flashNode->setOpacity(m_fields->flashOpacity);
            }
        }
        /*if (!m_fields->hasSetTestMode) {
            m_fields->hasSetTestMode = true;
            m_fields->previousTestMode = m_isTestMode;
        }
        if (m_fields->hasSetTestMode) {
            if (Hacks::isHackEnabled("Safe Mode") || Hacks::isAutoSafeModeActive()) {
                m_isTestMode = true;
            } else {
                m_isTestMode = m_fields->previousTestMode;
            }
        }
        */
        // whats the difference between m_fields and not using? i have no idea!
        if (Hacks::isCheating()) { // cheating
            if (!m_fields->isCheating) {
                m_fields->isCheating = true;
                if (Hacks::isHackEnabled("Cheat Indicator")) {
                    m_fields->cheatIndicator->removeFromParentAndCleanup(true);
                    m_fields->cheatIndicator = createCheatIndicator(true);
                    m_fields->prismNode->addChild(m_fields->cheatIndicator);
                }
            }
        } else { // not cheating
            if (m_fields->isCheating) {
                m_fields->isCheating = false;
                m_fields->cheatIndicator->removeFromParentAndCleanup(true);
                m_fields->cheatIndicator = createCheatIndicator(false);
                m_fields->prismNode->addChild(m_fields->cheatIndicator);
            }
        }
        if (Hacks::isHackEnabled("Instant Complete") && m_gameState.m_currentProgress < 5) {
            log::debug("CRIMINAL… criminal… criminal… criminal…");
            // funny message

            // Don't show if any form of safe mode is enabled, it gets VERY annoying otherwise
            //if (!(Hacks::isHackEnabled("Safe Mode") || Hacks::isHackEnabled("Auto Safe Mode"))) FLAlertLayer::create(nullptr, "Cheater!", "Just a warning, you will be <cr>banned off leaderboards</c> if you use this on rated levels. Consider this your <cy>warning</c>.", "OK", nullptr)->show();
        }
        float attemptOpacity = Hacks::getHack("Attempt Opacity")->value.floatValue;
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
                    //if (strlen(labelTest->getString()) < 6) {
                    std::string labelStr = labelTest->getString();
                    if (labelStr.ends_with("%")) {
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
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (Hacks::isHackEnabled("Accurate Percentage")) {
            if (m_fields->percentLabel == nullptr) return;
            std::stringstream percentStream;
            int numDigits = Hacks::getHack("Extra Percent Digits")->value.intValue;
            percentStream << std::fixed << std::setprecision(numDigits) << PlayLayer::getCurrentPercent() << "%";
            std::string percentStr = percentStream.str();
            m_fields->percentLabel->setString(percentStr.c_str());
        }
    }
    void levelComplete() {
        m_fields->hasCompletedLevel = true;
        if (!(Hacks::isHackEnabled("Safe Mode") || Hacks::isAutoSafeModeActive())) return PlayLayer::levelComplete();
        PlayLayer::resetLevel(); // haha
    }
};

/*
class $modify(GJBaseGameLayer) {
    void update(float dt) {
        GJBaseGameLayer::update(dt);
        if (PlayLayer::get() != nullptr) {
            auto playLayer = static_cast<PrismPlayLayer*>(PlayLayer::get());
        }
    }
};
*/

/*
class $modify(PlayLayer) {
    // Accurate Percentage

    /\*void levelComplete() {
        if (!Hacks::isHackEnabled("Safe Mode")) return PlayLayer::levelComplete();
        PlayLayer::resetLevel(); // haha
    }
    void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5) {
        if (!Hacks::isHackEnabled("Safe Mode")) return PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);
    }*\/ // WHY YOU HAVE DELAY
};
*/
