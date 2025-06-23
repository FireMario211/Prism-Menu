#include "hacks.hpp"
#include "CustomSettings.hpp"
#include "UI/PrismUI.hpp"

std::vector<HackItem> allHacks;

bool Hacks::Settings::settingContainsHack(const std::vector<matjson::Value>& objArr, const std::string& name) {
    for (const matjson::Value& obj : objArr) {
        if (obj.isObject()) {
            if (obj.get("name").unwrapOr("").asString().unwrapOrDefault() == name) {
                return true;
            }
        }
    }
    return false;
}

matjson::Value Hacks::Settings::getSettingValue(const std::vector<matjson::Value>& objArr, const std::string& name) {
    for (const matjson::Value& obj : objArr) {
        if (obj.isObject()) {
            if (obj.get("name").unwrapOr("").asString().unwrapOrDefault() == name) {
                return obj.get("value").unwrapOr(0);
            }
        }
    }
    return NULL;
}

void Hacks::Settings::setSettingValue(SettingHackStruct* settings, const HackItem& item, const matjson::Value& value) {
    if (item.data.contains("save") && item.data.get("save").unwrap().asBool().unwrapOrDefault() == false) return;
    auto array = settings->m_hackValues;
    auto name = item.name;
    auto it = std::find_if(array.begin(), array.end(), [name](const matjson::Value& obj) {
        return obj.isObject() && obj.get("name").unwrapOr("").asString().unwrapOrDefault() == name;
    });
    if (it != array.end()) {
        (*it)["value"] = value;
    } else {
        matjson::Value newObj;
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
    //if (Hacks::getHack("TPS Bypass")->value.intValue > 360) return true; // demon list does not allow anything higher!
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
void proceedWithReset(LevelInfoLayer* levelInfoLayer, GJGameLevel* level, bool resetStars, bool resetCoins) {
    geode::createQuickPopup(
            "Final Warning",
            fmt::format("Are you absolutely sure you want to <cr>reset stats</c> for <cy>{}</c>?\nYou <cr>cannot undo this</c> after it's done!\n\nPlease note that <cy>Uncomplete Level</c> is considered <cr>unstable</c>, meaning it may or may not work, or break something. Make sure to <cy>save backup</c> or backup your local save files before doing this.", level->m_levelName),
            "I am sure.", "Cancel",
        [level, levelInfoLayer, resetStars, resetCoins](auto, bool btn2) {
            if (!btn2) {
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
                        GSM->m_completedLevels->removeObjectForKey(fmt::format("{}", GSM->getStarLevelKey(level))); // star_{}
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
                        auto coinDict = GSM->m_verifiedUserCoins;
                        for (auto i = 0; i < level->m_coins; i++) {
                            auto key = level->getCoinKey(i + 1);
                            if (GSM->hasUserCoin(key) || GSM->hasPendingUserCoin(key)) {
                                GSM->setStat("12", GSM->getStat("12") - 1);
                            }
                        }
                    }
                }
                level->m_normalPercent = 0;
                level->m_newNormalPercent2 = 0;
                level->m_bestTime = 0;
                level->m_bestPoints = 0;
                level->m_isChkValid = 0;
                level->m_chk = 0;
                level->m_coinsVerified = 0;
                GLM->deleteLevel(level);
                levelInfoLayer->onBack(nullptr);
                //FLAlertLayer::create("Uncompleted Level", fmt::format("The level <cy>{}</c> ({}) has been reset!\nYou will need to download it again in order to play it.", level->m_levelName, level->m_levelID.value()), "OK")->show();
            }
        }, true, true
    );
    
}

