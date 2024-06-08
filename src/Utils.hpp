#pragma once 

#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Utils {
    public:
    static std::string getNodeName(cocos2d::CCObject* node) {
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
    static std::string setPrecision(float value, int streamsize) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(streamsize) << value;
        return oss.str();
    }
    static bool arrayContainsString(const matjson::Array& arr, const std::string& str) {
        return std::any_of(arr.begin(), arr.end(), [&](const matjson::Value& val) {
            return val.as_string() == str;
        });
    }
    static float getSliderValue(float current, float min, float max, bool inverse) {
        return (inverse) ? (current * (max - min) + min) : (current - min) / (max - min);
    }
    static std::unordered_map<std::string, cocos2d::enumKeyCodes> stringKeyArray() {
        return {
            { "[Unknown]", cocos2d::KEY_Unknown },
            { "[Backspace]", cocos2d::KEY_Backspace },
            { "[Tab]", cocos2d::KEY_Tab },
            { "[Enter]", cocos2d::KEY_Enter },
            { "[Left Shift]", cocos2d::KEY_LeftShift }, // not present??
            { "[Right Shift]", cocos2d::KEY_RightShift }, // not present??
            { "[Left Ctrl]", cocos2d::KEY_LeftControl }, // not present??
            { "[Right Ctrl]", cocos2d::KEY_RightContol }, // not present??
            { "[Alt]", cocos2d::KEY_Alt },
            { "[Ctrl]", cocos2d::KEY_Control },
            { "[Shift]", cocos2d::KEY_Shift },
            { "[Pause]", cocos2d::KEY_Pause },
            { "[Caps Lock]", cocos2d::KEY_CapsLock },
            { "[Escape]", cocos2d::KEY_Escape },
            { "[Space]", cocos2d::KEY_Space },
            { "[PgUp]", cocos2d::KEY_PageUp },
            { "[PgDown]", cocos2d::KEY_PageDown },
            { "[End]", cocos2d::KEY_End },
            { "[Home]", cocos2d::KEY_Home },
            { "[Left Arrow]", cocos2d::KEY_Left },
            { "[Up Arrow]", cocos2d::KEY_Up },
            { "[Down Arrow]", cocos2d::KEY_Down },
            { "[Right Arrow]", cocos2d::KEY_Right },
            { "[Print Screen]", cocos2d::KEY_PrintScreen },
            { "[Insert]", cocos2d::KEY_Insert },
            { "[Delete]", cocos2d::KEY_Delete },
            { "[End]", cocos2d::KEY_End },
            { "[Scroll Lock]", cocos2d::KEY_ScrollLock },
            { "[Num Lock]", cocos2d::KEY_Numlock },
            { "0", cocos2d::KEY_Zero },
            { "1", cocos2d::KEY_One },
            { "2", cocos2d::KEY_Two },
            { "3", cocos2d::KEY_Three },
            { "4", cocos2d::KEY_Four },
            { "5", cocos2d::KEY_Five },
            { "6", cocos2d::KEY_Six },
            { "7", cocos2d::KEY_Seven },
            { "8", cocos2d::KEY_Eight },
            { "9", cocos2d::KEY_Nine },
            { "A", cocos2d::KEY_A },
            { "B", cocos2d::KEY_B },
            { "C", cocos2d::KEY_C },
            { "D", cocos2d::KEY_D },
            { "E", cocos2d::KEY_E },
            { "F", cocos2d::KEY_F },
            { "G", cocos2d::KEY_G },
            { "H", cocos2d::KEY_H },
            { "I", cocos2d::KEY_I },
            { "J", cocos2d::KEY_J },
            { "K", cocos2d::KEY_K },
            { "L", cocos2d::KEY_L },
            { "M", cocos2d::KEY_M },
            { "N", cocos2d::KEY_N },
            { "O", cocos2d::KEY_O },
            { "P", cocos2d::KEY_P },
            { "Q", cocos2d::KEY_Q },
            { "R", cocos2d::KEY_R },
            { "S", cocos2d::KEY_S },
            { "T", cocos2d::KEY_T },
            { "U", cocos2d::KEY_U },
            { "V", cocos2d::KEY_V },
            { "W", cocos2d::KEY_W },
            { "X", cocos2d::KEY_X },
            { "Y", cocos2d::KEY_Y },
            { "Z", cocos2d::KEY_Z },
            { "[NumPad0]", cocos2d::KEY_NumPad0 },
            { "[NumPad1]", cocos2d::KEY_NumPad1 },
            { "[NumPad2]", cocos2d::KEY_NumPad2 },
            { "[NumPad3]", cocos2d::KEY_NumPad3 },
            { "[NumPad4]", cocos2d::KEY_NumPad4 },
            { "[NumPad5]", cocos2d::KEY_NumPad5 },
            { "[NumPad6]", cocos2d::KEY_NumPad6 },
            { "[NumPad7]", cocos2d::KEY_NumPad7 },
            { "[NumPad8]", cocos2d::KEY_NumPad8 },
            { "[NumPad9]", cocos2d::KEY_NumPad9 },
            { "[F1]", cocos2d::KEY_F1 },
            { "[F2]", cocos2d::KEY_F2 },
            { "[F3]", cocos2d::KEY_F3 },
            { "[F4]", cocos2d::KEY_F4 },
            { "[F5]", cocos2d::KEY_F5 },
            { "[F6]", cocos2d::KEY_F6 },
            { "[F7]", cocos2d::KEY_F7 },
            { "[F8]", cocos2d::KEY_F8 },
            { "[F9]", cocos2d::KEY_F9 },
            { "[F10]", cocos2d::KEY_F10 },
            { "[F11]", cocos2d::KEY_F11 },
            { "[F12]", cocos2d::KEY_F12 },
            { ",", cocos2d::KEY_OEMComma },
            { ".", cocos2d::KEY_OEMPeriod },
            { "-", cocos2d::KEY_OEMMinus },
        };
    }
    static cocos2d::enumKeyCodes keyToEnumKey(std::string key) {
        std::unordered_map<std::string, cocos2d::enumKeyCodes> keymap = stringKeyArray();
        if (auto it = keymap.find(key); it != keymap.end()) {
            return it->second;
        } else {
            return cocos2d::KEY_None;
        }
    }
    static std::string enumKeyToString(cocos2d::enumKeyCodes key) {
        std::unordered_map<cocos2d::enumKeyCodes, std::string> keymap;
        
        // Populate the keymap with key-value pairs
        for (const auto& pair : stringKeyArray()) {
            keymap[pair.second] = pair.first;
        }
        if (auto it = keymap.find(key); it != keymap.end()) {
            return it->second;
        } else {
            return "[None]"; // or any other appropriate default value
        }
    }
    template<typename T>
    static std::vector<std::vector<T>> paginate(const std::vector<T>& arr, size_t size) {
        std::vector<std::vector<T>> paginated;
        for (size_t i = 0; i < arr.size(); i += size) {
            std::vector<T> page;
            for (size_t j = i; j < std::min(i + size, arr.size()); ++j) {
                page.push_back(arr[j]);
            }
            paginated.push_back(page);
        }
        return paginated;
    }


};
