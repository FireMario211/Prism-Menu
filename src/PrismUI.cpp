#include "PrismUI.hpp"
#include "Themes.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/TextArea.hpp>
#include "Dropdown.h"

int currentMenuIndexGD = 0;

int calcLimit(int n) {
    return static_cast<int>(std::floor(std::log10(n)) + 1);
}

// rn im too lazy to make a "pch.h" like file
std::string getNodeNameF(cocos2d::CCObject* node) {
#ifdef GEODE_IS_WINDOWS
    return typeid(*node).name() + 6;
#else 
    {
        std::string ret;

        int status = 0;
        auto demangle = abi::__cxa_demangle(typeid(*node).name(), 0, 0, &status);
        if (status == 0) {
            ret = demangle;
        }
        free(demangle);

        return ret;
    }
#endif
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

cocos2d::extension::CCScale9Sprite* PrismUIButton::createCheckbox(bool check) {
    auto box = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 0.0f, 0.0f });
    box->setScale(0.3F);
    if (check) {
        auto checkmark = CCSprite::create("checkmark.png"_spr);
        checkmark->setScale(0.3F);
        checkmark->setAnchorPoint({0,0});
        checkmark->setPosition({6,15});
        box->addChild(checkmark);
        Themes::RGBAToCC(PrismUI::GetTheme()["CheckMark"], checkmark);
    }
    Themes::RGBAToCC(PrismUI::GetTheme()["Button"], box);
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
            return -80;
        case 1: // Player
            return -70;
        case 2: // Bypass
            return 57;
        case 3: // Creator
            return 29;
        case 4: // Misc
            return 85;
        case 5: // Settings
            return -81;
        default: return 0;
    }
}
float getContentSizeBasedOnCategory() { // someone give me a proper math formula ok thanks
    switch (currentMenuIndexGD) {
        case 0: // Global 
            return 260; // 260
        case 1: // Player
            return 328;
        case 2: // Bypass
            return 230; // 230
        case 3: // Creator
            return 230; // 230
        case 4: // Misc
            return 230; // 230
        case 5: // Settings
            return 400; // 400
        default: return 328;
    }
}

float getSliderValue(float current, float min, float max, bool inverse) {
    return (inverse) ? (current * (max - min) + min) : (current - min) / (max - min);
}

std::string setPrecision(float value, int streamsize) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(streamsize) << value;
    return oss.str();
}

