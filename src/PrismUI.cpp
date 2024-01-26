#include "PrismUI.hpp"
#include "themes.hpp"

int currentMenuIndexGD = 0;

int calcLimit(int n) {
    return static_cast<int>(std::floor(std::log10(n)) + 1);
}

int currentI = 0;

void PrismUI::ButtonState(int id, bool activated) {
    auto buttonBG = static_cast<cocos2d::extension::CCScale9Sprite*>(m_buttonMenu->getChildByID(fmt::format("prism-nav-bg-{}", id)));
    auto button = static_cast<CCMenuItemSpriteExtra*>(m_buttonMenu->getChildByID(fmt::format("prism-nav-btn-{}", id)));
    if (activated) {
        button->setColor({255,255,255});
        Themes::RGBAToCC(GetTheme()["ButtonActive"], buttonBG);
        currentMenuIndexGD = id;
    } else {
        Themes::RGBAToCC(GetTheme()["Button"], buttonBG);
        Themes::RGBAToCC(GetTheme()["Text"], button);
    }
}

cocos2d::extension::CCScale9Sprite* PrismUI::createCheckbox(bool check) {
    auto box = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 0.0f, 0.0f });
    box->setScale(0.3F);
    if (check) {
        auto checkmark = CCSprite::create("checkmark.png"_spr);
        checkmark->setScale(0.3F);
        checkmark->setAnchorPoint({0,0});
        checkmark->setPosition({6,15});
        box->addChild(checkmark);
        Themes::RGBAToCC(GetTheme()["CheckMark"], checkmark);
    }
    Themes::RGBAToCC(GetTheme()["Button"], box);
    return box;
}

float calculateYPosition(float x) { // love floating points
    // Formula: Y = mx + b
    float m = 85;
    float result = -m + (((10 - x) * m) / 2);
    std::cout << fmt::format("y = {},{}",result,x) << std::endl;
    return result;
}

int getYPosBasedOnCategory() { // someone give me a proper math formula ok thanks
    switch (currentMenuIndexGD) {
        case 0: // Global 
            return -15;
        case 1: // Player
            return -73;
        case 2: // Bypass
            return 150;
        case 3: // Creator
            return 125;
        case 4: // Misc
            return 0;
        case 5: // Settings
            return 55;
        default: return 0;
    }

}


void PrismUI::CreateHackItem(HackItem* hack) {
    const auto& obj = hack->data;
    std::string name = hack->name;
    std::string desc = hack->desc;
    auto opcodes = obj.get<matjson::Array>("opcodes");
    if (!((Hacks::isHackEnabled("Enable Patching") && obj.contains("winOnly")) || !obj.contains("winOnly") || name == "Enable Patching")) return;
    /*if (obj.contains("winOnly")) {
        //#ifndef GEODE_IS_WINDOWS
        ImGui::BeginDisabled(true);
        //#endif
    }
    if (obj.contains("win")) { // yeah idk what property okay
        #ifndef GEODE_IS_WINDOWS
        ImGui::BeginDisabled(true);
        #endif
    }*/ 
    auto label = CCLabelBMFont::create(name.c_str(), "PrismMenu.fnt"_spr);
    label->setAnchorPoint({0.0F, 0.5F});
    label->limitLabelWidth(150, 0.5F, .2F);
    float indexY = (currentI * 28) + 100;
    label->setPosition({20, indexY});
    m_content->addChild(label);
    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    //infoSpr->setScale(.5F);
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(PrismUI::onInfoBtn));
    infoBtn->setUserData(reinterpret_cast<void*>(hack));
    m_content->addChild(infoBtn);
    if (hack->value.type == ValueType::Bool) {
        auto checkbox = CCMenuItemToggler::create(
            createCheckbox(hack->value.boolValue), createCheckbox(!hack->value.boolValue),
            this,
            menu_selector(PrismUI::onBoolBtn)
        );
        checkbox->setUserData(reinterpret_cast<void*>(hack));
        checkbox->setPositionY(indexY);
        m_content->addChild(checkbox);
        if (obj.contains("win")) {
            #ifndef GEODE_IS_WINDOWS
            checkbox->setEnabled(false);
            #endif
        }
    } else if (hack->value.type == ValueType::Int) {
        auto min = obj.get<int>("min");
        auto max = obj.get<int>("max");
        auto input = InputNode::create(150.f, "...", "PrismMenu.fnt"_spr);
        input->setScale(.65f);
        input->getInput()->setString(std::to_string(hack->value.intValue));
        input->getInput()->setAllowedChars("0123456789");
        input->getInput()->setDelegate(this);
        
        input->setPosition({40, indexY});
        label->setPositionX(95);
        m_content->addChild(input);
    } else if (false) {
        Slider* m_slider = nullptr;

    }
    // thingy i was struggling with
    if (m_content->getContentSize().height > 230.0F) {
        m_scrollLayer->m_contentLayer->setContentSize(m_content->getContentSize() + CCSize { 0.f, 5.0 });
        //m_content->setPositionY(-0.5f * currentI);
    } else {
        m_scrollLayer->m_contentLayer->setContentSize(m_content->getContentSize());
        //m_content->setPositionY(220.0f * currentI);
    }
    infoBtn->setPosition({280, indexY});
    //m_content->setPositionY(calculateYPosition(currentI));
    m_content->setPositionY(getYPosBasedOnCategory());
    m_scrollLayer->moveToTop();
    currentI++;
}


