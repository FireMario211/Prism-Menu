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
    static bool arrayContainsString(const matjson::Array& arr, const std::string& str) {
        return std::any_of(arr.begin(), arr.end(), [&](const matjson::Value& val) {
            return val.as_string() == str;
        });
    }
    static float getSliderValue(float current, float min, float max, bool inverse) {
        return (inverse) ? (current * (max - min) + min) : (current - min) / (max - min);
    }
    template<typename T>
    static std::vector<std::vector<T>> paginate(const std::vector<T>& arr, size_t size) {
        std::vector<std::vector<T>> paginated;
        for (size_t i = 0; i < arr.size(); i += size) {
            std::vector<T> page;
            for (size_t j = i; j < std::min(i + size, arr.size()); ++j) {
                page.push_back(arr[j]);
            }
            paginated.push_back(page);
        }
        return paginated;
    }


};
