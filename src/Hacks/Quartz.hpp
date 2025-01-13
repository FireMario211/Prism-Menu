#pragma once
#include "../Misc/DownloadManager.hpp"
#include "../UI/Dropdown.h"
#include "Geode/ui/TextInput.hpp"
#include "../../includes/gdr/gdr.hpp"
#include <fstream>

enum MacroType {
    QUARTZ,
    MH,
    GDMO
};


struct PlayerFrame {
    bool fix = false;
    MacroType type;
    CCPoint pos;
    CCPoint realPos;
    float rotation;
    bool isRotating;
    float rotation_speed;
    CCPoint velocity;
    bool isSlope;
    bool wasSlope;
    bool player2;
    int frame;
    int buttonHold;
    bool isHolding;
};

struct QuartzInput : gdr::Input {
    PlayerFrame frameFix;

    QuartzInput() = default;

    QuartzInput(int frame, int button, bool player2, bool down)
        : Input(frame, button, player2, down) {}

    // thanks ninxout
    void parseExtension(gdr::json::object_t obj) override {

        // Quartz 
        if (obj.contains("qframe") && obj.contains("qframe_fix") && obj["qframe_fix"] == true) {
            frameFix.type = MacroType::QUARTZ;
            frameFix.fix = true;
            frameFix.frame = obj["frame"];
            frameFix.player2 = obj["qframe"]["2p"];
            frameFix.isHolding = obj["down"];
            frameFix.buttonHold = obj["btn"];
            frameFix.realPos.x = obj["qframe"]["realPosX"];
            frameFix.realPos.y = obj["qframe"]["realPosY"];
            frameFix.pos.x = obj["qframe"]["nodePosX"];
            frameFix.pos.y = obj["qframe"]["nodePosY"];
            frameFix.velocity.y = obj["qframe"]["velY"];
            frameFix.velocity.x = obj["qframe"]["velX"];
            frameFix.rotation = obj["qframe"]["rot"];
            frameFix.rotation_speed = obj["qframe"]["rot_speed"];
            frameFix.isRotating = obj["qframe"]["rotating"];
            frameFix.isSlope = obj["qframe"]["isSlope"];
            frameFix.wasSlope = obj["qframe"]["wasSlope"];
        }

        // MH_REPLAY 8.0-alpha-12a
        if (obj.contains("mhr_meta") && obj["mhr_meta"] == true) {
            frameFix.type = MacroType::MH;
            frameFix.fix = true;
            frameFix.frame = obj["frame"];
            frameFix.player2 = obj["2p"];
            frameFix.isHolding = obj["down"];
            frameFix.buttonHold = obj["btn"];
            frameFix.realPos.x = obj["mhr_x"];
            frameFix.realPos.y = obj["mhr_y"];
            frameFix.velocity.y = obj["mhr_yvel"];
        }
        // GDMO
        if (obj.contains("correction")) {
            frameFix.type = MacroType::GDMO;
            frameFix.fix = true;
            if (obj["correction"].contains("frame")) {
                frameFix.frame = obj["correction"]["frame"];
            } else {
                frameFix.frame = obj["frame"];
            }
            frameFix.player2 = obj["correction"]["player2"];
            frameFix.isHolding = obj["down"];
            frameFix.buttonHold = obj["btn"];
            frameFix.velocity.x = obj["correction"]["xVel"];
            frameFix.velocity.y = obj["correction"]["yVel"];
            frameFix.realPos.x = obj["correction"]["xPos"];
            frameFix.realPos.y = obj["correction"]["yPos"];
            frameFix.pos.x = obj["correction"]["nodeXPos"];
            frameFix.pos.y = obj["correction"]["nodeYPos"];
            frameFix.rotation = obj["correction"]["rotation"];
            frameFix.rotation_speed = obj["correction"]["rotationRate"];
        }
	}

	gdr::json::object_t saveExtension() const override {
        if (frameFix.fix && Hacks::isHackEnabled("Frame Fix") && frameFix.type == MacroType::QUARTZ) {
		    //return { {"xpos", xpos} };
            /*
            frameFix.isHolding = obj["down"];
            frameFix.buttonHold = obj["btn"];
            frameFix.realPos.x = obj["qframe"]["realPosX"];
            frameFix.realPos.y = obj["qframe"]["realPosY"];
            frameFix.pos.x = obj["qframe"]["nodePosX"];
            frameFix.pos.y = obj["qframe"]["nodePosY"];
            frameFix.velocity.y = obj["qframe"]["velY"];
            frameFix.velocity.x = obj["qframe"]["velX"];
            frameFix.rotation = obj["qframe"]["rot"];
            frameFix.rotation_speed = obj["qframe"]["rot_speed"];
            frameFix.isRotating = obj["qframe"]["rotating"];
            frameFix.isSlope = obj["qframe"]["isSlope"];
            frameFix.wasSlope = obj["qframe"]["wasSlope"];

             */
            return { { "qframe_fix", true }, {
                "qframe", {
                    {"2p", frameFix.player2},
                    {"realPosX", frameFix.realPos.x},
                    {"realPosY", frameFix.realPos.y},
                    {"nodePosX", frameFix.pos.x},
                    {"nodePosY", frameFix.pos.y},
                    {"velX", frameFix.velocity.x},
                    {"velY", frameFix.velocity.y},
                    {"rot", frameFix.rotation},
                    {"rot_speed", frameFix.rotation_speed},
                    {"rotating", frameFix.isRotating},
                    {"isSlope", frameFix.isSlope},
                    {"wasSlope", frameFix.wasSlope}
                }
            } };
        } else {
            return {};
        }
	}
};

