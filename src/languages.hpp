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
                    case 1: // French
                        file = Hacks::readFile("french.json");
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
};
