#pragma once
#include <Geode/Geode.hpp>
#include <algorithm>
#include <vector>
#include "../Utils.hpp"

using namespace geode::prelude;

/*
class Label : public CCLabelTTF {
    protected:
    public:
        void limitLabelWidth(float width, float defaultScale, float minScale) {
            this->setScale(1.0F);
            float scale = 1.F;
            if (width < this->getContentSize().width && (0.F < width)) {
                scale = width / this->getContentSize().width;
            }
            if (defaultScale != 0.F && defaultScale <= this->getScale()) {
                scale = defaultScale;
            }
            if (minScale != 0.F && (this->getScale() <= minScale)) {
                this->setScale(minScale);
            } else {
                this->setScale(scale);
            }
        }
        static Label * create(const char *string, const char *fontName) {
            auto pRet = static_cast<Label*>(CCLabelTTF::create(string, fontName, 36, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop));
            return pRet;
        }
};
*/ 
class Label : public CCNode {
    protected:
        CCLabelBMFont* m_bmLabel;
        CCSprite* m_ttfLabel;
        bool m_ttf;
        std::string replaceSymbolSpace(std::string str) {
            if (str.empty()) return str;
            //std::vector<std::string> symbols = { "§", "¬", "ª", "«", "¦", "¶" };
            std::vector<int> symbols = {167,172,170,171,166,182};
            auto strVec = Utils::utf8ToUtf32(str);
            if (strVec.size() < 2) return str;
            if (std::find(symbols.begin(), symbols.end(), strVec[1]) != symbols.end()) {
                strVec[1] = static_cast<int>(' ');
                return Utils::utf32ToUtf8(strVec);
            } else {
                return str;
            }
        }
        bool hasSymbol(std::string str) {
            if (str.empty()) return false;
            std::vector<int> symbols = {167,172,170,171,166,182};
            auto strVec = Utils::utf8ToUtf32(str);
            if (strVec.size() < 2) return false;
            return std::find(symbols.begin(), symbols.end(), strVec[1]) != symbols.end();
        }
        bool init(std::string string, const char *fontName, bool ttf) {
            if (!CCNode::init()) return false;
            m_ttf = ttf;
            this->setAnchorPoint({0, 0});
            if (ttf) {
                //m_ttfLabel = CCLabelTTF::create(string, fontName, 36, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
                
                auto ttfLabel = CCLabelTTF::create(replaceSymbolSpace(string).c_str(), fontName, 36, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
                m_ttfLabel = CCSprite::create();
                m_ttfLabel->setDisplayFrame(ttfLabel->displayFrame());
                this->addChild(m_ttfLabel);
                this->setContentSize(m_ttfLabel->getContentSize());
                if (hasSymbol(string)) {
                    std::vector<uint32_t> vec = {Utils::utf8ToUtf32(string)[1]};
                    auto logo = CCLabelBMFont::create(Utils::utf32ToUtf8(vec).c_str(), "PrismMenu.fnt"_spr);
                    m_ttfLabel->addChildAtPosition(logo, Anchor::Right);
                }
            } else {
                m_bmLabel = CCLabelBMFont::create(string.c_str(), fontName);
                this->addChild(m_bmLabel);
                this->setContentSize(m_bmLabel->getContentSize());
            }
            return true;
        }
    public:
        bool isTTF() {
            return m_ttf;
        }
        void setColor(const ccColor3B &color) {
            if (isTTF()) {
                m_ttfLabel->setColor(color);
            } else {
                m_bmLabel->setColor(color);
            }
        }
        void setOpacity(GLubyte opacity) {
            if (isTTF()) {
                m_ttfLabel->setOpacity(opacity);
            } else {
                m_bmLabel->setOpacity(opacity);
            }
        }
        void setAnchorPointTTF(CCPoint anchor) {
            if (isTTF()) {
                m_ttfLabel->setAnchorPoint(anchor);
            } else {
                m_bmLabel->setAnchorPoint(anchor);
            }
        }
        /*
        void limitLabelWidth(float width, float defaultScale, float minScale) {
            this->setScale(1.0F);
            float scale = 1.F;
            if (width < this->getContentSize().width && (0.F < width)) {
                scale = width / this->getContentSize().width;
            }
            if (defaultScale != 0.F && defaultScale <= this->getScale()) {
                scale = defaultScale;
            }
            if (minScale != 0.F && (this->getScale() <= minScale)) {
                this->setScale(minScale);
            } else {
                this->setScale(scale);
            }
        }*/ 
        void limitLabelWidth(float width,  float normalScale, float minScale) {
            float val = width / this->getContentSize().width;
            this->setScale(val > normalScale ? normalScale : val < minScale ? minScale : val);
        }
        /*
        CCRGBAProtocol* getColorLabel() {
            return (m_ttf) ? static_cast<CCRGBAProtocol*>(m_ttfLabel) : static_cast<CCRGBAProtocol*>(m_bmLabel);
        }
        */
        static Label* create(std::string string, const char *fontName, bool ttf) {
            auto pRet = new Label();
            if (pRet) {
                if (pRet->init(string, fontName, ttf)) {
                    pRet->autorelease();
                    return pRet;
                }
            }
            CC_SAFE_DELETE(pRet);
            return nullptr;
        }
        static Label* create(const char *string, int langID) {
            if (langID == 16) {
                return create(string, "NotoSans-Arabic.ttf"_spr, true);
            } else {
                return create(string, "PrismMenu.fnt"_spr, false);
            }
        }
};


/*
class Label : public CCNode {
    protected:
        CCLabelBMFont* m_bmLabel;
        CCLabelTTF* m_ttfLabel;
        bool m_ttf = false;
        virtual bool init(const char *string, const char *fontName, bool ttf) {
            if (!CCNode::init()) return false;
            m_bmLabel = CCLabelBMFont::create(string, fontName);
            this->setContentSize(m_bmLabel->getContentSize());
            this->setAnchorPoint({0.5, 0.5});
            if (ttf) {
                m_ttfLabel = CCLabelTTF::create(string, fontName, 36, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
                this->addChild(m_ttfLabel);
            } else {
            }
            this->addChild(m_bmLabel);
            return true;
        }
    public:
        CCNodeRGBA* getColorLabela() {
            return m_ttfLabel;
        }
        CCLabelBMFont* getColorLabel() {
            return m_bmLabel;
        }

        static Label* create(const char *string, const char *fontName, bool ttf) {
            auto pRet = new Label();
            if (pRet) {
                if (pRet->init(string, fontName, ttf)) {
                    pRet->autorelease();
                    return pRet;
                }
            }
            CC_SAFE_DELETE(pRet);
            return nullptr;
        }
};
*/