struct QuartzMacro : gdr::Replay<QuartzMacro, QuartzInput> {
    bool isEnabled = false;
    float loaf = 0;
    QuartzMacro() : Replay("Quartz", "1.0") {}
};

struct MacroFile {
    std::string file;
    std::string name;
    std::filesystem::path path;
    QuartzMacro macro;
};


extern QuartzMacro current_macro;

class MacroManager {
    public:
        static bool createMacro(std::string name, std::string desc, std::string fps, bool isJSON) {
            int intFPS = std::stoi(fps);
            auto saveDir = Mod::get()->getSaveDir().string();
            if (!std::filesystem::exists(saveDir + "/macros")) {
                std::filesystem::create_directory(saveDir + "/macros");
            }
            auto savePath = saveDir + "/macros/" + name + ".gdr";
            if (isJSON) savePath = savePath + ".json";
            if (std::filesystem::exists(savePath)) return false;
            if (std::filesystem::exists(savePath)) return false;
            QuartzMacro macro;
            macro.author = GJAccountManager::sharedState()->m_username;
            //macro.author = "FireeDev";
            macro.description = desc;
            macro.framerate = intFPS;
            macro.levelInfo.name = "";
            macro.levelInfo.id = 0;

            macro.gameVersion = 2.2074;
            std::ofstream f(savePath, std::ios::binary);
            auto data = macro.exportData(isJSON);

            f.write(reinterpret_cast<const char *>(data.data()), data.size());
            f.close();
            return true;
        }
        static QuartzMacro getMacro(std::string name) {
            auto saveDir = Mod::get()->getSaveDir().string();
            if (std::filesystem::exists(saveDir + "/macros")) {
                auto savePath = saveDir + "/macros/" + name;
                std::ifstream f(savePath, std::ios::binary);

                f.seekg(0, std::ios::end);
                size_t fileSize = f.tellg();
                f.seekg(0, std::ios::beg);

                std::vector<std::uint8_t> macroData(fileSize);

                f.read(reinterpret_cast<char *>(macroData.data()), fileSize);
                f.close();

                return QuartzMacro::importData(macroData, true, name.ends_with(".gdr.json"));
            } else {
                QuartzMacro macro;
                return macro;
            }
        }
        static bool exportMacro(std::string file, QuartzMacro macro) {
            if (macro.inputs.size() > 0) {
                auto max_it = std::max_element(macro.inputs.begin(), macro.inputs.end(), [](const QuartzInput& inputA, const QuartzInput& inputB) {
                    return inputA.frame < inputB.frame;
                });
                if (max_it != macro.inputs.end()) {
                    int lastFrame = max_it->frame;
                    log::info("Macro saved with {} inputs, and the last frame: {}", macro.inputs.size(), lastFrame);
                    macro.duration = lastFrame;
                }
            } else {
                log::warn("Macro was saved with no inputs!");
            }
            auto saveDir = Mod::get()->getSaveDir().string();
            if (!std::filesystem::exists(saveDir + "/macros")) {
                std::filesystem::create_directory(saveDir + "/macros");
            }
            auto savePath = saveDir + "/macros/" + file;
            if (!std::filesystem::exists(savePath)) return false;
            std::ofstream f(savePath, std::ios::binary);
            auto data = macro.exportData(file.ends_with(".gdr.json"));

            f.write(reinterpret_cast<const char *>(data.data()), data.size());
            f.close();
            return true;
        }
        static std::vector<MacroFile> getMacros() {
            auto saveDir = Mod::get()->getSaveDir().string();
            if (!std::filesystem::exists(saveDir + "/macros")) return {};
            std::vector<MacroFile> macros;
            for (const auto& entry : std::filesystem::directory_iterator(saveDir + "/macros")) {
                if (std::filesystem::is_regular_file(entry)) {
                    // remove the .gdr or .gdr.json 
                    std::string fileNameFull = entry.path().filename().string();
                    std::string fileName = entry.path().filename().string();
                    size_t dotPos = fileName.find_last_of('.');
                    if (dotPos != std::string::npos) {
                        std::string ext = fileName.substr(dotPos);
                        fileName = fileName.substr(0, dotPos);
                    }
                    std::ifstream f(entry.path(), std::ios::binary);

                    f.seekg(0, std::ios::end);
                    size_t fileSize = f.tellg();
                    f.seekg(0, std::ios::beg);

                    std::vector<std::uint8_t> macroData(fileSize);

                    f.read(reinterpret_cast<char *>(macroData.data()), fileSize);
                    f.close();

                    macros.push_back({
                        fileNameFull,
                        fileName,
                        entry.path(),
                        // false because why would we need to import input data!? that would be taking up so much unnecessary resources!
                        QuartzMacro::importData(macroData, false, fileNameFull.ends_with(".gdr.json"))
                    });
                }
            }
            return macros;
        }
};


