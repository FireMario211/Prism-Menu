// ok what is this
#ifndef HACKS_HPP
#define HACKS_HPP

#include "CustomSettings.hpp"
#include "Utils.hpp"
#include <Geode/utils/general.hpp>
#include <Geode/loader/Hook.hpp>
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
    Custom
};

// Struct to hold the value, along with its type
struct HackValue {
    ValueType type;
    union {
        bool boolValue;
        float floatValue;
        int intValue;
        HackValue* customValue;
    };

    HackValue(bool value) : type(ValueType::Bool), boolValue(value) {}

    HackValue(float value) : type(ValueType::Float), floatValue(value) {}

    HackValue(int value) : type(ValueType::Int), intValue(value) {}

    HackValue(HackValue* value) : type(ValueType::Custom), customValue(value) {}
};



struct HackItem {
    bool winOnly = false;
    std::string name;
    std::string desc;
    std::string type;
    std::vector<matjson::Value> opcodes;
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
    class Patching {
        public:
        // sorry dank idk a solution!
        static geode::Result<> withProtectedMemory(ptrdiff_t offset, const std::vector<uint8_t>& bytes) {
#ifdef GEODE_IS_WINDOWS
        DWORD oldProtect;
        void* address = reinterpret_cast<void*>(geode::base::get() + offset);

        if (VirtualProtect(address, bytes.size(), PAGE_READWRITE, &oldProtect)) {
            //callback(address);
            //THIS IS WHY WE CANT HAVE NICE THINGS, also im doing this because geodes patching is STUPID! keeps saying overlaps this overlaps that, and i cant even updateeBytes!
            if (!WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(geode::base::get() + offset), bytes.data(), bytes.size(), nullptr)) {
                return Err("Failed to write memory, aborting.");
            }
            VirtualProtect(address, bytes.size(), oldProtect, &oldProtect);
        } else {
            return Err("Failed to write memory.");
        }