void PrismUI::RegenCategory() {
    std::vector<matjson::Value> jsonArray;
    switch (currentMenuIndexGD) {
        case 0: // Global 
            jsonArray = matjson::parse(Hacks::getGlobalHacks()).as_array();
            break;
        case 1: // Player
            jsonArray = matjson::parse(Hacks::getPlayerHacks()).as_array();
            break;
        case 2: // Bypass
            jsonArray = matjson::parse(Hacks::getBypassHacks()).as_array();
            break;
        case 3: // Creator
            jsonArray = matjson::parse(Hacks::getCreatorHacks()).as_array();
            break;
        case 4: // Misc
            jsonArray = matjson::parse(Hacks::getMiscHacks()).as_array();
            break;
        case 5: // Settings
            jsonArray = matjson::parse(Hacks::getSettings()).as_array();
            break;
    }
    currentI = 0;
    for (int i = jsonArray.size(); i --> 0; ) { // ....what is this black magic
    //for (size_t i = 0; i < jsonArray.size(); i++) {
        const auto& obj = jsonArray[i];
        std::string name = obj.get<std::string>("name");
        HackItem* hack = Hacks::getHack(name);
        if (hack != nullptr) {
            CreateHackItem(hack);
        }
    }
}
void PrismUI::CreateButton(const char* name, int menuIndex) {
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    auto menu = CCMenu::create();
    auto node = CCLayer::create();

    auto label = CCLabelBMFont::create(name, "PrismMenu.fnt"_spr);
    //label->addChild(invisBG);
    label->limitLabelWidth(72, 1.0F, .2F);
    auto button = CCMenuItemSpriteExtra::create(label, this, menu_selector(PrismUI::onSideButton));
    button->setContentSize({90.0f, 35.0f});
    label->setPosition({90.0f / 2, 35.0f / 2});
    auto buttonBG = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    buttonBG->setContentSize({ 90.0f, 35.0f });
    Themes::RGBAToCC(GetTheme()["Button"], buttonBG);
    Themes::RGBAToCC(GetTheme()["Text"], button);
    if (currentMenuIndexGD == menuIndex) {
        button->setColor({255,255,255});
        Themes::RGBAToCC(GetTheme()["ButtonActive"], buttonBG);
    }
    //menu->addChild(buttonBG);
    buttonBG->setPosition(-163, (menuIndex * -38) + 97);
    button->setPosition(-163, (menuIndex * -38) + 97);
    buttonBG->setID(fmt::format("prism-nav-bg-{}", menuIndex));
    button->setID(fmt::format("prism-nav-btn-{}", menuIndex));
    button->setUserData(button);
    m_buttonMenu->addChild(buttonBG);
    m_buttonMenu->addChild(button);
    //menu->registerWithTouchDispatcher();
}

void PrismUI::onBoolBtn(CCObject* ret) {
    // will do other languages later, cyrillic is hard
    // TODO: create custom FLAlertLayer but instead use the Prism font instead of bigFont.fnt
    HackItem* hack = static_cast<HackItem*>(static_cast<CCMenuItemSpriteExtra*>(ret)->getUserData());
    auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
    std::string name = hack->name;
    hack->value.boolValue = !hack->value.boolValue;
    Hacks::Settings::setSettingValue(&settings, *hack, hack->value.boolValue);
    if (name == "Show Button") {
        auto prismButton = CCScene::get()->getChildByID("prism-icon");
        if (prismButton != nullptr) {
            prismButton->setVisible(hack->value.boolValue);
        }
    }
    if (name == "Instant Complete" && hack->value.boolValue) {
        FLAlertLayer::create(nullptr, "Cheater!", "Just a warning, you will be <cr>banned off leaderboards</c> if you use this on rated levels. Consider this your <cy>warning</c>.", "OK", nullptr)->show();
    }
}

void PrismUI::onSideButton(CCObject* ret) {
    m_content->removeAllChildrenWithCleanup(true);
    auto idStr = static_cast<CCNodeRGBA*>(ret)->getID();
    int id = std::stoi(idStr.substr(idStr.length() - 1));
    ButtonState(currentMenuIndexGD, false);
    ButtonState(id, true);
    RegenCategory();
}