bool PrismUIButton::init(HackItem* hack) {
    auto currentLanguage = Lang::getLanguage();
    menu = CCMenu::create();
    const auto& obj = hack->data;
    std::string name = hack->name;
    std::string desc = hack->desc;
    //auto opcodes = obj.get<matjson::Array>("opcodes");
    this->m_hack = hack;
    auto label = CCLabelBMFont::create(currentLanguage->name(name).c_str(), "PrismMenu.fnt"_spr);
    Themes::RGBAToCC(PrismUI::GetTheme()["Text"], label);
    label->setAnchorPoint({0.0F, 0.5F});
    label->limitLabelWidth(150, 0.5F, .2F);
    label->setPositionX(20);
    menu->addChild(label);
    //Themes::RGBAToCC(PrismUI::GetTheme()["Text"], reinterpret_cast<CCNodeRGBA*>(label)); // prrrobably not a good idea
    if (hack->value.type == ValueType::Bool) {
        auto checkbox = CCMenuItemToggler::create(
            createCheckbox(hack->value.boolValue), createCheckbox(!hack->value.boolValue),
            this,
            menu_selector(PrismUIButton::onBoolBtn)
        );
        //checkbox->setUserData(reinterpret_cast<void*>(hack));
        menu->addChild(checkbox);
        if (obj.contains("win")) {
            #ifndef GEODE_IS_WINDOWS
            checkbox->setEnabled(false);
            #endif
        } else if (name == "Enable Patching") {
            checkbox->setEnabled(false);
        }
    } else if (hack->value.type == ValueType::Int && hack->type != "dropdown" && !name.starts_with("Button Position")) {
        auto min = obj.get<int>("min");
        auto max = obj.get<int>("max");
        // TODO: add + and - buttons because according to some, android keyboard is BAD!
        m_input = InputNode::create(150.f, "...", "PrismMenu.fnt"_spr);
        m_input->getInput()->setString(std::to_string(hack->value.intValue));
        m_input->getInput()->setAllowedChars("0123456789");
        m_input->setPositionX(37);
        
        auto incBtnSpr = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
        incBtnSpr->setContentSize({ 50.0f, 50.0f });
        Themes::RGBAToCC(PrismUI::GetTheme()["Button"], incBtnSpr);
        incBtnSpr->setScale(.4F);

        auto decBtnSpr = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
        decBtnSpr->setContentSize({ 50.0f, 50.0f });
        Themes::RGBAToCC(PrismUI::GetTheme()["Button"], decBtnSpr);
        decBtnSpr->setScale(.4F);

        auto incBtnlabel = CCLabelBMFont::create("+", "PrismMenu.fnt"_spr);
        Themes::RGBAToCC(PrismUI::GetTheme()["Text"], incBtnlabel);
        incBtnlabel->setPosition({ incBtnSpr->getContentSize().width / 2, incBtnSpr->getContentSize().height / 2 });
        auto decBtnlabel = CCLabelBMFont::create("-", "PrismMenu.fnt"_spr);
        Themes::RGBAToCC(PrismUI::GetTheme()["Text"], decBtnlabel);
        decBtnlabel->setPosition({ incBtnSpr->getContentSize().width / 2, incBtnSpr->getContentSize().height / 2 });

        incBtnSpr->addChild(incBtnlabel);
        decBtnSpr->addChild(decBtnlabel);

        auto incBtn = CCMenuItemSpriteExtra::create(
            incBtnSpr,
            this,
            menu_selector(PrismUIButton::onIncBtn)
        );
        incBtn->setPositionX(124);
        menu->addChild(incBtn);
        auto decBtn = CCMenuItemSpriteExtra::create(
            decBtnSpr,
            this,
            menu_selector(PrismUIButton::onDecBtn)
        );
        decBtn->setPositionX(100);
        menu->addChild(decBtn);

        label->setPositionX(140);
    } else if (hack->value.type == ValueType::Float || name.starts_with("Button Position")) {
        auto value = (name.starts_with("Button Position")) ? hack->value.intValue : hack->value.floatValue;

        label->limitLabelWidth(72, 0.5F, .2F);
        auto min = obj.get<int>("min");
        auto max = obj.get<int>("max");
        m_input = InputNode::create(100.f, "...", "PrismMenu.fnt"_spr);
        m_input->getInput()->setString(
            name.starts_with("Button Position") ? std::to_string(value) : setPrecision(value, 3)
        );
        m_input->getInput()->setAllowedChars("0123456789.");
        m_input->setPositionX(21);
        label->setPositionX(190);
        Slider* slider = Slider::create(this, menu_selector(PrismUIButton::onFloatBtn), .6f);
        slider->setPositionX(122);
        slider->setValue(getSliderValue(value, min, max, false));
        menu->addChild(slider);
    } else if (hack->type == "dropdown" || hack->value.type == ValueType::Custom) {
        auto type = obj.get<std::string>("type");
        if (type == "button") {
            //const char* caption, int width, bool absolute, const char* font, const char* texture, float height, float scale
            //currentLanguage->name(name).c_str()
            auto btnSpr = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
            auto btnlabel = CCLabelBMFont::create(currentLanguage->name(name).c_str(), "PrismMenu.fnt"_spr);

            Themes::RGBAToCC(PrismUI::GetTheme()["Text"], btnlabel);
            btnlabel->limitLabelWidth(250, 1.0F, .2F);
            btnSpr->setContentSize({ 300.0f, 50.0f });
            btnlabel->setPosition({ btnSpr->getContentSize().width / 2, btnSpr->getContentSize().height / 2 });
            btnSpr->setScale(.5F);
            Themes::RGBAToCC(PrismUI::GetTheme()["Button"], btnSpr);
            btnSpr->addChild(btnlabel);
            auto btn = CCMenuItemSpriteExtra::create(
                btnSpr,
                this,
                menu_selector(PrismUIButton::onBtn)
            );
            btn->setPositionX(63);
            menu->addChild(btn);
            label->removeFromParentAndCleanup(true);
        } else if (type == "dropdown") {
            label->limitLabelWidth(72, 0.5F, .2F);
            auto values = obj.get<matjson::Array>("values");
            if (hack->name == "Theme") {
                values = Themes::getCurrentThemes();
            }
            auto dropdown = Dropdown::create(values, hack, menu_selector(PrismUIButton::onDropdownBtn));
            dropdown->menu->setPosition({78, 0});
            dropdown->menu->setScale(.75F);

            cocos::handleTouchPriority(dropdown->menu);
            label->setPositionX(180);
            /*for (int i = 0; i < dropdown->menu->getChildrenCount(); i++) {
                auto node = dropdown->menu->getChildren()->objectAtIndex(i);
                if (typeinfo_cast<CCNode*>(node)) {
                    menu->addChild(static_cast<CCNode*>(node));
                }
            }*/
            menu->addChild(dropdown->menu);
        }
    }
    if (m_input != nullptr) {
        Themes::RGBAToCC(PrismUI::GetTheme()["Button"], m_input);
        m_input->setScale(.65f);
        m_input->getInput()->setDelegate(this);
        Themes::RGBAToCC(PrismUI::GetTheme()["Text"], m_input->getInput()->m_placeholderLabel);
        menu->addChild(m_input);
    }
    menu->setPosition({0,0});
    this->addChild(menu);
    menu->registerWithTouchDispatcher();
    menu->setTouchEnabled(true);
    return true;
}

