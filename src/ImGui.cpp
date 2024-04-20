#include <Geode/modify/MenuLayer.hpp>
#ifndef NO_IMGUI
#include <imgui-cocos.hpp>
#include <codecvt>
#include "PrismButton.hpp"
#include "hacks.hpp"
#include "Languages.hpp"
#include "Themes.hpp"

int currentMenuIndex;

struct EditMode {
    std::string name = "";
    HackValue editedValue = 0.F;
    bool isEditing = false;
};

EditMode editingMode;
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

bool firstLoadI = false;
bool changedOpacity = false;
bool changedMenuScale = false;

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        if (firstLoadI) return true;
        firstLoadI = true;
        ImGuiCocos::get().setup([] {
            Themes::LoadTheme(Themes::getCurrentTheme());
        }).draw([] {
            auto prismButton = typeinfo_cast<PrismButton*>(CCScene::get()->getChildByID("prism-icon"));
            if (prismButton == nullptr) return;
            if (prismButton->showImGuiMenu) {
                // i cant use LoadTheme because 0160:err:virtual:allocate_virtual_memory out of memory for allocation, base (nil) size
                // well just in case 
                if (Hacks::isHackEnabled("Live Theme Editing") || changedMenuScale) {
                    Themes::LoadTheme(Themes::getCurrentTheme());
                    changedMenuScale = false;
                } else {
                    if (changedOpacity) {
                        Themes::UpdateOpacity(Themes::getCurrentTheme());
                        changedOpacity = false;
                    }
                }
                auto* director = CCDirector::sharedDirector();
                const auto winSize = director->getWinSize();
                const auto frameSize = director->getOpenGLView()->getFrameSize();
                float width = (1000 * frameSize.width) / 1366;
                float height = (500 * frameSize.height) / 768;
                float tableWidth = (220 * frameSize.width) / 1366;

                auto currentLanguage = Lang::getLanguage();
                ImGui::SetNextWindowSize({ width, height });
                ImGui::Begin("Prism Menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
                //231,245 - 451,701
                RoundTableBG();
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
                    ///ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f, 13.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f, 10.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
                    // here we do a genius trick, please dont ask me how i did this! ok! this took forever to do
                    // chat jippity gave icon ideas, i designed the player!
                    // oh and PLEASE dont complain about it being misaligned in the center
                    float buttonWidth = (194 * frameSize.width) / 1366;
                    //float buttonHeight = (60 * frameSize.height) / 768;
                    float buttonHeight = (55 * frameSize.height) / 768;
                    CreateButton(currentLanguage->name("§ Global").c_str(), 0, buttonWidth, buttonHeight);
                    CreateButton(currentLanguage->name("¬ Player").c_str(), 1, buttonWidth, buttonHeight);
                    CreateButton(currentLanguage->name("ª Bypass").c_str(), 2, buttonWidth, buttonHeight);
                    CreateButton(currentLanguage->name("« Creator").c_str(), 3, buttonWidth, buttonHeight);
                    CreateButton(currentLanguage->name("¦ Quartz Bot").c_str(), 4, buttonWidth, buttonHeight);
                    CreateButton(currentLanguage->name("··· Misc").c_str(), 5, buttonWidth, buttonHeight);

                    /*ImGui::Spacing();
                    ImGui::Spacing();
                    ImGui::PopStyleVar(2);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);*/
                    // ad "UI Labels"
                    CreateButton(currentLanguage->name("¶ Settings").c_str(), 6, buttonWidth, buttonHeight);
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
                        case 4: // Bot
                            ImGui::Text("This is currently not supported on ImGui.\nPlease switch to the GD Menu Style if you wish to use this.");
                            break;
                        case 5: // Misc
                            jsonArray = matjson::parse(Hacks::getMiscHacks()).as_array();
                            break;
                        case 6: // Settings
                            jsonArray = matjson::parse(Hacks::getSettings()).as_array();
                            ImGui::Text("%s", currentLanguage->name("Prism Menu by Firee").c_str());
                            //const char* version = "V1.3.0 (Geode)";
                            #ifdef GITHUB_ACTIONS
                            auto version = fmt::format("{} (Geode)", Mod::get()->getVersion().toString());
                            #else 
                            auto version = fmt::format("{}-Dev (Geode)", Mod::get()->getVersion().toString());
                            #endif
                           #ifdef GEODE_IS_WINDOWS
                            ImGui::Text("%s - Windows", version.c_str());
                            #else // why does android not like elif
                            #ifdef GEODE_IS_ANDROID
                                ImGui::Text("%s - Android", version.c_str());
                            #else 
                                ImGui::Text("%s - HOW by Spu7nix", version.c_str());
                            #endif
                            #endif
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
                        HackItem* hack = Hacks::getHack(name);
                        if (hack == nullptr) continue;
                        auto opcodes = hack->opcodes;
                        if (hack != nullptr) {
                            if (obj.contains("win")) { // yeah idk what property okay
                                #ifndef GEODE_IS_WINDOWS
                                ImGui::BeginDisabled(true);
                                #endif
                            }
                            if (hack->value.type == ValueType::Int && hack->type != "dropdown") {
                                auto min = obj.get<int>("min");
                                auto max = obj.get<int>("max");
                                int step = (obj.contains("step")) ? obj.get<int>("step") : 5;
                                int oldValue = hack->value.intValue;
                                if (ImGui::InputInt(currentLanguage->name(name).c_str(), &hack->value.intValue, step, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {
                                    if (min > hack->value.intValue || hack->value.intValue > max) {
                                        hack->value.intValue = oldValue;
                                        return;
                                    }
                                    auto prismButton = CCScene::get()->getChildByID("prism-icon");
                                    if (name == "FPS Bypass") {
                                        // from mats fps unlocker
                                        //Hacks::Settings::setSettingValue(&settings, *hack, hack->value.floatValue);
                                        
                                        auto GM = GameManager::sharedState();
                                        auto app = CCApplication::sharedApplication();
                                        app->setAnimationInterval(1.0 / static_cast<double>(hack->value.intValue));
                                        // ^^^ is this even necessary
                                        GM->m_customFPSTarget = hack->value.intValue;
                                        GM->setGameVariable("0116", true);
                                        GM->updateCustomFPS();
                                    } else if (name == "Button Position X") {
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.intValue);
                                        if (prismButton != nullptr) prismButton->setPositionX(hack->value.intValue);
                                    } else if (name == "Button Position Y") {
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.intValue);
                                        if (prismButton != nullptr) prismButton->setPositionY(hack->value.intValue);
                                    } else if (name == "Menu-Style") {
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.intValue);
                                        static_cast<PrismButton*>(prismButton)->showImGuiMenu = false;
                                    } else {
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.intValue);
                                    }
                                }
                            } else if (hack->value.type == ValueType::Float) {
                                auto min = obj.get<float>("min");
                                auto max = obj.get<float>("max");
                                if (!editingMode.isEditing || editingMode.name != name.c_str()) {
                                    if (ImGui::SliderFloat(currentLanguage->name(name).c_str(), &hack->value.floatValue, min, max, "%.3f", ImGuiSliderFlags_NoInput)) {
                                        if (name == "Speedhack") {
                                            if (hack->value.floatValue < 0.0F) return;
                                            Hacks::setPitch(hack->value.floatValue);
                                        } else {
                                            if (name == "Menu Opacity") {
                                                changedOpacity = true;
                                            } else if (name == "Menu Scale") {
                                                changedMenuScale = true;
                                            }
                                            Hacks::Settings::setSettingValue(&settings, *hack, hack->value.floatValue);
                                        }
                                    }
                                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                                        editingMode.isEditing = true;
                                        editingMode.name = hack->name;
                                        editingMode.editedValue.floatValue = hack->value.floatValue;
                                    }
                                } else {
                                    ImGui::BeginDisabled();
                                    ImGui::SliderFloat(currentLanguage->name(name).c_str(), &editingMode.editedValue.floatValue, min, max);
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
                                        } else if (name == "Menu Opacity") {
                                            changedOpacity = true;
                                        } else if (name == "Menu Scale") {
                                            changedMenuScale = true;
                                        }
                                    }
                                    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
                                        editingMode.isEditing = false;
                                        editingMode.name = "";
                                    }
                                }
                            } else if (hack->value.type == ValueType::Bool) {
                                if ((Hacks::isHackEnabled("Enable Patching") && obj.contains("winOnly")) || !obj.contains("winOnly") || name == "Enable Patching") {
                                    if (ImGui::Checkbox(currentLanguage->name(name).c_str(), &hack->value.boolValue)) {
                                        Hacks::Settings::setSettingValue(&settings, *hack, hack->value.boolValue);
                                        if (name == "Show Button") {
                                            auto prismButton = CCScene::get()->getChildByID("prism-icon");
                                            if (prismButton != nullptr) prismButton->setVisible(hack->value.boolValue);
                                        }
                                        if (name == "Instant Complete" && hack->value.boolValue) {
                                            FLAlertLayer::create(nullptr, "Cheater!", "Just a warning, you will be <cr>banned off leaderboards</c> if you use this on rated levels. Consider this your <cy>warning</c>.", "OK", nullptr)->show();
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
                                if (ImGui::InputTextWithHint(currentLanguage->name(name).c_str(), "C", hack->value.charValue, 2, ImGuiInputTextFlags_EnterReturnsTrue)) {
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
                                    if (ImGui::Button(currentLanguage->name(name).c_str())) {
                                        // TODO: move this to hacks.cpp
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
                                        } else if (name == "Show Graphic Options") {
                                            #ifdef GEODE_IS_ANDROID 
                                            VideoOptionsLayer::create()->show();
                                            #else 
                                            FLAlertLayer::create("Error", "This option can only be used on <cy>Android</c>!", "OK")->show();
                                            #endif
                                        } else if (name == "Uncomplete Level") {
                                            if (auto levelInfoLayer = getChildOfType<LevelInfoLayer>(CCScene::get(), 0)) {
                                                Hacks::resetLevel(levelInfoLayer, levelInfoLayer->m_level);
                                            } else {
                                                FLAlertLayer::create("Error", "You are not <cy>currently in the level page</c>! Please enter in a level page in order to <cg>reset the stats</c>.", "OK")->show();
                                            }
                                        } else { // how will this happen
                                            FLAlertLayer::create("unimplemented", "this button is not implemented", "okay")->show();
                                        }
                                        ImGui::OpenPopup(name.c_str());

                                    }
                                    // Popups
                                    if (ImGui::BeginPopupModal("Credits", nullptr)) {
                                        auto creditLine1 = currentLanguage->name(currentLanguage->name(
"Thank you to Electrify (ES), Jouca (FR), dank_meme01, savvacorgi, МегаСвятой (RU), Gazonk (BR), huhnmitferrari (DE), and Ignis (CS) for translations!"
                                        ));
                                        auto creditLine2 = currentLanguage->name(currentLanguage->name("And thank you for using the mod! I hope you enjoy using Prism Menu!"));
                                        ImGui::Text("%s", creditLine1.c_str());
                                        ImGui::Separator();
                                        ImGui::Text("%s", creditLine2.c_str());
                                        if (ImGui::Button(currentLanguage->name(currentLanguage->name("Close Popup").c_str()).c_str()))
                                            ImGui::CloseCurrentPopup();
                                        ImGui::EndPopup();
                                    }

                                } else if (type == "dropdown") {
                                    auto values = obj.get<matjson::Array>("values");
                                    if (hack->name == "Theme") {
                                        values = Themes::getCurrentThemes();
                                    }
                                    int selectedIndex = hack->value.intValue;
                                    if (ImGui::BeginCombo(currentLanguage->name(name).c_str(), values[selectedIndex].as_string().c_str())) {
                                        for (size_t i = 0; i < values.size(); i++) {
                                            const bool isSelected = (selectedIndex == i);
                                            if (ImGui::Selectable(values[i].as_string().c_str(), isSelected)) {
                                                hack->value.intValue = i;
                                                Hacks::Settings::setSettingValue(&settings, *hack, hack->value.intValue);
                                                if (name == "Menu-Style") {
                                                    prismButton->showImGuiMenu = !prismButton->showImGuiMenu;
                                                }
                                            }
                                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                                            if (isSelected)
                                                ImGui::SetItemDefaultFocus();
                                        }
                                        ImGui::EndCombo();
                                    }
                                }
                            }
                            if (obj.contains("win")) { // yeah idk what property okay
                                #ifndef GEODE_IS_WINDOWS
                                ImGui::EndDisabled();
                                #endif
                            }
                            if (ImGui::IsItemHovered() && Hacks::isHackEnabled("Show Tooltips")) {
                                if ((obj.contains("winOnly") && Hacks::isHackEnabled("Enable Patching")) || !obj.contains("winOnly")) {
                                    ImGui::BeginTooltip();
                                    ImGui::Text("%s", currentLanguage->desc(name, desc).c_str());
                                    ImGui::EndTooltip();
                                }
                            }
                        }
                    }
                    if (ImGui::Button("X")) {
                        prismButton->showImGuiMenu = !prismButton->showImGuiMenu;
                    }
                    ImGui::PopStyleVar();
                    ImGui::EndTable();
                }
                if (ImGui::BeginPopupModal("Success", nullptr)) {
                    ImGui::Text("The custom theme has successfully been imported! Restart your game to use it.");
                    if (ImGui::Button(currentLanguage->name("Close Popup").c_str()))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
                ImGui::End();
            }
        });
        return true;
    }
};
#endif