void Hacks::resetLevel(LevelInfoLayer* levelInfoLayer, GJGameLevel* level) {
    if (CCScene::get() == nullptr) return;
    auto prismUIExists = CCScene::get()->getChildByID("prism-menu");
    if (prismUIExists != nullptr) {
        static_cast<PrismUI*>(prismUIExists)->onClose(CCNode::create());
    }
    //FLAlertLayer::create("Notice", "This currently does not work on <cy>Mac OS</c> and <cy>iOS</c>", "OK")->show();
    if (level->m_dailyID > 0) {
        FLAlertLayer::create("Notice", "This currently does not work on <cy>daily</c> or <cy>weekly</c> levels.", "OK")->show();
        return;
    }
    if (level->isPlatformer()) {
        FLAlertLayer::create("Notice", "This currently does not work on <cy>Platformer Levels</c>\nThis will be implemented in a future update.", "OK")->show();
        return;
    }
    if (level->m_gauntletLevel || level->m_gauntletLevel2) {
        FLAlertLayer::create("Notice", "This currently does not work on <cy>Gauntlet Levels</c>\nThis will be implemented in a future update.", "OK")->show();
        return;
    }
    // up to 3
    if (level->m_normalPercent < 100) {
        geode::createQuickPopup(
            "Warning",
            "You have not <cy>completed the level</c>.\nAre you sure you want to <cr>reset the stats</c>?",
            "Yes", "No",
            [level, levelInfoLayer](auto, bool btn2) {
                if (!btn2) { // yes
                    proceedWithReset(levelInfoLayer, level, false, false);
                }
            }, true, true
        );
    } else {
        geode::createQuickPopup(
            "Confirm",
            fmt::format("Are you sure you want to <cr>reset the stats</c> for <cy>{}</c>?\nThis will clear <cg>normal percentages</c>, <cy>attempts</c>, <cy>jumps</c>, etc...", level->m_levelName),
            "Yes", "No",
            [level, levelInfoLayer](auto, bool btn2) {
                if (!btn2) { // yes
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
                            }, true, true);
                    } else {
                        proceedWithReset(levelInfoLayer, level, false, false);
                    }
                }
            }, true, true
        );
    }
}

/*
int global_timestamp = 0;

#include <Geode/modify/PlayLayer.hpp>
// src/Hacks/PhysicsBypass.cpp
class $modify(PlayLayer) {
    bool init(GJGameLevel *level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        global_timestamp = level->m_timestamp;
        return true;
    }
};
*/

float currentTPS = 240.F;
// because FOR SOME REASON PATCHING DOESNT WORK!
#ifndef GEODE_IS_MACOS
#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(PrismTPS, GJBaseGameLayer) {
    static void onModify(auto& self) {
        auto res = self.getHook("GJBaseGameLayer::getModifiedDelta");
        if (!res) {
            log::error("Something went horribly wrong while hooking GJBaseGameLayer::getModifiedDelta");
            return;
        }
        (void)res.unwrap()->disable();
        log::info("Disabled GJBaseGameLayer::getModifiedDelta");
    }
    float getModifiedDelta(float dt) {
        if (currentTPS == 240.F) return GJBaseGameLayer::getModifiedDelta(dt); // as a "just in case"
        auto tps = 1.f / currentTPS;
        double timer;
        if (m_resumeTimer < 1) {
            timer = (double)dt + m_extraDelta;
        } else {
            m_resumeTimer--;
            timer = m_extraDelta;
        }
        if (m_gameState.m_timeWarp < 1.0) {
            dt = (double)(m_gameState.m_timeWarp * tps);
        } else {
            dt = tps;
        }
        dt *= (double)(int)((double)(float)timer / dt);
        m_extraDelta = (double)(float)timer - dt;
        return (float)dt;
    }
};
#endif

