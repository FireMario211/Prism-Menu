#ifndef CUSTOM_SETTINGS_HPP
#define CUSTOM_SETTINGS_HPP
using namespace geode::prelude;

struct HackValuesStruct {
    std::string m_hackName;
    std::string m_hackType;
    matjson::Value m_value;
};

struct SettingHackStruct {
    std::vector<matjson::Value> m_hackValues;
};

template<>
struct matjson::Serialize<SettingHackStruct> {
    static Result<SettingHackStruct> fromJson(matjson::Value const& value) {
        GEODE_UNWRAP_INTO(auto hackValues, value.asArray());
        return Ok(SettingHackStruct {
            .m_hackValues = hackValues
        });
    }
    static matjson::Value toJson(SettingHackStruct const& value) {
        return value.m_hackValues;
    }
};


// im laughing, it wont compile if i put the struct SettingHackValue, but it will if i just do matjson::Array, hahahaHAHAHAHA
class SettingHackValue : public SettingBaseValueV3<std::vector<matjson::Value>> {
protected:
    // unsure if this is necessary...
    std::vector<matjson::Value> m_hackValues;
public:
    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
        auto res = std::make_shared<SettingHackValue>();
        auto root = checkJson(json, "SettingHackValue");
        res->parseBaseProperties(key, modID, root);
        root.checkUnknownKeys();
        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }
    SettingNodeV3* createNode(float width) override;
};

template <>
struct geode::SettingTypeForValueType<SettingHackStruct> {
    using SettingType = SettingHackValue;
};

class SettingHackNode : public SettingValueNodeV3<SettingHackValue> {
protected:
    bool init(std::shared_ptr<SettingHackValue> setting, float width) {
        if (!SettingValueNodeV3::init(setting, width))
            return false;
        return true;
    }
public:
    static SettingHackNode* create(std::shared_ptr<SettingHackValue> setting, float width) {
        auto ret = new SettingHackNode();
        if (ret && ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

/*
class SettingHackValue : public SettingBaseValueV3<SettingHackStruct> {
protected:
    matjson::Array m_hackValues;
public:
    SettingHackValue(std::string const& key, std::string const& modID, SettingHackStruct const& hacks)
      : SettingValue(key, modID), m_hackValues(hacks.m_hackValues) {}

    bool load(matjson::Value const& json) override {
        if (!json.is_array()) return false;
        m_hackValues = json.as_array();
        return true;
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

*/
#endif
