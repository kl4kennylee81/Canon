#include <stdio.h>
#include <cugl/cugl.h>
#include "Mode.hpp"

enum class ButtonActionType : int {
	MENUCHANGE, MODECHANGE, FXTRIGGER
};

class ButtonAction {
public:

	ButtonActionType type;

    virtual bool init(ButtonActionType t)
	{
		type = t;
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
        ButtonAction::init(ButtonActionType::MENUCHANGE);
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
	std::string nextLevel;
    std::string nextScreen;
    
    virtual bool init(Mode mode, std::string nextLevel = "", std::string nextScreen = ""){
        ButtonAction::init(ButtonActionType::MODECHANGE);
        this->modeTarget = mode;
        this->nextScreen = nextScreen;
		this->nextLevel = nextLevel;
        return true;
    }
    
    virtual bool init(std::string mode,std::string nextScreen)
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


	static std::shared_ptr<ModeChangeButtonAction> alloc(Mode mode, std::string nextLevel, std::string nextScreen) {
		std::shared_ptr<ModeChangeButtonAction> result = std::make_shared<ModeChangeButtonAction>();
		return (result->init(mode, nextLevel, nextScreen) ? result : nullptr);
	}
};

class FxTriggerButtonAction : public ButtonAction {
public:
    
    enum class FXType : int {
        PAUSE, RESUME, NONE
    };

    FXType fxKey;
    std::string nextScreen;
    
    static FXType stringToFXType(std::string event) {
        if (event == "PAUSE") return FXType::PAUSE;
        if (event == "RESUME") return FXType::RESUME;
        return FXType::NONE;
    }
    
    virtual bool init(std::string fx,std::string next)
	{
        ButtonAction::init(ButtonActionType::FXTRIGGER);
		fxKey = stringToFXType(fx);
        this->nextScreen = next;
		return true;
	}
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    
	FxTriggerButtonAction() : ButtonAction() {}
    static std::shared_ptr<FxTriggerButtonAction> alloc(std::string fx,std::string next) {
		std::shared_ptr<FxTriggerButtonAction> result = std::make_shared<FxTriggerButtonAction>();
		return (result->init(fx,next) ? result : nullptr);
	}
};
