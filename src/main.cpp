// Prism Menu by Firee
// please do not judge my coding, ok thank you!!!
#include "CustomSettings.hpp"
#include <Geode/utils/file.hpp>
#include "hacks.hpp"
#include "languages.hpp"
#include "themes.hpp"
#include <Geode/Geode.hpp>
//#include <geode.custom-keybinds/include/Keybinds.hpp>
//using namespace keybinds;

#include <iomanip>
#include <string>
#include <locale>
#include <codecvt>
#include <algorithm>

using namespace geode::prelude;

// ImGUI
bool showMenu = false;
int currentMenuIndex = 0;

struct EditMode {
    std::string name = "";
    HackValue editedValue = 0.F;
    bool isEditing = false;
};

EditMode editingMode;

#include <imgui-cocos.hpp>
#include <Geode/modify/MenuLayer.hpp>

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

struct ItemRectInfo {
    ImVec2 min;
    ImVec2 max;
};

ItemRectInfo g_ItemRectInfo;

void RoundTableBG() {
    auto currentTheme = Themes::getCurrentTheme();
    auto color = Themes::RGBAToImU32(currentTheme["TableRowBg"]);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 min = g_ItemRectInfo.min;
    ImVec2 max = g_ItemRectInfo.max;
    drawList->AddRectFilled(min, max, color, 15.0F, ImDrawFlags_RoundCornersAll);
}

// easier
void CreateButton(const char* name, int menuIndex, float buttonWidth, float buttonHeight) {
    if (currentMenuIndex == menuIndex) {
        auto currentTheme = Themes::getCurrentTheme();
        ImGui::BeginDisabled();
        //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, Themes::RGBAToImVec4(currentTheme["ButtonActive"]));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0F, 1.0F, 1.0F, 1.0F));
    }
    if (ImGui::Button(determineName(name, buttonWidth).c_str(), { buttonWidth, buttonHeight })) {
        currentMenuIndex = menuIndex;
    };
    if (currentMenuIndex == menuIndex) {
        ImGui::EndDisabled();
        ImGui::PopStyleColor(2);
    }
    //ImGui::PushStyleColor();
}

/*
for i in range(char2nr('A'), char2nr('Z'))
  call append(line("."), printf("{'%c', cocos2d::KEY_%c},", i, i))
endfor
*/

class PrismButton : public CCMenu {
protected:
    bool init() {
        if (!CCMenu::init())
            return false;
        HackItem* posX = Hacks::getHack("Button Position X");
        HackItem* posY = Hacks::getHack("Button Position Y");
        this->addChild(createButton(this));
        this->setPositionX(posX->value.intValue);
        this->setPositionY(posY->value.intValue);
        this->registerWithTouchDispatcher();
        this->setTouchEnabled(true);
        this->setZOrder(CCScene::get()->getHighestChildZ() + 100);
        return true;
    }
public:
    static CCMenuItemSpriteExtra* createButton(CCLayer* ret) {
        auto myButtonSpr = CircleButtonSprite::create(CCSprite::create("icon.png"_spr));
        return CCMenuItemSpriteExtra::create(myButtonSpr, ret, menu_selector(PrismButton::onButtonClicked));
    }
    void onButtonClicked(CCObject* p0) {
        showMenu = !showMenu;
    }

