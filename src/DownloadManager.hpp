// definitely not taken from GDUtils
#pragma once
// windows only because i cant get this to work, actually im too lazy
#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <fstream>

class BrownAlertDelegate : public FLAlertLayer {
    protected:
        // layer size is stored here
        cocos2d::CCSize m_pLrSize;
        cocos2d::extension::CCScale9Sprite* m_pBGSprite;

        virtual bool init(
            float width,
            float height,
            const char* bg = "GJ_square01.png",
            const char* _title = ""
        );
        virtual void setup() = 0;
        virtual void keyDown(cocos2d::enumKeyCodes) override;

        virtual void onClose(cocos2d::CCObject*);
};

class ProgressBar : public CCNode {
    protected:
        float m_pPercentage = 0;
        CCSprite* outerBg = CCSprite::create("slidergroove.png");
        CCSprite* progressBar = CCSprite::create("sliderBar.png");
        bool init();
    public:
        void testProgressbar();
        virtual void setProgress(float percentage);
        static ProgressBar* create();
};
class DownloadManager : public BrownAlertDelegate {
    LoadingCircle* loading_circle = LoadingCircle::create();
    protected:
        virtual void setup();
        cocos2d::CCSize m_sScrLayerSize;
        const char* m_sUrl;
        const char* m_sDestination;
        float m_fWidth;
        float m_fHeight;
        cocos2d::SEL_MenuHandler m_pSelector;
    public:
        static constexpr const float s_defWidth = 300.0f;
        static constexpr const float s_defHeight = 180.0f;
        static DownloadManager* create(const char* url, const char* destination, cocos2d::SEL_MenuHandler callback); //, cocos2d::SEL_MenuHandler callback
        
        // Downloader class
        static int progress_func(void*, double, double, double, double);
};
