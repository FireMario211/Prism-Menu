#pragma once
#include <Geode/Geode.hpp>
#ifndef NO_IMGUI
#include <imgui-cocos.hpp>
#endif
#include <Geode/ui/TextInput.hpp>
#include "Misc/Label.hpp"

using namespace geode::prelude;

extern std::vector<matjson::Value> currentThemes;

// as per request of acaruso, PLEASE DO NOT BE MAD AT ME ALK FOR USING NAMESPACE ok
namespace Themes {
    void addToCurrentThemes();
    std::vector<matjson::Value> getCurrentThemes();
    /*struct Color {
        unsigned char r, g, b, a;
        ccColor3B getColor() const {
            return ccc3(r, g, b);
        }
        unsigned char getOpacity() const {
            return a;
        }
    };*/
    #ifndef NO_IMGUI
    // TODO: reduce this
    ImVec4 RGBAToImVec4(matjson::Value rgba);
    ImU32 RGBAToImU32(matjson::Value rgba);
    void UpdateOpacity(matjson::Value theme);
    void LoadTheme(matjson::Value theme);
    #endif
    void RGBAToCC(matjson::Value rgba, CCNodeRGBA* obj);
    void RGBAToCC(matjson::Value rgba, CCLabelBMFont* obj);
    void RGBAToCC(matjson::Value rgba, Label* obj);
    matjson::Value getCurrentTheme();
};
