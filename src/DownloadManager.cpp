// definitely not taken from GDUtils
#include "DownloadManager.hpp"

bool BrownAlertDelegate::init(float _w, float _h, const char* _spr, const char* _title) {
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    this->m_pLrSize = cocos2d::CCSize { _w, _h };

    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    this->m_mainLayer = cocos2d::CCLayer::create();
    this->addChild(this->m_mainLayer);

    auto bg = cocos2d::extension::CCScale9Sprite::create(_spr, { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize(this->m_pLrSize);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    this->m_mainLayer->addChild(bg);
    this->m_pBGSprite = bg;

    this->m_buttonMenu = cocos2d::CCMenu::create();
    this->m_mainLayer->addChild(this->m_buttonMenu);

    if (strlen(_title) > 0) {
        auto title = cocos2d::CCLabelBMFont::create(_title, "bigFont.fnt");

        title->limitLabelWidth(this->m_pLrSize.width * 4, .8f, .2f);
        title->setPosition(
            winSize.width / 2,
            winSize.height / 2 + this->m_pLrSize.height / 2 - 25
        );

        this->m_mainLayer->addChild(title);
    }

    setup();
    this->registerWithTouchDispatcher();
    
    auto closeSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(1.0f);

    auto closeBtn = CCMenuItemSpriteExtra::create(
        closeSpr,
        this,
        (cocos2d::SEL_MenuHandler)&BrownAlertDelegate::onClose
    );
    closeBtn->setUserData(reinterpret_cast<void*>(this));

    this->m_buttonMenu->addChild(closeBtn);

    closeBtn->setPosition( - _w / 2, _h / 2 );

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    return true;
}

void BrownAlertDelegate::keyDown(cocos2d::enumKeyCodes key) {
    if (key == cocos2d::enumKeyCodes::KEY_Escape)
        return onClose(nullptr);
    if (key == cocos2d::enumKeyCodes::KEY_Space)
        return;
    
    return FLAlertLayer::keyDown(key);
}

void BrownAlertDelegate::onClose(cocos2d::CCObject* pSender) {
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
};

#include <math.h>
#include <Geode/utils/web.hpp>
#include <Geode/utils/file.hpp>

bool ProgressBar::init() { // on purpose im not even providing any params because why would we!?
    progressBar->setPosition({3, 8});
    progressBar->setAnchorPoint({ 0, 0.5 });
    progressBar->setTextureRect(CCRect(0, 0, 204, 8));
    ccTexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT }; // tf is this nonsense mat
    progressBar->getTexture()->setTexParameters(&params);
    this->addChild(outerBg);
    progressBar->setZOrder(-1);
    outerBg->addChild(progressBar);
    return true;
}

void ProgressBar::setProgress(float percentage) {
    this->m_pPercentage = percentage;
    float realPercentage = (204/100) * this->m_pPercentage; // "Result of integer division used in a floating point" ok i know clangd warnings wow
    progressBar->setTextureRect(CCRect(0, 0, realPercentage, 8));
}

ProgressBar* ProgressBar::create() {
    auto pRet = new ProgressBar();

    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}
// ------
// credits https://stackoverflow.com/a/1639047
int DownloadManager::progress_func(void*, double totalDownload, double downloadAmount, double TotalToUpload, double NowUploaded) {
    //std::cout << "progress_func(" << totalDownload << ", " << downloadAmount << ")" << std::endl;
    if (totalDownload <= 0.0) return 0;
    float amountDownloaded = round((downloadAmount / totalDownload) * 100);
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    auto layer = reinterpret_cast<ProgressBar*>(reinterpret_cast<CCLayer*>(reinterpret_cast<CCLayer*>(scene->getChildByTag(6942084))->getChildren()->objectAtIndex(0))->getChildByTag(4592));
    if (layer != nullptr) {
        layer->setProgress(amountDownloaded);
    }
    return 0;
}

void DownloadManager::setup() {
    log::debug("DownloadManager::setup()");
    //log::debug(m_sUrl);
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    loading_circle->setParentLayer(this);
    loading_circle->setScale(0.675f);
    loading_circle->setPositionY(20);
    loading_circle->show();
    auto progressBar = ProgressBar::create();
    progressBar->setPosition({ 280, 130 });
    progressBar->setProgress(0.0F);
    progressBar->setTag(4592);
    this->setTag(6942084);
    this->m_mainLayer->addChild(progressBar);


    auto text = cocos2d::CCLabelBMFont::create("Do not close this menu while the file is being downloaded.", "chatFont.fnt");
    text->setPosition(
        winSize.width / 2,
        winSize.height / 2 - 18
    ); // needs to be 100
    text->setAlignment(cocos2d::kCCTextAlignmentCenter);
    text->setTag(39);
    text->setScale(.5F);
    this->m_mainLayer->addChild(text);
    setTouchEnabled(true);

    // def not copied from geode hahahaha

    web::AsyncWebRequest()
        .join("download-langs")
        .fetch(this->m_sUrl)
        .into(this->m_sDestination)
        .then([this](auto) {
            log::debug("dumpToFile");
            //ProcessLambdas::callMenuHandler(this, this->m_pSelector);
            this->onClose(CCNode::create());
        })
        .expect([](std::string const& err) {
            FLAlertLayer::create(nullptr, "Error!", "An error occured while trying to send a request to the servers. Check <cy>logs</c> for more information.", "OK", nullptr, 200.0F)->show();
            log::error("Error downloading: {}", err);
        })
        .progress([](auto&, double now, double total) {
            progress_func(NULL, total, now, 0.0, 0.0);
        });
}

DownloadManager* DownloadManager::create(const char* url, const char* destination, cocos2d::SEL_MenuHandler selector) {
    auto pRet = new DownloadManager();
    if (pRet) {
        pRet->m_pSelector = selector;
        pRet->m_sUrl = url;
        pRet->m_sDestination = destination;
        if (pRet->init(DownloadManager::s_defWidth, DownloadManager::s_defHeight, "GJ_square01.png", "Downloading...")) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};
