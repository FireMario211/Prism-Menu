#include "CreditsMenu.hpp"
#include <vector>
#include "Geode/utils/web.hpp"
#include "PrismUI.hpp"
#include "../Utils.hpp"

/*
void CreditsMenu::onFireeBtn(CCObject*) {
}
*/

std::vector<CreditUser> users = {
    {
        "Firee",
        98,
        "Developer",
        { 125, 0, 255 },
        { 255, 255, 255 },
        { 255, 255, 255 },
        true,
        "https://www.youtube.com/@gdfiree",
        6253758
    },
    /*
    {
        "Tulpenkiste",
        0,
        "Contributor",
        { 255, 255, 255 },
        { 255, 255, 255 },
        { 255, 255, 255 },
        true,
        "https://www.youtube.com/@gdfiree"
    },*/
    {
        "TheSillyDoggo",
        373,
        "Logo Base",
        { 255,255,255 },
        { 255, 185, 255 },
        { 255, 255, 255 },
        true,
        "https://github.com/TheSillyDoggo",
        16778880
    },
    {
        "Lartum",
        1,
        "Bot Idea",
        { 255, 255, 255 },
        { 175,175,175 },
        { 0, 0, 0 },
        false,
        "https://www.youtube.com/@lartum",
        20531331
    },
    {
        "Electrify",
        107,
        "Translator (ES)",
        { 0, 200, 255 },
        { 1, 254, 253 },
        { 1, 254, 253 },
        true,
        "https://www.youtube.com/channel/UCxYliOd2aV6iZuc5wuG9QxA"
    },
    {
        "ItzLucasOG",
        1,
        "Translator (ES)",
        { 192, 255, 160 },
        { 255, 255, 255 },
        { 255, 255, 255 },
        true,
        "https://twitter.com/ItzLucasOG"
    },
    {
        "Hero",
        388,
        "Translator (ES)",
        { 255, 0, 0 },
        { 0, 0, 255 },
        { 0,0,0 },
        false,
        "",
        4651585
    },

    {
        "Jouca",
        133,
        "Translator (FR)",
        {255, 0, 255},
        {255, 0, 125},
        {175, 0, 240},
        true,
        "https://twitter.com/JoucaJouca",
        7026949
    },
    {
        "dankmeme01",
        1,
        "Proofreader (RU)",
        { 125, 125, 255 },
        { 0, 200, 255 },
        { 0,0,0 },
        false,
        "https://github.com/dankmeme01"
    },
    {
        "MegaSa1nt",
        1,
        "Translator (RU)",
        { 0, 255, 0 },
        { 0, 255, 255 },
        { 0,0,0 },
        false,
        "",
        5500048
    },

    {
        "Savvacorgi",
        373,
        "Translator (RU)",
        { 17,255,255 },
        { 255, 255, 255 },
        { 0,0,0 },
        false,
        "",
        20491619
    },
    {
        "Gazonk",
        90,
        "Translator (PT)",
        { 64,64,64 },
        { 255,185,0 },
        { 255,185,0 },
        true,
        "https://www.youtube.com/channel/UCgp5Tql2b6Y1Xcni82pPxmA",
        25477021
    },
    {
        "huhnmitferrari",
        0,
        "Translator (DE)",
        { 0, 255, 0 },
        { 0, 255, 255 },
        { 0,0,0 },
        false,
        ""
    },
    {
        "Ignis",
        132,
        "Translator (CS)",
        { 185,0,255 },
        { 0,125,255 },
        { 125,0,255 },
        true,
        "https://twitter.com/IgnisPeaks0",
        14029092
    },
    {
        "RzaIX",
        49,
        "Translator (Indonesian)",
        { 0,0,0 },
        { 255,255,255 },
        { 255,255,0 },
        true,
        "https://twitter.com/RzaIX_",
        13831248
    },
    {
        "KONDIROTSU",
        471,
        "Translator (Polish)",
        { 125,125,255 },
        { 255, 255, 255 },
        { 255,255,255 },
        true,
        "",
        13743938
    },

    // not done
    {
        "olaf_294",
        1,
        "Translator (PL)",
        { 0, 255, 0 },
        { 0, 255, 255 },
        { 0,0,0 },
        false,
        ""
    },
    {
        "hori",
        35,
        "Translator (MS)",
        { 0, 0, 0 },
        { 255, 255, 255 },
        { 255, 255, 255 },
        true,
        "",
        11055703
    },
    {
        "slideglide",
        98,
        "Translator (TR)",
        { 192,255,160 },
        { 255, 255, 255 },
        { 192,255,160 },
        true,
        "",
        26979350
    },
    {
        "tienanh109",
        107,
        "Translator (VI)",
        { 255,125,0 },
        { 0,255,255 },
        { 0,175,75 },
        true,
        "https://youtube.com/@tienanh_90",
        25423937
    },
    {
        "AftonGMD",
        2,
        "Translator (AR)",
        { 185,0,255 },
        { 255, 255, 255 },
        { 185,0,255 },
        true,
        "",
        25197313
    },
    {
        "dawgismeh69",
        345,
        "Bot Name",
        { 160,255,255 },
        { 0,125,255 },
        { 0,0,0 },
        false,
        "",
        27624348
    },
    {
        "Viatron",
        260,
        "Translator (JP)",
        { 84, 54, 127 },
        { 190, 181, 255 },
        { 0, 0, 0 },
        false,
        ""
    },
    {
        "WALVe",
        74,
        "Translator (FR)",
        { 0, 0, 0 },
        { 125,125,255 },
        { 255,125,0 },
        true,
        ""
    },
    {
        "QuadrupleA",
        102,
        "Translator (EL)",
        { 253, 212, 206 },
        { 64, 64, 64 },
        { 0, 0, 0 },
        false,
        "",
        26271597
    },
    {
        "Lancelot220",
        134,
        "Translator (UK)",
        { 0,0,255 },
        { 255,0,0 },
        { 185,0,255 },
        true,
        "",
        14199510
    },
    {
        "DarkKing",
        1,
        "Proofreader (TR)",
        { 0,0,0 },
        { 255,0,0 },
        { 255,0,0 },
        true,
        "",
        19331971
    },
    {
        "AprilWaterfalls",
        98,
        "Translator (IT)",
        { 125, 125, 255 },
        { 225, 219, 255 },
        { 125, 125, 255 },
        true,
        "https://x.com/aprilwaterfalls",
        25221134
    },
    {
        "Rexosouli",
        35,
        "Translator (FR)",
        { 0,200,255 },
        { 0,255,255 },
        { 0,255,255 },
        true,
        "https://discord.com/users/805514311547748403",
        18038950
    },
    {
        "HiimAndrewYT",
        111,
        "Translator (RO)",
        { 0, 255, 0 },
        { 0, 255, 255 },
        { 0,0,0 },
        false,
        "https://youtube.com/@MyNamesAndrew",
        26767554
    },
    {
        "M336",
        35,
        "Translator (FR)",
        { 0, 0, 0 },
        { 255, 0, 0 },
        { 255, 0, 0 },
        true,
        "https://www.youtube.com/channel/UC__S0detWlP2mWrsXMkkvFg",
        24710147
    },
    {
        "ItzRatchett",
        275,
        "Proofreader (PT)",
        { 0, 125, 125 },
        { 0, 255, 255 },
        { 0, 255, 0 },
        true,
        "",
        28982364
    },
    {
        "Pan Polak",
        379,
        "Translator (PL)",
        { 255, 255, 255 },
        { 0, 255, 255 },
        { 255, 255, 255 },
        true,
        "",
        20527327
    },
    {
        "Sosuke Aoi",
        459,
        "Translator (FR)",
        { 255, 255, 255 },
        { 0, 0, 0 },
        { 255, 255, 255 },
        true,
        "https://www.youtube.com/channel/UC_wDsDqnDOkpC2yPb7pEZWw"
    },
    {
        "Adamorocco",
        35,
        "Translator (AR)",
        { 64, 64, 64, },
        { 255, 0, 0 },
        { 255, 255, 255 },
        true,
        "",
        25682776
    }
};

