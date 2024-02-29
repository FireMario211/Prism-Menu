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

bool Hacks::isAutoSafeModeActive() {
    if (!Hacks::isHackEnabled("Auto Safe Mode")) return false;
    return Hacks::isCheating();
}

/*
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/GJGameLevel.hpp>
class $modify(GameStatsManager) {
    int getStat(const char* p0) {
        log::info("GameStatsManager::getStat({})", p0);
        return GameStatsManager::getStat(p0);
    }
    void setStat(const char* p0, int p1) {
        log::info("GameStatsManager::setStat({}, {})", p0, p1);
        return GameStatsManager::setStat(p0, p1);
    }
};

class $modify(GJGameLevel) {
    void savePercentage(int p0, bool p1, int p2, int p3, bool p4) {
        log::info("GJGameLevel::savePercentage({},{},{},{},{})", p0,p1,p2,p3,p4);
        return GJGameLevel::savePercentage(p0,p1,p2,p3,p4);
    }
    void saveNewScore(int p0, int p1) {
        log::info("GJGameLevel::saveNewScore({}, {})", p0, p1);
        return GJGameLevel::saveNewScore(p0,p1);
    }
};
*/

//GJGameLevel::savePercentage
////GJGameLevel::saveNewScore

// sorry, some people are rushing me to release this, youll get your chance Mac OS users.
#ifndef GEODE_IS_MACOS
void proceedWithReset(LevelInfoLayer* levelInfoLayer, GJGameLevel* level, bool resetStars, bool resetCoins) {
    geode::createQuickPopup(
            "Final Warning",
            fmt::format("Are you absolutely sure you want to <cr>reset stats</c> for <cy>{}</c>?\nYou <cr>cannot undo this</c> after it's done!\n\nPlease note that <cy>Uncomplete Level</c> is considered <cr>unstable</c>, meaning it may or may not work, or break something. Make sure to <cy>save backup</c> or save your local files before doing this. (you can blame the people who rushed me to work on this)", level->m_levelName),
            "Cancel", "I am sure.",
        [level, levelInfoLayer, resetStars, resetCoins](auto, bool btn2) {
            if (btn2) {
                auto GLM = GameLevelManager::sharedState();
                auto GSM = GameStatsManager::sharedState();
                /*CCArray* levelids = GSM->m_completedLevels->allKeys();
                CCObject* obj;
                CCARRAY_FOREACH(levelids, obj) {
                    std::string levelid = (static_cast<CCString*>(obj))->getCString();
                    std::cout << levelid << std::endl;
                }
                // wth is trueString
                std::cout << GSM->m_trueString->getCString() << std::endl;
                std::cout << GSM->m_completedLevels->count() << std::endl;*/
                if (level->m_normalPercent >= 100 && GSM->hasCompletedLevel(level)) {
                    int levelid = level->m_levelID;
                    GSM->setStat("4", GSM->getStat("4") - 1); // completed levels
                    GSM->m_completedLevels->removeObjectForKey(fmt::format("c_{}", levelid));
                    if (resetStars) {
                        //GSM->m_completedLevels->removeObjectForKey(fmt::format("unique_{}", levelid));
                        GSM->m_completedLevels->removeObjectForKey(fmt::format("star_{}", levelid));
                        GSM->m_completedLevels->removeObjectForKey(fmt::format("demon_{}", levelid));
                        if (level->isPlatformer()) {
                            GSM->setStat("28", GSM->getStat("28") - level->m_stars); // moons
                        } else {
                            GSM->setStat("6", GSM->getStat("6") - level->m_stars); // stars
                        }
                        if (level->m_demon > 0) {
                            GSM->setStat("5", GSM->getStat("5") - 1); // demons
                        }
                    }
                    if (resetCoins) {
                        GSM->setStat("8", GSM->getStat("8") - level->m_coinsVerified);
                    }
                }
                level->m_normalPercent = 0;
                level->m_newNormalPercent2 = 0;
                level->m_isChkValid = 0;
                level->m_chk = 0;
                level->m_coinsVerified = 0;
                GLM->deleteLevel(level);
                levelInfoLayer->onBack(nullptr);
                FLAlertLayer::create("Uncompleted Level", fmt::format("The level <cy>{}</c> ({}) has been reset!\nYou will need to download it again in order to play it.", level->m_levelName, level->m_levelID.value()), "OK")->show();
            }
        }
    );
    
}

#endif

void Hacks::resetLevel(LevelInfoLayer* levelInfoLayer, GJGameLevel* level) {
#ifdef GEODE_IS_MACOS 
    FLAlertLayer::create("Notice", "This currently does not work on <cy>Mac OS</c>\n(you can blame the people who rushed me to work on this)", "OK")->show();
#else 
    if (level->m_dailyID > 0) {
        FLAlertLayer::create("Notice", "This currently does not work on <cy>daily</c> or <cy>weekly</c> levels.", "OK")->show();
        return;
    }
    if (level->isPlatformer()) {
        FLAlertLayer::create("Notice", "This currently does not work on <cy>Platformer Levels</c>\nThis will be implemented in a future update.", "OK")->show();
        return;
    }
    // up to 3
    if (level->m_normalPercent < 100) {
        geode::createQuickPopup(
            "Warning",
            "You have not <cy>completed the level</c>.\nAre you sure you want to <cr>reset the stats</c>?",
            "No", "Yes",
            [level, levelInfoLayer](auto, bool btn2) {
                if (btn2) { // yes
                    proceedWithReset(levelInfoLayer, level, false, false);
                }
            }
        );
    } else {
        geode::createQuickPopup(
            "Confirm",
            fmt::format("Are you sure you want to <cr>reset the stats</c> for <cy>{}</c>?\nThis will clear <cg>normal percentages</c>, <cy>attempts</c>, <cy>jumps</c>, etc...", level->m_levelName),
            "No", "Yes",
            [level, levelInfoLayer](auto, bool btn2) {
                if (btn2) { // yes
                    if (level->m_stars > 0) { // assume rated
                         geode::createQuickPopup(
                            "Confirm",
                            "It appears that this level is <cy>rated!</c>\nWould you like to also reset <cy>stars / moons</c>, <cr>demon count</c>, and coins from this level?",
                            "Keep", "Remove",
                            [level, levelInfoLayer](auto, bool btn2) {
                                if (btn2) {
                                    proceedWithReset(levelInfoLayer, level, level->m_stars > 0, level->m_coinsVerified > 0);
                                } else {
                                    proceedWithReset(levelInfoLayer, level, false, false);
                                }
                            });
                    } else {
                        proceedWithReset(levelInfoLayer, level, false, false);
                    }
                }
            }
        );
    }
#endif 
}
