#include "UIData.hpp"
#include "GameState.hpp"
#include "Util.hpp"

using namespace cugl;


bool ButtonUIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
    std::string buttonType = json->get("buttonAction")->getString("type");
    
	if (buttonType == "menuChange")
    {
        auto buttonJson = json->get("buttonAction");
        buttonAction = MenuChangeButtonAction::alloc(buttonJson->getString("buttonTarget"));
    }
	else if (buttonType == "modeChange")
    {
        auto buttonJson = json->get("buttonAction");
        buttonAction = ModeChangeButtonAction::alloc(buttonJson->getString("buttonTarget"),buttonJson->getString("nextScreen"));
    }
	else if (buttonType == "fxTrigger")
    {
        auto buttonJson = json->get("buttonAction");
        buttonAction = FxTriggerButtonAction::alloc(buttonJson->getString("fxKey"),buttonJson->getString("nextScreen"),buttonJson->getString("chapterData"));
    }
    fontKey = json->getString("fontKey");
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

std::shared_ptr<cugl::Node> ButtonUIData::dataToNode(std::shared_ptr<GenericAssetManager> assets,std::map<std::string,std::string> fontMap){
    
	std::shared_ptr<Button> button;

	if (uiBackgroundKey != "") {
		auto buttonTexture = PolygonNode::allocWithTexture(assets->get<Texture>(uiBackgroundKey));
		button = Button::alloc(buttonTexture);
        button->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        button->setPosition(Vec2(this->x * GAME_SCENE_WIDTH, this->y * Util::getGameSceneHeight()));
        
        // scale to width and height
        cugl::Size size = button->getSize();
        button->setScale(Vec2(this->width * GAME_SCENE_WIDTH / size.width, this->height * Util::getGameSceneHeight() / size.height));
	}
	
    // we put the label as a seperate entity from the button so we don't scale the font
    // we also want labels to be centered like textData
    else if (buttonLabel != "") {
        std::shared_ptr<Label> label;
        std::string actualFont = fontMap.at(this->fontKey);
		label = Label::alloc(buttonLabel, assets->get<Font>(actualFont));
		label->setForeground(Color4::WHITE);
        
		button = Button::alloc(label);
        float labelX = (this->width/2.0 + this->x) * GAME_SCENE_WIDTH;
        float labelY = (this->height/2.0 + this->y) * Util::getGameSceneHeight();
        
        button->setPosition(labelX,labelY);
        button->setAnchor(Vec2::ANCHOR_MIDDLE);
    }

    return button;
}

std::shared_ptr<cugl::Node> TextUIData::dataToNode(std::shared_ptr<GenericAssetManager> assets,std::map<std::string,std::string> fontMap){
    std::string actualFont = fontMap.at(this->fontKey);
    std::shared_ptr<Label> label = Label::alloc(textValue, assets->get<Font>(actualFont));
	// maybe consider having option to set color in data file
	label->setForeground(Color4::WHITE);
	label->setAnchor(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(this->x * GAME_SCENE_WIDTH, this->y * Util::getGameSceneHeight()));
    
    // don't scale to width and height to keep font size uniform
    
    return label;
}

std::shared_ptr<cugl::Node> ImageUIData::dataToNode(std::shared_ptr<GenericAssetManager> assets,std::map<std::string,std::string> fontMap){
    std::shared_ptr<Node> imageNode = PolygonNode::allocWithTexture(assets->get<Texture>(this->textureKey));
	imageNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    imageNode->setPosition(Vec2(this->x * GAME_SCENE_WIDTH , this->y * Util::getGameSceneHeight()));

    // scale to width and height
	cugl::Size size = imageNode->getSize();
	imageNode->setScale(Vec2(this->width * GAME_SCENE_WIDTH / size.width, this->height * Util::getGameSceneHeight() / size.height));
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
	Data::preload(json);
	return true;
}

bool UIData::materialize() {
	return true;
}

std::shared_ptr<cugl::Node> UIData::dataToNode(std::shared_ptr<GenericAssetManager> assets,std::map<std::string,std::string> fontMap){
    return Node::alloc();
}