void CreditsMenu::renderPage() {
    pageLabel->setString(fmt::format("Page {}/{}", currentPage + 1, paginate.size()).c_str());
    m_content->removeAllChildrenWithCleanup(true);
    int currentIOffset = 2;

    int maxI = currentIOffset;
    int currentI = 0;
    int currentX = 0;
    float offsetY = 80;

    if (currentPage < paginate.size()) {
        for (CreditUser user : paginate[currentPage]) {
            auto icon = SimplePlayer::create(user.iconID);
            icon->setColor(user.firstColor);
            icon->setSecondColor(user.secondColor);
            if (user.glow) {
                icon->setGlowOutline(user.glowColor);
            }
            icon->updateColors();
            auto iconBtn = CCMenuItemSpriteExtra::create(static_cast<CCSprite*>(icon->getChildren()->objectAtIndex(0)), this, menu_selector(CreditsMenu::onLinkClick));
            //iconBtn->setContentSize(static_cast<CCSprite*>(icon->getChildren()->objectAtIndex(0))->getContentSize());
            /*iconBtn->setContentSize({32,32});
            iconBtn->setAnchorPoint({0,0});*/
            iconBtn->setID(std::to_string(currentI).c_str());
            //static_cast<SimplePlayer*>(iconBtn->getChildren()->objectAtIndex(0))->setPosition({16,16});
            auto nameLabel = CCLabelBMFont::create(user.name.c_str(), "goldFont.fnt");
            auto statusLabel = CCLabelBMFont::create(user.status.c_str(), "bigFont.fnt");
            iconBtn->setPosition({((float)currentX * 90.F) + 20, currentIOffset * offsetY});
            nameLabel->setPosition({((float)currentX * 90.F) + 20, (currentIOffset * offsetY) + 30});
            statusLabel->setPosition({((float)currentX * 90.F) + 20, (currentIOffset * offsetY) - 25});
            m_content->addChild(iconBtn);

            nameLabel->limitLabelWidth(70, 0.8F, .2F);
            statusLabel->limitLabelWidth(70, 0.7F, .2F);
            m_content->addChild(nameLabel);
            m_content->addChild(statusLabel);
            currentX++;
            currentI++;
            if (currentX % 3 == 0) {
                currentX = 0;
                currentIOffset--;
            }
        }
    }
}

