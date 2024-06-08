#pragma once 
#include <Geode/Geode.hpp>
#include "../Misc/DownloadManager.hpp"
#include <Geode/Bindings.hpp>
// TODO: Use custom box texture with ->setColor

using namespace geode::prelude;

class IntroUI : public BrownAlertDelegate {
    protected:
        CCMenu* m_items;
        float m_fWidth = s_defWidth;
        float m_fHeight = s_defHeight;
        virtual void keyDown(cocos2d::enumKeyCodes) override;
        virtual void setup() override;
        void onLang(CCObject*);
        void onSliderMove(bool x);
        void onSliderMoveX(CCObject*);
        void onSliderMoveY(CCObject*);
    public:
        void onSkip(CCObject*);
        void onContinue(CCObject*);
        void onConfirm(CCObject*);
        static constexpr const float s_defWidth = 250.0f;
        static constexpr const float s_defHeight = 160.0f;
        static IntroUI* create();
};

