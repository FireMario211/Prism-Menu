// ok what is this
#ifndef HACKS_HPP
#define HACKS_HPP

#include "CustomSettings.hpp"
#include "Utils.hpp"
#include <Geode/utils/general.hpp>
#include <Geode/Geode.hpp>
#include <algorithm>

#ifdef GEODE_IS_WINDOWS
#include <Geode/platform/windows.hpp>
//#include <battery/embed.hpp>
#endif

/*
{
    "name": "Hack", // Name of the Hack
    "desc": "Description", // Description (When you hover)
    "opcodes": [ // Bytes to patch
        {"addr": "0x0", "cocos": true, "on": "90", "off": "90"}
    ]
    "winOnly": true, // Whether or not the hack is considered Windows only.
    "type": "bool" // Bool, String
}
*/
using namespace geode::prelude;

// this was painful, and gave me depression as soon as i realized that i could just do hooks.
// also before you say "use resources", yeah i kinda realized that too late, maybe next update
// Credits to gdhm and megahack v5
enum class ValueType {
    Bool,
    Float,
    Int,
    Char,
    Custom
};

// Struct to hold the value, along with its type
struct HackValue {
    ValueType type;
    union {
        bool boolValue;
        float floatValue;
        int intValue;
        char* charValue;
        HackValue* customValue;
    };

    HackValue(bool value) : type(ValueType::Bool), boolValue(value) {}

    HackValue(float value) : type(ValueType::Float), floatValue(value) {}

    HackValue(int value) : type(ValueType::Int), intValue(value) {}

    HackValue(char* value) : type(ValueType::Char), charValue(value) {}

    HackValue(HackValue* value) : type(ValueType::Custom), customValue(value) {}
};



struct HackItem {
    bool winOnly = false;
    std::string name;
    std::string desc;
    std::string type;
    matjson::Array opcodes;
    HackValue value;
    matjson::Value data = {};
    bool focused = false;
};



extern std::vector<HackItem> allHacks; // this fixes everything.

class Hacks {
    public:
    // pleas dont judge me 
    static std::string readFile(const std::string& filename) {
        std::string filePath = Mod::get()->getResourcesDir().string() + "/" + filename;
        std::ifstream file(filePath);
        std::stringstream buffer;
        if (file) {
            buffer << file.rdbuf();
            return buffer.str();
        } else {
            return "{}";
        }
    }
    static std::string readFile(const std::string& filename, bool) {
        std::string filePath = filename;
        std::ifstream file(filePath);
        std::stringstream buffer;
        if (file) {
            buffer << file.rdbuf();
            return buffer.str();
        } else {
            return "{}";
        }
    }
    static std::vector<std::string> getCheats() {
        return {
            "Noclip",
            "Instant Complete",
            "No Spikes",
            "No Hitbox",
            "No Solids",
            "Freeze Player",
            "Jump Hack",
            "Layout Mode",
            "Show Hidden Objects",
            "Change Gravity",
            "Force Platformer Mode",
            "No Mirror Transition",
            "Instant Mirror Portal",
            "Show Hitboxes",
            "No Shaders",
            "Playback",
            "Disable Camera Effects"
        };
    }
    static bool isCheating();

    static bool isAutoSafeModeActive();

    static void resetLevel(LevelInfoLayer* levelInfoLayer, GJGameLevel* level);

    class Settings {
        public: 
            static bool settingContainsHack(const matjson::Array& objArr, const std::string& name);
            static matjson::Value getSettingValue(const matjson::Array& objArr, const std::string& name);
            static void setSettingValue(SettingHackStruct* hackValues, const HackItem& item, const matjson::Value& value);
    };
    