    static PrismButton* create() {
        auto ret = new PrismButton();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

PrismButton* prismButton;

bool firstLoad = false;
// early load is amazing!
class $modify(MyCustomMenu, MenuLayer) {
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
        if (firstLoad) return true;
        firstLoad = true;
        log::info("Prism Menu loaded! Enjoy the mod.");
        ImGuiCocos::get().setup([] {
            /*
            // Future Dark style by rewrking from ImThemes
            ImGuiStyle& style = ImGui::GetStyle();
            ImGuiIO& io = ImGui::GetIO();
            ImFontConfig config;
            config.GlyphRanges = io.Fonts->GetGlyphRangesCyrillic();
            std::string fontName = (Mod::get()->getResourcesDir() / "PrismMenu.otf").string();
            //std::string fontName = (Mod::get()->getResourcesDir() / "Hack-Regular.ttf").string();
            io.Fonts->AddFontFromFileTTF(fontName.c_str(), 24, &config);
            io.Fonts->AddFontFromFileTTF(fontName.c_str(), 30, &config);
            // TODO, look at gamershackers or idk his cchannels tutorials on adding images to add icons
            
            style.Alpha = 1.0f;
            style.DisabledAlpha = 1.0f;
            style.WindowPadding = ImVec2(12.0f, 12.0f);
            style.WindowRounding = 15.0f;
            style.WindowBorderSize = 0.0f;
            style.WindowMinSize = ImVec2(20.0f, 20.0f);
            style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
            style.WindowMenuButtonPosition = ImGuiDir_None;
            
            style.ChildRounding = 0.0f;
            style.ChildBorderSize = 1.0f;
            
            style.PopupRounding = 0.0f;
            style.PopupBorderSize = 1.0f;
            
            style.FramePadding = ImVec2(6.0f, 6.0f);
            style.FrameRounding = 0.0f;
            style.FrameBorderSize = 0.0f;
            
            style.ItemSpacing = ImVec2(12.0f, 6.0f);
            style.ItemInnerSpacing = ImVec2(6.0f, 3.0f);
            
            style.CellPadding = ImVec2(12.0f, 6.0f);
            
            style.IndentSpacing = 20.0f;
            style.ColumnsMinSpacing = 6.0f;
            style.ScrollbarSize = 12.0f;
            style.ScrollbarRounding = 20.0f;
            style.GrabMinSize = 12.0f;
            style.GrabRounding = 0.0f;
            style.TabRounding = 0.0f;
            style.TabBorderSize = 0.0f;
            style.TabMinWidthForCloseButton = 0.0f;
            style.ColorButtonPosition = ImGuiDir_Right;
            style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
            style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
            
            float currentOpacity = Hacks::getHack("Menu Opacity")->value.floatValue;
            style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, currentOpacity);
            style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, currentOpacity);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, currentOpacity);
            style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
            style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
            style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
            style.Colors[ImGuiCol_Header] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
            style.Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
            style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
            style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
            style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
            style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
            style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
            style.Colors[ImGuiCol_TabActive] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
            style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
            style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f);
            style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
            ///style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
            ///style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
            style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 0.0f);
            style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
            //style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
            //style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
            style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 0.5f);
            style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 0.5f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
            style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
            style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
            //style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);*/ 
            Themes::LoadTheme(Themes::getCurrentTheme());
        }).draw([] {
            if (showMenu) {
                // i cant use LoadTheme because 0160:err:virtual:allocate_virtual_memory out of memory for allocation, base (nil) size
                // well just in case 
                if (Hacks::isHackEnabled("Live Theme Editing")) {
                    Themes::LoadTheme(Themes::getCurrentTheme());
                } else {
                    Themes::UpdateOpacity(Themes::getCurrentTheme());
                }

                auto* director = CCDirector::sharedDirector();
                const auto winSize = director->getWinSize();
                const auto frameSize = director->getOpenGLView()->getFrameSize();
                //std::cout << fmt::format("{},{} and {},{}",winSize.width,winSize.height,frameSize.width,frameSize.height) << std::endl;
                // 570, 320
                //ImGui::SetNextWindowSize({ 1000, 480 }/*, ImGuiCond_FirstUseEver*/);
                //ImGui::SetNextWindowSize({ 1.8F * winSize.width, 1.5F * winSize.height });
                float width = (1000 * frameSize.width) / 1366;
                float height = (500 * frameSize.height) / 768;
                float tableWidth = (220 * frameSize.width) / 1366;

                ImGui::SetNextWindowSize({ width, height });
                ImGui::Begin("Prism Menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
                //231,245 - 451,701
                RoundTableBG();
                int currentLang = Hacks::getHack("Language")->value.intValue;
                if (ImGui::BeginTable("table_left", 1, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV, { tableWidth, 0 })) // ImGuiTableFlags_RowBg | ImGuiTableFlags_None |
                {
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, 0);
                    //ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, 4);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    // Categories
                    ImFont* oldFont = ImGui::GetFont(); // Save the current font
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // Change to a different font in the font atlas

                    ImGui::Spacing();
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f, 13.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
                    // here we do a genius trick, please dont ask me how i did this! ok! this took forever to do
                    // chat jippity gave icon ideas, i designed the player!
                    // oh and PLEASE dont complain about it being misaligned in the center
                    float buttonWidth = (194 * frameSize.width) / 1366;
                    float buttonHeight = (60 * frameSize.height) / 768;
                    CreateButton(Lang::get(currentLang)->name("§ Global").c_str(), 0, buttonWidth, buttonHeight);
                    CreateButton(Lang::get(currentLang)->name("¬ Player").c_str(), 1, buttonWidth, buttonHeight);
                    CreateButton(Lang::get(currentLang)->name("ª Bypass").c_str(), 2, buttonWidth, buttonHeight);
                    CreateButton(Lang::get(currentLang)->name("« Creator").c_str(), 3, buttonWidth, buttonHeight);
                    CreateButton(Lang::get(currentLang)->name("··· Misc").c_str(), 4, buttonWidth, buttonHeight);

                    ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::PopStyleVar(2);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
                    // ad "UI Labels"
                    CreateButton(Lang::get(currentLang)->name("¶ Settings").c_str(), 5, buttonWidth, buttonHeight);
                    ImGui::PopStyleVar();
                    ImGui::PopFont(); // Restore the old font
                    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f, 15.0f));
                    ImGui::Spacing();
                    ImGui::EndTable();
                }
                g_ItemRectInfo.min = ImGui::GetItemRectMin();
                g_ItemRectInfo.max = ImGui::GetItemRectMax();
                ImGui::SameLine();
                if (ImGui::BeginTable("table1_right", 1, ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)) { //ImGuiTableFlags_ScrollX | 
                    // Buttons
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, 0);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    std::vector<matjson::Value> jsonArray;
                    switch (currentMenuIndex) {
                        case 0: { // Global 
                            jsonArray = matjson::parse(Hacks::getGlobalHacks()).as_array();
                            break;
                        }
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
                            ImGui::Text("%s", Lang::get(currentLang)->name("Prism Menu by Firee").c_str());
                            ImGui::Text("V1.0.0 (Geode)");
                            ImGui::Separator();
                            break;
                    }
                    ImGui::Spacing();
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
                    auto settings = Mod::get()->getSavedValue<SettingHackStruct>("values");
                    for (size_t i = 0; i < jsonArray.size(); i++) {
                        const auto& obj = jsonArray[i];
                        std::string name = obj.get<std::string>("name");
                        std::string desc = obj.get<std::string>("desc");
                        auto opcodes = obj.get<matjson::Array>("opcodes");
                        HackItem* hack = Hacks::getHack(name);
                        if (hack != nullptr) {
                            if (obj.contains("winOnly")) {
                                //#ifndef GEODE_IS_WINDOWS
                                ImGui::BeginDisabled(true);
                                //#endif
                            }
                            if (obj.contains("win")) { // yeah idk what property okay
                                #ifndef GEODE_IS_WINDOWS
                                ImGui::BeginDisabled(true);
                                #endif
                            }
                            if (hack->value.type == ValueType::Int && hack->type != "dropdown") {
                                auto min = obj.get<int>("min");
                                auto max = obj.get<int>("max");
                                if (ImGui::InputInt(Lang::get(currentLang)->name(name).c_str(), &hack->value.intValue, 10, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {
                                    if (name == "FPS Bypass") {
                                        // from mats fps unlocker
                                        //Hacks::Settings::setSettingValue(&settings, *hack, hack->value.floatValue);
                                        auto app = CCApplication::sharedApplication();
                                        app->setAnimationInterval(1.0 / static_cast<double>(hack->value.intValue));
                                    } else if (name == "Button Position X") {
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.intValue);
                                        prismButton->setPositionX(hack->value.intValue);
                                    } else if (name == "Button Position Y") {
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.intValue);
                                        prismButton->setPositionY(hack->value.intValue);
                                    }
                                }
                            } else if (hack->value.type == ValueType::Float) {
                                auto min = obj.get<float>("min");
                                auto max = obj.get<float>("max");
                                if (!editingMode.isEditing || editingMode.name != name.c_str()) {
                                    if (ImGui::SliderFloat(Lang::get(currentLang)->name(name).c_str(), &hack->value.floatValue, min, max, "%.3f", ImGuiSliderFlags_NoInput)) {
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.floatValue);
                                        if (name == "Speedhack") {
                                            if (hack->value.floatValue < 0.0F) return;
                                            Hacks::setPitch(hack->value.floatValue);
                                            CCDirector::sharedDirector()->getScheduler()->setTimeScale(hack->value.floatValue);
                                        }
                                    }
                                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                                        editingMode.isEditing = true;
                                        editingMode.name = hack->name;
                                        editingMode.editedValue.floatValue = hack->value.floatValue;
                                    }
                                } else {
                                    ImGui::BeginDisabled();
                                    ImGui::SliderFloat(Lang::get(currentLang)->name(name).c_str(), &editingMode.editedValue.floatValue, min, max);
                                    ImGui::EndDisabled();
                                    ImGui::SetKeyboardFocusHere();
                                    if (ImGui::InputFloat("##EditValue", &editingMode.editedValue.floatValue, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)) {
                                        hack->value.floatValue = editingMode.editedValue.floatValue;
                                        editingMode.isEditing = false;
                                        editingMode.name = "";
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.floatValue);
                                        if (name == "Speedhack") {
                                            if (hack->value.floatValue < 0.0F) {
                                                hack->value.floatValue = 1.0F;
                                            }
                                            Hacks::setPitch(hack->value.floatValue);
                                            CCDirector::sharedDirector()->getScheduler()->setTimeScale(hack->value.floatValue);
                                        }
                                    }
                                    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
                                        editingMode.isEditing = false;
                                        editingMode.name = "";
                                    }
                                }
                            } else if (hack->value.type == ValueType::Bool) {
                                if ((Hacks::isHackEnabled("Enable Patching") && obj.contains("winOnly")) || !obj.contains("winOnly") || name == "Enable Patching") {
                                    if (ImGui::Checkbox(Lang::get(currentLang)->name(name).c_str(), &hack->value.boolValue)) {
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.boolValue);
                                        if (name == "Show Button") {
                                            prismButton->setVisible(hack->value.boolValue);
                                        }
                                        if (Hacks::isHackEnabled("Enable Patching") && !opcodes.empty()) {
#ifdef GEODE_IS_WINDOWS
                                            Hacks::applyPatches(name, opcodes, hack->value.boolValue);
#endif
                                        }
                                    }
                                }
                            } else if (hack->value.type == ValueType::Char) {
                                /*auto oldValue = hack->value.charValue;
                                if (ImGui::InputTextWithHint(Lang::get(currentLang)->name(name).c_str(), "C", hack->value.charValue, 2, ImGuiInputTextFlags_EnterReturnsTrue)) {
                                    if (name == "Open Menu Keybind") {
                                        auto it = charToKeyMap.find(*hack->value.charValue);
                                        if (it != charToKeyMap.end()) {
                                            std::cout << "Save" << std::endl;
                                            auto inputKeybind = it->second;
                                            BindManager::get()->removeBindable("prism-menu"_spr);
                                            BindManager::get()->registerBindable({
                                                "prism-menu"_spr,
                                                "Toggle Prism Menu",
                                                "Keybind for toggling when to show Prism Menu's Mod Menu.",
                                                { Keybind::create(KEY_A, Modifier::None) },
                                                "Prism Menu/ModMenu"
                                            });
                                            BindManager::get()->save();
                                        } else {
                                            hack->value.charValue = oldValue;
                                        }
                                        
                                    }
                                }*/
                            } else if (hack->type == "dropdown" || hack->value.type == ValueType::Custom) {
                                auto type = obj.get<std::string>("type");
                                if (type == "button") {
                                    if (ImGui::Button(Lang::get(currentLang)->name(name).c_str())) {
                                        if (name == "Restore Defaults") {
                                            Hacks::processJSON(true);
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
                                        }
                                        ImGui::OpenPopup(name.c_str());
                                        
                                    }
                                    // Popups
                                    if (ImGui::BeginPopupModal("Credits", nullptr)) {
                                        auto creditLine1 = Lang::get(currentLang)->name(Lang::get(currentLang)->name("Thank you to Electrify (ES), Jouca (FR), and dank_meme01 (RU) for translations!"));
                                        auto creditLine2 = Lang::get(currentLang)->name(Lang::get(currentLang)->name("And thank you for using the mod! I hope you enjoy using Prism Menu!"));
                                        ImGui::Text("%s", creditLine1.c_str());
                                        ImGui::Separator();
                                        ImGui::Text("%s", creditLine2.c_str());
                                        if (ImGui::Button(Lang::get(currentLang)->name(Lang::get(currentLang)->name("Close Popup").c_str()).c_str()))
                                            ImGui::CloseCurrentPopup();
                                        ImGui::EndPopup();
                                    }
                                    
                                } else if (type == "dropdown") {
                                    auto values = obj.get<matjson::Array>("values");
                                    if (hack->name == "Theme") {
                                        values = Themes::getCurrentThemes();
                                    }
                                    int selectedIndex = hack->value.intValue;
                                    if (ImGui::BeginCombo(Lang::get(currentLang)->name(name).c_str(), values[selectedIndex].as_string().c_str())) {
                                        for (int i = 0; i < values.size(); i++) {
                                            const bool isSelected = (selectedIndex == i);
                                            if (ImGui::Selectable(values[i].as_string().c_str(), isSelected)) {
                                                hack->value.intValue = i;
                                                Hacks::Settings::setSettingValue(&settings, *hack, hack->value.intValue);
                                            }
                                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                                            if (isSelected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                }
                            }
                            if (obj.contains("winOnly")) {
                                //#ifndef GEODE_IS_WINDOWS
                                ImGui::EndDisabled();
                                //#endif
                            }
                            if (obj.contains("win")) { // yeah idk what property okay
                                #ifndef GEODE_IS_WINDOWS
                                ImGui::EndDisabled();
                                #endif
                            }
                            if (ImGui::IsItemHovered() && Hacks::isHackEnabled("Show Tooltips")) {
                                if ((obj.contains("winOnly") && Hacks::isHackEnabled("Enable Patching")) || !obj.contains("winOnly")) {
                                    ImGui::BeginTooltip();
                                    ImGui::Text("%s", Lang::get(currentLang)->desc(desc).c_str());
                                    ImGui::EndTooltip();
                                }
                            }
                        }
                    }
                    
                    ImGui::PopStyleVar();
                    ImGui::EndTable();
                }
                if (ImGui::BeginPopupModal("Success", nullptr)) {
                    ImGui::Text("The custom theme has successfully been imported! Restart your game to use it.");
                    if (ImGui::Button(Lang::get(currentLang)->name("Close Popup").c_str()))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
                ImGui::End();
            }
        });
        prismButton = PrismButton::create();
        prismButton->setVisible(Hacks::isHackEnabled("Show Button"));
        SceneManager::get()->keepAcrossScenes(prismButton);
        return true;
    }
};
$on_mod(Loaded) {
    SettingHackStruct val { matjson::Array() };
    Mod::get()->addCustomSetting<SettingHackValue>("values", val);
    Hacks::processJSON(false);
    Themes::addToCurrentThemes();
    //auto getKeybindHack = Hacks::getHack("Open Menu Keybind");
    //char inputKeybind = 'C';
    //if (getKeybindHack != nullptr) inputKeybind = *(getKeybindHack->value.charValue);
    /*
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
            showMenu = !showMenu;
            return true;
        }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, arr);
    }
};

// sorry, not sorry. for some reason the allHacks vector isnt being initialized in the other .cpp file
// i completely wasted my time writing this whole patch script, and i kinda want android + mac support soooo

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>
#include <Geode/modify/CCTransitionFade.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

class $modify(GJBaseGameLayer) {

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
};

// showing the icon for android users lol
class $modify(PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        auto menu = dynamic_cast<CCMenu*>(this->getChildren()->objectAtIndex(8));
        if (menu == nullptr) return;
        auto button = PrismButton::createButton(this);
        button->setPositionX(-240);
        menu->addChild(button);
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
    bool initedDeath = false;
    GameObject* m_antiCheatObject; // removing after lol
    GJGameLevel* m_gameLevel;
    GJLevelType oldLevelType;
    CircleButtonSprite* cheatIndicator;
    bool isCheating = false;
    int updateInit = 0;
    // Anticheat Bypass, Noclip, No Spikes, No Solids
    void destroyPlayer(PlayerObject *p0, GameObject *p1) {
        //bool m_isTestMode = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x413);
        //std::cout << m_isTestMode << std::endl;
        if (Hacks::isHackEnabled("Enable Patching")) return PlayLayer::destroyPlayer(p0, p1);
        bool noclipDisabled = !Hacks::isHackEnabled("No Solids") && !Hacks::isHackEnabled("Noclip");
        if ((noclipDisabled && !Hacks::isHackEnabled("No Spikes"))) return PlayLayer::destroyPlayer(p0, p1);
        if (Hacks::isHackEnabled("No Spikes") && noclipDisabled && p1 == nullptr) return PlayLayer::destroyPlayer(p0, p1); // why solids are nullptr is beyond my comprehension.
        if (Hacks::isHackEnabled("Anticheat Bypass")) {
            if (!m_fields->initedDeath) {
                // bad coding
                for (int offset = 0x0; ; offset += 0x1) {
                    GameObject* val = reinterpret_cast<GameObject*>(reinterpret_cast<uintptr_t>(this) + offset);
                    if (val == p1) {
                        log::debug("Found Anticheat Object at offset 0x{}", offset);
                        m_fields->m_antiCheatObject = p1;
                        break;
                    }
                }
                //m_antiCheatObject = p1;
                m_fields->initedDeath = true;
            }
            if (p1 == m_fields->m_antiCheatObject) { // nice AC robert
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
        // 0xaa9
        bool m_isTestMode = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x3d4 + 0x4); // absolutely cursed
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
        m_fields->m_gameLevel = p0;
        m_fields->oldLevelType = p0->m_levelType;
        if (Hacks::isHackEnabled("Level Edit")) {
            //m_fields->m_gameLevel->m_levelType = static_cast<GJLevelType>(2);
        }
        auto testModeLabel = dynamic_cast<CCLabelBMFont*>(this->getChildren()->objectAtIndex(this->getChildrenCount() - 1));
        if (testModeLabel != nullptr && !strcmp(testModeLabel->getString(), "Testmode") && Hacks::isHackEnabled("Hide Testmode")) {
            testModeLabel->setVisible(false);
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
        if (prismButton != nullptr && Hacks::isHackEnabled("Show Button")) prismButton->setVisible(true);
        m_fields->m_gameLevel->m_levelType = m_fields->oldLevelType;
        m_fields->initedDeath = false;
        PlayLayer::onQuit();
    }
    void postUpdate(float p0) {
        // whats the difference between m_fields and not using? i have no idea!
        auto progressBar = dynamic_cast<CCSprite*>(this->getChildren()->objectAtIndex(6));
        auto percentLabel = dynamic_cast<CCLabelBMFont*>(this->getChildren()->objectAtIndex(7));
        auto node = dynamic_cast<CCNode*>(this->getChildren()->objectAtIndex(0));
        m_fields->cheatIndicator->setVisible(Hacks::isHackEnabled("Cheat Indicator"));
        if ( // i dont know what are considered "cheats"
            Hacks::isHackEnabled("Noclip") ||
            Hacks::isHackEnabled("No Spikes") ||
            Hacks::isHackEnabled("Freeze Player") ||
            Hacks::isHackEnabled("No Mirror Transition") ||
            Hacks::isHackEnabled("Instant Mirror Portal") ||
            Hacks::isHackEnabled("Jump Hack") ||
            Hacks::isHackEnabled("Instant Complete") ||
            Hacks::isHackEnabled("Force Platformer Mode") ||
            Hacks::isHackEnabled("Change Gravity")
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
        if (progressBar == nullptr || percentLabel == nullptr || node == nullptr) return PlayLayer::postUpdate(p0);
        auto layer = dynamic_cast<CCLayer*>(node->getChildren()->objectAtIndex(2));
        if (layer != nullptr) {
            if (layer->getChildrenCount() > 100) {
                // can this please be the solution
                auto attemptLabel = dynamic_cast<CCLabelBMFont*>(layer->getChildren()->objectAtIndex(layer->getChildrenCount() - 1));
                if (attemptLabel != nullptr) {
                    attemptLabel->setVisible(!Hacks::isHackEnabled("Hide Attempts"));
                    attemptLabel->setOpacity(Hacks::getHack("Attempt Opacity")->value.floatValue * 255);
                }
            }
        }
        int currentPosition = Hacks::getHack("Progress Bar Position")->value.intValue;
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        if (Hacks::isHackEnabled("No Progress Bar")) {
            if (this->m_fields->previousPositionX == 0.0F) {
                this->m_fields->previousPositionX = percentLabel->getPositionX();
            }
            progressBar->setVisible(false);
            percentLabel->setPositionX(winSize.width / 2);
        } else {
            if (m_fields->previousPositionX != 0.0F) {
                progressBar->setVisible(true);
                percentLabel->setPositionX(this->m_fields->previousPositionX);
            }
        }
        progressBar->setRotation(0.0F);
        switch (currentPosition) {
            case 0: // Top (312)
                progressBar->setPositionY(winSize.height - 8);
                percentLabel->setPositionY(winSize.height - 8);
                break;
            case 1: // Bottom (10)
                progressBar->setPositionY(10);
                percentLabel->setPositionY(10);
                break;
            case 2: // Left // 275
                progressBar->setPosition({ 10, winSize.height / 2 });
                percentLabel->setPosition({ 5, winSize.height - 45 }); // 275
                progressBar->setRotation(-90.0F);
                break;
            case 3: // Right
                progressBar->setPosition({ winSize.width - 10, winSize.height / 2 });
                percentLabel->setPosition({ winSize.width - 40, winSize.height - 45 });
                progressBar->setRotation(-90.0F);
                break;
        }
        PlayLayer::postUpdate(p0);
    }
    
    // Accurate Percentage
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (Hacks::isHackEnabled("Enable Patching") || m_fields->m_gameLevel == nullptr) return;
        if (Hacks::isHackEnabled("Accurate Percentage")) {
            auto percentLabel = dynamic_cast<CCLabelBMFont*>(this->getChildren()->objectAtIndex(7));
            if (percentLabel == nullptr) return;
            float percent = (this->m_player1->getPositionX() / m_fields->m_gameLevel->m_levelLength) * 100; // 6
            std::string percentStr = std::to_string(percent) + "%";
            percentLabel->setString(percentStr.c_str());
        }
    }
    /*void levelComplete() {
        if (!Hacks::isHackEnabled("Safe Mode") || Hacks::isHackEnabled("Enable Patching")) return PlayLayer::levelComplete();
        PlayLayer::resetLevel(); // haha
    }*/
};


