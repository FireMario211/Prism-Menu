#pragma once
#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include "Languages.hpp"
// TODO: Use custom box texture with ->setColor

using namespace geode::prelude;

class PrismUIButton : public CCNode, public TextInputDelegate {
    protected:
        CCMenu* menu;
        HackItem* m_hack;
        Slider* m_slider;
        InputNode* m_input;
        bool editedInputNode = false;
        virtual bool init(HackItem* hack);
        void onBoolBtn(CCObject*);
        void intChanged();
        void onIncBtn(CCObject*);
        void onDecBtn(CCObject*);
        void textChanged(CCTextInputNode* input) override;
        void textInputOpened(CCTextInputNode* input) override;
        void textInputClosed(CCTextInputNode* input) override;
        void onFloatBtn(CCObject*);
        void onBtn(CCObject*);
        void onDropdownBtn(CCObject*);
        cocos2d::extension::CCScale9Sprite* createCheckbox(bool checked);
    public:
        void onInfoBtn(CCObject*);
        static PrismUIButton* create(HackItem* hack);
};

class PrismUI : public FLAlertLayer {
    protected:
        // layer size is stored here
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
            float height
        );
        virtual void keyDown(cocos2d::enumKeyCodes) override;
        void CreateHackItem(HackItem* item, bool);
        void RegenCategory();
        void CreateButton(const char* name, int menuIndex);
        void ButtonState(int id, bool activated);
        void onSideButton(CCObject* ret);

        //void updateOpacity(CCObject*);
    public:
        virtual void onClose(cocos2d::CCObject*);
        static constexpr const float s_defWidth = 450.0f;
        static constexpr const float s_defHeight = 250.0f;
        //static constexpr const float s_defWidth = 400.0f;
        //static constexpr const float s_defHeight = 280.0f;
        void updateLabel();
        static matjson::Object GetTheme();
        static PrismUI* create();
};
