#include <stdio.h>
#include <cugl/cugl.h>

class ButtonAction {
public:

 std::string type;

    virtual bool init(std::string t)
	{
		type = t;
		return true;
	}

	ButtonAction() {}

    static std::shared_ptr<ButtonAction> alloc(std::string t) {
		std::shared_ptr<ButtonAction> result = std::make_shared<ButtonAction>();
		return (result->init(t) ? result : nullptr);
	}
};

class UIChangeButtonAction : public ButtonAction {
public:
    std::string uiTarget;
    virtual bool init(std::string t, std::string tar)
	{
		ButtonAction::alloc(t);
		uiTarget = tar;
		return true;
	}
	UIChangeButtonAction() : ButtonAction() {}
    static std::shared_ptr<UIChangeButtonAction> alloc(std::string t, std::string tar) {
		std::shared_ptr<UIChangeButtonAction> result = std::make_shared<UIChangeButtonAction>();
		return (result->init(t, tar) ? result : nullptr);
	}
};

class MenuChangeButtonAction : public ButtonAction {
public:
    std::string menuTarget;
    virtual bool init(std::string t, std::string tar)
	{
		ButtonAction::alloc(t);
		menuTarget = tar;
		return true;
	}
	MenuChangeButtonAction() : ButtonAction() {}
    static std::shared_ptr<MenuChangeButtonAction> alloc(std::string t, std::string tar) {
		std::shared_ptr<MenuChangeButtonAction> result = std::make_shared<MenuChangeButtonAction>();
		return (result->init(t, tar) ? result : nullptr);
	}
};

class FxTriggerButtonAction : public ButtonAction {
public:
    std::string fxKey;
    virtual bool init(std::string t, std::string fx)
	{
		ButtonAction::alloc(t);
		fxKey = fx;
		return true;
	}
	FxTriggerButtonAction() : ButtonAction() {}
    static std::shared_ptr<FxTriggerButtonAction> alloc(std::string t, std::string fx) {
		std::shared_ptr<FxTriggerButtonAction> result = std::make_shared<FxTriggerButtonAction>();
		return (result->init(t, fx) ? result : nullptr);
	}
};