void PrismUIButton::onBoolBtn(CCObject* ret) {
    auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
    std::string name = m_hack->name;
    m_hack->value.boolValue = !m_hack->value.boolValue;
    if (name == "Instant Complete") { // making sure people dont leave this on by accident 
        Hacks::Settings::setSettingValue(&settings, *m_hack, false);
    } else {
        Hacks::Settings::setSettingValue(&settings, *m_hack, m_hack->value.boolValue);
    }
    if (name == "Show Button") {
        auto prismButton = CCScene::get()->getChildByID("prism-icon");
        if (prismButton != nullptr) {
            prismButton->setVisible(m_hack->value.boolValue);
        }
    }
    if (name == "Instant Complete" && m_hack->value.boolValue) {
        FLAlertLayer::create(nullptr, "Cheater!", "Just a warning, you will be <cr>banned off leaderboards</c> if you use this on rated levels. Consider this your <cy>warning</c>.", "OK", nullptr)->show();
    }
}

void PrismUIButton::onFloatBtn(CCObject* ret) {
    auto slider = static_cast<SliderThumb*>(ret);
    auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
    std::string name = m_hack->name;
    auto min = m_hack->data.get<int>("min");
    auto max = m_hack->data.get<int>("max");
    float actualValue = getSliderValue(slider->getValue(), min, max, true);
    if (!name.starts_with("Button Position")) {
        m_hack->value.floatValue = actualValue;
    } else {
        m_hack->value.intValue = actualValue;
    }
    m_input->getInput()->setString(
        name.starts_with("Button Position") ? std::to_string(m_hack->value.intValue) : setPrecision(m_hack->value.floatValue, 3)
    );
    if (name == "Speedhack") {
        if (m_hack->value.floatValue < 0.0F) return;
        Hacks::setPitch(m_hack->value.floatValue);
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(m_hack->value.floatValue);
    } else {
        if (!name.starts_with("Button Position")) {
            Hacks::Settings::setSettingValue(&settings, *m_hack, m_hack->value.floatValue);
        } else {
            intChanged();
            Hacks::Settings::setSettingValue(&settings, *m_hack, m_hack->value.intValue);
        }
    }
}

