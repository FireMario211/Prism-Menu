// Creator Hacks 
// These can include simple things like making the Editor experience better, or allowing users to copy levels that weren't copyable because they aren't uploaded from 2.2 and the creator chose to not allow copies.
#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(EditLevelLayer) {
    // Verify Hack, No (C) Mark
    void onShare(CCObject* sender) {
        if (!Hacks::isHackEnabled("Verify Hack") && !Hacks::isHackEnabled("No (C) Mark")) return EditLevelLayer::onShare(sender);
        if (Hacks::isHackEnabled("Verify Hack")) {
            auto isVerified = this->m_level->m_isVerified;
            this->m_level->m_isVerified = true;
            //this->m_level->m_isVerified = isVerified;
        }
        if (Hacks::isHackEnabled("No (C) Mark")) {
            auto isVerified = this->m_level->m_originalLevel;
            this->m_level->m_originalLevel = true;
        }
        EditLevelLayer::onShare(sender);
    }
};

class $modify(LevelInfoLayer) {
    // Copy Hack
    bool init(GJGameLevel *p0, bool p1) {
        if (!LevelInfoLayer::init(p0, p1)) return false;
        if (Hacks::isHackEnabled("Copy Hack")) {
            auto gm = GameManager::sharedState();
            if (gm->m_playerUserID == p0->m_userID) return true;
            if (m_cloneBtn == nullptr) return true;
            auto aCloneBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png"), this, menu_selector(LevelInfoLayer::confirmClone));
            aCloneBtn->setPosition(m_cloneBtn->getPosition());
            m_cloneBtn->getParent()->addChild(aCloneBtn);
            m_cloneBtn->setVisible(false);
        }
        return true;
    }
};
