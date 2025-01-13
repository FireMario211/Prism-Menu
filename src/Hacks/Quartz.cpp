// TODO: move code to separate files
#include "Quartz.hpp"
#include "../UI/PrismUI.hpp"
#include "../hacks.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
// yes this code is messy, ill move it later
bool changedMacro = false;
bool SelectMacroUI::init(float _w, float _h, Dropdown* dropdown) {
    if (CCScene::get() != nullptr) {
        if (auto prismMenu = typeinfo_cast<PrismUI*>(CCScene::get()->getChildByID("prism-menu"))) {
            prismMenu->toggleVisibility();
        }
    }
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    this->m_mainLayer = cocos2d::CCLayer::create();
    this->addChild(m_mainLayer);
    this->m_buttonMenu = cocos2d::CCMenu::create();
    this->m_dropdown = dropdown;
    m_buttonMenu->setZOrder(1);
    m_mainLayer->addChild(this->m_buttonMenu);

    auto closeSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(1.0f);

    auto closeBtn = CCMenuItemSpriteExtra::create(
        closeSpr,
        this,
        (cocos2d::SEL_MenuHandler)&SelectMacroUI::onClose
    );
    closeBtn->setUserData(reinterpret_cast<void*>(this));
    this->m_buttonMenu->addChild(closeBtn);

    closeBtn->setPosition( (- _w / 2) - 20, (_h / 2) + 10 );

    // height, width, height
    auto listView = ListView::create(createCells(), 50, _w, _h);
    m_listLayer = GJListLayer::create(listView, nullptr, {0, 0, 0, 180}, _w, _h, 0);
    m_listLayer->setPosition({ winSize / 2 - m_listLayer->getScaledContentSize() / 2 });
    this->m_mainLayer->addChild(m_listLayer);

    this->registerWithTouchDispatcher();

    cocos::handleTouchPriority(this);
    this->setKeypadEnabled(true);
    this->setKeyboardEnabled(true);
    this->setTouchEnabled(true);
    return true;
}

CCArray* SelectMacroUI::createCells() {
    auto macros = MacroManager::getMacros();
    auto items = CCArray::create();
    for (auto const& item : macros) {
        auto cell = MacroItemCell::create(this, item);
        items->addObject(cell);
    }
    return items;
}

void SelectMacroUI::keyDown(cocos2d::enumKeyCodes key) {
    if (key == cocos2d::enumKeyCodes::KEY_Escape)
        return onClose(nullptr);
    if (key == cocos2d::enumKeyCodes::KEY_Tab)
        return onClose(nullptr);
    if (key == cocos2d::enumKeyCodes::KEY_Space)
        return;
    
    return FLAlertLayer::keyDown(key);
}

void SelectMacroUI::keybackClicked() {
    onClose(nullptr);
};

void SelectMacroUI::onClose(cocos2d::CCObject*) {
    if (CCScene::get() != nullptr) {
        if (auto prismMenu = typeinfo_cast<PrismUI*>(CCScene::get()->getChildByID("prism-menu"))) {
            prismMenu->toggleVisibility();
        }
    }
    this->removeFromParentAndCleanup(true);
};


void SelectMacroUI::onClick(CCObject* sender) {
    CCMenuItemSpriteExtra* menuItem = static_cast<CCMenuItemSpriteExtra*>(sender);
    SelectMacroUI::create(static_cast<Dropdown*>(menuItem->getUserData()))->show();
}

SelectMacroUI* SelectMacroUI::create(Dropdown* dropdown) {
    auto pRet = new SelectMacroUI();
    if (pRet && pRet->init(SelectMacroUI::s_defWidth, SelectMacroUI::s_defHeight, dropdown)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};

MacroItemCell* MacroItemCell::create(SelectMacroUI* list, MacroFile item) {
    auto pRet = new MacroItemCell();
    if (pRet) {
        if (pRet->init(list, item)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};

bool MacroItemCell::init(SelectMacroUI* list, MacroFile item) {
    m_fWidth = 330.0F;
    m_fHeight = 50.0F;
    m_file = item;
    m_list = list;
    this->setContentSize({ m_fWidth, m_fHeight });
    
    menu = CCMenu::create();
    menu->setPosition({m_fWidth / 2, m_fHeight / 2});
    menu->setContentSize({m_fWidth - 20, m_fHeight});
    menu->setAnchorPoint({.5f, .5f});
    this->addChild(menu);
    
    
    auto nameLbl = CCLabelBMFont::create(item.name.c_str(), "bigFont.fnt");
    nameLbl->limitLabelWidth(180, 1.0F, 0.2F);
    nameLbl->setPositionX(-65);
    menu->addChild(nameLbl);
    if (m_file.macro.invalid) {
        auto errorBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"), this, menu_selector(MacroItemCell::onError));
        errorBtn->setPositionX(80);
        menu->addChild(errorBtn);
    } else {
        auto selectBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("Select"), this, menu_selector(MacroItemCell::onSelect));
        selectBtn->setPositionX(80);
        menu->addChild(selectBtn);
    }

    auto trashBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png"),
        this,
        menu_selector(MacroItemCell::onTrash)
    );
    trashBtn->setPositionX(147);
    menu->addChild(trashBtn);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");

    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr,
        this,
        menu_selector(MacroItemCell::onInfo)
    );
    infoBtn->setPositionX(173);

    menu->addChild(infoBtn);

    menu->updateLayout();
    return true;
}

void MacroItemCell::onError(CCObject*) {
    FLAlertLayer::create("Error", "This macro could <cr>not be loaded!</c>\nThis may be because the macro <cg>is corrupted</c>, has <cg>invalid formatting</c>, or <cy>another reason</c>.", "OK")->show();
}

void MacroItemCell::onTrash(CCObject*) {
    geode::createQuickPopup(
        "Confirm",
        fmt::format("Are you sure you want to <cr>delete this macro</c>?\nFile Name: <cg>{}</c>", m_file.file),
        "Yes", "No",
        [this](auto, bool btn2) {
            if (!btn2) { // yes
                auto saveDir = Mod::get()->getSaveDir().string();
                if (std::filesystem::exists(saveDir + "/macros")) {
                    auto savePath = saveDir + "/macros/" + m_file.file;
                    if (std::filesystem::exists(savePath)) {
                        if (std::filesystem::remove(savePath)) {
                            FLAlertLayer::create("Success!", "Successfully deleted the macro.", "OK")->show();
                            m_list->m_dropdown->lbl->setString("None");
                            HackItem* item = Hacks::getHack("Macro");
                            if (item == nullptr) {
                                log::error("Couldn't find hack item \"Macro\"");
                            } else {
                                std::vector<matjson::Value> arr;
                                arr.push_back("None");
                                item->data["values"] = arr;
                                changedMacro = false;
                            }
                            m_list->onClose(nullptr);
                        }
                    }
                }
            }
        }, true, true
    );
}

void MacroItemCell::onInfo(CCObject*) {
    FLAlertLayer::create(
        nullptr,
        m_file.name.c_str(),
        fmt::format(
            "By <cy>{}</c>\nFile Name: <cg>{}</c>\nDescription: <cy>{}</c>\nLevel: <cg>{}</c> ({})\nTPS: <cy>{}</c>\nDuration: <cg>{}</c>\nFrame Fix: {}\n\n<cr>Made with</c> {} (v{})",
            m_file.macro.author,
            m_file.file,
            m_file.macro.description,
            m_file.macro.levelInfo.name, m_file.macro.levelInfo.id,
            m_file.macro.framerate,
            m_file.macro.duration,
            (!m_file.macro.inputs.empty() && m_file.macro.inputs[0].frameFix.fix) ? "<cy>Yes</c>" : "<cr>No</c>",
            m_file.macro.botInfo.name, m_file.macro.botInfo.version
        ).c_str(),
        "OK",
        nullptr, 350.F
    )->show();
}
void MacroItemCell::onSelect(CCObject*) {
    m_list->m_dropdown->lbl->setString(m_file.file.c_str());
    HackItem* item = Hacks::getHack("Macro");
    if (item == nullptr) {
        log::error("Couldn't find hack item \"Macro\"");
    } else {
        std::vector<matjson::Value> arr;
        arr.push_back(m_file.file);
        item->data["values"] = arr;
        changedMacro = true;
    }
    m_list->onClose(nullptr);
}

