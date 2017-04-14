#include "UIData.hpp"

using namespace cugl;


bool ButtonUIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
    std::string buttonType = json->get("buttonAction")->getString("type");

	if (buttonType == "menuChange") { buttonAction = std::dynamic_pointer_cast<ButtonAction>(
		MenuChangeButtonAction::alloc(ButtonActionType::MENUCHANGE, json->get("buttonAction")->getString("buttonTarget"))); }
	else if (buttonType == "modeChange") { buttonAction = std::dynamic_pointer_cast<ButtonAction>(
		ModeChangeButtonAction::alloc(ButtonActionType::MODECHANGE, json->get("buttonAction")->getString("buttonTarget"))); }
	else if (buttonType == "fxTrigger") { buttonAction = std::dynamic_pointer_cast<ButtonAction>(
		FxTriggerButtonAction::alloc(ButtonActionType::FXTRIGGER, json->getString("fxKey"))); }

	uiBackgroundKey = json->getString("uiBackgroundKey");
	buttonLabel = json->getString("buttonLabel");
    type = UIDataType::BUTTON;
	UIData::preload(json); // call to super
	return true;
}

bool TextUIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	textValue = json->getString("textValue");
	fontKey = json->getString("fontKey");
    type = UIDataType::TEXT;
	UIData::preload(json);
	return true;
}

bool ImageUIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	textureKey = json->getString("textureKey");
    type = UIDataType::IMAGE;
	UIData::preload(json);
	return true;
}

std::shared_ptr<cugl::Node> ButtonUIData::dataToNode(std::shared_ptr<GenericAssetManager> assets){
    return Node::alloc();
}

std::shared_ptr<cugl::Node> TextUIData::dataToNode(std::shared_ptr<GenericAssetManager> assets){
    std::shared_ptr<Label> label = Label::alloc(textValue, assets->get<Font>(fontKey));
    label->setPosition(Vec2(this->x,this->y));
    
    // scale to width and height
    return label;
}

std::shared_ptr<cugl::Node> ImageUIData::dataToNode(std::shared_ptr<GenericAssetManager> assets){
    std::shared_ptr<Node> imageNode = PolygonNode::allocWithTexture(assets->get<Texture>(this->textureKey));
    imageNode->setPosition(Vec2(this->x,this->y));
    
    // scale to width and height
    return imageNode;
}


std::shared_ptr<JsonValue> UIData::toJsonValue()
{
	return JsonValue::allocNull();
}

std::string UIData::serialize() {
	return toJsonValue()->toString();
}

bool UIData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool UIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	UIKey = json->key();
	x = json->getInt("x");
	y = json->getInt("y");
	width = json->getInt("width");
	height = json->getInt("height");
	return true;
}

bool UIData::materialize() {
	return true;
}

std::shared_ptr<cugl::Node> UIData::dataToNode(std::shared_ptr<GenericAssetManager> assets){
    return Node::alloc();
}
