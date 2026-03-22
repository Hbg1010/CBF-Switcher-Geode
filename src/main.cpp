#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

#define GET_SETTING_BOOL(mod, key) mod->getSettingValue<bool>(key)
#define GET_SETTING_STRING(mod, key) mod->getSettingValue<std::string>(key)

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
static void setCBFModeInLevel(bool isPlat) {
    auto ThisMod = Mod::get();
    if (auto CBF = Loader::get()->getLoadedMod("syzzi.click_between_frames")) {
        std::string modeName;
        if (isPlat) {
            modeName = GET_SETTING_STRING(ThisMod, "platformerSettings"); 
        } else {
            modeName = GET_SETTING_STRING(ThisMod, "classicSetting");
        }

        CBFMode mode = modeFromString(modeName);
        setCbfMode(mode, CBF);
    }
    
}

class $modify(plStuff, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!GET_SETTING_BOOL(Mod::get(), "softtoggle")) {
            // editor override  
            if (level->m_levelID == 0 && GET_SETTING_BOOL(Mod::get(), "ToggleCBFInEditor")) {
                if (auto CBF = Loader::get()->getLoadedMod("syzzi.click_between_frames")) {
                    std::string cbfMode = GET_SETTING_STRING(Mod::get(), "EditorMode");
                    setCbfMode(
                        modeFromString(cbfMode),
                        CBF
                    );
                }
            // not editor lol
            } else {
                setCBFModeInLevel(level->isPlatformer());
            }
        }
        return PlayLayer::init(level, useReplay, dontCreateObjects);
    }
};