void MacroItemCell::draw() {
    auto size = this->getContentSize();
    cocos2d::ccDrawColor4B(0, 0, 0, 75);
    glLineWidth(2.0f);
    cocos2d::ccDrawLine({ 1.0f, 0.0f }, { size.width - 1.0f, 0.0f });
    cocos2d::ccDrawLine({ 1.0f, size.height }, { size.width - 1.0f, size.height });
}

void CreateMacroUI::setup() {
    if (CCScene::get() != nullptr) {
        if (auto prismMenu = typeinfo_cast<PrismUI*>(CCScene::get()->getChildByID("prism-menu"))) {
            prismMenu->toggleVisibility();
        }
    }
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_inputName = TextInput::create(150.f, "Macro Name...", "chatFont.fnt");
    m_inputName->setMaxCharCount(50);
    m_inputName->setFilter("QWERTZUIOPASDFGHJKLYXCVBNMqwertzuiopasdfghjklyxcvbnm0123456789.-_[]!() ");
    m_inputDesc = TextInput::create(150.f, "Macro Description... (Optional)", "chatFont.fnt");
    m_inputDesc->setCommonFilter(CommonFilter::Any);
    m_inputDesc->setMaxCharCount(250);

    auto fpsLabel = CCLabelBMFont::create("TPS", "bigFont.fnt");
    fpsLabel->setPosition({-75, -32});
    this->m_buttonMenu->addChild(fpsLabel);
    m_fpsInput = TextInput::create(50.f, "TPS...", "chatFont.fnt");
    auto currentTPS = Hacks::getHack("TPS Bypass");
    m_fpsInput->setString("240");
    if (currentTPS != nullptr) {
        m_fpsInput->setString(std::to_string(currentTPS->value.intValue).c_str());
    }
    //std::string fpsToStr = CCDirector::sharedDirector()->m_pszFPS;
    //log::debug("Set {} as FPS", fpsToStr.substr(fpsToStr.find(":") + 2));
    //m_fpsInput->setString(fpsToStr.substr(fpsToStr.find(":") + 2));
    m_fpsInput->setMaxCharCount(8);
    m_fpsInput->setCommonFilter(CommonFilter::Int);
    m_fpsInput->setPosition({5, -32});
    this->m_buttonMenu->addChild(m_fpsInput);

    auto fpsLeftBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png"),
        this,
        menu_selector(CreateMacroUI::onLeftFPS)
    );
    fpsLeftBtn->setPosition({-30, -32});
    auto fpsRightBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png"),
        this,
        menu_selector(CreateMacroUI::onRightFPS)
    );
    fpsRightBtn->setPosition({40, -32});
    this->m_buttonMenu->addChild(fpsLeftBtn);
    this->m_buttonMenu->addChild(fpsRightBtn);
    m_inputName->setPositionY(40);
    m_inputDesc->setPositionY(5);
    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");

    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr,
        this,
        menu_selector(CreateMacroUI::onInfo)
    );

    this->m_buttonMenu->addChild(infoBtn);

    infoBtn->setPosition( m_fWidth / 2, m_fHeight / 2 );

    //ButtonSprite::create(caption, 0, 0, "goldFont.fnt", "GJ_button_01.png", .0f, 1.f);
    auto gdrBtnSpr = ButtonSprite::create("Create .gdr", m_fWidth / 3, 0, "bigFont.fnt", "GJ_button_01.png", .0f, 1.f);
    auto gdrJsonBtnSpr = ButtonSprite::create("Create .gdr.json", m_fWidth / 3, 0, "bigFont.fnt", "GJ_button_01.png", .0f, 1.f);

    auto createButton = CCMenuItemSpriteExtra::create(gdrBtnSpr, this, menu_selector(CreateMacroUI::onCreate));
    auto createButtonJson = CCMenuItemSpriteExtra::create(gdrJsonBtnSpr, this, menu_selector(CreateMacroUI::onCreateJSON));
    this->m_buttonMenu->addChild(m_inputName);
    this->m_buttonMenu->addChild(m_inputDesc);
    createButton->setPosition({-60.F, -75.F});
    createButtonJson->setPosition({60.F, -75.F});
    this->m_buttonMenu->addChild(createButton);
    this->m_buttonMenu->addChild(createButtonJson);
    cocos::handleTouchPriority(this);
}

void CreateMacroUI::onLeftFPS(CCObject*) {
    int fpsNumber = std::stoi(m_fpsInput->getString());
    fpsNumber -= 5;
    m_fpsInput->setString(std::to_string(fpsNumber));
}
void CreateMacroUI::onRightFPS(CCObject*) {
    int fpsNumber = std::stoi(m_fpsInput->getString());
    fpsNumber += 5;
    m_fpsInput->setString(std::to_string(fpsNumber));
}

void CreateMacroUI::onInfo(CCObject*) {
    FLAlertLayer::create(nullptr, "Creating a Macro", "The <cy>Macro Name</c> specifies the file name of the macro.\nThe <cy>Macro Description</c> can be anything, or nothing. It's recommended to add any additional details about the macro in the description if needed.\nThe <cy>TPS</c> is the <cg>physics framerate</c> of the macro.\n<cy>Create .gdr</c> is generally recommended in case the macro size is too large. However if you want to easily edit your macro through <cg>file editing</c>, <cy>Create .gdr.json</c> is recommended.", "OK", nullptr, 450.F)->show();
}

void CreateMacroUI::onCreate(CCObject* sender) {
    if (m_inputName->getString().length() == 0) return FLAlertLayer::create("Error", "Please enter in a <cy>Macro Name</c>!", "OK")->show();
    if (m_fpsInput->getString().length() == 0) return FLAlertLayer::create("Error", "Please enter in the <cy>TPS</c>!", "OK")->show();
    if (!MacroManager::createMacro(m_inputName->getString(), m_inputDesc->getString(), m_fpsInput->getString(), false)) {
        FLAlertLayer::create("Error", "A macro with a similar name already exists!", "OK")->show();
    }
    onClose(sender);
}
void CreateMacroUI::onCreateJSON(CCObject* sender) {
    if (m_inputName->getString().length() == 0) return FLAlertLayer::create("Error", "Please enter in a <cy>Macro Name</c>!", "OK")->show();
    if (m_fpsInput->getString().length() == 0) return FLAlertLayer::create("Error", "Please enter in the <cy>TPS</c>!", "OK")->show();
    if (!MacroManager::createMacro(m_inputName->getString(), m_inputDesc->getString(), m_fpsInput->getString(), true)) {
        FLAlertLayer::create("Error", "A macro with a similar name already exists!", "OK")->show();
    }
    onClose(sender);
}

void CreateMacroUI::onClick(CCObject*) {
    CreateMacroUI::create()->show();
}

CreateMacroUI* CreateMacroUI::create() {
    auto pRet = new CreateMacroUI();
    if (pRet && pRet->init(CreateMacroUI::s_defWidth, CreateMacroUI::s_defHeight, "GJ_square01.png", "Create Macro")) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};

