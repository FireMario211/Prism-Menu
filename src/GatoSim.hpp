#pragma once
// this is all a spoiler!
#include <Geode/Geode.hpp>


struct PetItem {
    std::string name;
    std::string desc;
    int price;
    int tier = 1;
    int amount;
};



using namespace geode::prelude;
class GatoSim : public CCLayer, public FLAlertLayerProtocol {
    // TODO: add some json file
    // maybe turn this into some sort of "cookie clicker" because why not
    std::string name;
    int pet = 0;
    std::vector<std::string> toi;
    bool hasDoneDaily = false;

    protected:
        CCMenu* m_buttonMenu;
        virtual bool init();
        virtual void keybackClicked();
    public:
        static void onButton();
        virtual void onClose(cocos2d::CCObject*);
        static GatoSim* create();
};
