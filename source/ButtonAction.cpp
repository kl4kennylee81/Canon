#include "ButtonAction.hpp"

using namespace cugl;

bool ButtonAction::preload(const std::shared_ptr<cugl::JsonValue>& json){
	std::string activeKey = json->getString("active");
	init(activeKey);
    return true;
}

bool MenuChangeButtonAction::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string bt = json->getString("buttonTarget");
	std::string activeKey = json->getString("active");
    init(activeKey, bt);
	ButtonAction::preload(json);
    return true;
}

bool ModeChangeButtonAction::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string bt = json->getString("buttonTarget");
    std::string nScreen = json->getString("nextScreen");
    std::string nLevel = json->getString("nextLevel");
	std::string activeKey = json->getString("active");
    init(activeKey, stringToMode(bt),nLevel,nScreen);
	ButtonAction::preload(json);
    return true;
}

bool FxTriggerButtonAction::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string fx = json->getString("fxKey");
    std::string nScreen = json->getString("nextScreen");
	std::string cData = json->getString("chapterData");
	std::string activeKey = json->getString("active");
    init(activeKey, fx, nScreen, cData);
	ButtonAction::preload(json);
    return true;
}