// the actual macro handling process

// sorry, too lazy to figure out how to get the class out
QuartzMacro current_macro;
int keepSpeedhackUntilFrame = -1;

std::vector<QuartzInput> fixInputs(const std::vector<QuartzInput>& inputs) {
    if (inputs.size() <= 1) {
        return inputs;
    }

    std::vector<QuartzInput> filteredInputs;
    filteredInputs.push_back(inputs[0]);

    for (size_t i = 1; i < inputs.size(); ++i) {
        if (inputs[i].down != inputs[i-1].down) {
            filteredInputs.push_back(inputs[i]);
        }
    }
    return filteredInputs;
}

// could probably make a ctor but too lazy!
PlayerFrame PlayerToFrame(PlayerObject* player, int frameNum, bool player2, bool isHolding, int buttonID) {
    PlayerFrame frame;
    frame.frame = frameNum;
    frame.rotation = player->getRotation();
    frame.rotation_speed = player->m_rotationSpeed;
    frame.isRotating = player->m_isRotating;
    frame.isSlope = player->m_isOnSlope;
    frame.wasSlope = player->m_wasOnSlope;
    frame.pos.x = player->getPositionX();
    frame.pos.y = player->getPositionY();
    frame.realPos = player->m_position;
    frame.velocity.x = player->m_platformerXVelocity;
    frame.velocity.y = player->m_yVelocity;
    frame.player2 = player2;
    frame.isHolding = isHolding;
    frame.buttonHold = buttonID;
    return frame;
}

void UpdatePlayer(PlayerObject* player, PlayerFrame frame) {
    player->setPosition(frame.pos);
    player->m_position = frame.realPos;
    player->setRotation(frame.rotation);
    player->m_rotationSpeed = frame.rotation_speed;
    player->m_isRotating = frame.isRotating;
    player->m_isOnSlope = frame.isSlope;
    player->m_wasOnSlope = frame.wasSlope;
    player->m_yVelocity = frame.velocity.y;
    player->m_platformerXVelocity = frame.velocity.x;
}
void UpdatePlayerType(PlayerObject* player, PlayerFrame frame, MacroType type) {
    if (frame.realPos.y == -1) {
        player->m_position.x = frame.realPos.x;
    } else {
        player->m_position = frame.realPos;
        player->m_yVelocity = frame.velocity.y;
    }
    if (type == MacroType::MH) return;
    player->setPosition(frame.pos);
    player->setRotation(frame.rotation);
    player->m_rotationSpeed = frame.rotation_speed;
    if (type == MacroType::QUARTZ) {
        player->m_isRotating = frame.isRotating;
        player->m_isOnSlope = frame.isSlope;
        player->m_wasOnSlope = frame.wasSlope;
    }
    player->m_platformerXVelocity = frame.velocity.x;
}

bool STOPTIME = false;

bool stillHolding1 = false;
bool stillHolding2 = false;
bool hadHeld = false;
bool macroSpeedhack = false;

bool GotoFrameUI::setup() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto cSize = m_size;
    this->setTitle("Goto Frame");
    
    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr,
        this,
        menu_selector(GotoFrameUI::onInfoBtn)
    );
    m_buttonMenu->addChildAtPosition(infoBtn, Anchor::TopRight);

    auto frameLabel = CCLabelBMFont::create("Frame", "bigFont.fnt");
    frameLabel->setScale(.75F);
    m_frame = TextInput::create(150.f, "Frame...", "bigFont.fnt");
    m_frame->setMaxCharCount(50);
    m_frame->setCommonFilter(CommonFilter::Uint);

    auto gotoBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Go to Frame"),
        this,
        menu_selector(GotoFrameUI::gotoFrameBtn)
    );

    m_buttonMenu->addChildAtPosition(frameLabel, Anchor::Center, {0, 35});
    m_buttonMenu->addChildAtPosition(m_frame, Anchor::Center);
    m_buttonMenu->addChildAtPosition(gotoBtn, Anchor::Bottom, {0, 40});
    
    STOPTIME = true;
    return true;
}

bool ClearFramesUI::setup() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto cSize = m_size;
    this->setTitle("Clear Frames");
    
    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr,
        this,
        menu_selector(ClearFramesUI::onInfoBtn)
    );
    m_buttonMenu->addChildAtPosition(infoBtn, Anchor::TopRight);
    
    //m_buttonMenu->addChildAtPosition(slider, Anchor::Center, {0, -40});

    return true;
}

bool dxDebug = false;

class $modify(QuartzPlayLayer, PlayLayer) {
    struct Fields {
        float bot_frame_dt = 0.F;
        int bot_frame = 0;
        int clicks = 0;
        int lastInputFrame = 0;
        int previousFrame = 0;
        bool playLayerPostUpdate = false;

        int playLayerInit = 0;

        Slider* m_slider;
        CCLabelBMFont* frameLabel;

        CCMenu* buttons;
        // why didnt i use checkboxes
        CCMenuItemSpriteExtra* stopBtn;
        CCMenuItemSpriteExtra* playBtn;
        CCMenuItemSpriteExtra* nextBtn;
        CCMenuItemToggler* speedhackBtn;

        CCMenuItemSpriteExtra* inputStateBtn1_p1;
        CCMenuItemSpriteExtra* inputStateBtn2_p1;

        CCMenuItemSpriteExtra* inputStateBtn1_p2;
        CCMenuItemSpriteExtra* inputStateBtn2_p2;
        //std::vector<int> checkpoints;
        std::map<CheckpointObject*, int> checkpoints = {};
        bool replay = false;
        bool started = false;

        bool holdingP1 = false;
        bool holdingP2 = false;

        bool realHoldingP1 = false;
        bool realHoldingP2 = false;
        bool changedInputP1 = false;
        bool changedInputP2 = false;

        bool player2Visible = false;

        std::vector<PlayerFrame> player_frames;
        CCPoint previousPosition;

        CCScale9Sprite* m_dxDebug;

    };
    /*bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        
        return true;
    }*/