void CreditsMenu::nextPage(CCObject*) {
    currentPage++;
    if (currentPage >= paginate.size()) currentPage = 0;
    //nextBtn->setVisible(currentPage < paginate.size() - 1);
    //prevBtn->setVisible(true);
    renderPage();
}
void CreditsMenu::prevPage(CCObject*) {
    currentPage--;
    if (currentPage < 0 || currentPage > paginate.size()) currentPage = paginate.size() - 1;
    //prevBtn->setVisible(currentPage > 0);
    //nextBtn->setVisible(true);
    renderPage();
}
#include <Geode/modify/EndLevelLayer.hpp>
int avfdvb=0;

class $modify(EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();
        std::string chk = "kcabyalP";
        reverse(chk.begin(), chk.end());
        if (Hacks::isHackEnabled(chk)) {
            avfdvb=0;
            if (auto layer = typeinfo_cast<CCLayer*>(this->getChildren()->objectAtIndex(0))) {
                for (int i = 0; i < layer->getChildrenCount(); i++) {
                    auto node = layer->getChildren()->objectAtIndex(i);
                    if (auto spr = typeinfo_cast<CCSprite*>(node)) {
                        if (spr->getPositionY() == 282) {
                            ++avfdvb;
                            spr->setScale(0);
                            if (avfdvb > 3) break;
                        }
                    }
                }
            }
        }
    }
};

