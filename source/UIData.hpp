#ifndef UIData_hpp
#define UIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "GenericAssetManager.hpp"
#include "ButtonAction.hpp"
#include <map>

enum class UIDataType : int {
	BUTTON, TEXT, IMAGE
};

class UIData : Data {
public:

	std::string UIKey;
	std::string uiBackgroundKey;
	UIDataType type;
	float x;
	float y;
	float width;
	float height;

	bool init(std::string uiKey, std::string bg, UIDataType t, float x, float y, float w, float h)
	{
		this->UIKey = uiKey;
		this->uiBackgroundKey = bg;
		this->type = t;
		this->x = x;
		this->y = y;
		this->width = w;
		this->height = h;
		return true;
	}

	UIData() : Data() {}

	static std::shared_ptr<UIData> alloc(std::string uiKey, std::string bg, UIDataType t, float x, float y, float w, float h) {
		std::shared_ptr<UIData> result = std::make_shared<UIData>();
		return (result->init(uiKey, bg, t, x, y, w, h) ? result : nullptr);
	}

	virtual std::shared_ptr<cugl::Node> dataToNode(std::shared_ptr<GenericAssetManager> assets,std::map<std::string,std::string> fontMap);

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue();

	virtual std::string serialize();

	virtual bool preload(const std::string& file);

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);

	virtual bool materialize();
};

class ButtonUIData : public UIData {
public:
	std::shared_ptr<ButtonAction> buttonAction;
	std::string buttonLabel;
    std::string fontKey;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	virtual std::shared_ptr<cugl::Node> dataToNode(std::shared_ptr<GenericAssetManager> assets,std::map<std::string,std::string> fontMap) override;

	bool init(std::string uiKey, std::string bg, float x, float y, float w, float h, std::shared_ptr<ButtonAction> ba, std::string bLabel)
	{
		UIData::init(uiKey, bg, UIDataType::BUTTON, x, y, w, h);
		buttonAction = ba;
		buttonLabel = bLabel;
		return true;
	}

	static std::shared_ptr<ButtonUIData> alloc(std::string uiKey, std::string bg, float x, float y, float w, float h, std::shared_ptr<ButtonAction> ba, std::string bLabel) {
		std::shared_ptr<ButtonUIData> result = std::make_shared<ButtonUIData>();
		return (result->init(uiKey, bg, x, y, w, h, ba, bLabel) ? result : nullptr);
	}

	ButtonUIData() : UIData() {}
};

class TextUIData : public UIData {
public:
	std::string textValue;
	std::string fontKey;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	virtual std::shared_ptr<cugl::Node> dataToNode(std::shared_ptr<GenericAssetManager> assets,std::map<std::string,std::string> fontMap) override;

	static std::shared_ptr<TextUIData> alloc(std::string uiKey, std::string bg, float x, float y, float w, float h, std::string tv, std::string fKey) {
		std::shared_ptr<TextUIData> result = std::make_shared<TextUIData>();
		return (result->init(uiKey, bg, x, y, w, h, tv, fKey) ? result : nullptr);
	}
	bool init(std::string uiKey, std::string bg, float x, float y, float w, float h, std::string tv, std::string fKey)
	{
		UIData::init(uiKey, bg, UIDataType::TEXT, x, y, w, h);
		textValue = tv;
		fontKey = fKey;
		return true;
	}

	TextUIData() : UIData() {}
};

class ImageUIData : public UIData {
public:
	std::string textureKey;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	virtual std::shared_ptr<cugl::Node> dataToNode(std::shared_ptr<GenericAssetManager> assets,std::map<std::string,std::string> fontMap) override;

	static std::shared_ptr<ImageUIData> alloc(std::string uiKey, std::string bg, float x, float y, float w, float h, std::string tKey) {
		std::shared_ptr<ImageUIData> result = std::make_shared<ImageUIData>();
		return (result->init(uiKey, bg, x, y, w, h, tKey) ? result : nullptr);
	}
	bool init(std::string uiKey, std::string bg, float x, float y, float w, float h, std::string tKey)
	{
		UIData::init(uiKey, bg, UIDataType::IMAGE, x, y, w, h);
		textureKey = tKey;
		return true;
	}

	ImageUIData() : UIData() {}
};



#endif /* UIData_hpp */