/*
class $modify(PlayLayer) {
    float previousPositionX = 0.0F;
    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        
        
    }
    // Safe Mode
    void update(float p0) {
        if (Hacks::isHackEnabled("Safe Mode") && !Hacks::isHackEnabled("Enable Patching")) {
            m_isTestMode = true;
        }

        
        
        //std::cout << fmt::format("{},{},{}", this->unk508, this->unk518, this->unknown4d8) << std::endl;
        return PlayLayer::update(p0);
    }
    bool init(GJGameLevel* p0) {
        if (Hacks::isHackEnabled("Practice Music")) {
            GameStatsManager::sharedState()->toggleEnableItem(UnlockType::GJItem, 0x11, true);
        }
        return PlayLayer::init(p0);
    }

    // Accurate Percentage
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (Hacks::isHackEnabled("Enable Patching")) return;
        if (Hacks::isHackEnabled("Accurate Percentage") || Hacks::isHackEnabled("Precise Accurate Percentage")) {
            float percent = (this->m_player1->getPositionX() / m_levelLength) * 100; // 6
            std::string percentStr = std::to_string(percent) + "%";
            if (!Hacks::isHackEnabled("Precise Accurate Percentage")) {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2) << percent << "%";
                percentStr = oss.str();
            }
            m_percentLabel->setString(percentStr.c_str());
        }
    }

    /\*void levelComplete() {
        if (!Hacks::isHackEnabled("Safe Mode") || Hacks::isHackEnabled("Enable Patching")) return PlayLayer::levelComplete();
        PlayLayer::resetLevel(); // haha
    }
    void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5) {
        if (!Hacks::isHackEnabled("Safe Mode")) return PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);
    }*\/ // WHY YOU HAVE DELAY
};
*/
class $modify(PlayerObject) {
    // No Solids, Everything Hurts
    /*bool collidedWithObject(float fl, GameObject* obj) { // what is the point of not having p2, because this doesnt work without it
        if (Hacks::isHackEnabled("Enable Patching")) return PlayerObject::collidedWithObject(fl, obj);
        if (Hacks::isHackEnabled("Everything Hurts")) { // not accurate to the original, but whats the point of this hack!? also apparently i had && this->m_isInPlayLayer but forgor what its for
            //        std::cout << (typeid(*this).name() + 6) << std::endl;
            //std::cout << (typeid(*(this->getParent()->getParent()->getParent())).name() + 6) << std::endl;
            auto* playLayer = static_cast<PlayLayer*>(this->getParent()->getParent()->getParent()); // bruh
            if (playLayer != nullptr) {
                playLayer->destroyPlayer(this, obj);
            }
            std::cout << "WHY ARE YOU CRASHING" << std::endl;
        }
        if (!Hacks::isHackEnabled("No Solids")) return PlayerObject::collidedWithObject(fl, obj);
        return PlayerObject::collidedWithObject(fl, obj);
    }*/
    #ifndef GEODE_IS_ANDROID // for whatever reason, fields arent found!
    bool was_platformer;
    float old_gravity;
    /*bool collidedWithObject(float fl, GameObject* obj,  cocos2d::CCRect p0, bool p1) {
        return PlayerObject::collidedWithObject(fl, obj, p0, p1);
    }*/
    // Freeze Player
    void update(float dt) {
        if (!m_fields->was_platformer) {
            m_fields->was_platformer = this->m_isPlatformer;
        }
        if (Hacks::isHackEnabled("Force Platformer Mode")) {
            togglePlatformerMode(true);
        } else {
            togglePlatformerMode(m_fields->was_platformer);
        }
        auto gravityHack = Hacks::getHack("Gravity Value");
        if (Hacks::isHackEnabled("Change Gravity")) { // assume its enabled 
            m_gravityMod = gravityHack->value.floatValue;
        } else {
            m_gravityMod = m_fields->old_gravity;
        }
        if (Hacks::isHackEnabled("Instant Complete")) return;
        if (Hacks::isHackEnabled("Enable Patching") || !Hacks::isHackEnabled("Freeze Player")) return PlayerObject::update(dt);
    }
    bool init(int p0, int p1, GJBaseGameLayer *p2, cocos2d::CCLayer *p3, bool p4) {
        if (!PlayerObject::init(p0,p1,p2,p3,p4)) return false;
        m_fields->old_gravity = m_gravityMod;
        return true;
    }