void PrismUIButton::onBtn(CCObject* ret) {
    auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
    std::string name = m_hack->name;
    auto prismUI = this->getParent()->getParent()->getParent()->getParent()->getParent();
    if (name == "Restore Defaults") {
        Hacks::processJSON(true);
        // what is this, i have to use getParent 5 times, WHY
        static_cast<PrismUI*>(prismUI)->onClose(ret);
    } else if (name == "Import Theme") {
        geode::FileSetting::Filter filter;
        filter.description = "Theme (*.json)";
        filter.files.insert("*.json");
        file::pickFile(
            file::PickMode::OpenFile,
            {
                dirs::getGameDir(),
                { filter }
            },
            [&](ghc::filesystem::path path) {
                auto saveDir = Mod::get()->getSaveDir().string();
                if (!ghc::filesystem::exists(saveDir + "/themes")) {
                    ghc::filesystem::create_directory(saveDir + "/themes");
                }
                ghc::filesystem::copy_file(path, saveDir + "/themes/" + path.filename().string());
                //ImGui::OpenPopup("Success");
                FLAlertLayer::create("Success!", "The <cy>theme</c> has successfully been imported! Restart your game to use it.", "OK")->show();
            }
        );
    } else if (name == "Reset Speedhack") {
        HackItem* speedHack = Hacks::getHack("Speedhack");
        speedHack->value.floatValue = 1.0F;
        Hacks::Settings::setSettingValue(&settings, *speedHack, 1.0F);
        Hacks::setPitch(1.0F);
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0F);
        static_cast<PrismUI*>(prismUI)->onClose(ret);
    } else {
        FLAlertLayer::create("Not working...yet.", "This will be added in a <cy>future update</c>!", "OK")->show();
    }
}

void PrismUIButton::onDropdownBtn(CCObject* sender) {
    auto obj = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto hack = static_cast<HackItem*>(obj->getUserData());
    auto idStr = obj->getID();
    static_cast<CCLabelBMFont*>(obj->getParent()->getParent()->getChildByID("selected-label"))->setString((static_cast<CCLabelBMFont*>(obj->getChildren()->objectAtIndex(0)))->getString());

    auto obj2 = static_cast<CCMenuItemSprite*>(obj->getParent()->getParent()->getChildByID("flip-btn"));
    bool expanded = obj2->getScaleY() < 0 ? true : false;
    #ifdef GEODE_IS_WINDOWS
    obj2->runAction(CCEaseBackOut::create(CCScaleTo::create(0.5f, 0.75f, (!expanded ? -0.75f : 0.75f))));
    #else
    obj2->runAction(CCScaleTo::create(0.5f, 0.75f, (!expanded ? -0.75f : 0.75f)));
    #endif

    auto parent = obj2->getParent();
    auto background = parent->getChildByID("background");
    auto ddmenu = parent->getChildByID("dropdown-menu");

    int h = ddmenu->getChildrenCount() + 1;
    h = expanded ? h : 1;

    ddmenu->setVisible(expanded);
    background->setContentSize({background->getContentSize().width, (25 / background->getScale()) * h});
    int index = std::stoi(idStr.substr(idStr.length() - 1));

    auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
    hack->value.intValue = index;
    Hacks::Settings::setSettingValue(&settings, *hack, hack->value.intValue);
    if (hack->name == "Menu Style") {
        auto obj = static_cast<PrismUI*>(this->getParent()->getParent()->getParent()->getParent()->getParent()->getParent()->getParent());
        obj->onClose(sender);
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
    auto btn = PrismUIButton::create(hack);
    float indexY = (currentI * 28) + 100;
    
    // TODO: create custom sprite so people dont complain
    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    //infoSpr->setScale(.5F);
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(PrismUIButton::onInfoBtn));
    infoBtn->setUserData(reinterpret_cast<void*>(hack));
    m_content->addChild(infoBtn);
    
    btn->setPositionY(indexY);
    m_content->addChild(btn);
    // thingy i was struggling with
    if (m_content->getContentSize().height > 230.0F) {
        //m_scrollLayer->m_contentLayer->setContentSize(m_content->getContentSize() + CCSize { 0.f, 1.0 });
        m_scrollLayer->m_contentLayer->setContentSize(m_content->getContentSize());
        //m_content->setPositionY(-0.5f * currentI);
    } else {
        m_scrollLayer->m_contentLayer->setContentSize(m_content->getContentSize());
        //m_content->setPositionY(220.0f * currentI);
    }
    m_scrollLayer->m_contentLayer->setContentSize({m_content->getContentSize().width, getContentSizeBasedOnCategory()});
    infoBtn->setPosition({280, indexY});
    //m_content->setPositionY(calculateYPosition(currentI));
    m_content->setPositionY(getYPosBasedOnCategory());
    m_scrollLayer->moveToTop();
    currentI++;
}