    void RecreateInputState(int state, bool player2, bool force) {
        if (m_fields->inputStateBtn1_p1 == nullptr || m_fields->inputStateBtn2_p1 == nullptr) return;
        if (m_fields->inputStateBtn1_p2 == nullptr || m_fields->inputStateBtn2_p2 == nullptr) return;
        if (!Hacks::isHackEnabled("Record") && !Hacks::isHackEnabled("Playback")) return;
        if (m_fields->replay || force) {
            if (player2) {
                log::debug("[P2] change state to {}", state);
                m_fields->realHoldingP2 = state == 2;
                m_fields->inputStateBtn1_p2->setVisible((state == 1));
                m_fields->inputStateBtn2_p2->setVisible((state == 2));
            } else {
                log::debug("[P1] change state to {}", state);
                m_fields->realHoldingP1 = state == 2;
                m_fields->inputStateBtn1_p1->setVisible((state == 1));
                m_fields->inputStateBtn2_p1->setVisible((state == 2));
            }
        }
    }
    void startGame() {
        STOPTIME = false;
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        PlayLayer::startGame();
        if (Hacks::isHackEnabled("Playback") || Hacks::isHackEnabled("Record")) {
            auto macroItem = Hacks::getHack("Macro");
            if (macroItem != nullptr) {
                std::string value = macroItem->data["values"].asArray().unwrap()[0].asString().unwrapOrDefault();
                changedMacro = false;
                if (value != "None") {
                    current_macro = MacroManager::getMacro(value);
                    current_macro.isEnabled = true;
                    if (Hacks::isHackEnabled("Record")) {
                        current_macro.inputs = {};
                        current_macro.duration = 0.F;
                        current_macro.levelInfo.name = m_level->m_levelName;
                        current_macro.levelInfo.id = m_level->m_levelID;
                        current_macro.ldm = m_level->m_lowDetailModeToggled;
                    } else if (Hacks::isHackEnabled("Playback")) {
                        if (current_macro.inputs.size() == 0) {
                            current_macro.isEnabled = false;
                            m_fields->started = false;
                            FLAlertLayer::create("Error", "You are attempting to <cy>playback a macro</c> that <cr>has no inputs!</c>\nConsider <cy>recording the macro</c> to play it back!", "OK")->show();
                            return;
                        } else {
                            std::sort(current_macro.inputs.begin(), current_macro.inputs.end(), [](const QuartzInput& a, const QuartzInput& b) {
                                return a.frame < b.frame;
                            });
                            if (!current_macro.inputs.empty()) {
                                //current_macro.inputs = fixInputs(current_macro.inputs);
                                m_fields->lastInputFrame = current_macro.inputs.back().frame;
                            }
                        }
                    }
                    Hacks::setTPS(current_macro.framerate);
                } else {
                    current_macro.isEnabled = false;
                    m_fields->started = false;
                    if (Hacks::isHackEnabled("Record")) {
                        FLAlertLayer::create("Error", "You are attempting to <cy>record a macro</c> that <cr>is not selected!</c>\nConsider either <cy>creating a macro</c> to record, or <cy>selecting a macro</c> to playback.", "OK")->show();
                    } else if (Hacks::isHackEnabled("Playback")) {
                        FLAlertLayer::create("Error", "You are attempting to <cy>playback a macro</c> that <cr>is not selected!</c>\nConsider either <cy>selecting a macro</c> to playback.", "OK")->show();
                    }
                    return;
                }
            }
        }
        if (Hacks::isHackEnabled("Playback") && Hacks::isHackEnabled("Record")) {
            FLAlertLayer::create("Error", "You can't have both <cy>Record</c> and <cy>Playback</c> enabled!", "OK")->show();
            m_fields->started = false;
            return;
        }
        m_fields->buttons = CCMenu::create();
        m_fields->started = true;
        m_fields->m_slider = Slider::create(this, menu_selector(QuartzPlayLayer::onSliderSelect), 1.4f);
        m_fields->m_slider->setValue(0);
        m_fields->m_slider->setPositionY(60);
        m_fields->frameLabel = CCLabelBMFont::create("0/0", "chatFont.fnt");
        m_fields->frameLabel->setPositionY(80);
        m_fields->buttons->addChild(m_fields->frameLabel);
        m_fields->buttons->addChild(m_fields->m_slider);

        auto prevArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        auto nextArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        nextArrow->setFlipX(true);
        m_fields->nextBtn = CCMenuItemSpriteExtra::create(
            nextArrow,
            this,
            menu_selector(QuartzPlayLayer::nextFrame)
        );
        auto prevBtn = CCMenuItemSpriteExtra::create(
            prevArrow,
            this,
            menu_selector(QuartzPlayLayer::prevFrame)
        );
        m_fields->playBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_playEditorBtn_001.png"),
            this,
            menu_selector(QuartzPlayLayer::toggleFramePlayback)
        );
        m_fields->stopBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_stopEditorBtn_001.png"),
            this,
            menu_selector(QuartzPlayLayer::toggleFramePlayback)
        );
        m_fields->playBtn->setVisible(false);

        m_fields->playBtn->setPositionY(25);
        m_fields->stopBtn->setPositionY(m_fields->playBtn->getPositionY());
        prevBtn->setPosition({-40, 25});
        m_fields->nextBtn->setPosition({40, 25});


        auto relSpr = CCSprite::create("GJ_button_01.png");
        auto relTxt = CCLabelBMFont::create("Rel", "bigFont.fnt");
        relTxt->setScale(.55F);
        relTxt->setAnchorPoint({0, 0});
        relTxt->setPosition({4, 12});
        relSpr->addChild(relTxt);
        auto holdSpr = CCSprite::create("GJ_button_06.png");
        auto holdTxt = CCLabelBMFont::create("Hold", "bigFont.fnt");
        holdTxt->setScale(.4F);
        holdTxt->setAnchorPoint({0, 0});
        holdTxt->setPosition({4, 14});
        holdSpr->addChild(holdTxt);

        m_fields->inputStateBtn1_p1 = CCMenuItemSpriteExtra::create(
            relSpr,
            this,
            menu_selector(QuartzPlayLayer::markInputP1)
        );
        m_fields->inputStateBtn2_p1 = CCMenuItemSpriteExtra::create(
            holdSpr,
            this,
            menu_selector(QuartzPlayLayer::markInputP1)
        );
        m_fields->inputStateBtn1_p2 = CCMenuItemSpriteExtra::create(
            relSpr,
            this,
            menu_selector(QuartzPlayLayer::markInputP2)
        );
        m_fields->inputStateBtn2_p2 = CCMenuItemSpriteExtra::create(
            holdSpr,
            this,
            menu_selector(QuartzPlayLayer::markInputP2)
        );
        m_fields->inputStateBtn1_p1->setPosition({ (winSize.width / 2) - 90, 55 });
        m_fields->inputStateBtn2_p1->setPosition(m_fields->inputStateBtn1_p1->getPosition());

        m_fields->inputStateBtn1_p2->setPosition({ (winSize.width / 2) - 40, 55 });
        m_fields->inputStateBtn2_p2->setPosition(m_fields->inputStateBtn1_p2->getPosition());

        m_fields->inputStateBtn1_p2->setVisible(false);
        m_fields->inputStateBtn2_p2->setVisible(false);
        RecreateInputState(1, false, true);
        RecreateInputState(1, true, true);

        // what was i thinking, i couldve just create this as a ButtonSprite but ok
        auto clearSpr = CCSprite::create("GJ_button_03.png");
        auto clearTxt = CCLabelBMFont::create("Clear", "bigFont.fnt");
        clearTxt->setScale(.3F);
        clearTxt->setAnchorPoint({0, 0});
        clearTxt->setPosition({4, 15});
        clearSpr->addChild(clearTxt);

        auto gotoSpr = CCSprite::create("GJ_button_05.png");
        auto gotoTxt = CCLabelBMFont::create("Goto", "bigFont.fnt");
        gotoTxt->setScale(.4F);
        gotoTxt->setAnchorPoint({0, 0});
        gotoTxt->setPosition({3, 14});
        gotoSpr->addChild(gotoTxt);

        auto gotoBtn = CCMenuItemSpriteExtra::create(
            gotoSpr,
            this,
            menu_selector(QuartzPlayLayer::gotoBtn)
        );
        auto clearBtn = CCMenuItemSpriteExtra::create(
            clearSpr,
            this,
            menu_selector(QuartzPlayLayer::clearBtn)
        );

        gotoBtn->setPosition({80, 25});
        clearBtn->setPosition({125, 25});

        auto clockSprGreen = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"));
        clockSprGreen->setScale(0.75F);
        auto clockSprRed = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"), CircleBaseColor::Pink);
        clockSprRed->setScale(0.75F);
        m_fields->speedhackBtn = CCMenuItemToggler::create(
            clockSprGreen,
            clockSprRed,
            this,
            menu_selector(QuartzPlayLayer::onSpeedhackBtn)
        );
        m_fields->speedhackBtn->setPosition({-85, 25});
        m_fields->buttons->addChild(prevBtn);
        m_fields->buttons->addChild(m_fields->nextBtn);
        m_fields->buttons->addChild(m_fields->playBtn);
        m_fields->buttons->addChild(m_fields->stopBtn);
        m_fields->buttons->addChild(m_fields->inputStateBtn1_p1);
        m_fields->buttons->addChild(m_fields->inputStateBtn2_p1);
        m_fields->buttons->addChild(m_fields->inputStateBtn1_p2);
        m_fields->buttons->addChild(m_fields->inputStateBtn2_p2);
        m_fields->buttons->addChild(gotoBtn);
        //m_fields->buttons->addChild(clearBtn); // TODO: do this later
        m_fields->buttons->addChild(m_fields->speedhackBtn);
        m_fields->buttons->setID("macro-editor"_spr);
        m_fields->buttons->setVisible(Hacks::isHackEnabled("Macro Editor"));
        m_uiLayer->addChild(m_fields->buttons);
        m_fields->buttons->setPositionY(0);

        // TODO: add advance and prev frame for +5 and -5, oh and also allow clearing from right or clearing a section
        // here
    }

    void gotoBtn(CCObject*) {
        GotoFrameUI::create()->show();
    }
    void clearBtn(CCObject*) {
        ClearFramesUI::create()->show();
    }
    void onSpeedhackBtn(CCObject*) {
        macroSpeedhack = !macroSpeedhack; 
        if (auto speedhack = Hacks::getHack("Speedhack")) {
            // TODO: make this customizable in a future update
            speedhack->value.floatValue = (macroSpeedhack) ? 0.25F : 1.0F;
            Hacks::setPitch(speedhack->value.floatValue);
        }
    }

    void markInputP1(CCObject*) {
        int bot_frame = m_fields->bot_frame;
        auto input = std::find_if(current_macro.inputs.begin(), current_macro.inputs.end(), [bot_frame](const QuartzInput& input) {
            return input.frame == bot_frame && input.player2 == false;
        });
        if (input != current_macro.inputs.end()) {
            m_fields->holdingP1 = false;
            input->down = !input->down;
            m_fields->changedInputP1 = input->down;
            log::debug("[P1] Change {} to {}", m_fields->bot_frame, input->down);
            GJBaseGameLayer::handleButton(input->down, input->button, true);
            RecreateInputState((input->down) ? 2 : 1, false, false);
        } else {
            QuartzInput newInput(bot_frame, 1, false, true);
            auto insertPos = std::find_if(current_macro.inputs.begin(), current_macro.inputs.end(), [bot_frame](const QuartzInput& input) {
                return input.frame > bot_frame && input.player2 == false;
            });
            m_fields->changedInputP1 = newInput.down;
            log::debug("[P1] Insert new frame at {}", m_fields->bot_frame);
            current_macro.inputs.insert(insertPos, newInput);
            GJBaseGameLayer::handleButton(newInput.down, newInput.button, true);
            m_fields->holdingP1 = true;
            RecreateInputState(2, false, false);
        }
        // because the input technically is gone after the frame once you chance it. how else am i supposed to calculate this!?
        for (int i = m_fields->player_frames.size(); i > bot_frame; i--) {
            auto input_player = std::find_if(m_fields->player_frames.begin(), m_fields->player_frames.end(), [i](const PlayerFrame& input) {
                return input.frame == i && input.player2 == false;
            });
            if (input_player != m_fields->player_frames.end()) {
                m_fields->player_frames.pop_back();
            }
        }
        /*
        auto input = std::find_if(current_macro.inputs.begin(), current_macro.inputs.end(), [this, bot_frame](QuartzInput input) {
            bool condition = input.frame == bot_frame && input.player2 == false;
            if (condition) {
                input.down = !input.down;
                RecreateInputState((input.down) ? 2 : 1, false);
            }
            return condition;
        });
        if (input == current_macro.inputs.end()) {
            QuartzInput newInput(bot_frame, 1, false, true);
            auto insertPos = std::find_if(current_macro.inputs.begin(), current_macro.inputs.end(), [bot_frame](const QuartzInput& input) {
                return input.frame > bot_frame;
            });

            // Insert the new element at the desired position
            current_macro.inputs.insert(insertPos, newInput);
            RecreateInputState(2, false);
        }*/
    }

    void markInputP2(CCObject*) {
        int bot_frame = m_fields->bot_frame;
        auto input = std::find_if(current_macro.inputs.begin(), current_macro.inputs.end(), [bot_frame](const QuartzInput& input) {
            return input.frame == bot_frame && input.player2 == true;
        });
        if (input != current_macro.inputs.end()) {
            m_fields->holdingP2 = false;
            input->down = !input->down;
            m_fields->changedInputP2 = input->down;
            log::debug("[P2] Change {} to {}", m_fields->bot_frame, input->down);
            GJBaseGameLayer::handleButton(input->down, input->button, false);
            RecreateInputState((input->down) ? 2 : 1, true, false);
        } else {
            QuartzInput newInput(bot_frame, 1, true, true);
            auto insertPos = std::find_if(current_macro.inputs.begin(), current_macro.inputs.end(), [bot_frame](const QuartzInput& input) {
                return input.frame > bot_frame && input.player2 == true;
            });
            m_fields->changedInputP2 = newInput.down;
            log::debug("[P2] Insert new frame at {}", m_fields->bot_frame);
            current_macro.inputs.insert(insertPos, newInput);
            GJBaseGameLayer::handleButton(newInput.down, newInput.button, false);
            m_fields->holdingP2 = true;
            RecreateInputState(2, true, false);
        }
        for (int i = m_fields->player_frames.size(); i > bot_frame; i--) {
            auto input_player = std::find_if(m_fields->player_frames.begin(), m_fields->player_frames.end(), [i](const PlayerFrame& input) {
                return input.frame == i && input.player2 == true;
            });
            if (input_player != m_fields->player_frames.end()) {
                m_fields->player_frames.pop_back();
            }
        }
    }

    void prevFrame(CCObject*) {
        if (m_fields->bot_frame <= 0 || !STOPTIME) return;
        m_fields->bot_frame -= 2;
        m_gameState.m_currentProgress -= 2;
        int bot_frame = m_fields->bot_frame;
        auto input_player = std::find_if(m_fields->player_frames.begin(), m_fields->player_frames.end(), [bot_frame](const PlayerFrame& input) {
            return input.frame == bot_frame && input.player2 == false;
        });
        auto input_player2 = std::find_if(m_fields->player_frames.begin(), m_fields->player_frames.end(), [bot_frame](const PlayerFrame& input) {
            return input.frame == bot_frame && input.player2 == true;
        });
        if (input_player != m_fields->player_frames.end() || input_player2 != m_fields->player_frames.end()) {
            if (input_player2 != m_fields->player_frames.end()) {
                UpdatePlayer(m_player2, *input_player2);

                m_fields->changedInputP2 = (m_fields->changedInputP2 != input_player2->isHolding);
                if (m_fields->changedInputP2) {
                    GJBaseGameLayer::handleButton(input_player2->isHolding, input_player2->buttonHold, true);
                }
                RecreateInputState((input_player2->isHolding) ? 2 : 1, true, false);
            }
            if (input_player != m_fields->player_frames.end()) {
                UpdatePlayer(m_player1, *input_player);

                m_fields->changedInputP1 = (m_fields->changedInputP1 != input_player->isHolding);
                if (m_fields->changedInputP1) {
                    GJBaseGameLayer::handleButton(input_player->isHolding, input_player->buttonHold, false);
                }
                RecreateInputState((input_player->isHolding) ? 2 : 1, true, false);
            }
        }
        STOPTIME = false;
        GJBaseGameLayer::update(1.F / current_macro.framerate);
        STOPTIME = true;
        log::debug("[-] Frame {}", m_fields->bot_frame);
    }
    void nextFrame(CCObject*) {
        //if (m_fields->bot_frame >= m_fields->lastInputFrame || !STOPTIME) return;
        if (!STOPTIME) return;
        int bot_frame = m_fields->bot_frame + 1;
        //std::cout << fmt::format("frame {} populated {}", bot_frame, m_fields->player_frames.size()) << std::endl;
        auto input_player = std::find_if(m_fields->player_frames.begin(), m_fields->player_frames.end(), [bot_frame](const PlayerFrame& input) {
            return input.frame == bot_frame && input.player2 == false;
        });
        auto input_player2 = std::find_if(m_fields->player_frames.begin(), m_fields->player_frames.end(), [bot_frame](const PlayerFrame& input) {
            return input.frame == bot_frame && input.player2 == true;
        });
        if (input_player != m_fields->player_frames.end()) {
            m_fields->bot_frame = input_player->frame;
            m_gameState.m_currentProgress = m_fields->bot_frame;
            if (input_player2 != m_fields->player_frames.end()) {
                UpdatePlayer(m_player2, *input_player2);
                m_fields->changedInputP2 = (m_fields->changedInputP2 != input_player2->isHolding);
                if (m_fields->changedInputP2) {
                    GJBaseGameLayer::handleButton(input_player2->isHolding, input_player2->buttonHold, true);
                }
                RecreateInputState((input_player2->isHolding) ? 2 : 1, true, false);
            }
            if (input_player != m_fields->player_frames.end()) {
                UpdatePlayer(m_player1, *input_player);
                m_fields->changedInputP1 = (m_fields->changedInputP1 != input_player->isHolding);
                if (m_fields->changedInputP1) {
                    GJBaseGameLayer::handleButton(input_player->isHolding, input_player->buttonHold, false);
                }
                RecreateInputState((input_player->isHolding) ? 2 : 1, true, false);
            }
        }
        STOPTIME = false;
        GJBaseGameLayer::update(1.F / current_macro.framerate);
        STOPTIME = true;
        log::debug("[+] Frame {}", m_fields->bot_frame);
    }

    void toggleFramePlayback(CCObject*) {
        STOPTIME = !STOPTIME;
        m_fields->playBtn->setVisible(STOPTIME);
        m_fields->stopBtn->setVisible(!STOPTIME);
    }

    void onSliderSelect(CCObject*) {
        if (!STOPTIME) return;
        int bot_frame = static_cast<int>(Utils::getSliderValue(m_fields->m_slider->getThumb()->getValue(), 0, m_fields->lastInputFrame, true));
        //std::cout << fmt::format("frame {} populated {}", bot_frame, m_fields->player_frames.size()) << std::endl;
        auto input_player = std::find_if(m_fields->player_frames.begin(), m_fields->player_frames.end(), [bot_frame](const PlayerFrame& input) {
            return input.frame == bot_frame && input.player2 == false;
        });
        auto input_player2 = std::find_if(m_fields->player_frames.begin(), m_fields->player_frames.end(), [bot_frame](const PlayerFrame& input) {
            return input.frame == bot_frame && input.player2 == true;
        });
        if (input_player != m_fields->player_frames.end()) {
            m_fields->bot_frame = input_player->frame;
            m_gameState.m_currentProgress = m_fields->bot_frame;
            if (input_player2 != m_fields->player_frames.end()) {
                UpdatePlayer(m_player2, *input_player2);
                GJBaseGameLayer::handleButton(input_player2->isHolding, input_player2->buttonHold, true);
                RecreateInputState((input_player2->isHolding) ? 2 : 1, true, false);
            } else {
                GJBaseGameLayer::handleButton(input_player->isHolding, input_player->buttonHold, false);
                RecreateInputState((input_player->isHolding) ? 2 : 1, true, false);
            }
            UpdatePlayer(m_player1, *input_player);
        }
        STOPTIME = false;
        GJBaseGameLayer::update(1.F / current_macro.framerate);
        STOPTIME = true;
    }

    void onQuit() {
        m_fields->bot_frame_dt = 0.F;
        m_fields->bot_frame = 0;
        m_fields->previousFrame = 0;
        current_macro.loaf = 0.0F;
        current_macro.isEnabled = false;
        m_fields->playLayerPostUpdate = false;
        Hacks::setTPS();
        PlayLayer::onQuit();
    }
    void resetLevel() {
        // here
        if (m_fields->checkpoints.size() == 0 || !m_isPracticeMode) {
            m_fields->bot_frame_dt = 0.F;
            m_fields->bot_frame = 0;
            m_fields->previousFrame = 0;
            m_fields->checkpoints.clear();
            RecreateInputState(1, false, false);
            RecreateInputState(1, true, false);
            m_fields->playLayerPostUpdate = false;
        }
        current_macro.loaf = 0.0F;
        if (m_fields->replay) {
            Hacks::setTPS(current_macro.framerate);
        } else {
            Hacks::setTPS();
        }
        /*stillHolding1 = false;
        stillHolding2 = false;*/
        PlayLayer::resetLevel();
    }
    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);
        if (m_player1->m_position == m_player1->m_startPosition) {
            m_fields->playLayerPostUpdate = true;
        }
        if ((Hacks::isHackEnabled("Playback") || Hacks::isHackEnabled("Record")) && changedMacro) {
            auto macroItem = Hacks::getHack("Macro");
            if (macroItem != nullptr) {
                std::string value = macroItem->data["values"].asArray().unwrap()[0].asString().unwrapOrDefault();
                if (value != "None") {
                    current_macro = MacroManager::getMacro(value);
                    current_macro.isEnabled = true;
                    changedMacro = false;
                    m_fields->bot_frame = 0;
                    m_fields->previousFrame = 0;
                    PlayLayer::resetLevel();
                    if (Hacks::isHackEnabled("Record")) {
                        current_macro.inputs = {};
                        current_macro.levelInfo.name = m_level->m_levelName;
                        current_macro.levelInfo.id = m_level->m_levelID;
                        current_macro.ldm = m_level->m_lowDetailModeToggled;
                    } else if (Hacks::isHackEnabled("Playback")) {
                        if (!current_macro.inputs.empty())
                            m_fields->lastInputFrame = current_macro.inputs.back().frame;
                    }
                }
                changedMacro = false;
            }
        }
    }
    void storeCheckpoint(CheckpointObject* p0) {
        if (m_fields->replay) {
            m_fields->checkpoints[p0] = m_fields->bot_frame;
        }
		PlayLayer::storeCheckpoint(p0);
	}
    void loadFromCheckpoint(CheckpointObject* p0) {
        if (m_fields->replay) {
            RecreateInputState(1, false, false);
            RecreateInputState(1, true, false);
            // COMO NO ME
            if (m_fields->checkpoints.size() > 0 && m_fields->checkpoints.contains(p0) && Hacks::isHackEnabled("Record")) {
                int lastFrame = m_fields->checkpoints[p0];
                std::erase_if(current_macro.inputs, [lastFrame](const QuartzInput& input) {
                    if (input.frame > lastFrame) {
                        //log::debug("Deleted input at frame {}", input.frame);
                        return true;
                    } else {
                        return false;
                    }
                });
                log::info("Removed all inputs after frame {}", lastFrame);
                m_fields->bot_frame = lastFrame;
                if ((stillHolding1 || stillHolding2) && !hadHeld) {
                    bool player2Vis = m_player2 != nullptr && m_player2->isRunning();
                    QuartzInput input1;
                    QuartzInput input2;
                    bool hasReld1 = false;
                    bool hasReld2 = false;
                    if (stillHolding1 && !player2Vis) {
                        input1 = QuartzInput(lastFrame + 1, 1, false, false);
                        hasReld1 = true;
                    } else if (stillHolding1 && !stillHolding2 && player2Vis) {
                        input2 = QuartzInput(lastFrame + 1, 1, true, false);
                        //hadHeld = false;
                        hasReld2 = true;
                    } else if (stillHolding1 && stillHolding2 && player2Vis) {
                        input1 = QuartzInput(lastFrame + 1, 1, false, false);
                        input2 = QuartzInput(lastFrame + 1, 1, true, false);
                        hasReld1 = true;
                        hasReld2 = true;
                    }
                    if (hasReld1 || hasReld2) {
                        if (Hacks::isHackEnabled("Frame Fix")) {
                            if (hasReld1) {
                                PlayerFrame frameFixed = PlayerToFrame(m_player1, lastFrame + 1, false, false, 1);
                                frameFixed.fix = true;
                                frameFixed.type = MacroType::QUARTZ;
                                input1.frameFix = frameFixed;
                            }
                            if (hasReld2) {
                                PlayerFrame frameFixed = PlayerToFrame(m_player2, lastFrame + 1, true, false, 1);
                                frameFixed.fix = true;
                                frameFixed.type = MacroType::QUARTZ;
                                input2.frameFix = frameFixed;
                            }
                        }
                        if (hasReld1) current_macro.inputs.push_back(input1);
                        if (hasReld2) current_macro.inputs.push_back(input2);
                        log::debug("Fix Release Input at frame {}", lastFrame);
                    }
                }
            }
        }
		PlayLayer::loadFromCheckpoint(p0);
	}
};
// int m_unk1f8; // used in PlayLayer::getCurrentPercent

