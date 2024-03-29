#pragma once
#include <Geode/Geode.hpp>
#ifndef NO_IMGUI
#include <imgui-cocos.hpp>
#endif
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

extern matjson::Array currentThemes;

// as per request of acaruso, PLEASE DO NOT BE MAD AT ME ALK FOR USING NAMESPACE ok
namespace Themes {
    void addToCurrentThemes();
    matjson::Array getCurrentThemes();
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
    void UpdateOpacity(matjson::Object theme);
    void LoadTheme(matjson::Object theme);
    #endif
    void RGBAToCC(matjson::Value rgba, CCNodeRGBA* obj);
    void RGBAToCC(matjson::Value rgba, CCLabelBMFont* obj);
    matjson::Object getCurrentTheme();
};
