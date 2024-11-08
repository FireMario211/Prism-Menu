#pragma once
#include <unordered_map>
#include "hacks.hpp"
//#include <battery/embed.hpp>


class Lang {
    matjson::Array langFile;
    int langId = 0;
    public:
        static std::unique_ptr<Lang> getLanguage();
        static std::unique_ptr<Lang> get(int langId) {
            auto lang = std::make_unique<Lang>();
            if (langId > 0) {
                std::string file = "{}";
                switch (langId) {
                    case 1: // French
                        file = Hacks::readFile("french.json");
                        break;
                    case 2: // Portuguese
                        //file = b::embed<"resources/langs/russian.json">().str();
                        file = Hacks::readFile("portuguesebr.json");
                        break;
                    case 3: // German
                        file = Hacks::readFile("german.json");
                        break;
                    case 4: // Russian
                        file = Hacks::readFile("russian.json");
                        break;
                    case 5: // Czech
                        file = Hacks::readFile("czech.json");
                        break;
                    case 6: // Indonesian
                        file = Hacks::readFile("indonesian.json");
                        break;
                    case 7: // Spanish
                        file = Hacks::readFile("spanish.json");
                        break;
                    case 8: // Polish
                        file = Hacks::readFile("polish.json");
                        break;
                    case 9: // Malay
                        file = Hacks::readFile("malay.json");
                        break;
                    case 10: // Turkish
                        file = Hacks::readFile("turkish.json");
                        break;
                    case 11: // Vietnamese
                        file = Hacks::readFile("vietnamese.json");
                        break;
                    case 12: // Ukrainian
                        file = Hacks::readFile("ukrainian.json");
                        break;
                    case 13: // Italian
                        file = Hacks::readFile("italian.json");
                        break;
                    case 14: // Romanian
                        file = Hacks::readFile("romanian.json");
                        break;
                    case 15: // Japanese
                        file = Hacks::readFile("japanese.json");
                        break;
                    case 16: // Greek
                        file = Hacks::readFile("greek.json");
                        break;
                    default: // anything else should be discarded
                        langId = 0;
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
            if (this->langId > 50) this->langId = 0; // for some reason it goes to 72655368 which makes ZERO sense
            if (this->langId == 0) return key;
            auto obj = this->find(key);
            if (obj == nullptr) return key;
            return obj.get<std::string>("name");
        }
        std::string desc(std::string key, std::string original) {
            if (this->langId == 0) return original;
            auto obj = this->find(key);
            if (obj == nullptr) return key;
            return obj.get<std::string>("desc");
        }
        int getLangID() {
            return langId;
        }
};
