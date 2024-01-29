#include "Languages.hpp"

std::unique_ptr<Lang> Lang::getLanguage() {
    int langID = Hacks::getHack("Language")->value.intValue;
    // i feel like this would cause a memory leak somehoww
    return Lang::get(langID);
}

