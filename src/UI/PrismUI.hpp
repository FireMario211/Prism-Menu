#pragma once
#include <Geode/Geode.hpp>
#include "../Misc/DownloadManager.hpp"
#include <Geode/loader/Event.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/ui/TextInput.hpp>
#include "../Languages.hpp"
// TODO: Use custom box texture with ->setColor

using namespace geode::prelude;

using FileEvent = Task<Result<std::filesystem::path>>;

class PrismUIButton : public CCNode, public TextInputDelegate, public CCKeyboardDelegate {
    protected:
        EventListener<FileEvent> m_listener;
        CCMenu* menu;
        HackItem* m_hack;
        Slider* m_slider;
        TextInput* m_input;
        Lang* currentLanguage;
        bool editedInputNode = false;
        virtual bool init(HackItem* hack);
        void onBoolBtn(CCObject*);
        void intChanged();
        void onIncBtn(CCObject*);
        void onDecBtn(CCObject*);
        void onCharBtn(CCObject*);
        void textChanged(CCTextInputNode* input) override;
        void textInputOpened(CCTextInputNode* input) override;
        void textInputClosed(CCTextInputNode* input) override;
        bool allowTextInput(CCTextInputNode*) override;
        void onFloatBtn(CCObject*);
        void onBtn(CCObject*);
        void onDropdownBtn(CCObject*);
        cocos2d::extension::CCScale9Sprite* createCheckbox(bool checked);
    public:
        CCMenu* getMenu() { return menu; }
        Slider* getSlider() { return m_slider; }
        TextInput* getInputNode() { return m_input; }
        HackItem* getHack() { return m_hack; }

        void onInfoBtn(CCObject*);
        static PrismUIButton* create(HackItem* hack, Lang* lang);
};


class CharUI : public BrownAlertDelegate {
    protected:
        float m_fWidth = s_defWidth;
        CCLabelBMFont* m_currentKeyLbl;
        cocos2d::enumKeyCodes m_currentKey;
        float m_fHeight = s_defHeight;
        virtual void keyDown(cocos2d::enumKeyCodes) override;
        PrismUIButton* m_prismButton;
        virtual void setup() override;
    public:
        void keyPressed(cocos2d::enumKeyCodes);
        void onConfirm(CCObject*);
        static constexpr const float s_defWidth = 250.0f;
        static constexpr const float s_defHeight = 140.0f;
        static CharUI* create(PrismUIButton* button);
};


class PrismUI : public FLAlertLayer {
    protected:
        // layer size is stored here
        cocos2d::CCSize m_pLrSize;
        cocos2d::extension::CCScale9Sprite* m_pBGSprite;
        cocos2d::extension::CCScale9Sprite* m_pNavigation;
        std::vector<cocos2d::extension::CCScale9Sprite*> m_pNavButtons;
        CCScrollLayerExt* m_scrollLayer;
        CCMenu* m_content;
        std::unique_ptr<Lang> m_currentLang;
        float m_fWidth;
        float m_fHeight;

        virtual bool init(
            float width,
            float height
        );
        virtual void keyDown(cocos2d::enumKeyCodes) override;
        virtual void keybackClicked();
        void CreateHackItem(HackItem*);
        void RegenCategory();
        void CreateButton(const char* name, int menuIndex);
        void ButtonState(int id, bool activated);
        void onSideButton(CCObject* ret);

        //void updateOpacity(CCObject*);
    public:
        void toggleVisibility();
        virtual void onClose(cocos2d::CCObject*);
        static constexpr const float s_defWidth = 450.0f;
        static constexpr const float s_defHeight = 250.0f;
        //static constexpr const float s_defWidth = 400.0f;
        //static constexpr const float s_defHeight = 280.0f;
        void updateLabel();
        static matjson::Value GetTheme();
        static PrismUI* create();
        static void fixVSync();
};
