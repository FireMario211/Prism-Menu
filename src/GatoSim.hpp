#pragma once
// this is all a spoiler!
#include <Geode/Geode.hpp>

struct ShopItem {
    std::string name;
    std::string desc;
    int price;
};

using namespace geode::prelude;
class GatoSim : public CCLayer {
    // TODO: add some json file
    private:
    int fod = 0;
    int drank = 0;
    std::vector<std::string> toi;
    bool hasDoneDaily = false;
    int bal = 250;
    int satis = 0;
};