void PrismUIButton::textChanged(CCTextInputNode* input) {
    if (!editedInputNode) return;
    Themes::RGBAToCC(PrismUI::GetTheme()["Text"], input->m_placeholderLabel);
    const auto& obj = m_hack->data;
    auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
    std::string name = m_hack->name;
    auto min = m_hack->data.get<int>("min");
    auto max = m_hack->data.get<int>("max");
    if (m_hack->value.type == ValueType::Int) {
        int value = min;
        try { // hA
            value = std::stoi(input->getString());
        } catch (const std::exception& e) {
            auto def = m_hack->data.get<int>("default");
            value = def;
        }
        if (value == m_hack->value.intValue) return;
        if (m_hack->value.intValue > max || min > m_hack->value.intValue) {
            if (m_hack->value.intValue > max) {
                m_hack->value.intValue = max;
            } else {
                m_hack->value.intValue = min;
            }
        } else {
            m_hack->value.intValue = value;
        }
        Hacks::Settings::setSettingValue(&settings, *m_hack, m_hack->value.intValue);
    } else { // assume float
        float value = min;
        try { // hA
            value = std::stof(input->getString());
        } catch (const std::exception& e) {
            auto def = m_hack->data.get<float>("default");
            value = def;
        }
        if (value == m_hack->value.floatValue) return;
        if (m_hack->value.floatValue > max || min > m_hack->value.floatValue) {
            if (m_hack->value.floatValue > max) {
                m_hack->value.floatValue = max;
            } else {
                m_hack->value.floatValue = min;
            }
        } else {
            m_hack->value.floatValue = value;
        }
        Hacks::Settings::setSettingValue(&settings, *m_hack, m_hack->value.floatValue);
    }
}
void PrismUIButton::intChanged() {
    std::string name = m_hack->name;
    auto prismButton = CCScene::get()->getChildByID("prism-icon");
    if (name == "FPS Bypass") {
        // from mats fps unlocker
        //Hacks::Settings::setSettingValue(&settings, *hack, hack->value.floatValue);
        auto app = CCApplication::sharedApplication();
        app->setAnimationInterval(1.0 / static_cast<double>(m_hack->value.intValue));
    } else if (name == "Button Position X") {
        prismButton->setPositionX(m_hack->value.intValue);
    } else if (name == "Button Position Y") {
        prismButton->setPositionY(m_hack->value.intValue);
    }
}

void PrismUIButton::textInputOpened(CCTextInputNode* input) { // basically onIntBtn
    if (input->getString().size() == 0) return;
    editedInputNode = true;
    Themes::RGBAToCC(PrismUI::GetTheme()["Text"], input->m_placeholderLabel);
}

void PrismUIButton::textInputClosed(CCTextInputNode* input) { // basically onIntBtn
    if (input->getString().size() == 0) return;
    editedInputNode = false;
    std::string name = m_hack->name;
    Themes::RGBAToCC(PrismUI::GetTheme()["Text"], input->m_placeholderLabel);
    if (m_hack->type == "float") {
        input->setString(setPrecision(m_hack->value.floatValue, 3));
        if (name == "Speedhack") {
            if (m_hack->value.floatValue < 0.0F) return;
            Hacks::setPitch(m_hack->value.floatValue);
            CCDirector::sharedDirector()->getScheduler()->setTimeScale(m_hack->value.floatValue);
        }
    } else {
        input->setString(std::to_string(m_hack->value.intValue));
        intChanged();
    }
}