class $modify(QuartzGJBGL, GJBaseGameLayer) {
    void update(float dt) {
        if (STOPTIME) return;
        GJBaseGameLayer::update(dt);
    }
    void processCommands(float realDt) {
        if (STOPTIME) return;
        if (!Hacks::isHackEnabled("Playback") && !Hacks::isHackEnabled("Record") && !Hacks::isHackEnabled("Macro Editor")) return GJBaseGameLayer::processCommands(realDt);
        float currentFPS = 1.F / realDt;
        float macroFPS = current_macro.framerate;
        if (PlayLayer::get() != nullptr) {
            auto playLayer = static_cast<QuartzPlayLayer*>(QuartzPlayLayer::get());
            if (!playLayer->m_fields->started || !playLayer->m_fields->playLayerPostUpdate) return GJBaseGameLayer::processCommands(realDt);
            playLayer->m_fields->replay = Hacks::isHackEnabled("Playback") || Hacks::isHackEnabled("Record");
            float dt = realDt;
            playLayer->m_fields->previousPosition = m_player1->getPosition();
            playLayer->m_fields->player2Visible = m_player2 != nullptr && m_player2->isRunning();
            if (playLayer->m_fields->buttons != nullptr) {
                playLayer->m_fields->buttons->setVisible(Hacks::isHackEnabled("Macro Editor"));
            }
            if (playLayer->m_fields->lastInputFrame != 0 && playLayer->m_fields->bot_frame > playLayer->m_fields->lastInputFrame) {
                playLayer->m_fields->lastInputFrame++;
            } else if (playLayer->m_fields->lastInputFrame == 0 && Hacks::isHackEnabled("Record")) {
                playLayer->m_fields->lastInputFrame = playLayer->m_fields->bot_frame + 1; // does + 2 even matter? it gets reset anyways!
            }
            bool checkIfMoved = false;
            if (m_level->isPlatformer()) {
                checkIfMoved = true;
            } else {
                checkIfMoved = m_player1->m_position != m_player1->m_startPosition;
            }
            if (!m_player1->m_isDead && checkIfMoved) {
                if (!playLayer->m_fields->player2Visible && (playLayer->m_fields->inputStateBtn1_p2 != nullptr && playLayer->m_fields->inputStateBtn2_p2 != nullptr)) {
                    playLayer->m_fields->inputStateBtn1_p2->setVisible(false);
                    playLayer->m_fields->inputStateBtn2_p2->setVisible(false);
                }
                playLayer->m_fields->bot_frame_dt += dt;

                // here

                float addDt = dt * macroFPS;
                float fixedDt = 1.0 / macroFPS;
                playLayer->m_fields->bot_frame = m_gameState.m_currentProgress;
                if (Hacks::isHackEnabled("Fixed FPS")) {
                    dt = fixedDt;
                }
                if (m_gameState.m_currentProgress >= keepSpeedhackUntilFrame && keepSpeedhackUntilFrame != -1) {
                    macroSpeedhack = false;
                    keepSpeedhackUntilFrame = -1;
                    STOPTIME = true;
                    playLayer->m_fields->playBtn->setVisible(STOPTIME);
                    playLayer->m_fields->stopBtn->setVisible(!STOPTIME);
                    if (auto speedhack = Hacks::getHack("Speedhack")) {
                        speedhack->value.floatValue = 1.0F;
                        Hacks::setPitch(speedhack->value.floatValue);
                    }
                }
                playLayer->m_fields->frameLabel->setString(fmt::format("{}/{} ({}/{})", playLayer->m_fields->bot_frame, playLayer->m_fields->lastInputFrame, currentFPS, macroFPS, dt).c_str());
                playLayer->m_fields->m_slider->setValue(Utils::getSliderValue(playLayer->m_fields->bot_frame, 0, playLayer->m_fields->lastInputFrame, false));
                if (Hacks::isHackEnabled("Playback")) {
                    //if (current_macro.inputs.size() > m_fields->bot_frame) {
                    int bot_frame = playLayer->m_fields->bot_frame;
                    /*std::vector<QuartzInput> matchingInputs;
                    for (auto it = current_macro.inputs.begin(); it != current_macro.inputs.end(); it++) {
                        if (it->frame == bot_frame) {
                            matchingInputs.push_back(*it);
                        }
                    }*/
                    bool hadDone = false;
                    auto input1 = std::find_if(current_macro.inputs.begin(), current_macro.inputs.end(), [bot_frame](const QuartzInput& input) {
                        return input.frame == bot_frame && !input.player2;
                    });
                    auto input2 = std::find_if(current_macro.inputs.begin(), current_macro.inputs.end(), [bot_frame](const QuartzInput& input) {
                        return input.frame == bot_frame && input.player2;
                    });

                    if (input1 != current_macro.inputs.end() || input2 != current_macro.inputs.end()) {
                        bool player1 = input1 != current_macro.inputs.end();
                        bool player2 = input2 != current_macro.inputs.end();

                        int agfvcbeaga = playLayer->m_fields->playLayerInit;
                        int acheronbyriotandmore = playLayer->m_fields->lastInputFrame;
                        agfvcbeaga = (0x00000000000818A6 + 0x0000000000040E53 + 0x0000000000041453 - 0x00000000000C2EF9);
                        if ((agfvcbeaga ^ 0x0000000000040C53)) return;
                        playLayer->m_fields->playLayerInit = agfvcbeaga;
                        if (!!!(agfvcbeaga == playLayer->m_fields->playLayerInit)) return;

                        if (playLayer->m_fields->previousFrame != playLayer->m_fields->bot_frame) {
                            if (player1) {
                                playLayer->m_fields->changedInputP1 = (playLayer->m_fields->changedInputP1 != input1->down);
                            }
                            if (player2) {
                                playLayer->m_fields->changedInputP2 = (playLayer->m_fields->changedInputP2 != input2->down);
                            }
                        }
                        if (player1) {
                            if (!dxDebug) {
                                GJBaseGameLayer::handleButton(input1->down, input1->button, !input1->player2);
                            }
                            playLayer->m_fields->changedInputP1 = input1->down;
                        }
                        if (player2) {
                            if (!dxDebug) {
                                GJBaseGameLayer::handleButton(input2->down, input2->button, !input2->player2);
                            }
                            playLayer->m_fields->changedInputP2 = input2->down;
                        }
                        
                        playLayer->m_fields->previousFrame = playLayer->m_fields->bot_frame;
                        if (player1) playLayer->m_fields->player_frames.push_back(PlayerToFrame(m_player1, playLayer->m_fields->bot_frame, false, input1->down, input1->button));
                        //(m_player2 != nullptr && (m_player2->getPositionX() != 0 && m_player2->m_position.x != 0)
                        if (playLayer->m_fields->player2Visible) {
                            if (player2) {
                                playLayer->m_fields->player_frames.push_back(PlayerToFrame(m_player2, playLayer->m_fields->bot_frame, true, input2->down, input2->button));
                            } else if (player1) playLayer->m_fields->player_frames.push_back(PlayerToFrame(m_player2, playLayer->m_fields->bot_frame, true, input1->down, input1->button));
                        }
                        if (((player1 && input1->frameFix.fix) || (player2 && input2->frameFix.fix)) && Hacks::isHackEnabled("Frame Fix")) {
                            if (player1) {
                                /*if (input1->frameFix.player2) {
                                    UpdatePlayerType(m_player2, input1->frameFix, input1->frameFix.type);
                                } else {
                                    UpdatePlayerType(m_player1, input1->frameFix, input1->frameFix.type);
                                }*/ 
                                if (!input1->frameFix.player2) {
                                    UpdatePlayerType(m_player1, input1->frameFix, input1->frameFix.type);
                                }
                            }
                            if (player2) {
                                if (input2->frameFix.player2) {
                                    UpdatePlayerType(m_player2, input2->frameFix, input2->frameFix.type);
                                } else {
                                    UpdatePlayerType(m_player1, input2->frameFix, input2->frameFix.type);
                                }
                            }
                        }
                        hadDone = true;
                        if (player1) {
                            playLayer->RecreateInputState((input1->down) ? 2 : 1, input1->player2, false);
                            if (!input1->down && !input1->player2) {
                                playLayer->m_fields->holdingP1 = false;
                                playLayer->m_fields->realHoldingP1 = false;
                            }
                            if (!input1->down && input1->player2) {
                                playLayer->m_fields->holdingP2 = false;
                                playLayer->m_fields->realHoldingP2 = false;
                            }
                        }
                        if (player2) {
                            playLayer->RecreateInputState((input2->down) ? 2 : 1, input2->player2, false);
                            if (!input2->down && input2->player2) {
                                playLayer->m_fields->holdingP2 = false;
                                playLayer->m_fields->realHoldingP2 = false;
                            }
                        }
                    } else if (playLayer->m_fields->holdingP1 || playLayer->m_fields->holdingP2) {
                        QuartzInput newInput(bot_frame, 1, playLayer->m_fields->holdingP2, true);
                        auto insertPos = std::find_if(current_macro.inputs.begin(), current_macro.inputs.end(), [bot_frame](const QuartzInput& input) {
                            return input.frame > bot_frame;
                        });

                        // Insert the new element at the desired position
                        log::debug("[Hold] Insert new frame at {}", playLayer->m_fields->bot_frame);
                        current_macro.inputs.insert(insertPos, newInput);
                        if (!dxDebug) {
                            GJBaseGameLayer::handleButton(newInput.down, newInput.button, newInput.player2);
                        }
                        playLayer->RecreateInputState(2, playLayer->m_fields->holdingP2, false);
                    }
                    if (!hadDone) {
                        playLayer->m_fields->player_frames.push_back(PlayerToFrame(m_player1, playLayer->m_fields->bot_frame, false, playLayer->m_fields->realHoldingP2, 1));
                        if (playLayer->m_fields->player2Visible) {
                            playLayer->m_fields->player_frames.push_back(PlayerToFrame(m_player2, playLayer->m_fields->bot_frame, true, playLayer->m_fields->realHoldingP2, 1));
                        }
                    }
                } 
            }
            GJBaseGameLayer::processCommands(dt);
        } else {
            GJBaseGameLayer::processCommands(realDt);
        }
    }
    void handleButton(bool push, int button, bool player1) {
        //if (STOPTIME) return;
        if (player1) stillHolding1 = push;
        if (!player1) stillHolding2 = push;
        GJBaseGameLayer::handleButton(push,button,player1);
        if (PlayLayer::get() != nullptr && Hacks::isHackEnabled("Record")) {
            auto playLayer = static_cast<QuartzPlayLayer*>(QuartzPlayLayer::get());
            QuartzInput input(playLayer->m_fields->bot_frame, button, !player1, push);
            if (Hacks::isHackEnabled("Frame Fix")) {
                if (player1) {
                    PlayerFrame frameFixed = PlayerToFrame(m_player1, playLayer->m_fields->bot_frame, !player1, push, button);
                    frameFixed.fix = true;
                    frameFixed.type = MacroType::QUARTZ;
                    input.frameFix = frameFixed;
                } else {
                    PlayerFrame frameFixed = PlayerToFrame(m_player2, playLayer->m_fields->bot_frame, !player1, push, button);
                    frameFixed.fix = true;
                    frameFixed.type = MacroType::QUARTZ;
                    input.frameFix = frameFixed;
                }
            }
            // geode needs to stop logging this in the .txt files
            log::debug("[{}] Frame {}", (push) ? "HOLD" : "RELEASE", playLayer->m_fields->bot_frame);
            current_macro.inputs.push_back(input);
        }
    }
};