    void playerDestroyed(bool p0) {
        m_fields->was_platformer = this->m_isPlatformer;
        PlayerObject::playerDestroyed(p0);
    }
    #endif
    void pushButton(PlayerButton p0) {
        if (!Hacks::isHackEnabled("Enable Patching") && Hacks::isHackEnabled("Jump Hack")) PlayerObject::boostPlayer(10.0F); // idk if i should make this customizable
        PlayerObject::pushButton(p0);
    }
};

class $modify(GJGameLevel) {
    // Safe Mode (a just incase)
    void savePercentage(int p0, bool p1, int p2, int p3, bool p4) {
        if (!Hacks::isHackEnabled("Safe Mode") || Hacks::isHackEnabled("Enable Patching")) {
            GJGameLevel::savePercentage(p0, p1, p2, p3, p4);
        }
    }
};

class $modify(CCTransitionFade) {
    // No Transition
    bool initWithDuration(float t, cocos2d::CCScene* scene, cocos2d::ccColor3B const& color)  {
        if (!Hacks::isHackEnabled("No Transition") || Hacks::isHackEnabled("Enable Patching")) {
            return CCTransitionFade::initWithDuration(t, scene, color);
        } else {
            return CCTransitionFade::initWithDuration(0.0F, scene, color);
        }
    }
};

