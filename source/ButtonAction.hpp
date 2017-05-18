#include <stdio.h>
#include <cugl/cugl.h>
#include "Mode.hpp"

enum class ButtonActionType : int {
	MENU_CHANGE, MODE_CHANGE, FX_TRIGGER
};

enum class ChapterSwitchData : int {
	NEXT, PREVIOUS
};

class ButtonAction {
public:

	ButtonActionType type;
	std::string active;

    virtual bool init(ButtonActionType t)
	{
		type = t;
		return true;
	}

	virtual bool init(std::string activeKey) {
		active = activeKey;
		return true;
	}
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);

	ButtonAction() {}

    static std::shared_ptr<ButtonAction> alloc(ButtonActionType t) {
		std::shared_ptr<ButtonAction> result = std::make_shared<ButtonAction>();
		return (result->init(t) ? result : nullptr);
	}
};

class MenuChangeButtonAction : public ButtonAction {
public:
    std::string menuTarget;
    virtual bool init(std::string tar)
	{
        ButtonAction::init(ButtonActionType::MENU_CHANGE);
		menuTarget = tar;
		return true;
	}
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
	MenuChangeButtonAction() : ButtonAction() {}
    static std::shared_ptr<MenuChangeButtonAction> alloc(std::string tar) {
		std::shared_ptr<MenuChangeButtonAction> result = std::make_shared<MenuChangeButtonAction>();
		return (result->init(tar) ? result : nullptr);
	}
};

class ModeChangeButtonAction : public ButtonAction {
public:
    Mode modeTarget;
    std::string levelSelectDataKey; // key to the static levelSelectData selected (the next level)
    std::string nextScreen;
    
    
    virtual bool init(Mode mode, std::string nextScreen = "",std::string lsdKey = ""){
        ButtonAction::init(ButtonActionType::MODE_CHANGE);
        this->modeTarget = mode;
        this->nextScreen = nextScreen;
        this->levelSelectDataKey = lsdKey;
        return true;
    }
    
    virtual bool init(std::string mode,std::string nextScreen = "",std::string nextLevel = "")
	{
        return init(stringToMode(mode),nextScreen);
	}
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
	ModeChangeButtonAction() : ButtonAction() {}
    
    static std::shared_ptr<ModeChangeButtonAction> alloc(Mode mode,std::string nextScreen) {
        std::shared_ptr<ModeChangeButtonAction> result = std::make_shared<ModeChangeButtonAction>();
        return (result->init(mode,nextScreen) ? result : nullptr);
    }
    
    static std::shared_ptr<ModeChangeButtonAction> alloc(std::string mode,std::string nextScreen) {
		std::shared_ptr<ModeChangeButtonAction> result = std::make_shared<ModeChangeButtonAction>();
		return (result->init(mode,nextScreen) ? result : nullptr);
	}


	static std::shared_ptr<ModeChangeButtonAction> alloc(Mode mode, std::string nextScreen, std::string lsdKey) {
		std::shared_ptr<ModeChangeButtonAction> result = std::make_shared<ModeChangeButtonAction>();
		return (result->init(mode, nextScreen, lsdKey) ? result : nullptr);
	}
};

class FxTriggerButtonAction : public ButtonAction {
public:
    
    enum class FXType : int {
        PAUSE, RESUME, RETRY, SWITCH_CHAPTER, NONE
    };

    FXType fxKey;
    std::string nextScreen;
	std::string chapterData;
    
    static FXType stringToFXType(std::string event) {
        if (event == "PAUSE") return FXType::PAUSE;
        if (event == "RESUME") return FXType::RESUME;
        if (event == "RETRY") return FXType::RETRY;
		if (event == "SWITCH_CHAPTER") return FXType::SWITCH_CHAPTER;
        return FXType::NONE;
    }

	static ChapterSwitchData stringToChapterSwitchData(std::string cData) {
		if (cData == "NEXT") return ChapterSwitchData::NEXT;
		if (cData == "PREVIOUS") return ChapterSwitchData::PREVIOUS;
		return ChapterSwitchData::NEXT;
	}
    
    virtual bool init(std::string fx, std::string next, std::string cd)
	{
        ButtonAction::init(ButtonActionType::FX_TRIGGER);
		fxKey = stringToFXType(fx);
        nextScreen = next;
		chapterData = cd;
		return true;
	}
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    
	FxTriggerButtonAction() : ButtonAction() {}
    static std::shared_ptr<FxTriggerButtonAction> alloc(std::string fx,std::string next, std::string cd) {
		std::shared_ptr<FxTriggerButtonAction> result = std::make_shared<FxTriggerButtonAction>();
		return (result->init(fx, next, cd) ? result : nullptr);
	}
};
