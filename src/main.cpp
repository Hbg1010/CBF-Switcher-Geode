#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// represents cbf modes
enum CBFMode {
    Disabled,
    CBF,
    COS
};

// sets CBF mode using enum
static void setCbfMode(CBFMode setting, Mod* CBF) {
    CBF->setSettingValue("soft-toggle", CBFMode::Disabled == setting);
    CBF->setSettingValue("click-on-steps", CBFMode::COS == setting);
}

// turns string from settings into enum
static CBFMode modeFromString(std::string& mode) {
    if (mode == "CBF") {
        return CBFMode::CBF;
    } else if (mode == "COS") {
        return CBFMode::COS;
    } else {
        return CBFMode::Disabled;
    }
}

// sets entirely based off of editor mode
static void setCBFMode(bool isPlat) {
    auto ThisMod = Mod::get();
    
    if (!ThisMod->getSettingValue<bool>("softtoggle")) {
        if (auto CBF = Loader::get()->getLoadedMod("syzzi.click_between_frames")) {
            std::string modeName;
            if (isPlat) {
                modeName = ThisMod->getSettingValue<std::string>("platformerSettings"); 
            } else {
                modeName = ThisMod->getSettingValue<std::string>("classicSetting");
            }

            CBFMode mode = modeFromString(modeName);
            setCbfMode(mode, CBF);
        }
    }
}

class $modify(plStuff, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        setCBFMode(level->isPlatformer()); // happens before
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        return true;
    }
};

class $modify(EUIToggle, EditorUI) {
    bool init(LevelEditorLayer* level) {
        Mod* modpointer = Mod::get();
        if (modpointer->getSettingValue<bool>("ToggleCBFInEditor")) {
            if (auto CBF = Loader::get()->getLoadedMod("syzzi.click_between_frames")) {
                std::string cbfMode = modpointer->getSettingValue<std::string>("EditorMode");
                setCbfMode(
                    modeFromString(cbfMode),
                    CBF
                );
            }
        }
        return EditorUI::init(level);
    }
};