#ifndef GEODE_IS_ANDROID
class $modify(GameObject) {
    // Layout Mode
    void setVisible(bool v) {
        /*int aaa = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x14E); // we do the funny because someone forgor pad for windows
        if (aaa > 1600000000) {
            //int targetValue = 44;
            GameObjectType targetValue = GameObjectType::Decoration;
            for (int offset = 0x0; ; offset += 0x1) {
                GameObjectType val = *reinterpret_cast<GameObjectType*>(reinterpret_cast<uintptr_t>(this) + offset);
                //int val = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offset);
                //std::cout << "Offset: 0x" << std::hex << offset << std::dec << ", Value: " << val << std::endl;
                if (val == targetValue) {
                    //std::cout << "Found target " << targetValue << " at offset 0x" << std::hex << offset << std::dec << std::endl;
                    log::info(fmt::format("Found target at offset 0x{}", offset));;
                    break;
                }
            }
        //}*/
        // yes letes overwrite fields!
        // plesae commit add paddings for android64 and windows kthx
        if (!Hacks::isHackEnabled("Layout Mode")) return GameObject::setVisible(v);
        int objectID = -1;
        GameObjectType objectType;
        int objectTypeInt = -1;
#ifdef GEODE_IS_WINDOWS
        objectID = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x384); // absolutely cursed
        objectType = *reinterpret_cast<GameObjectType*>(reinterpret_cast<uintptr_t>(this) + 0x31c);
#else
    #ifdef GEODE_IS_ANDROID32
        objectID = this->m_objectID;
        objectType = this->m_objectType;
    #else // android 64 (BROKEN)
        objectType = *reinterpret_cast<GameObjectType*>(reinterpret_cast<uintptr_t>(this) + 0x904);
    #endif
#endif
        GameObject::setVisible(v);
#ifdef GEODE_IS_WINDOWS
        if (objectType == GameObjectType::Decoration && objectID != 44) { // 44 being practice mode checkpoint, because thats a "decoration"
            GameObject::setVisible(false);
        } else {
            GameObject::setVisible(v);
        }
#else
        if (objectType == GameObjectType::Decoration) {
            GameObject::setVisible(false);
        } else {
            GameObject::setVisible(v);
        }
#endif
    }
    /*
    void objectFromVector(gd::vector<gd::string> &p0, gd::vector<void *> &p1, GJBaseGameLayer *p2, bool p3) {
        return GameObject::objectFromVector(p0,p1,p2,p3);
    }*/ // someone got the wrong bindings!
    /*
    

    */
};
#endif

