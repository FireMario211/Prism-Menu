#pragma once
#include <unordered_map>
#include "hacks.hpp"
//#include <battery/embed.hpp>
class Lang {
    matjson::Array langFile;
    int langId = 0;
    public:
        static Lang* get(int langId) {
            Lang* lang = new Lang();
            // 0 = English
            // 1 = Spanish
            // 2 = Russian
            if (langId > 0) {
                std::string file = "{}";
                switch (langId) {
                    case 1: // Spanish
                        file = Hacks::readFile("spanish.json");
                        break;
                    case 2: // Russian
                        //file = b::embed<"resources/langs/russian.json">().str();
                        break;
                }
                lang->langFile = matjson::parse(file).as_array();
                lang->langId = langId;
            }
            return lang;
        }
        matjson::Value find(std::string key) {
            for (size_t i = 0; i < langFile.size(); ++i) {
                if (langFile[i].contains(key)) {
                    return langFile[i].get<matjson::Value>(key.c_str());
                }
            }
            return nullptr;
        }
        std::string name(std::string key) {
            if (this->langId == 0) return key;
            auto obj = this->find(key);
            if (obj == nullptr) return key;
            return obj.get<std::string>("name");
        }
        std::string desc(std::string key) {
            if (this->langId == 0) return key;
            auto obj = this->find(key);
            if (obj == nullptr) return key;
            return obj.get<std::string>("desc");
        }
        /*
        static std::string toLocaleName(std::string key, int langIndex) {
            if (langIndex == 0) return key;
            // this is stupid
            std::unordered_map<int, std::unordered_map<std::string, std::string>> translations = {
                {1, { // Spanish
                        {"Anticheat Bypass", "Anticheat Bypass"},
                        {"Practice Music", "Práctica de Música"},
                        {"Safe Mode", "Modo Seguro"},
                        {"Show tooltips", "Mostrar sugerencias"},
                        {"Language", "Idioma"},
                        {"Restore Defaults", "Restablecer la configuración predeterminada"}
                    }
                },
            };
            // Attempt to find the translation for the given key and language index
            auto it = translations[langIndex].find(key);

            // If the translation for the specified language index is found
            if (it != translations[langIndex].end()) {
                // Check for the key in the translation
                return it->second;
            } else {
      return key;
            }
            return key;
        }
        static std::string toLocaleDesc(std::string key, int langIndex) {
            if (langIndex == 0) return key;
            // this is stupid
            std::unordered_map<int, std::unordered_map<std::string, std::string>> translations = {
                {1, { // Spanish
                        {"Disables all anticheats. [Levels, Editor, etc]", "Disables all anticheats. [Levels, Editor, etc]"},
                        {"Allows the level's music to be played when in Practice Mode.","Allows the level's music to be played when in Practice Mode."},
                        {"Pauses any progress from being done on the level.","Pauses any progress from being done on the level."},
                        {"Allows for decimals in the level percentage.","Allows for decimals in the level percentage."},
                        {"Allows for decimals in the level percentage. [Beyond thousandths place]","Allows for decimals in the level percentage. [Beyond thousandths place]"},
                        {"Removes all decoration associated with a level.","Removes all decoration associated with a level."},
                        {"Shorterns scene transition time to 0s.","Shorterns scene transition time to 0s."},
                        {"Changes the speed of the game and audio.","Changes the speed of the game and audio."},
                        {"Unlocks your FPS to be higher than 60 or your monitor's refresh rate.","Unlocks your FPS to be higher than 60 or your monitor's refresh rate."},
                        {"Makes the player invincible.","Makes the player invincible."},
                        {"Ignores spike objects.","Ignores spike objects."},
                        {"Disables all object hitboxes. Special objects are disabled.","Disables all object hitboxes. Special objects are disabled."},
                        {"Disables most solid hitboxes. Special objects still work.","Disables most solid hitboxes. Special objects still work."},
                        {"Owie.","Owie."},
                        {"Prevents the screen from being flipped upon a mirror portal.","Prevents the screen from being flipped upon a mirror portal."},
                        {"Reverse portal now instantly switches.","Reverse portal now instantly switches."},
                        {"Freezes player movement.","Freezes player movement."},
                        {"Allows for jumping in mid-air.","Allows for jumping in mid-air."},
                        {"Unlocks all icons.","Unlocks all icons."},
                        {"Allows for unlimited text length in text inputs.","Allows for unlimited text length in text inputs."},
                        {"Lets you input any character in all text inputs.","Lets you input any character in all text inputs."},
                        {"Lets sliders be dragged beyond the visible limit.","Lets sliders be dragged beyond the visible limit."},
                        {"Unlocks locked demon levels.","Unlocks locked demon levels."},
                        {"Unlocks the guard's vault.","Unlocks the guard's vault."},
                        {"Unlocks the keymaster's vault.","Unlocks the keymaster's vault."},
                        {"Unlocks the keymaster's basement.","Unlocks the keymaster's basement."},
                        {"Whether or not tooltips should be shown when hovering over mods.", "Determina si se deben mostrar o no sugerencias al pasar el cursor sobre los mods."},
                        {"Sets the language for Prism Menu!", "¡Establece el idioma para Menú de Prism!"},
                        {"Disables all mods and resets everything to default.", "Desactiva todos los mods y restablece todo a los valores predeterminados."}

                    }
                },
            };
            // Attempt to find the translation for the given key and language index
            auto it = translations[langIndex].find(key);

            // If the translation for the specified language index is found
            if (it != translations[langIndex].end()) {
                // Check for the key in the translation
                return it->second;
            } else {
                return key;
            }
            return key;
        }*/
};
