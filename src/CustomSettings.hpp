#ifndef CUSTOM_SETTINGS_HPP
#define CUSTOM_SETTINGS_HPP
#include <Geode/loader/SettingNode.hpp>
using namespace geode::prelude;

struct HackValuesStruct {
    std::string m_hackName;
    std::string m_hackType;
    matjson::Value m_value;
};

struct SettingHackStruct {
    matjson::Array m_hackValues;
};

template<>
struct matjson::Serialize<SettingHackStruct> {
    static SettingHackStruct from_json(matjson::Value const& value) {
        return SettingHackStruct {
            .m_hackValues = value.as_array(),
        };
    }
    static bool is_json(matjson::Value const& a) { // thank you mat!
        return a.is_array();
    }
    static matjson::Value to_json(SettingHackStruct const& value) {
        return value.m_hackValues;
    }
};


class SettingHackValue;

class SettingHackValue : public SettingValue {
protected:
    matjson::Array m_hackValues;
public:
    SettingHackValue(std::string const& key, std::string const& modID, SettingHackStruct const& hacks)
      : SettingValue(key, modID), m_hackValues(hacks.m_hackValues) {}

    bool load(matjson::Value const& json) override {
        // how this doesnt crash on android is beyond me
        try {
            //m_pos = static_cast<int>(json.as<int>());
            m_hackValues = json.as_array();
            return true;
        } catch(...) {
            return false;
        }
    }
    bool save(matjson::Value& json) const override {
        json = m_hackValues;
        return true;
    }
    SettingNode* createNode(float width) override;
    void setHacks(SettingHackStruct const& value) {
        m_hackValues = value.m_hackValues;
    }
    SettingHackStruct getHacks() const {
        return { m_hackValues };
    }
};

template<>
struct SettingValueSetter<SettingHackStruct> {
    static SettingHackStruct get(SettingValue* setting) {
        auto hackSetting = static_cast<SettingHackValue*>(setting);
        struct SettingHackStruct defaultStruct = { hackSetting->getHacks() };
        return defaultStruct;
    };
    static void set(SettingHackValue* setting, SettingHackStruct const& value) {
        setting->setHacks(value);
    };
};

class SettingHackNode : public SettingNode {
protected:
    bool init(SettingHackValue* value, float width) {
        if (!SettingNode::init(value))
            return false;
        return true;
    }
public:
    void commit() override {
        // Let the UI know you have committed the value
        this->dispatchCommitted();
    }

    // Geode calls this to query if the setting value has been changed, 
    // and those changes haven't been committed
    bool hasUncommittedChanges() override {
        return false;
    }

    // Geode calls this to query if the setting has a value that is 
    // different from its default value
    bool hasNonDefaultValue() override {
        return true;
    }

    // Geode calls this to reset the setting's value back to default
    void resetToDefault() override {

    }
    static SettingHackNode* create(SettingHackValue* value, float width) {
        auto ret = new SettingHackNode;
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
#endif