class $modify(GameManager) {
    bool isIconUnlocked(int _id, IconType _type) {
        if (!Hacks::isHackEnabled("Icon Bypass") || Hacks::isHackEnabled("Enable Patching")) return GameManager::isIconUnlocked(_id, _type);
        return true;
    }
    bool isColorUnlocked(int _id, UnlockType _type) {
        if (!Hacks::isHackEnabled("Icon Bypass") || Hacks::isHackEnabled("Enable Patching")) return GameManager::isColorUnlocked(_id, _type);
        return true;
    }
};

class $modify(CCTextInputNode) {
    bool init(float width, float height, const char *caption, const char *thonburi, int maxCharCount, const char *font) {
        bool ret = CCTextInputNode::init(width, height, caption, thonburi, maxCharCount, font);
        /*
         *    
                      piVar3 = (astruct *)
               CCTextInputNode::create
                         ((float)uVar17,(float)((ulonglong)uVar17 >> 0x20),(char *)0x43960000,
                          (char *)0x42480000,(int)"Level Name","Thonburi");
      (**(code **)(*(int *)this + 0xe0))
                (this,piVar3,0x12,*(code **)(*(int *)this + 0xe0),uVar18,pcVar12);
      piVar3->field330_0x154 = this + 0x11c;
      (**(code **)(piVar3->field0_0x0 + 0x24))(piVar3,1);
      (**(code **)(piVar3->field0_0x0 + 0x5c))(piVar3,aCStack_188);
      piVar3->field331_0x158 = 0x14; << THIS
         */
        //std::cout << fmt::format("maxCharCount = {}", reinterpret_cast<float*>(maxCharCount)) << std::endl;
        return ret;
    }
    // Text Length, Character Filter broken for now on 2.2
    /*bool onTextFieldInsertText(CCTextFieldTTF *p0, const char *p1, int p2, cocos2d::enumKeyCodes p3) {
        if (Hacks::isHackEnabled("Character Filter") && !Hacks::isHackEnabled("Enable Patching")) this->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,-!?:;)(/\\\"\'`*= +-_%[]<>|@&^#{}%$~");
        if (Hacks::isHackEnabled("Text Length") && !Hacks::isHackEnabled("Enable Patching")) this->m_maxLabelLength = -1;
        // this wont work but why is it private!?
        return CCTextInputNode::onTextFieldInsertText(p0, p1, p2, p3);
    }*/
};
class $modify(GameStatsManager) {
    bool isItemUnlocked(UnlockType p0, int p1) {
        if (p0 != UnlockType::GJItem && p1 != 17) return GameStatsManager::isItemUnlocked(p0,p1);
        if (Hacks::isHackEnabled("Practice Music")) {
            return true;
        } else {
            return GameStatsManager::isItemUnlocked(p0,p1);
        }
    }
};

