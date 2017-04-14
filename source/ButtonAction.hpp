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
	MenuChangeButtonAction() : ButtonAction() {}
    static std::shared_ptr<MenuChangeButtonAction> alloc(std::string tar) {
		std::shared_ptr<MenuChangeButtonAction> result = std::make_shared<MenuChangeButtonAction>();
		return (result->init(tar) ? result : nullptr);
	}
};

class ModeChangeButtonAction : public ButtonAction {
public:
    Mode modeTarget;
    std::string nextScreen; // key to the next screen/next level handled differently depending on the mode
    
    virtual bool init(Mode mode,std::string nextScreen){
        ButtonAction::init(ButtonActionType::MODECHANGE);
        modeTarget = mode;
        nextScreen = nextScreen;
        return true;
    }
    
    virtual bool init(std::string mode,std::string nextScreen)
	{
        return init(stringToMode(mode),nextScreen);
	}
    
	ModeChangeButtonAction() : ButtonAction() {}
    
    static std::shared_ptr<ModeChangeButtonAction> alloc(Mode mode,std::string nextScreen) {
        std::shared_ptr<ModeChangeButtonAction> result = std::make_shared<ModeChangeButtonAction>();
        return (result->init(mode,nextScreen) ? result : nullptr);
    }
    
    static std::shared_ptr<ModeChangeButtonAction> alloc(std::string mode,std::string nextScreen) {
		std::shared_ptr<ModeChangeButtonAction> result = std::make_shared<ModeChangeButtonAction>();
		return (result->init(mode,nextScreen) ? result : nullptr);
	}
};

class FxTriggerButtonAction : public ButtonAction {
public:
    std::string fxKey;
    virtual bool init(std::string fx)
	{
        ButtonAction::init(ButtonActionType::FXTRIGGER);
		fxKey = fx;
		return true;
	}
	FxTriggerButtonAction() : ButtonAction() {}
    static std::shared_ptr<FxTriggerButtonAction> alloc(std::string fx) {
		std::shared_ptr<FxTriggerButtonAction> result = std::make_shared<FxTriggerButtonAction>();
		return (result->init(fx) ? result : nullptr);
	}
};
