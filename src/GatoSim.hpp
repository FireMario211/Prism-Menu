#pragma once
// this is all a spoiler!
#include <Geode/Geode.hpp>

struct ShopItem {
    std::string name;
    std::string desc;
    int price;
};

using namespace geode::prelude;
class GatoSim : public CCLayer, public FLAlertLayerProtocol {
    // TODO: add some json file
    // maybe turn this into some sort of "cookie clicker" because why not
    int fod = 0;
    int drank = 0;
    std::vector<std::string> toi;
    bool hasDoneDaily = false;
    int bal = 250;
    int satis = 0;

    protected:
        CCMenu* m_buttonMenu;
        virtual bool init();
        virtual void keybackClicked();
    public:
        static void onButton();
        virtual void onClose(cocos2d::CCObject*);
        static GatoSim* create();
};