/*
class $modify(GameStatsManager) {
    // Almost all bypass
    int getStat(char const* type) { // isnt this supposed to be int
        int ret = GameStatsManager::getStat(type);
        int typeInt = std::stoi(type);
        //std::cout << fmt::format("type = {}, ret = {}", type, ret) << std::endl;
        if (typeInt == 8 && Hacks::isHackEnabled("Main Levels") && !Hacks::isHackEnabled("Enable Patching")) { // main level
            return 30;
        }
        if (typeInt == 12 && Hacks::isHackEnabled("Guard Vault") && !Hacks::isHackEnabled("Enable Patching")) { // guard vault
            return 10;
        }
        if (typeInt == 13 && Hacks::isHackEnabled("Keymaster Vault") && !Hacks::isHackEnabled("Enable Patching")) { // keymaster vault
            return 100;
        }
        if (typeInt == 21 && Hacks::isHackEnabled("Treasure Room") && !Hacks::isHackEnabled("Enable Patching")) { // treasure room
            return 5;
        }
        if (typeInt == 14 && Hacks::isHackEnabled("Free Shop Items") && !Hacks::isHackEnabled("Enable Patching")) { // free shop items
            return 1000000; // lol what
        }
        if (typeInt == 6 && Hacks::isHackEnabled("Backup Stars Limit") && !Hacks::isHackEnabled("Enable Patching")) { // backup stars limit
            return 10;
        }
        return ret;
    }
};
*/