void CreditsMenu::setup() {
    if (auto prismMenu = typeinfo_cast<PrismUI*>(CCScene::get()->getChildByID("prism-menu"))) {
        prismMenu->toggleVisibility();
    }
    paginate = Utils::paginate(users, 6);
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto prevArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto nextArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextArrow->setFlipX(true);
    nextBtn = CCMenuItemSpriteExtra::create(
        nextArrow,
        this,
        menu_selector(CreditsMenu::nextPage)
    );
    prevBtn = CCMenuItemSpriteExtra::create(
        prevArrow,
        this,
        menu_selector(CreditsMenu::prevPage)
    );
    nextBtn->setPosition({ (this->m_pLrSize.width) - 120, 0 });
    prevBtn->setPosition({ (-this->m_pLrSize.width) + 120, 0 });
    this->m_buttonMenu->addChild(prevBtn);
    this->m_buttonMenu->addChild(nextBtn);
    //prevBtn->setVisible(false);
    pageLabel = CCLabelBMFont::create(fmt::format("Page 1/{}", paginate.size()).c_str(), "chatFont.fnt");
    pageLabel->setPositionY(-90);
    pageLabel->setScale(.5F);
    pageLabel->setColor({0,0,0});
    pageLabel->setOpacity(256 / 1.5); // def a good idea!
    this->m_buttonMenu->addChild(pageLabel);
        /*m_scrollLayer = ScrollLayer::create({ 0, 0, 260.0F, 160.0F }, true);
    m_content = CCMenu::create();
    m_content->setZOrder(2);
    m_content->setPositionX(20);
    m_content->registerWithTouchDispatcher();
    //m_scrollLayer->m_contentLayer->setPositionY(-320.F);
    m_scrollLayer->m_contentLayer->addChild(m_content);

    m_scrollLayer->setTouchEnabled(true);
    m_scrollLayer->setPosition({-125, -98});

    m_buttonMenu->addChild(m_scrollLayer);
    
    int currentIOffset = (users.size() / 3);

    int maxI = currentIOffset;
    int currentI = 0;
    int currentX = 0;
    float offsetY = 80;
    m_content->setPositionY(40);
    for (auto it = users.begin(); it != users.end(); it++) {
        const auto& user = *it;
        auto icon = SimplePlayer::create(user.iconID);
        icon->setColor(user.firstColor);
        icon->setSecondColor(user.secondColor);
        if (user.glow) {
            icon->setGlowOutline(user.glowColor);
        }
        icon->updateColors();
        auto iconBtn = CCMenuItemSpriteExtra::create(icon, m_content, menu_selector(CreditsMenu::onLinkClick));
        iconBtn->setID(std::to_string(currentI).c_str());
        auto nameLabel = CCLabelBMFont::create(user.name.c_str(), "goldFont.fnt");
        auto statusLabel = CCLabelBMFont::create(user.status.c_str(), "bigFont.fnt");
        iconBtn->setPosition({(currentX * 90) + 15, currentIOffset * offsetY});
        nameLabel->setPosition({(currentX * 90) + 15, (currentIOffset * offsetY) + 30});
        statusLabel->setPosition({(currentX * 90) + 15, (currentIOffset * offsetY) - 25});
        m_content->addChild(iconBtn);

        nameLabel->limitLabelWidth(70, 0.8F, .2F);
        statusLabel->limitLabelWidth(70, 0.7F, .2F);
        m_content->addChild(nameLabel);
        m_content->addChild(statusLabel);
        currentX++;
        currentI++;
        if (currentX % 3 == 0) {
            currentX = 0;
            currentIOffset--;
        }
        if (m_content->getContentSize().height > 160.0F) {
            //m_scrollLayer->m_contentLayer->setContentSize(m_content->getContentSize() * (int)(maxI / 5));
            m_scrollLayer->m_contentLayer->setContentSize({m_content->getContentSize().width, 480.F});
        } else {
            m_scrollLayer->m_contentLayer->setContentSize(m_content->getContentSize());
        }

        m_scrollLayer->moveToTop();
    }
    m_scrollLayer->setTouchEnabled(true);
    */

    m_content = CCMenu::create();
    m_buttonMenu->addChild(m_content);
    m_content->setPosition({(-m_fWidth / 2) + 40, -(m_fHeight / 2) - 38});
    renderPage();
    m_content->registerWithTouchDispatcher();
    m_content->setTouchEnabled(true);
    cocos::handleTouchPriority(this);
}