        return Ok();
#endif
        return Err("withProtectedMemory unimplemented");
        }
    };
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
            "No Rotate",
            "Hide Player",
            "Freeze Player",
            "Jump Hack",
            "Layout Mode",
            "Show Hidden Objects",
            "Hide Level",
            "Change Gravity",
            "Force Platformer Mode",
            "No Mirror Transition",
            "Instant Mirror Portal",
            "Show Hitboxes",
            "No Shaders",
            "Playback",
            "Disable Camera Effects",
            "Auto Clicker"
        };
    }
    static bool isCheating();

    static bool isAutoSafeModeActive();

    static void resetLevel(LevelInfoLayer* levelInfoLayer, GJGameLevel* level);

    class Settings {
        public: 
            static bool settingContainsHack(const std::vector<matjson::Value>& objArr, const std::string& name);
            static matjson::Value getSettingValue(const std::vector<matjson::Value>& objArr, const std::string& name);
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
            bool ignoreSave = false;
            std::string hackStr;
            switch (x) {
                case 0: // Global
                    hackStr = Hacks::getGlobalHacks();
                    break;
                case 1: // Player 
                    hackStr = Hacks::getPlayerHacks();
                    break;
                case 2: // Bypass
                    hackStr = Hacks::getBypassHacks();
                    break;
                case 3: // Creator
                    hackStr = Hacks::getCreatorHacks();
                    break;
                case 4: // Bot
                    hackStr = Hacks::getBotHacks();
                    break;
                case 5: // Misc
                    hackStr = Hacks::getMiscHacks();
                    break;
                case 6: // Settings
                    hackStr = Hacks::getSettings();
                    break;
            }
            auto jsonArrayRes = matjson::parse(hackStr).unwrapOrDefault().asArray();
            if (jsonArrayRes.isOk()) {
                auto jsonArray = jsonArrayRes.unwrap();
                for (size_t y = 0; y < jsonArray.size(); ++y) {
                    const auto& obj = jsonArray[y];
                    auto h_name = obj.get("name");
                    auto h_desc = obj.get("desc");
                    auto h_type = obj.get("type");
                    if (h_name.isErr() || h_desc.isErr() || h_type.isErr()) continue;
                    auto name = h_name.unwrap().asString().unwrapOrDefault();
                    auto desc = h_desc.unwrap().asString().unwrapOrDefault();
                    auto type = h_type.unwrap().asString().unwrapOrDefault();
                    std::vector<matjson::Value> opcodes;
                    if (obj.contains("platforms")) {
                        auto platformsRes = obj.get("platforms");
                        if (platformsRes.isOk()) {
                            auto platforms = platformsRes.unwrap().asArray().unwrapOr(std::vector<matjson::Value> {});
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
                    }
                    if (obj.contains("opcodes")) {
                        auto opcodeRes = obj.get("opcodes");
                        if (opcodeRes.isOk()) {
                            auto opcodeObj = opcodeRes.unwrap();
                            #ifdef GEODE_IS_WINDOWS 
                            if (opcodeObj.contains("win")) opcodes = opcodeObj["win"].asArray().unwrap();
                            #endif
                            #ifdef GEODE_IS_MACOS
                            if (opcodeObj.contains("mac")) opcodes = opcodeObj["mac"].asArray().unwrap();
                            #endif
                            #ifdef GEODE_IS_ANDROID32
                            if (opcodeObj.contains("android32")) opcodes = opcodeObj["android32"].asArray().unwrap();
                            #endif
                            #ifdef GEODE_IS_ANDROID64
                            if (opcodeObj.contains("android64")) opcodes = opcodeObj["android64"].asArray().unwrap();
                            #endif
                            #ifdef GEODE_IS_IOS
                            if (opcodeObj.contains("ios")) opcodes = opcodeObj["ios"].asArray().unwrap();
                            #endif
                        }
                    }
                    if (obj.contains("save") && obj.get("save").unwrapOr(false).asBool().unwrapOrDefault() == false) {
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
                            name,
                            desc,
                            type,
                            opcodes,
                            false
                        };
                        item.data = obj;
                        if (!ignoreSave) {
                            if (Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                                item.value = Hacks::Settings::getSettingValue(settings, name).asBool().unwrapOrDefault();
                            } else {
                                Hacks::Settings::setSettingValue(&hackValues, item, false);
                            }
                        }
                        allHacks.push_back(item);
                    } else {
                        bool settingExists = false;
                        HackValue value(new HackValue(false));
                        if (type == "bool") {
                            if (!ignoreSave && Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                                value = Hacks::Settings::getSettingValue(settings, name).asBool().unwrapOrDefault();
                                settingExists = true;
                            } else {
                                value = HackValue(obj.get("default").unwrapOr(false).asBool().unwrapOrDefault());
                            }
                        } else if (type == "int" || type == "dropdown") {
                            if (!ignoreSave && Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                                value = (int)Hacks::Settings::getSettingValue(settings, name).asInt().unwrapOrDefault();
                                settingExists = true;
                            } else {
                                value = HackValue((int)obj.get("default").unwrapOr(0).asInt().unwrapOrDefault());
                            }
                        } else if (type == "float") {
                            if (!ignoreSave && Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                                value = static_cast<float>(Hacks::Settings::getSettingValue(settings, name).asDouble().unwrapOrDefault());
                                settingExists = true;
                            } else {
                                value = HackValue(static_cast<float>(obj.get("default").unwrapOr(0).asDouble().unwrapOrDefault()));
                            }
                        } else if (type == "char") { /* else if (type == "string") {
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
                        } */
                            //
                            //
                            if (!ignoreSave && Hacks::Settings::settingContainsHack(settings, name) && !reset) {
                                value = (int)Hacks::Settings::getSettingValue(settings, name).asInt().unwrapOrDefault();
                                settingExists = true;
                            } else {
                                value = HackValue(static_cast<int>(Utils::keyToEnumKey(obj.get("default").unwrapOr(0).asString().unwrapOrDefault())));
                            }
                        } else {
                            value = HackValue(new HackValue(false));
                        }
                        HackItem item = {
                            obj.contains("winOnly"),
                            name,
                            desc,
                            type,
                            opcodes,
                            value
                        };
                        item.data = obj;
                        if (!settingExists && !ignoreSave) {
                            // i dont know any other way ok
                            if (type == "bool") {
                                Hacks::Settings::setSettingValue(&hackValues, item, item.value.boolValue);
                            } else if (type == "string") {
                                //Hacks::Settings::setSettingValue(&hackValues, item, std::string(item.value.charValue));
                            } else if (type == "int" || type == "dropdown" || type == "char") {
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
    }
    static void applyPatches(std::string name, std::vector<matjson::Value> const& opcodes, bool isEnabled) {
        // chat jippity
        for (const auto& opcode : opcodes) {
            std::string addrStr = opcode.get("addr").unwrapOr("").asString().unwrapOrDefault();
            std::string bytesStr = opcode.get((isEnabled) ? "on" : "off").unwrapOr("").asString().unwrapOrDefault();
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
                if (res.isErr()) {
                    log::error("Something went wrong when trying to patch \"{}\" | {}", name, res.err());
                } else {
                    log::info("[Exists] [{}] Patch {} with addr {}", ((isEnabled) ? "+" : "-"), name, addrStr);
                }
                continue;
            }

            // ooo reinterpret_cast so bad!!!
            if (opcode.contains("cocos")) {
#ifdef GEODE_IS_WINDOWS // everything else is statically linked!
                auto res = Mod::get()->patch(
                    reinterpret_cast<void*>(base::getCocos() + std::stoul(addrStr, nullptr, 16)),
                    bytes
                );
                if (res.isErr()) {
                    log::error("Something went wrong when trying to patch \"{}\" | {}", name, res.err());
                } else {
                    res.unwrap()->setAutoEnable(false);
                    log::info("[{}] Patch {} with addr {}", ((isEnabled) ? "+" : "-"), name, addrStr);
                }
#endif
            } else {
#ifndef GEODE_IS_IOS
                auto res = Mod::get()->patch(
                    reinterpret_cast<void*>(base::get() + std::stoul(addrStr, nullptr, 16)),
                    bytes
                );
                if (res.isErr()) {
                    log::error("Something went wrong when trying to patch \"{}\" | {}", name, res.err());
                } else {
                    res.unwrap()->setAutoEnable(false);
                    log::info("[{}] Patch {} with addr {}", ((isEnabled) ? "+" : "-"), name, addrStr);
                }
#endif
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
    }
};
#endif
