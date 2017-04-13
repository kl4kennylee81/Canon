#include <stdio.h>
#include <cugl/cugl.h>

using std::string;

class ButtonAction {
public:

	string type;

	virtual bool init(string t)
	{
		type = t;
		return true;
	}

	ButtonAction() {}

	static std::shared_ptr<ButtonAction> alloc(string t) {
		std::shared_ptr<ButtonAction> result = std::make_shared<ButtonAction>();
		return (result->init(t) ? result : nullptr);
	}
};

class UIChangeButtonAction : ButtonAction {
public:
	string uiTarget;
	virtual bool init(string t, string tar)
	{
		ButtonAction::alloc(t);
		uiTarget = tar;
		return true;
	}
	UIChangeButtonAction() : ButtonAction() {}
	static std::shared_ptr<UIChangeButtonAction> alloc(string t, string tar) {
		std::shared_ptr<UIChangeButtonAction> result = std::make_shared<UIChangeButtonAction>();
		return (result->init(t, tar) ? result : nullptr);
	}
};

class MenuChangeButtonAction : ButtonAction {
public:
	string menuTarget;
	virtual bool init(string t, string tar)
	{
		ButtonAction::alloc(t);
		menuTarget = tar;
		return true;
	}
	MenuChangeButtonAction() : ButtonAction() {}
	static std::shared_ptr<MenuChangeButtonAction> alloc(string t, string tar) {
		std::shared_ptr<MenuChangeButtonAction> result = std::make_shared<MenuChangeButtonAction>();
		return (result->init(t, tar) ? result : nullptr);
	}
};

class FxTriggerButtonAction : ButtonAction {
public:
	string fxKey;
	virtual bool init(string t, string fx)
	{
		ButtonAction::alloc(t);
		fxKey = fx;
		return true;
	}
	FxTriggerButtonAction() : ButtonAction() {}
	static std::shared_ptr<FxTriggerButtonAction> alloc(string t, string fx) {
		std::shared_ptr<FxTriggerButtonAction> result = std::make_shared<FxTriggerButtonAction>();
		return (result->init(t, fx) ? result : nullptr);
	}
};