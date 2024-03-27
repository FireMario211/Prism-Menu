#pragma once
/*#include <Geode/Geode.hpp>
using namespace geode::prelude;*/ 
#include "../Misc/DownloadManager.hpp"
struct CreditUser {
    std::string name;
    int iconID;
    std::string status; // what they helped with
    ccColor3B firstColor;
    ccColor3B secondColor;
    ccColor3B glowColor;
    bool glow;
    std::string url;
    int accountID = -1; // for gd in case they dont provide their social
};

/*
class CreditsMenu : public FLAlertLayer {
    protected:
        cocos2d::CCSize m_pLrSize;
        cocos2d::extension::CCScale9Sprite* m_pBGSprite;
        cocos2d::extension::CCScale9Sprite* m_pNavigation;
        std::vector<cocos2d::extension::CCScale9Sprite*> m_pNavButtons;
        CCScrollLayerExt* m_scrollLayer;
        CCMenu* m_content;
        virtual bool init(
            float width,
            float height
        );
        virtual void keyDown(cocos2d::enumKeyCodes) override;
        virtual void keybackClicked();
        float m_fWidth = s_defWidth;
        float m_fHeight = s_defHeight;
        void createItems();
        virtual void setup();
        cocos2d::CCSize m_sScrLayerSize;
    public:
        virtual void onClose(cocos2d::CCObject*);
        static constexpr const float s_defWidth = 300.0f;
        static constexpr const float s_defHeight = 200.0f;
        static CreditsMenu* create();
};
*/ 

class CreditsMenu : public BrownAlertDelegate {
    protected:
        CCScrollLayerExt* m_scrollLayer;
        CCMenu* m_content;
        std::vector<std::vector<CreditUser>> paginate;
        float m_fWidth = s_defWidth;
        float m_fHeight = s_defHeight;
        void createItems();
        virtual void setup();

        CCMenuItemSpriteExtra* nextBtn;
        CCMenuItemSpriteExtra* prevBtn;
        CCLabelBMFont* pageLabel;

        size_t currentPage = 0;
        size_t maxPages = -1;
        void renderPage();
        void nextPage(CCObject*);
        void prevPage(CCObject*);
    public:
        void onLinkClick(CCObject*);
        static constexpr const float s_defWidth = 300.0f;
        static constexpr const float s_defHeight = 200.0f;
        static CreditsMenu* create();
};
