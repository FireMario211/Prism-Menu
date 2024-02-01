#pragma once
#include <Geode/Geode.hpp>
#include "hacks.hpp"

using namespace geode::prelude;

extern matjson::Array currentThemes;

class Themes {
    public:
    static void addToCurrentThemes();
    static matjson::Array getCurrentThemes();
    static void RGBAToCC(matjson::Value rgba, CCNodeRGBA* obj) {
        float currentOpacity = Hacks::getHack("Menu Opacity")->value.floatValue;
        rgba = rgba.as_array();
        float alpha = static_cast<float>(rgba[3].as_double());
        ccColor3B color;
        color.r = static_cast<float>(rgba[0].as_double());
        color.g = static_cast<float>(rgba[1].as_double());
        color.b = static_cast<float>(rgba[2].as_double());
        obj->setColor(color);
        obj->setOpacity((alpha == -1) ? currentOpacity * 255.0F : alpha);
    }
    static void RGBAToCC(matjson::Value rgba, CCLabelBMFont* obj) {
        float currentOpacity = Hacks::getHack("Menu Opacity")->value.floatValue;
        rgba = rgba.as_array();
        float alpha = static_cast<float>(rgba[3].as_double());
        ccColor3B color;
        color.r = static_cast<float>(rgba[0].as_double());
        color.g = static_cast<float>(rgba[1].as_double());
        color.b = static_cast<float>(rgba[2].as_double());
        obj->setColor(color);
        obj->setOpacity((alpha == -1) ? currentOpacity * 255.0F : alpha);
    }
    static matjson::Object getCurrentTheme();
};
