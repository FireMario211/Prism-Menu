#pragma once
#include <imgui-cocos.hpp>
#include <Geode/Geode.hpp>
#include "hacks.hpp"

static matjson::Array currentThemes;

class Themes {
    public:
    static void addToCurrentThemes() {
        HackItem* hack = Hacks::getHack("Theme");
        if (hack == nullptr) return;
        auto themeArray = hack->data.get<matjson::Array>("values");
        auto saveDir = Mod::get()->getSaveDir().string();
        auto array = hack->data.get<matjson::Array>("values");
        if (ghc::filesystem::exists(saveDir + "/themes")) {
            for (const auto & entry : std::filesystem::directory_iterator(saveDir + "/themes")) {
                if (entry.path().extension() == ".json") {
                    auto name = entry.path().filename().string();
                    name = name.substr(0, name.length() - 5); // remove the .json
                    array.push_back(name + " (Custom)");
                }
            }
        }
        currentThemes = array;
    }
    static matjson::Array getCurrentThemes() {
        return currentThemes;
    }
    static ImVec4 RGBAToImVec4(matjson::Value rgba) {
        float currentOpacity = Hacks::getHack("Menu Opacity")->value.floatValue;
        rgba = rgba.as_array();
        float alpha = static_cast<float>(rgba[3].as_double());
        return ImVec4(
            static_cast<float>(rgba[0].as_double()) / 255.0F,
            static_cast<float>(rgba[1].as_double()) / 255.0F,
            static_cast<float>(rgba[2].as_double()) / 255.0F,
            (alpha == -1) ? currentOpacity : alpha / 255.0F
        );
    }
    static ImU32 RGBAToImU32(matjson::Value rgba) {
        rgba = rgba.as_array();
        float alpha = static_cast<float>(rgba[3].as_double());
        return IM_COL32(
            static_cast<float>(rgba[0].as_double()),
            static_cast<float>(rgba[1].as_double()),
            static_cast<float>(rgba[2].as_double()),
            alpha
        );
    }
    static void UpdateOpacity(matjson::Object theme) {
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg] = RGBAToImVec4(theme["BG"]);
        style.Colors[ImGuiCol_ChildBg] = RGBAToImVec4(theme["BG"]);
        style.Colors[ImGuiCol_PopupBg] = RGBAToImVec4(theme["BG"]);
    }
    static matjson::Object getCurrentTheme() {
        auto themes = matjson::parse(Hacks::getThemes()).as_object();
        HackItem* hack = Hacks::getHack("Theme");
        if (currentThemes.empty()) return matjson::Object {};
        auto value = currentThemes[hack->value.intValue];
        if (!value.is_string()) return matjson::Object {};
        auto valueStr = value.as_string();
        if (valueStr.ends_with("(Custom)") && std::find(currentThemes.begin(), currentThemes.end(), valueStr) != currentThemes.end()) {
            // custom themes!
            auto saveDir = Mod::get()->getSaveDir().string();
            valueStr = valueStr.substr(0, valueStr.length() - 9); // remove the (Custom)
            auto themeData = Hacks::readFile(saveDir + "/themes/" + valueStr + ".json", true);
            if (themeData == "{}") return themes["Future Dark"].as_object();
            return matjson::parse(themeData).as_object();
        } else {
            if (!themes.contains(valueStr)) return themes["Future Dark"].as_object();
            return themes[valueStr].as_object();
        }
    }
    static void LoadTheme(matjson::Object theme) {
        // Future Dark style by rewrking from ImThemes
        ImGuiStyle& style = ImGui::GetStyle();
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig config;
        config.GlyphRanges = io.Fonts->GetGlyphRangesCyrillic();
        std::string fontName = (Mod::get()->getResourcesDir() / "PrismMenu.otf").string();
        //std::string fontName = (Mod::get()->getResourcesDir() / "Hack-Regular.ttf").string();
        float menuScale = Hacks::getHack("Menu Scale")->value.floatValue;
        io.Fonts->AddFontFromFileTTF(fontName.c_str(), theme["FontSize"].as_int() * menuScale, &config);
        io.Fonts->AddFontFromFileTTF(fontName.c_str(), theme["IconSize"].as_int() * menuScale, &config);
        
        style.Alpha = 1.0f;
        style.DisabledAlpha = 1.0f;
        style.WindowPadding = ImVec2(12.0f, 12.0f);
        style.WindowRounding = theme["WindowRounding"].as_int();
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
        //style.ScrollbarSize = (12.0f * menuScale);
        style.ScrollbarSize = (24.0f * menuScale);
        style.ScrollbarRounding = 20.0f;
        style.GrabMinSize = 12.0f;
        style.GrabRounding = 0.0f;
        style.TabRounding = 0.0f;
        style.TabBorderSize = 0.0f;
        style.TabMinWidthForCloseButton = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
        style.Colors[ImGuiCol_Text] = RGBAToImVec4(theme["Text"]);
        style.Colors[ImGuiCol_TextDisabled] = RGBAToImVec4(theme["TextDisabled"]);
        style.Colors[ImGuiCol_WindowBg] = RGBAToImVec4(theme["BG"]);
        style.Colors[ImGuiCol_ChildBg] = RGBAToImVec4(theme["BG"]);
        style.Colors[ImGuiCol_PopupBg] = RGBAToImVec4(theme["BG"]);
        style.Colors[ImGuiCol_Border] = RGBAToImVec4(theme["Border"]);
        style.Colors[ImGuiCol_BorderShadow] = RGBAToImVec4(theme["BorderShadow"]);
        style.Colors[ImGuiCol_FrameBg] = RGBAToImVec4(theme["FrameBg"]);
        style.Colors[ImGuiCol_FrameBgHovered] = RGBAToImVec4(theme["FrameBgHovered"]);
        style.Colors[ImGuiCol_FrameBgActive] = RGBAToImVec4(theme["FrameBgActive"]);
        style.Colors[ImGuiCol_TitleBg] = RGBAToImVec4(theme["TitleBg"]);
        style.Colors[ImGuiCol_TitleBgActive] = RGBAToImVec4(theme["TitleBgActive"]);
        style.Colors[ImGuiCol_TitleBgCollapsed] = RGBAToImVec4(theme["TitleBgCollapsed"]);
        style.Colors[ImGuiCol_MenuBarBg] = RGBAToImVec4(theme["MenuBarBg"]);
        style.Colors[ImGuiCol_ScrollbarBg] = RGBAToImVec4(theme["ScrollbarBg"]);
        style.Colors[ImGuiCol_ScrollbarGrab] = RGBAToImVec4(theme["ScrollbarGrab"]);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = RGBAToImVec4(theme["ScrollbarGrabHovered"]);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = RGBAToImVec4(theme["ScrollbarGrabActive"]);
        style.Colors[ImGuiCol_CheckMark] = RGBAToImVec4(theme["CheckMark"]);
        style.Colors[ImGuiCol_SliderGrab] = RGBAToImVec4(theme["SliderGrab"]);
        style.Colors[ImGuiCol_SliderGrabActive] = RGBAToImVec4(theme["SliderGrabActive"]);
        style.Colors[ImGuiCol_Button] = RGBAToImVec4(theme["Button"]);
        style.Colors[ImGuiCol_ButtonHovered] = RGBAToImVec4(theme["ButtonHovered"]);
        style.Colors[ImGuiCol_ButtonActive] = RGBAToImVec4(theme["ButtonActive"]);
        style.Colors[ImGuiCol_Header] = RGBAToImVec4(theme["Header"]);
        style.Colors[ImGuiCol_HeaderHovered] = RGBAToImVec4(theme["HeaderHovered"]);
        style.Colors[ImGuiCol_HeaderActive] = RGBAToImVec4(theme["HeaderActive"]);
        style.Colors[ImGuiCol_Separator] = RGBAToImVec4(theme["Separator"]);
        style.Colors[ImGuiCol_SeparatorHovered] = RGBAToImVec4(theme["SeparatorHovered"]);
        style.Colors[ImGuiCol_SeparatorActive] = RGBAToImVec4(theme["SeparatorActive"]);
        style.Colors[ImGuiCol_ResizeGrip] = RGBAToImVec4(theme["ResizeGrip"]);
        style.Colors[ImGuiCol_ResizeGripHovered] = RGBAToImVec4(theme["ResizeGripHovered"]);
        style.Colors[ImGuiCol_ResizeGripActive] = RGBAToImVec4(theme["ResizeGripActive"]);
        style.Colors[ImGuiCol_Tab] = RGBAToImVec4(theme["Tab"]);
        style.Colors[ImGuiCol_TabHovered] = RGBAToImVec4(theme["TabHovered"]);
        style.Colors[ImGuiCol_TabActive] = RGBAToImVec4(theme["TabActive"]);
        style.Colors[ImGuiCol_TabUnfocused] = RGBAToImVec4(theme["TabUnfocused"]);
        style.Colors[ImGuiCol_TabUnfocusedActive] = RGBAToImVec4(theme["TabUnfocusedActive"]);
        style.Colors[ImGuiCol_TableHeaderBg] = RGBAToImVec4(theme["TableHeaderBg"]);
        style.Colors[ImGuiCol_TableBorderStrong] = RGBAToImVec4(theme["TableBorderStrong"]);
        style.Colors[ImGuiCol_TableBorderLight] = RGBAToImVec4(theme["TableBorderLight"]);
        style.Colors[ImGuiCol_TableRowBg] = RGBAToImVec4(theme["TableRowBg"]);
        style.Colors[ImGuiCol_TableRowBgAlt] = RGBAToImVec4(theme["TableRowBgAlt"]);
        style.Colors[ImGuiCol_TextSelectedBg] = RGBAToImVec4(theme["TextSelectedBg"]);
        style.Colors[ImGuiCol_DragDropTarget] = RGBAToImVec4(theme["DragDropTarget"]);
        style.Colors[ImGuiCol_NavHighlight] = RGBAToImVec4(theme["NavHighlight"]);
        style.Colors[ImGuiCol_NavWindowingHighlight] = RGBAToImVec4(theme["NavWindowingHighlight"]);
        style.Colors[ImGuiCol_NavWindowingDimBg] = RGBAToImVec4(theme["NavWindowingDimBg"]);
        style.Colors[ImGuiCol_ModalWindowDimBg] = RGBAToImVec4(theme["ModalWindowDimBg"]);
        /*style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f);*/
    }
};
