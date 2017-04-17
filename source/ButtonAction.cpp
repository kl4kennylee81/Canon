#include "ButtonAction.hpp"

using namespace cugl;

bool ButtonAction::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool MenuChangeButtonAction::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string bt = json->getString("buttonTarget");
    init(bt);
    return true;
}

bool ModeChangeButtonAction::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string bt = json->getString("buttonTarget");
    std::string nScreen = json->getString("nextScreen");
    std::string nLevel = json->getString("nextLevel");
    init(stringToMode(bt),nLevel,nScreen);
    return true;
}

bool FxTriggerButtonAction::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string fx = json->getString("fxKey");
    std::string nScreen = json->getString("nextScreen");
    init(fx,nScreen);
    return true;
}
