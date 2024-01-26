#pragma once
#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include "languages.hpp"
// TODO: Use custom box texture with ->setColor

using namespace geode::prelude;
class PrismUI : public FLAlertLayer, TextInputDelegate {
    protected:
        // layer size is stored here
        Lang* currentLanguage;
        matjson::Array themeArr; // solution for the operator= copy problem
        cocos2d::CCSize m_pLrSize;
        cocos2d::CCSize m_sScrLayerSize;
        cocos2d::extension::CCScale9Sprite* m_pBGSprite;
        cocos2d::extension::CCScale9Sprite* m_pNavigation;
        std::vector<cocos2d::extension::CCScale9Sprite*> m_pNavButtons;
        CCScrollLayerExt* m_scrollLayer;
        CCMenu* m_content;
        float m_fWidth;
        float m_fHeight;

        virtual bool init(
            float width,
            float height,
            matjson::Object theme,
            Lang* currentLang
        );
        virtual void keyDown(cocos2d::enumKeyCodes) override;
        virtual void onClose(cocos2d::CCObject*);
        void CreateHackItem(HackItem* item);
        void RegenCategory();
        void CreateButton(const char* name, int menuIndex);
        void ButtonState(int id, bool activated);
        void onSideButton(CCObject* ret);
        cocos2d::extension::CCScale9Sprite* createCheckbox(bool checked);
        matjson::Object GetTheme();

        void onInfoBtn(CCObject*);
        void onBoolBtn(CCObject*);
        //void updateOpacity(CCObject*);
    public:
        static constexpr const float s_defWidth = 450.0f;
        static constexpr const float s_defHeight = 250.0f;
        //static constexpr const float s_defWidth = 400.0f;
        //static constexpr const float s_defHeight = 280.0f;
        static PrismUI* create(matjson::Object theme, Lang* currentLang);
};
