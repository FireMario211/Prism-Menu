#include "../hacks.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <Geode/modify/ShaderLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCDrawNode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

// no create method?????????????
class $modify(ShaderMiscHacks, ShaderLayer) {
    // No Shaders
    void performCalculations() {
        if (!Hacks::isHackEnabled("No Shaders")) return ShaderLayer::performCalculations();
    }
};

std::vector<std::pair<CCRect, CCRect>> playerHitboxTrail;

class $modify(GJBGLMiscHacks, GJBaseGameLayer) {
    void processCommands(float dt) {
        GJBaseGameLayer::processCommands(dt);
        if (Hacks::isHackEnabled("Show Hitbox Trail")) {
            playerHitboxTrail.push_back(
                std::make_pair(m_player1->getObjectRect(), m_player1->getObjectRect(0.25F, 0.25F))
            );
            // definitely not going to cause problems later!
            while (playerHitboxTrail.size() > ((Hacks::getHack("Hitbox Trail Length") != nullptr) ? Hacks::getHack("Hitbox Trail Length")->value.intValue : 1) && !playerHitboxTrail.empty()) {
                playerHitboxTrail.erase(playerHitboxTrail.begin());
            }
        }
    }
    void update(float dt) {
        GJBaseGameLayer::update(dt);
        if (Hacks::isHackEnabled("Low FPS Pause") && dt > 0) {
            if (auto playLayer = PlayLayer::get()) {
                int value = (Hacks::getHack("Pause below FPS") != nullptr) ? Hacks::getHack("Pause below FPS")->value.intValue : 1;
                // probably a better way to do this
                float fpsFloat = (1.0F / dt);
                auto fps = static_cast<int>(fpsFloat);
                if (fps <= value) {
                    log::info("Automatically paused at {} FPS. (Minimum: {})", fps, value);
                    playLayer->pauseGame(true);
                }
            }
        }
    }
};

inline void showLevelEditorHitboxes(LevelEditorLayer* ptr, bool mode) {
    GameManager::get()->setGameVariable("0045", mode);
    ptr->m_isDebugDrawEnabled = mode;
}

void drawPlayerHitboxes(CCDrawNode* drawNode, std::pair<CCRect, CCRect> rect, bool circle) {
    auto innerRect = rect.second;
    drawNode->drawRect(
            CCPoint(innerRect.getMinX(), innerRect.getMinY()),
            CCPoint(innerRect.getMaxX(), innerRect.getMaxY()),
            {0.0f, 0.0f, 0.0f, 0.0f},
            0.25f, {0.0f, 0.0f, 1.0f, 1.0f}
    );
    auto outerRect = rect.first;
    drawNode->drawRect(
            CCPoint(outerRect.getMinX(), outerRect.getMinY()),
            CCPoint(outerRect.getMaxX(), outerRect.getMaxY()),
            {0.0f, 0.0f, 0.0f, 0.0f},
            0.25f, {1.0f, 0.0f, 0.0f, 1.0f}
    );
    if (circle) {
        drawNode->drawCircle(
                CCPoint(outerRect.getMidX(), outerRect.getMidY()),
                outerRect.getMaxX() - outerRect.getMidX(),
                {0.0f, 0.0f, 0.0f, 0.0f},
                0.25f, {1.0f, 0.2f, 0.2f, 1.0f},
                1000
        );
    }
}

class $modify(LevelEditorLayer) {
    struct Fields {
        bool lastShowHitboxes = false;
    };
    void updateVisibility(float p0) {
        LevelEditorLayer::updateVisibility(p0);
        bool showHitboxes = Hacks::isHackEnabled("Show Hitboxes");
        if (showHitboxes) {
            if (!m_isDebugDrawEnabled) showLevelEditorHitboxes(this, true);
            drawPlayerHitboxes(m_debugDrawNode, std::make_pair(m_player1->getObjectRect(), m_player1->getObjectRect(0.25f, 0.25f)), true);
            if (Hacks::isHackEnabled("Show Hitbox Trail")) {
                for (auto trail : playerHitboxTrail) {
                    drawPlayerHitboxes(m_debugDrawNode, trail, false);
                }
            }
        }
        else if (showHitboxes != m_fields->lastShowHitboxes)
            showLevelEditorHitboxes(this, false);
        m_fields->lastShowHitboxes = showHitboxes;
    }
};

class $modify(PLMiscHacks, PlayLayer) {
    void updateVisibility(float p0) {
        //log::info("{} or {}", sizeof(m_debugDrawNode), sizeof(*m_debugDrawNode));
        //log::info("and {:#x}", offsetof(GJBaseGameLayer, m_debugDrawNode));
        /*if (Hacks::isHackEnabled("No Pulse")) {
            FMODAudioEngine::sharedEngine()->updateMetering();
        }*/
        PlayLayer::updateVisibility(p0);
        if (Hacks::isHackEnabled("Show Hitboxes") || (Hacks::isHackEnabled("Show Hitboxes on Death") && m_player1->m_isDead)) {
            if (!m_debugDrawNode->isVisible()) m_debugDrawNode->setVisible(true);
            PlayLayer::updateDebugDraw();
            drawPlayerHitboxes(m_debugDrawNode, std::make_pair(m_player1->getObjectRect(), m_player1->getObjectRect(0.25f, 0.25f)), true);
            if (Hacks::isHackEnabled("Show Hitbox Trail")) {
                for (auto trail : playerHitboxTrail) {
                    drawPlayerHitboxes(m_debugDrawNode, trail, false);
                }
            }
        }
        else if (m_debugDrawNode->isVisible() && !(m_isPracticeMode && m_isDebugDrawEnabled))
            m_debugDrawNode->setVisible(false);
    }
    void resetLevel() {
        PlayLayer::resetLevel();
        playerHitboxTrail.clear();
    }
    void postUpdate(float dt) {
        if (Hacks::isHackEnabled("Disable Camera Effects")) PlayLayer::resetCamera();
        /*char a[offsetof(GJBaseGameLayer, m_debugDrawNode)];
        a = 1;
        char b[offsetof(GJBaseGameLayer, m_isDebugDrawEnabled)];
        b = 1;*/
        PlayLayer::postUpdate(dt);
    }
};

class $modify(DrawMiscHacks, CCDrawNode) {
    bool drawPolygon(cocos2d::CCPoint* p0, unsigned int p1, const cocos2d::ccColor4F& p2,
                     float stroke, const cocos2d::ccColor4F& p4) {
        auto playLayer = PlayLayer::get();
        auto editLayer = LevelEditorLayer::get();
        GJBaseGameLayer* baseLayer = nullptr;
        if (playLayer) baseLayer = typeinfo_cast<GJBaseGameLayer*>(playLayer);
        else if (editLayer) baseLayer = typeinfo_cast<GJBaseGameLayer*>(editLayer);
        if (baseLayer && this == baseLayer->m_debugDrawNode)
            stroke *= Hacks::getHack("Hitbox Stroke")->value.floatValue;
        return CCDrawNode::drawPolygon(p0, p1, p2, stroke, p4);
    }
};

// BROKEN ON ANDROID
//#ifdef GEODE_IS_WINDOWS
#if 0
class $modify(FMODAudioEngine) {
    void updateMetering() {
        //std::cout << "ok" << std::endl;
        if (!Hacks::isHackEnabled("No Pulse")) FMODAudioEngine::updateMetering();
    }
};
#endif
//#endif