void GotoFrameUI::gotoFrameBtn(CCObject*) {
    onClose(nullptr);
    if (PlayLayer::get() != nullptr) {
        auto playLayer = static_cast<QuartzPlayLayer*>(QuartzPlayLayer::get());
        int value = std::stoi(m_frame->getInputNode()->getString());
        if (value == 0) return playLayer->resetLevel();
        if (value > playLayer->m_fields->lastInputFrame) value = playLayer->m_fields->lastInputFrame;
        if (playLayer->m_fields->bot_frame > value) playLayer->resetLevel();
        if (auto speedhack = Hacks::getHack("Speedhack")) {
            keepSpeedhackUntilFrame = value;
            macroSpeedhack = false;
            STOPTIME = false;
            speedhack->value.floatValue = 10.0F;
            Hacks::setPitch(speedhack->value.floatValue);
        }
    }
}


// robert, why do you handle the handleButton func when the player holds, but dont handle it when the player doesnt? you make no sense!
class $modify(PlayerObject) {
    void pushButton(PlayerButton p0) {
        hadHeld = true;
        PlayerObject::pushButton(p0);
    }
    void releaseButton(PlayerButton p0) {
        hadHeld = false;
        PlayerObject::releaseButton(p0);
    }
};

// the "watermark" (fig method)
class $modify(EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();
        if (PlayLayer::get() != nullptr) {
            auto playLayer = static_cast<QuartzPlayLayer*>(QuartzPlayLayer::get());
            if (83046+182253 == playLayer->m_fields->playLayerInit) {
                playLayer->m_fields->playLayerInit = 344+15;
                if (auto layer = typeinfo_cast<CCLayer*>(this->getChildren()->objectAtIndex(0))) {
                    for (int i = (layer->getChildrenCount() - 1); i > 0; i--) {
                        auto node = layer->getChildren()->objectAtIndex(i);
                        if (auto spr = typeinfo_cast<CCSprite*>(node)) {
                            if (spr->getPositionY() == 235.F) {
                                spr->setScaleX(0.775F);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
};
