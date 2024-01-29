#include "Themes.hpp"

matjson::Array currentThemes;

void Themes::addToCurrentThemes() {
    HackItem* hack = Hacks::getHack("Theme");
    if (hack == nullptr) return;
    auto themeArray = hack->data.get<matjson::Array>("values");
    auto saveDir = Mod::get()->getSaveDir().string();
    auto array = hack->data.get<matjson::Array>("values");
    if (ghc::filesystem::exists(saveDir + "/themes")) {
        for (const auto & entry : std::filesystem::directory_iterator(saveDir + "/themes")) {
            if (entry.path().extension() == ".json") {
                auto name = entry.path().filename().string();
                name = name.substr(0, name.length() - 5); // remove the .json
                array.push_back(name + " (Custom)");
            }
        }
    }
    currentThemes = array;
}

matjson::Object Themes::getCurrentTheme() {
    auto themes = matjson::parse(Hacks::getThemes()).as_object();
    HackItem* hack = Hacks::getHack("Theme");
    if (hack == nullptr) return themes["Future Dark"].as_object();
    if (currentThemes.empty()) return themes["Future Dark"].as_object();
    auto value = currentThemes[hack->value.intValue];
    if (!value.is_string()) return themes["Future Dark"].as_object();
    auto valueStr = value.as_string();
    if (valueStr.ends_with("(Custom)") && std::find(currentThemes.begin(), currentThemes.end(), valueStr) != currentThemes.end()) {
        // custom themes!
        auto saveDir = Mod::get()->getSaveDir().string();
        valueStr = valueStr.substr(0, valueStr.length() - 9); // remove the (Custom)
        auto themeData = Hacks::readFile(saveDir + "/themes/" + valueStr + ".json", true);
        if (themeData == "{}") return themes["Future Dark"].as_object();
        return matjson::parse(themeData).as_object();
    } else {
        if (!themes.contains(valueStr)) return themes["Future Dark"].as_object();
        return themes[valueStr].as_object();
    }
}

matjson::Array Themes::getCurrentThemes() {
    return currentThemes;
}
