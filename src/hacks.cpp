#include "hacks.hpp"
#include "CustomSettings.hpp"

std::vector<HackItem> allHacks;

bool Hacks::Settings::settingContainsHack(const matjson::Array& objArr, const std::string& name) {
    for (const matjson::Value& obj : objArr) {
        if (obj.is_object()) {
            if (obj.get<std::string>("name") == name) {
                return true;
            }
        }
    }
    return false;
}

matjson::Value Hacks::Settings::getSettingValue(const matjson::Array& objArr, const std::string& name) {
    for (const matjson::Value& obj : objArr) {
        if (obj.is_object()) {
            if (obj.get<std::string>("name") == name) {
                return obj.get<matjson::Value>("value");
            }
        }
    }
    return NULL;
}

void Hacks::Settings::setSettingValue(SettingHackStruct* settings, const HackItem& item, const matjson::Value& value) {
    auto array = settings->m_hackValues;
    auto name = item.name;
    auto it = std::find_if(array.begin(), array.end(), [name](const matjson::Value& obj) {
        return obj.is_object() && obj.get<std::string>("name") == name;
    });
    if (it != array.end()) {
        (*it)["value"] = value;
    } else {
        auto newObj = matjson::Object();
        newObj["name"] = name;
        newObj["value"] = value;
        newObj["type"] = item.type;
        array.push_back(newObj);
    }
    settings->m_hackValues = array;
    Mod::get()->setSavedValue("values", *settings);
}

bool Hacks::isCheating() {
    auto cheats = Hacks::getCheats();
    if (Hacks::getHack("Speedhack")->value.floatValue != 1.0f) return true;
    bool cheating = false;
    for (const auto& cheat : cheats) {
        if (Hacks::isHackEnabled(cheat)) {
            cheating = true;
            break;
        }
    }
    return cheating;
}

// could instead just move this into the Hacks class but idk
bool Hacks::isAutoSafeModeActive() {
    if (!Hacks::isHackEnabled("Auto Safe Mode")) return false;
    return Hacks::isCheating();
}
