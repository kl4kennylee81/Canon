#include "UIData.hpp"
#include "GameState.hpp"

using namespace cugl;


bool ButtonUIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
    std::string buttonType = json->get("buttonAction")->getString("type");

	if (buttonType == "menuChange") { buttonAction = std::dynamic_pointer_cast<ButtonAction>(
		MenuChangeButtonAction::alloc(json->get("buttonAction")->getString("buttonTarget"))); }
	else if (buttonType == "modeChange") { buttonAction = std::dynamic_pointer_cast<ButtonAction>(
		ModeChangeButtonAction::alloc(json->get("buttonAction")->getString("buttonTarget"),json->get("buttonAction")->getString("nextScreen"))); }
	else if (buttonType == "fxTrigger") { buttonAction = std::dynamic_pointer_cast<ButtonAction>(
		FxTriggerButtonAction::alloc(json->getString("fxKey"))); }

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
    auto buttonTexture = PolygonNode::allocWithTexture(assets->get<Texture>(uiBackgroundKey));
    std::shared_ptr<Button> button = Button::alloc(buttonTexture);
	button->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	button->setPosition(Vec2(this->x * GAME_SCENE_WIDTH, this->y * GameState::getGameSceneHeight()));

	// scale to width and height
	cugl::Size size = button->getSize();
	button->setScale(Vec2(this->width * GAME_SCENE_WIDTH / size.width, this->height * GameState::getGameSceneHeight() / size.height));

    return button;
}

std::shared_ptr<cugl::Node> TextUIData::dataToNode(std::shared_ptr<GenericAssetManager> assets){
    std::shared_ptr<Label> label = Label::alloc(textValue, assets->get<Font>(fontKey));
	label->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    label->setPosition(Vec2(this->x * GAME_SCENE_WIDTH, this->y * GameState::getGameSceneHeight()));

	cugl::Size size = label->getSize();
	label->setScale(Vec2(this->width * GAME_SCENE_WIDTH / size.width, this->height * GameState::getGameSceneHeight() / size.height));
    
    // scale to width and height
    return label;
}

std::shared_ptr<cugl::Node> TextUIData::dataToNode(std::shared_ptr<GenericAssetManager> assets, std::string text) {
	std::shared_ptr<Label> label = Label::alloc(textValue, assets->get<Font>(fontKey));
	label->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	label->setPosition(Vec2(this->x * GAME_SCENE_WIDTH, this->y * GameState::getGameSceneHeight()));

	label->setText(text, true);
	// todo: have the color be part of the uidata files?
	label->setForeground(Color4::WHITE);
	cugl::Size size = label->getSize();
	label->setScale(Vec2(this->width * GAME_SCENE_WIDTH / size.width, this->height * GameState::getGameSceneHeight() / size.height));

	// scale to width and height
	return label;
}

std::shared_ptr<cugl::Node> ImageUIData::dataToNode(std::shared_ptr<GenericAssetManager> assets){
    std::shared_ptr<Node> imageNode = PolygonNode::allocWithTexture(assets->get<Texture>(this->textureKey));
	imageNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    imageNode->setPosition(Vec2(this->x * GAME_SCENE_WIDTH , this->y * GameState::getGameSceneHeight()));

    // scale to width and height
	cugl::Size size = imageNode->getSize();
	imageNode->setScale(Vec2(this->width * GAME_SCENE_WIDTH / size.width, this->height * GameState::getGameSceneHeight() / size.height));
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
	x = json->getFloat("x");
	y = json->getFloat("y");
	width = json->getFloat("width");
	height = json->getFloat("height");
	return true;
}

bool UIData::materialize() {
	return true;
}

std::shared_ptr<cugl::Node> UIData::dataToNode(std::shared_ptr<GenericAssetManager> assets){
    return Node::alloc();
}