/*
class $modify(EditLevelLayer) {
    // Verify Hack, No (C) Mark
    void onShare(CCObject* sender) {
        if (!Hacks::isHackEnabled("Verify Hack") && !Hacks::isHackEnabled("No (C) Mark")) return EditLevelLayer::onShare(sender);
        if (Hacks::isHackEnabled("Verify Hack")) {
            auto isVerified = this->m_level->m_isVerified;
            this->m_level->m_isVerified = true;
            //this->m_level->m_isVerified = isVerified;
        }
        if (Hacks::isHackEnabled("No (C) Mark")) {
            auto isVerified = this->m_level->m_originalLevel;
            this->m_level->m_originalLevel = true;
        }
        EditLevelLayer::onShare(sender);
    }
};
*/

class $modify(LevelInfoLayer) {
    // Copy Hack
    bool init(GJGameLevel *p0, bool p1) {
        if (!LevelInfoLayer::init(p0, p1)) return false;
        if (Hacks::isHackEnabled("Copy Hack")) {
            auto gm = GameManager::sharedState();
            if (gm->m_playerUserID_a == p0->m_userID) return true;
            if (gm->m_playerUserID_b == p0->m_userID) return true;
            if (gm->m_playerUserID == p0->m_userID) return true;
            if ((gm->m_playerUserID_a + gm->m_playerUserID_b) == p0->m_userID) return true;
            auto aCloneBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png"), this, menu_selector(LevelInfoLayer::confirmClone));
            aCloneBtn->setPosition(m_cloneBtn->getPosition());
            m_cloneBtn->getParent()->addChild(aCloneBtn);
            m_cloneBtn->setVisible(false);
        }
        return true;
    }
};