void PrismUIButton::onIncBtn(CCObject* ret) {
    auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
    const auto& obj = m_hack->data;
    auto max = m_hack->data.get<int>("max");
    int step = (obj.contains("step")) ? obj.get<int>("step") : 5;
    m_hack->value.intValue += step;
    if (m_hack->value.intValue > max) {
        m_hack->value.intValue = max;
    }
    Hacks::Settings::setSettingValue(&settings, *m_hack, m_hack->value.intValue);
    m_input->setString(std::to_string(m_hack->value.intValue));
    intChanged();
}
void PrismUIButton::onDecBtn(CCObject* ret) {
    auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
    const auto& obj = m_hack->data;
    auto min = m_hack->data.get<int>("min");
    int step = (obj.contains("step")) ? obj.get<int>("step") : 5;
    m_hack->value.intValue -= step; 
    if (min > m_hack->value.intValue) {
        m_hack->value.intValue = min;
    }
    Hacks::Settings::setSettingValue(&settings, *m_hack, m_hack->value.intValue);
    m_input->setString(std::to_string(m_hack->value.intValue));
    intChanged();
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
    for (auto it = jsonArray.end() - 1; it != jsonArray.begin() - 1; it--) {
        const auto& obj = *it;
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



void PrismUI::onSideButton(CCObject* ret) {
    m_content->removeAllChildrenWithCleanup(true);
    auto idStr = static_cast<CCNodeRGBA*>(ret)->getID();
    int id = std::stoi(idStr.substr(idStr.length() - 1));
    ButtonState(currentMenuIndexGD, false);
    ButtonState(id, true);
    RegenCategory();
    cocos::handleTouchPriority(this);
}


double calculateTextScale(const std::string& inputString, double minScale = 0.2, double maxScale = 0.5) {
    int numCharacters = static_cast<int>(inputString.size());
    double scale = minScale + (numCharacters / 100.0) * (maxScale - minScale); // Assuming a range of 0-100 characters
    return std::max(minScale, std::min(scale, maxScale));
}

void PrismUIButton::onInfoBtn(CCObject* ret) {
    // will do other languages later, cyrillic is hard
    // TODO: create custom FLAlertLayer but instead use the Prism font instead of bigFont.fnt
    HackItem* hack = static_cast<HackItem*>(static_cast<PrismUIButton*>(ret)->getUserData());
    auto currentLanguage = Lang::getLanguage();
    auto name = currentLanguage->name(hack->name);
    auto desc = currentLanguage->desc(hack->name, hack->desc);
    auto flAlert = FLAlertLayer::create(name.c_str(), desc.c_str(), "OK");
    TextArea* lines;
    CCLabelBMFont* title;
    for (int i = 0; i < flAlert->m_mainLayer->getChildrenCount(); i++) {
        auto objC = flAlert->m_mainLayer->getChildren()->objectAtIndex(i); // objective-c yes
        if (getNodeNameF(objC) == "TextArea") {
            lines = static_cast<TextArea*>(objC);
        } else if (getNodeNameF(objC) == "cocos2d::CCLabelBMFont") {
            title = static_cast<CCLabelBMFont*>(objC);
        }
    }
    if (lines != nullptr && title != nullptr) {
        // probably a bad idea memory-wise
        auto newTitle = CCLabelBMFont::create(name.c_str(), "PrismMenu.fnt"_spr);
        newTitle->setPosition({title->getPositionX(), title->getPositionY() - 15});
        newTitle->limitLabelWidth(256, 0.75F, 0.25F);
        newTitle->setZOrder(title->getZOrder());
        Themes::RGBAToCC(PrismUI::GetTheme()["ButtonActive"], newTitle);
        title->removeFromParentAndCleanup(true);
        flAlert->m_mainLayer->addChild(newTitle);
        // undocumented!
        /* 
              piVar4 = (int *)TextArea::create((basic_string)&local_5c,"chatFont.fnt",(float)uVar13,
                                       (float)((ulonglong)uVar13 >> 0x20),(CCPoint)0x0,scale,
                                       SUB41(_param_7 - 60.0,0));

            what a red herring the (CCPoint)0x0 is, its actually { 0.5, 0.0 }
        */
        // static TextArea *TextArea::create(std::string str, const char *font, float width, float height, const cocos2d::CCPoint &anchor, float scale, bool disableColor) ACTUALLY THE ORDER IS WRONG
        //create(std::string str, const char* font, float scale, float width, CCPoint anchor, float height, bool disableColor) - correct one!
        // 
        // "PrismMenu.fnt"_spr
        /*auto newLines = TextArea::create(
            hack->desc.c_str(), "PrismMenu.fnt"_spr, 1.0F,
            240.F, { 0.5, 0 }, 20.F, false);*/
        auto newLines = TextArea::create(
            desc.c_str(), "PrismMenu.fnt"_spr, 0.7F, //0.8
            280.F, { 0.5, 0 }, 20.F, false);
        //auto newLines = geode::GEODE_DLL::SimpleTextArea::create(hack->desc.c_str());
        //const std::string& text, const std::string& font, const float scale, const float width
        newLines->setScale(.75F);
        //newLines->setScale(.75F);
        //newLines->setPosition({lines->getPositionX(), lines->getPositionY() - 10});
        newLines->setPosition({336, lines->getPositionY() - 10});
        newLines->setZOrder(lines->getZOrder());
        lines->removeFromParentAndCleanup(true);
        //lines->setVisible(false);
        flAlert->m_mainLayer->addChild(newLines);
    }
    flAlert->show();

}

matjson::Object PrismUI::GetTheme() {
    return Themes::getCurrentTheme();
}

bool PrismUI::init(float _w, float _h) {
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    auto currentLanguage = Lang::getLanguage();
    this->m_pLrSize = cocos2d::CCSize { _w, _h };
    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    this->m_mainLayer = cocos2d::CCLayer::create();
    this->addChild(this->m_mainLayer);

    auto bg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    auto bgBehind = cocos2d::extension::CCScale9Sprite::create("GJ_square07.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize(this->m_pLrSize - 3);
    bgBehind->setContentSize(this->m_pLrSize);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    bgBehind->setPosition(winSize.width / 2, winSize.height / 2);

    this->m_mainLayer->addChild(bgBehind);
    this->m_mainLayer->addChild(bg);
    bg->setID("prism-bg");
    this->m_pBGSprite = bg;

    this->m_buttonMenu = cocos2d::CCMenu::create();
    this->m_mainLayer->addChild(this->m_buttonMenu);

    // TODO: create custom sprite so people dont complain
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
    
    auto prismButton = CCScene::get()->getChildByID("prism-icon");
    if (prismButton != nullptr) {
        static_cast<CCMenuItemSpriteExtra*>(prismButton->getChildren()->objectAtIndex(0))->setEnabled(false);
    }

    auto sideBar = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    sideBar->setContentSize({ 100.0f, 230.0f });
    //sideBar->setPosition(125, bg->getContentSize().height / 2);
    sideBar->setPosition(60, _h / 2);
    Themes::RGBAToCC(GetTheme()["TableRowBg"], sideBar);

    auto buttonBG = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    buttonBG->setContentSize({ 320.0f, 230.0f });
    //buttonBG->setPosition(bg->getContentSize().width - 230, bg->getContentSize().height / 2);
    buttonBG->setPosition(_w - 173, _h / 2);
    Themes::RGBAToCC(GetTheme()["TableRowBg"], buttonBG);

    bg->addChild(sideBar);
    bg->addChild(buttonBG);
    CreateButton(currentLanguage->name("§ Global").c_str(), 0);
    CreateButton(currentLanguage->name("¬ Player").c_str(), 1);
    CreateButton(currentLanguage->name("ª Bypass").c_str(), 2);
    CreateButton(currentLanguage->name("« Creator").c_str(), 3);
    CreateButton(currentLanguage->name("··· Misc").c_str(), 4);
    CreateButton(currentLanguage->name("¶ Settings").c_str(), 5);
    Themes::RGBAToCC(GetTheme()["BG"], bg);
    Themes::RGBAToCC(GetTheme()["BG"], bgBehind);
    bgBehind->setColor({255, 255, 255});
        
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
    auto prismButton = CCScene::get()->getChildByID("prism-icon");
    if (prismButton != nullptr) {
        static_cast<CCMenuItemSpriteExtra*>(prismButton->getChildren()->objectAtIndex(0))->setEnabled(true);
    }
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
};

PrismUI* PrismUI::create() {
    auto pRet = new PrismUI();
    if (pRet) {
        if (pRet->init(PrismUI::s_defWidth, PrismUI::s_defHeight)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};

PrismUIButton* PrismUIButton::create(HackItem* hack) {
    auto pRet = new PrismUIButton();
    if (pRet) {
        if (pRet->init(hack)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};