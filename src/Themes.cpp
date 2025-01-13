#include "Themes.hpp"
#include "hacks.hpp"

std::vector<matjson::Value> currentThemes;

void Themes::addToCurrentThemes() {
    HackItem* hack = Hacks::getHack("Theme");
    if (hack == nullptr) return;
    auto themeArrayRes = hack->data.get("values");
    std::vector<matjson::Value> emptyArray;
    if (themeArrayRes.isOk()) {
        auto themeArray = themeArrayRes.unwrap().asArray().unwrapOr(emptyArray);
        auto saveDir = Mod::get()->getSaveDir().string();
        auto arrayRes = hack->data.get("values");
        if (arrayRes.isOk()) {
            auto array = arrayRes.unwrap().asArray().unwrapOr(emptyArray);
            if (std::filesystem::exists(saveDir + "/themes")) {
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
    }
}

matjson::Value Themes::getCurrentTheme() {
    auto themes = matjson::parse(Hacks::getThemes()).unwrapOrDefault();
    HackItem* hack = Hacks::getHack("Theme");
    if (hack == nullptr) return themes["Future Dark"];
    if (currentThemes.empty()) return themes["Future Dark"];
    auto value = currentThemes[hack->value.intValue];
    if (!value.isString()) return themes["Future Dark"];
    auto valueStr = value.asString().unwrapOrDefault();
    if (valueStr.ends_with("(Custom)") && std::find(currentThemes.begin(), currentThemes.end(), valueStr) != currentThemes.end()) {
        // custom themes!
        auto saveDir = Mod::get()->getSaveDir().string();
        valueStr = valueStr.substr(0, valueStr.length() - 9); // remove the (Custom)
        auto themeDataStr = Hacks::readFile(saveDir + "/themes/" + valueStr + ".json", true);
        if (themeDataStr == "{}") return themes["Future Dark"];
        auto themeData = matjson::parse(themeDataStr).unwrapOrDefault();
        if (themeData["Version"] != "1.6.0") {
            themeData["InfoButton"] = themeData["ButtonActive"];
        }
        return themeData;
        //ButtonActive
    } else {
        if (!themes.contains(valueStr)) return themes["Future Dark"];
        return themes[valueStr];
    }
}

std::vector<matjson::Value> Themes::getCurrentThemes() {
    return currentThemes;
}

#ifndef NO_IMGUI

ImVec4 Themes::RGBAToImVec4(matjson::Value rgba) {
    float currentOpacity = Hacks::getHack("Menu Opacity")->value.floatValue;
    std::vector<matjson::Value> emptyArray;
    rgba = rgba.asArray().unwrapOr(emptyArray);
    float alpha = rgba[3].asDouble().unwrapOrDefault();
    return ImVec4(
        rgba[0].asDouble().unwrapOrDefault() / 255.0F,
        rgba[1].asDouble().unwrapOrDefault() / 255.0F,
        rgba[2].asDouble().unwrapOrDefault() / 255.0F,
        (alpha == -1) ? currentOpacity : alpha / 255.0F
    );
}
ImU32 Themes::RGBAToImU32(matjson::Value rgba) {
    std::vector<matjson::Value> emptyArray;
    rgba = rgba.asArray().unwrapOr(emptyArray);
    float alpha = rgba[3].asDouble().unwrapOrDefault();
    return IM_COL32(
        rgba[0].asDouble().unwrapOrDefault(),
        rgba[1].asDouble().unwrapOrDefault(),
        rgba[2].asDouble().unwrapOrDefault(),
        alpha
    );
}
void Themes::UpdateOpacity(matjson::Value theme) {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = RGBAToImVec4(theme["BG"]);
    style.Colors[ImGuiCol_ChildBg] = RGBAToImVec4(theme["BG"]);
    style.Colors[ImGuiCol_PopupBg] = RGBAToImVec4(theme["BG"]);
}


// other funcs
void Themes::LoadTheme(matjson::Value theme) {
    auto lang = Hacks::getHack("Language");
    if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Start");
    ImVector<ImWchar> ranges;
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    if (lang != nullptr && lang->value.intValue != 0) {
        // Future Dark style by rewrking from ImThemes
        ImFontGlyphRangesBuilder builder;

        // probably a bad idea to loop 127 times every frame, but this will only happen if the user has like "Live Theme Editing" enabled
        // Add Czech character ranges manually
        
        if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Setup Builder");

        for (int i = 0x0100; i <= 0x017F; ++i) { // Czech characters in Unicode range
            builder.AddChar(i);
        }

        if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Add Czech characters");

        builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic()); // russian chars

        if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Add Cyrillic characters");
        builder.AddRanges(io.Fonts->GetGlyphRangesVietnamese()); // vietnamese chars
        if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Add Vietnamese characters");
        //builder.AddRanges(io.Fonts->GetGlyphRangesJapanese()); // JP chars
        if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Add Japanese characters");
        builder.BuildRanges(&ranges);
        if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Build ranges");
    }
    std::string fontName = (Mod::get()->getResourcesDir() / "PrismMenu.otf").string();
    //std::string fontName = (Mod::get()->getResourcesDir() / "Hack-Regular.ttf").string();
    float menuScale = Hacks::getHack("Menu Scale")->value.floatValue;

    io.Fonts->AddFontFromFileTTF(fontName.c_str(), theme["FontSize"].asInt().unwrapOrDefault() * menuScale, NULL, ranges.Data);
    if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Add FontSize TTF");
    io.Fonts->AddFontFromFileTTF(fontName.c_str(), theme["IconSize"].asInt().unwrapOrDefault() * menuScale, NULL, ranges.Data);
    if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Add IconSize TTF");

    style.Alpha = 1.0f;
    style.DisabledAlpha = 1.0f;
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.WindowRounding = theme["WindowRounding"].asInt().unwrapOrDefault();
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
    style.Colors[ImGuiCol_ScrollbarBg] = RGBAToImVec4(theme["ScrollbarBg"]);
    style.Colors[ImGuiCol_ScrollbarGrab] = RGBAToImVec4(theme["ScrollbarGrab"]);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = RGBAToImVec4(theme["ScrollbarGrabHovered"]);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = RGBAToImVec4(theme["ScrollbarGrabActive"]);
    style.Colors[ImGuiCol_CheckMark] = RGBAToImVec4(theme["CheckMark"]);
    style.Colors[ImGuiCol_SliderGrab] = RGBAToImVec4(theme["SliderGrab"]);
    style.Colors[ImGuiCol_SliderGrabActive] = RGBAToImVec4(theme["SliderGrabActive"]);
    style.Colors[ImGuiCol_Button] = RGBAToImVec4(theme["Button"]);
    style.Colors[ImGuiCol_Header] = RGBAToImVec4(theme["ResizeGrip"]);
    style.Colors[ImGuiCol_ButtonHovered] = RGBAToImVec4(theme["ButtonHovered"]);
    style.Colors[ImGuiCol_ButtonActive] = RGBAToImVec4(theme["ButtonActive"]);
    style.Colors[ImGuiCol_Separator] = RGBAToImVec4(theme["Separator"]);
    style.Colors[ImGuiCol_SeparatorHovered] = RGBAToImVec4(theme["SeparatorHovered"]);
    style.Colors[ImGuiCol_SeparatorActive] = RGBAToImVec4(theme["SeparatorActive"]);
    style.Colors[ImGuiCol_ResizeGrip] = RGBAToImVec4(theme["ResizeGrip"]);
    style.Colors[ImGuiCol_ResizeGripHovered] = RGBAToImVec4(theme["ResizeGripHovered"]);
    style.Colors[ImGuiCol_ResizeGripActive] = RGBAToImVec4(theme["ResizeGripActive"]);
    style.Colors[ImGuiCol_TableBorderStrong] = RGBAToImVec4(theme["TableBorderStrong"]);
    style.Colors[ImGuiCol_TableBorderLight] = RGBAToImVec4(theme["TableBorderLight"]);
    style.Colors[ImGuiCol_TableRowBg] = RGBAToImVec4(theme["TableRowBg"]);
    style.Colors[ImGuiCol_TableRowBgAlt] = RGBAToImVec4(theme["TableRowBgAlt"]);
    style.Colors[ImGuiCol_TextSelectedBg] = RGBAToImVec4(theme["TextSelectedBg"]);
    style.Colors[ImGuiCol_ModalWindowDimBg] = RGBAToImVec4(matjson::parse("[73, 77, 100, 128]").unwrapOrDefault());
    /*
            "Text": [255, 255, 255, 255],
    "TextDisabled": [70, 81, 115, 255],
    "BG": [20, 22, 26, -1],
    "Border": [40, 43, 49, 255],
    "BorderShadow": [20, 22, 26, 255],
    "ScrollbarBg": [12, 14, 18, 255],
    "ScrollbarGrab": [30, 34, 38, 255],
    "ScrollbarGrabHovered": [25, 43, 49, 255],
    "ScrollbarGrabActive": [30, 34, 38, 255],
    "CheckMark": [127, 131, 255, 255],
    "SliderGrab": [127, 131, 255, 255],
    "SliderGrabActive": [137, 141, 255, 255],
    "Button": [30, 34, 38, 255],
    "ButtonHovered": [50, 45, 139, 255],
    "ButtonActive": [60, 55, 152, 255],
    "Separator": [25, 47, 64, 255],
    "SeparatorHovered": [25, 47, 64, 255],
    "SeparatorActive": [25, 47, 64, 255],
    "ResizeGrip": [30, 34, 38, 255],
    "ResizeGripHovered": [50, 45, 139, 255],
    "ResizeGripActive": [60, 55, 152, 255],
    "TableRowBg": [30, 34, 38, 128],
    "TableRowBgAlt": [25, 27, 31, 128],
    "TextSelectedBg": [60, 55, 152, 255]
    */
    if (!Hacks::isHackEnabled("Live Theme Editing")) log::debug("Themes::LoadTheme - Loaded Theme!");
}
#endif

void Themes::RGBAToCC(matjson::Value rgba, CCNodeRGBA* obj) {
    float currentOpacity = Hacks::getHack("Menu Opacity")->value.floatValue;
    std::vector<matjson::Value> emptyArray;
    rgba = rgba.asArray().unwrapOr(emptyArray);
    if (rgba.size() != 4) return;
    float alpha = rgba[3].asDouble().unwrapOrDefault();
    ccColor3B color;
    color.r = rgba[0].asDouble().unwrapOrDefault();
    color.g = rgba[1].asDouble().unwrapOrDefault();
    color.b = rgba[2].asDouble().unwrapOrDefault();
    obj->setColor(color);
    obj->setOpacity((alpha == -1) ? currentOpacity * 255.0F : alpha);
}
void Themes::RGBAToCC(matjson::Value rgba, CCLabelBMFont* obj) {
    float currentOpacity = Hacks::getHack("Menu Opacity")->value.floatValue;
    std::vector<matjson::Value> emptyArray;
    rgba = rgba.asArray().unwrapOr(emptyArray);
    if (rgba.size() != 4) return;
    float alpha = rgba[3].asDouble().unwrapOrDefault();
    ccColor3B color;
    color.r = rgba[0].asDouble().unwrapOrDefault();
    color.g = rgba[1].asDouble().unwrapOrDefault();
    color.b = rgba[2].asDouble().unwrapOrDefault();
    obj->setColor(color);
    obj->setOpacity((alpha == -1) ? currentOpacity * 255.0F : alpha);
}
void Themes::RGBAToCC(matjson::Value rgba, Label* obj) {
    float currentOpacity = Hacks::getHack("Menu Opacity")->value.floatValue;
    std::vector<matjson::Value> emptyArray;
    rgba = rgba.asArray().unwrapOr(emptyArray);
    if (rgba.size() != 4) return;
    float alpha = rgba[3].asDouble().unwrapOrDefault();
    ccColor3B color;
    color.r = rgba[0].asDouble().unwrapOrDefault();
    color.g = rgba[1].asDouble().unwrapOrDefault();
    color.b = rgba[2].asDouble().unwrapOrDefault();
    obj->setColor(color);
    obj->setOpacity((alpha == -1) ? currentOpacity * 255.0F : alpha);
}