void PrismUI::onInfoBtn(CCObject* ret) {
    // will do other languages later, cyrillic is hard
    // TODO: create custom FLAlertLayer but instead use the Prism font instead of bigFont.fnt
    HackItem* hack = static_cast<HackItem*>(static_cast<CCMenuItemSpriteExtra*>(ret)->getUserData());
    FLAlertLayer::create(hack->name.c_str(), hack->desc.c_str(), "OK")->show();
}

matjson::Object PrismUI::GetTheme() {
    return themeArr[0].as_object();
}

bool PrismUI::init(float _w, float _h, matjson::Object theme, Lang* currentLang) {
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    this->m_pLrSize = cocos2d::CCSize { _w, _h };
    this->currentLanguage = currentLang;
    this->themeArr.push_back(theme);
    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    this->m_mainLayer = cocos2d::CCLayer::create();
    this->addChild(this->m_mainLayer);

    auto bg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    auto bgBehind = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize(this->m_pLrSize - 3);
    bgBehind->setContentSize(this->m_pLrSize);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    bgBehind->setPosition(winSize.width / 2, winSize.height / 2);

    //float currentOpacity = Hacks::getHack("Menu Opacity")->value.floatValue;
    //bgBehind->setOpacity(currentOpacity * 255);
    this->m_mainLayer->addChild(bgBehind);
    this->m_mainLayer->addChild(bg);
    bg->setID("prism-bg");
    this->m_pBGSprite = bg;

    this->m_buttonMenu = cocos2d::CCMenu::create();
    this->m_mainLayer->addChild(this->m_buttonMenu);

    auto closeSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(1.0f);

    auto closeBtn = CCMenuItemSpriteExtra::create(
        closeSpr,
        this,
        (cocos2d::SEL_MenuHandler)&PrismUI::onClose
    );
    closeBtn->setUserData(reinterpret_cast<void*>(this));

    this->m_buttonMenu->addChild(closeBtn);

    closeBtn->setPosition( - _w / 2, _h / 2 );


    auto sideBar = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    sideBar->setContentSize({ 100.0f, 230.0f });
    //sideBar->setPosition(125, bg->getContentSize().height / 2);
    sideBar->setPosition(60, _h / 2);
    Themes::RGBAToCC(theme["TableRowBg"], sideBar);

    auto buttonBG = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    buttonBG->setContentSize({ 320.0f, 230.0f });
    //buttonBG->setPosition(bg->getContentSize().width - 230, bg->getContentSize().height / 2);
    buttonBG->setPosition(_w - 173, _h / 2);
    Themes::RGBAToCC(theme["TableRowBg"], buttonBG);

    bg->addChild(sideBar);
    bg->addChild(buttonBG);
    CreateButton(currentLanguage->name("§ Global").c_str(), 0);
    CreateButton(currentLanguage->name("¬ Player").c_str(), 1);
    CreateButton(currentLanguage->name("ª Bypass").c_str(), 2);
    CreateButton(currentLanguage->name("« Creator").c_str(), 3);
    CreateButton(currentLanguage->name("··· Misc").c_str(), 4);
    CreateButton(currentLanguage->name("¶ Settings").c_str(), 5);
    Themes::RGBAToCC(theme["BG"], bg);
        
    m_scrollLayer = ScrollLayer::create({ 0, 0, 320.0F, 230.0F }, true);
    m_scrollLayer->setPosition(_w - 273, 45);
    m_content = CCMenu::create();
    m_content->setZOrder(2);
    m_content->setPositionX(20);
    m_content->registerWithTouchDispatcher();

    m_scrollLayer->m_contentLayer->addChild(m_content);

    m_scrollLayer->setTouchEnabled(true);

    this->m_mainLayer->addChild(m_scrollLayer);
    this->registerWithTouchDispatcher();

    cocos::handleTouchPriority(this);
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    RegenCategory();
    return true;
}

void PrismUI::keyDown(cocos2d::enumKeyCodes key) {
    if (key == cocos2d::enumKeyCodes::KEY_Escape)
        return onClose(nullptr);
    if (key == cocos2d::enumKeyCodes::KEY_Tab)
        return onClose(nullptr);
    if (key == cocos2d::enumKeyCodes::KEY_Space)
        return;
    
    return FLAlertLayer::keyDown(key);
}

void PrismUI::onClose(cocos2d::CCObject* pSender) {
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
};

PrismUI* PrismUI::create(matjson::Object theme, Lang* lang) {
    auto pRet = new PrismUI();
    if (pRet) {
        if (pRet->init(PrismUI::s_defWidth, PrismUI::s_defHeight, theme, lang)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};