    static bool isHackEnabled(const std::string& name) {
        for (auto& hack : allHacks) {
            if (hack.name == name) {
                return hack.value.boolValue;
            }
        }
        return false;
    }
    // Function to get a checkbox by name
    static HackItem* getHack(const std::string& name) {
        for (auto& hack : allHacks) {
            if (hack.name == name) {
                return &hack;
            }
        }
        return nullptr; // Return nullptr if checkbox with the specified name is not found
    }
    // probably a bad idea
    static void processJSON(bool reset) {
        allHacks = {};
        auto hackValues = Mod::get()->getSavedValue<SettingHackStruct>("values");
        auto settings = hackValues.m_hackValues;
        
        for (size_t x = 0; x < 7; x++) {
            std::vector<matjson::Value> jsonArray;
            bool ignoreSave = false;
            switch (x) {
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
                    jsonArray = matjson::parse(Hacks::getBotHacks()).as_array();
                    break;
                case 5: // Misc
                    jsonArray = matjson::parse(Hacks::getMiscHacks()).as_array();
                    break;
                case 6: // Settings
                    jsonArray = matjson::parse(Hacks::getSettings()).as_array();
                    break;
            }
            for (size_t y = 0; y < jsonArray.size(); ++y) {
                const auto& obj = jsonArray[y];
                auto name = obj.get<std::string>("name");
                matjson::Array opcodes;
                if (obj.contains("platforms")) {
                    auto platforms = obj.get<matjson::Array>("platforms");
                    #ifdef GEODE_IS_WINDOWS 
                    if (!Utils::arrayContainsString(platforms, "win")) continue;
                    #endif
                    #ifdef GEODE_IS_MACOS
                    if (!Utils::arrayContainsString(platforms, "mac")) continue;
                    #endif
                    #ifdef GEODE_IS_ANDROID32
                    if (!Utils::arrayContainsString(platforms, "android32")) continue;
                    #endif
                    #ifdef GEODE_IS_ANDROID64
                    if (!Utils::arrayContainsString(platforms, "android64")) continue;
                    #endif
                    #ifdef GEODE_IS_IOS
                    if (!Utils::arrayContainsString(platforms, "ios")) continue;
                    #endif
                }
                if (obj.contains("opcodes")) {
                    auto opcodeObj = obj.get<matjson::Object>("opcodes");
                    #ifdef GEODE_IS_WINDOWS 
                    if (opcodeObj.contains("win")) opcodes = opcodeObj["win"].as_array();
                    #endif
                    #ifdef GEODE_IS_MACOS
                    if (opcodeObj.contains("mac")) opcodes = opcodeObj["mac"].as_array();
                    #endif
                    #ifdef GEODE_IS_ANDROID32
                    if (opcodeObj.contains("android32")) opcodes = opcodeObj["android32"].as_array();
                    #endif
                    #ifdef GEODE_IS_ANDROID64
                    if (opcodeObj.contains("android64")) opcodes = opcodeObj["android64"].as_array();
                    #endif
                    #ifdef GEODE_IS_IOS
                    if (opcodeObj.contains("ios")) opcodes = opcodeObj["ios"].as_array();
                    #endif
                }
                if (obj.contains("save") && obj.get<bool>("save") == false) {
                    ignoreSave = true;
                }
                if (!obj.contains("default")) {
                    /*allHacks.push_back({
                        false,
                        obj.contains("winOnly"),
                        obj.get<std::string>("name"),
                        obj.get<std::string>("desc"),
                        obj.get<std::string>("type"),
                        obj.get<matjson::Array>("opcodes"),
                        0.0F
                    });*/ 
                    HackItem item = {
                        obj.contains("winOnly"),
                        obj.get<std::string>("name"),
                        obj.get<std::string>("desc"),
                        obj.get<std::string>("type"),
                        opcodes,
                        false
                    };
                    item.data = obj;
                    if (!ignoreSave) {
                        if (Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                            item.value = Hacks::Settings::getSettingValue(settings, name).as_bool();
                        } else {
                            Hacks::Settings::setSettingValue(&hackValues, item, false);
                        }
                    }
                    allHacks.push_back(item);
                } else {
                    auto type = obj.get<std::string>("type");
                    bool settingExists = false;
                    HackValue value(new HackValue(false));
                    if (type == "bool") {
                        if (!ignoreSave && Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                            value = Hacks::Settings::getSettingValue(settings, name).as_bool();
                            settingExists = true;
                        } else {
                            value = HackValue(obj.get<bool>("default"));
                        }
                    } else if (type == "int" || type == "dropdown") {
                        if (!ignoreSave && Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                            value = Hacks::Settings::getSettingValue(settings, name).as_int();
                            settingExists = true;
                        } else {
                            value = HackValue(obj.get<int>("default"));
                        }
                    } else if (type == "float") {
                        if (!ignoreSave && Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                            value = static_cast<float>(Hacks::Settings::getSettingValue(settings, name).as_double());
                            settingExists = true;
                        } else {
                            value = HackValue(static_cast<float>(obj.get<double>("default")));
                        }
                    } else if (type == "string") {
                        if (!ignoreSave && Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                            std::string val = Hacks::Settings::getSettingValue(settings, name).as_string();
                            char* charVal = new char[val.length() + 1];
                            std::strcpy(charVal, val.c_str());
                            value = HackValue(charVal);
                            settingExists = true;
                        } else {
                            std::string val = obj.get<std::string>("default");
                            char* charVal = new char[val.length() + 1];
                            std::strcpy(charVal, val.c_str());
                            value = HackValue(charVal);
                        }
                    } else {
                        value = HackValue(new HackValue(false));
                    }
                    HackItem item = {
                        obj.contains("winOnly"),
                        obj.get<std::string>("name"),
                        obj.get<std::string>("desc"),
                        obj.get<std::string>("type"),
                        opcodes,
                        value
                    };
                    item.data = obj;
                    if (!settingExists && !ignoreSave) {
                        // i dont know any other way ok
                        if (type == "bool") {
                            Hacks::Settings::setSettingValue(&hackValues, item, item.value.boolValue);
                        } else if (type == "string") {
                            Hacks::Settings::setSettingValue(&hackValues, item, std::string(item.value.charValue));
                        } else if (type == "int" || type == "dropdown") {
                            Hacks::Settings::setSettingValue(&hackValues, item, item.value.intValue);
                        } else if (type == "float") {
                            Hacks::Settings::setSettingValue(&hackValues, item, item.value.floatValue);
                        }
                    }
                    allHacks.push_back(item);
                }
            }
        }
    }
    static void applyPatches(std::string name, matjson::Array const& opcodes, bool isEnabled) {
        // chat jippity
        for (const auto& opcode : opcodes) {
            std::string addrStr = opcode.get<std::string>("addr");
            std::string bytesStr = opcode.get<std::string>((isEnabled) ? "on" : "off");
            std::vector<uint8_t> bytes;
            std::istringstream hexStream(bytesStr);
            std::string byteStr;
            while (hexStream >> std::setw(2) >> byteStr) {
                bytes.push_back(static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16)));
            }

