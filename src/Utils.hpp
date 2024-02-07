#pragma once 

class Utils {
    public:
    static std::string getNodeName(cocos2d::CCObject* node) {
    #ifdef GEODE_IS_WINDOWS
        return typeid(*node).name() + 6;
    #else 
        {
            std::string ret;

            int status = 0;
            auto demangle = abi::__cxa_demangle(typeid(*node).name(), 0, 0, &status);
            if (status == 0) {
                ret = demangle;
            }
            free(demangle);

            return ret;
        }
    #endif
    }
    static std::string setPrecision(float value, int streamsize) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(streamsize) << value;
        return oss.str();
    }


};