void CreditsMenu::onLinkClick(CCObject* sender) {
    auto menuItem = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto userIndex = std::stoi(menuItem->getID());
    // def wont go wrong!
    auto user = paginate[currentPage][userIndex];
    if (user.url != "") {
        if (user.accountID == -1) {
            geode::createQuickPopup(
                "Hold up!",
                fmt::format("Links are spooky! Are you sure you want to go to <cy>{}</c>?", user.url),
                "Cancel", "Yes",
                [user](auto, bool btn2) {
                    if (btn2) {
                        web::openLinkInBrowser(user.url);
                    }
                }, true, true
            );
        } else {
            geode::createQuickPopup(
                "Confirm",
                fmt::format("Do you want to go to <cy>{}</c>? Or open the profile of <cg>{}</c>?", user.url, user.name),
                "Open URL", "Open Profile",
                [user](auto, bool btn2) {
                    if (!btn2) {
                        web::openLinkInBrowser(user.url);
                    } else {
                        ProfilePage::create(user.accountID, false)->show();
                    }
                }, true, true
            );
        }
    } else {
        if (user.accountID != -1) {
            ProfilePage::create(user.accountID, false)->show();
        }
        /*GameLevelManager* glm = GameLevelManager::sharedState();

        auto data = static_cast<CCDictionary*>(static_cast<CCNode*>(pSender)->getUserObject());
        auto accountID = data->valueForKey("accountID")->getCString();
        auto playerID = data->valueForKey("playerID")->getCString();
        auto username = data->valueForKey("username")->getCString();

        glm->storeUserName(std::stoi(playerID), std::stoi(accountID), username);*/

        // i juist realized robert does it!?
    }
}

CreditsMenu* CreditsMenu::create() {
    auto pRet = new CreditsMenu();
    if (pRet && pRet->init(CreditsMenu::s_defWidth, CreditsMenu::s_defHeight, "GJ_square01.png", "Credits")) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};
/*
CCArray* CreditsMenu::createCells() {
    auto items = CCArray::create();
    for (auto const& item : users) {
        auto cell = UserCell::create(this, item);
        items->addObject(cell);
    }
    return items;
}

UserCell* UserCell::create(CreditsMenu* list, CreditUser user) {
    auto pRet = new UserCell();
    if (pRet) {
        if (pRet->init(list, user)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
};

void UserCell::onLinkClick(CCObject* sender) {

}

bool UserCell::init(CreditsMenu* list, CreditUser user) {
    m_fWidth = 330.0F;
    m_fHeight = 50.0F;
    m_user = user;
    this->setContentSize({ m_fWidth, m_fHeight });

    menu = CCMenu::create();
    menu->setPosition({m_fWidth / 2, m_fHeight / 2});
    menu->setContentSize({m_fWidth - 20, m_fHeight});
    menu->setAnchorPoint({.5f, .5f});
    menu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoScale(false)
            ->setCrossAxisOverflow(false)
            ->setGap(10)
    );
    this->addChild(menu);
    
    /\*
    auto nameLbl = CCLabelBMFont::create(
        fmt::format(
            "{} ({})",
            m_item.name,
            (m_item.type == ItemType::Normal) ? std::to_string(m_item.amount) : "Special"
        ).c_str(),
        m_item.type == ItemType::Normal ? "bigFont.fnt" : "goldFont.fnt"
    );
    nameLbl->limitLabelWidth(180, 1.0F, 0.2F);
    menu->addChild(nameLbl);
    menu->addChild(CCLabelBMFont::create(fmt::format("P{}", m_item.price).c_str(), "chatFont.fnt"));
    auto buyBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("Buy"), this, menu_selector(ItemCell::onBuy));
    menu->addChild(buyBtn);
    menu->updateLayout();
    buyBtn->setPositionX(m_fWidth - 30);*\/
    return true;
}

void UserCell::draw() {
    auto size = this->getContentSize();
    cocos2d::ccDrawColor4B(0, 0, 0, 75);
    glLineWidth(2.0f);
    cocos2d::ccDrawLine({ 1.0f, 0.0f }, { size.width - 1.0f, 0.0f });
    cocos2d::ccDrawLine({ 1.0f, size.height }, { size.width - 1.0f, size.height });
}*/