bool hasChangedTPS = false;
void Hacks::setTPS(int tps) {
    // im actually too lazy to hook GJBaseGameLayer::update and do the calculations, ok!?
    if (tps != 240 && !hasChangedTPS) {
        hasChangedTPS = true;
    }
    if (!hasChangedTPS) return;
    currentTPS = tps;
#if 0
#if defined(GEODE_IS_WINDOWS) // why is windows the only one with 1 addr!?
    uintptr_t addr1 = 0x607008;
    uintptr_t addr2 = 0x0;
#elif defined(GEODE_IS_ANDROID32)
    uintptr_t addr1 = 0x4640fc; // float 
    uintptr_t addr2 = 0x4640f0; // double
#elif defined(GEODE_IS_ANDROID64)
    uintptr_t addr1 = 0x8384b8; // float
    uintptr_t addr2 = 0x8384c0; // double
#elif defined(GEODE_IS_INTEL_MAC)
    uintptr_t addr2 = 0x7e9c60; // double TODO
    uintptr_t addr1 = 0x7e9ac0; // float TODO
#elif defined(GEODE_IS_ARM_MAC)
    uintptr_t addr2 = 0x7e9c60; // double TODO
    uintptr_t addr1 = 0x7e9ac0; // float TODO
#elif defined(GEODE_IS_IOS)
    uintptr_t addr1 = 0x6426ec;
    uintptr_t addr2 = 0x642b60;
#endif
// sorry ios!
    auto patches = Mod::get()->getPatches();
    if (addr1 != 0x0) {
        float value = 1.0f / (float)tps;
        uint8_t bytes[sizeof(float)];
        std::memcpy(bytes, &value, sizeof(float));
        std::vector<uint8_t> bytesVec(bytes, bytes + sizeof(float));
        auto patch1 = std::find_if(patches.begin(), patches.end(), [addr1](Patch* const patch) {
            return patch->getAddress() == base::get() + addr1;
        });
        if (patch1 != patches.end()) {
            (void)(*patch1)->updateBytes(bytesVec);
        } else {
            auto res1 = Mod::get()->patch(reinterpret_cast<void*>(base::get() + addr1), bytesVec);
            if (res1.isErr()) {
                log::error("[1] Something went wrong when trying to patch TPS Bypass | {}", res1.err());
            }
        }
    }
    //
    
    if (addr2 != 0x0) {
        double value2 = 1.0f / (double)tps;
        uint8_t bytes2[sizeof(double)];
        std::memcpy(bytes2, &value2, sizeof(double));
        std::vector<uint8_t> bytesVec2(bytes2, bytes2 + sizeof(double));
        auto patch2 = std::find_if(patches.begin(), patches.end(), [addr2](Patch* const patch) {
            return patch->getAddress() == base::get() + addr2;
        });
        if (patch2 != patches.end()) {
            (void)(*patch2)->updateBytes(bytesVec2);
        } else {
            auto res2 = Mod::get()->patch(reinterpret_cast<void*>(base::get() + addr2), bytesVec2);
            if (res2.isErr()) {
                log::error("[2] Something went wrong when trying to patch TPS Bypass | {}", res2.err());
            }
        }
    }
#endif
    log::debug("Changed TPS to {}", tps);
    for (auto& hook : Mod::get()->getHooks()) {
        if (hook->getDisplayName() == "GJBaseGameLayer::getModifiedDelta") {
            if (tps == 240.F) {
                log::info("Disable GJBaseGameLayer::getModifiedDelta");
                (void)hook->disable();
            } else {
                log::info("Enable GJBaseGameLayer::getModifiedDelta");
                (void)hook->enable();
            }
            break;
        }
    }

    // oh and uhh, this is expanded from gdmo ok. please dont hurt me maxnut
    // src/Hacks/PhysicsBypass.cpp
    if (PlayLayer::get() != nullptr && PlayLayer::get()->m_level->m_timestamp > 0) {
        int global_timestamp = PlayLayer::get()->m_level->m_timestamp; // temp solution

        float timestampMultiplier = (tps / 240.f);
		float stepsMultiplier = (global_timestamp * timestampMultiplier) / PlayLayer::get()->m_level->m_timestamp;
		PlayLayer::get()->m_level->m_timestamp = global_timestamp * timestampMultiplier;
		PlayLayer::get()->m_gameState.m_currentProgress *= stepsMultiplier;
    }
}