            // because stupid ownership!
            auto patches = Mod::get()->getPatches();
            auto patch = std::find_if(patches.begin(), patches.end(), [addrStr, opcode](Patch* const patch) {
                auto aBase = base::get();
                #ifdef GEODE_IS_WINDOWS
                if (opcode.contains("cocos")) aBase = base::getCocos();
                #endif
                return patch->getAddress() == aBase + std::stoul(addrStr, nullptr, 16);
            });
            if (patch != patches.end()) {
                auto res = (*patch)->updateBytes(bytes);
                if (res.has_error()) {
                    log::error("Something went wrong when trying to patch \"{}\" | {}", name, res.error());
                } else {
                    log::info("[{}] Patch {} with addr {}", ((isEnabled) ? "+" : "-"), name, addrStr);
                }
                return;
            }

            // ooo reinterpret_cast so bad!!!
            if (opcode.contains("cocos")) {
#ifdef GEODE_IS_WINDOWS // everything else is statically linked!
                auto res = Mod::get()->patch(
                    reinterpret_cast<void*>(base::getCocos() + std::stoul(addrStr, nullptr, 16)),
                    bytes
                );
                if (res.has_error()) {
                    log::error("Something went wrong when trying to patch \"{}\" | {}", name, res.error());
                } else {
                    res.unwrap()->setAutoEnable(false);
                    log::info("[{}] Patch {} with addr {}", ((isEnabled) ? "+" : "-"), name, addrStr);
                }
#endif
            } else {
                auto res = Mod::get()->patch(
                    reinterpret_cast<void*>(base::get() + std::stoul(addrStr, nullptr, 16)),
                    bytes
                );
                if (res.has_error()) {
                    log::error("Something went wrong when trying to patch \"{}\" | {}", name, res.error());
                } else {
                    res.unwrap()->setAutoEnable(false);
                    log::info("[{}] Patch {} with addr {}", ((isEnabled) ? "+" : "-"), name, addrStr);
                }
            }
        }
    }
    static std::string getGlobalHacks() {
        return readFile("global.json");
    }
    static std::string getPlayerHacks() {
        // Render Hitbox, Show Hitbox on Death, Jump Hack, Trail Always On, Trail Always Off, Inversed Trail, Hide Attempts, No Pulse, No Flash, Coins on Practice, Always Show Coins, Ignore ESC, No Particles, Instant Respawn, No Death Effect, No Respawn Blink, Suicide, Checkpoint Fix
        return readFile("player.json");
    }
    static std::string getBypassHacks() {
        return readFile("bypass.json");
    }
    static std::string getCreatorHacks() {
        return readFile("creator.json");
    }
    static std::string getMiscHacks() {
        return readFile("misc.json");
    }
    static std::string getBotHacks() {
        return readFile("bot.json");
    }
    static std::string getSettings() {
        return readFile("settings.json");
    }
    static std::string getThemes() {
        return readFile("themes.json");
    }
    // other hacks 
    static void setTPS(int tps);
    static void setTPS() {
        auto hack = Hacks::getHack("TPS Bypass");
        if (hack != nullptr) {
            setTPS(hack->value.intValue);
        } else {
            Hacks::setTPS(240);
        }
    }
    static void setPitch(float pitch) {
#if !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS) // TODO: figure out why it turns to creepypasta when tuning down speedhack
        if (!Hacks::isHackEnabled("Speedhack Audio")) {
            pitch = 1.0F;
        }
        auto fmod = FMODAudioEngine::sharedEngine();
        FMOD_RESULT result; // ensuring
        FMOD::ChannelGroup* master_group = nullptr;
        //FMOD::SoundGroup* sound_group = nullptr;
        result = fmod->m_system->getMasterChannelGroup(&master_group);
        if (result == FMOD_OK) {
            master_group->setPitch(pitch);
        }
        #ifdef GEODE_IS_WINDOWS // please commit support android fields, ADD THE FIELDS ON 2.205 PLS, SFX DOESNT WORK PROPERLY WITHOUT IT
        // robert decided to change it up to sfx, huh
        fmod->m_globalChannel->setPitch(pitch);
        #endif
        //fmod->pitchForIdx()
#endif
    }
};
#endif