class SelectMacroUI : public FLAlertLayer {
    protected:
        GJListLayer* m_listLayer;
        virtual bool init(
            float width,
            float height,
            Dropdown* dropdown
        );
        virtual void keyDown(cocos2d::enumKeyCodes) override;
        virtual void keybackClicked();
        CCArray* createCells();
    public:
        Dropdown* m_dropdown;
        void onClick(CCObject*);
        virtual void onClose(cocos2d::CCObject*);
        static constexpr const float s_defWidth = 360.0f;
        static constexpr const float s_defHeight = 240.0f;
        static SelectMacroUI* create(Dropdown* dropdown);
};

class MacroItemCell : public CCLayer { //TableViewCell {
    protected:
        float m_fWidth;
        float m_fHeight;
        CCMenu* menu;
        MacroFile m_file;
        SelectMacroUI* m_list;
        bool init(SelectMacroUI* list, MacroFile m_file);
        void draw() override;
        void onSelect(CCObject*);
        void onInfo(CCObject*);
        void onTrash(CCObject*);
        void onError(CCObject*);
    public:
        static MacroItemCell* create(SelectMacroUI* list, MacroFile m_file);
};

/*
class SelectMacroUI : public BrownAlertDelegate {
    protected:
        CCScrollLayerExt* m_scrollLayer;
        CCMenu* m_content;
        float m_fWidth = s_defWidth;
        float m_fHeight = s_defHeight;
        void createItems();
        virtual void setup();
        void renderPage();
    public:
        void onClick(CCObject*);
        void onLinkClick(CCObject*);
        static constexpr const float s_defWidth = 300.0f;
        static constexpr const float s_defHeight = 200.0f;
        static SelectMacroUI* create();
};*/

class CreateMacroUI : public BrownAlertDelegate {
    protected:
        float m_fWidth = s_defWidth;
        TextInput* m_inputName;
        TextInput* m_inputDesc;
        TextInput* m_fpsInput;
        float m_fHeight = s_defHeight;
        virtual void setup();
    public:
        void onClick(CCObject*);
        void onCreate(CCObject*);
        void onCreateJSON(CCObject*);
        void onInfo(CCObject*);
        void onLeftFPS(CCObject*);
        void onRightFPS(CCObject*);
        static constexpr const float s_defWidth = 250.0f;
        static constexpr const float s_defHeight = 200.0f;
        static CreateMacroUI* create();
};


// i forgot geode popups existed, maybe i should migrate to them
class GotoFrameUI : public geode::Popup<> {
protected:
    TextInput* m_frame;
    bool setup() override;
    void onInfoBtn(CCObject*) {
        FLAlertLayer::create(
            "About",
            "This will <cy>advance</c> to whichever frame you want to go to!\nPlease note that <cr>this may cause lag</c>, as it will reset the level and <cy>speed up</c> the game to reach the frame.\n\nMake sure you have <cy>Noclip</c> enabled if the macro <cr>is inaccurate</c>.",
            "OK"
        )->show();
    }
    void gotoFrameBtn(CCObject*);

public:
    static GotoFrameUI* create() {
        auto ret = new GotoFrameUI();
        if (ret->initAnchored(240.f, 160.f)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};

class ClearFramesUI : public geode::Popup<> {
protected:
    bool setup() override;
    void onInfoBtn(CCObject*) {
        FLAlertLayer::create(
            nullptr,
            "About",
            "This menu can <cy>clear</c> two frames!\nThe <cl>Select Left</c> button will set the <cp>Right Frame Selection</c> to the <cy>current frame</c> you are on, and the <cp>Left Frame Selection</c> to <cy>0</c>\nThe <cl>Select Right</c> button will set the <cp>Left Frame Selection</c> to the <cy>current frame</c> you are on, and the <cp>Right Frame Selection</c> to <cy>the maximum inputs</c>.\nThe <cg>indicator</c> on the top shows your current selection based on the <cy>input selections</c> you set.",
            "OK",
            nullptr,
            350.0F
        )->show();
    }

public:
    static ClearFramesUI* create() {
        auto ret = new ClearFramesUI();
        if (ret->initAnchored(240.f, 160.f)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};
