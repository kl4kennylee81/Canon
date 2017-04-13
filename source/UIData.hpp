#ifndef UIData_hpp
#define UIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "GenericAssetManager.hpp"

enum class UIDataType : int {
	BUTTON, TEXT, IMAGE
};

class ButtonAction {
public:

	std::string type;
	std::string buttonTarget;

	bool init(std::string t, std::string bt)
	{
		type = t;
		buttonTarget = bt;
		return true;
	}

	ButtonAction() {}

	static std::shared_ptr<ButtonAction> alloc(std::string t, std::string bt) {
		std::shared_ptr<ButtonAction> result = std::make_shared<ButtonAction>();
		return (result->init(t, bt) ? result : nullptr);
	}

	static ButtonAction getButtonAction(const std::shared_ptr<cugl::JsonValue> & json)
	{
		ButtonAction result = ButtonAction();
		result.init(json->getString("type"), json->getString("buttonTarget"));
		return result;
	}
    
};


class UIData : Data {
public:

	std::string UIKey;
	UIDataType type;
	int x;
	int y;
	int width;
	int height;

	bool init(std::string uiKey, UIDataType t, int x, int y, int w, int h)
	{
		this->UIKey = uiKey;
		this->type = t;
		this->x = x;
		this->y = y;
		this->width = w;
		this->height = h;
        return true;
	}

	UIData() : Data() {}

	static std::shared_ptr<UIData> alloc(std::string uiKey, UIDataType t, int x, int y, int w, int h) {
		std::shared_ptr<UIData> result = std::make_shared<UIData>();
		return (result->init(uiKey, t, x, y, w, h) ? result : nullptr);
	}
    
    virtual std::shared_ptr<cugl::Node> dataToNode(std::shared_ptr<GenericAssetManager> assets);

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue();

	virtual std::string serialize();

	virtual bool preload(const std::string& file);

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);

	virtual bool materialize();
};

class ButtonUIData : public UIData {
public:
	ButtonAction buttonAction;
	std::string buttonLabel;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    virtual std::shared_ptr<cugl::Node> dataToNode(std::shared_ptr<GenericAssetManager> assets) override;
    
	static std::shared_ptr<ButtonUIData> alloc(std::string uiKey, UIDataType t, int x, int y, int w, int h, ButtonAction ba, std::string bLabel) {
		std::shared_ptr<ButtonUIData> result = std::make_shared<ButtonUIData>();
		return (result->init( uiKey,  t,  x,  y,  w,  h,  ba,  bLabel) ? result : nullptr);
	}
	bool init(std::string uiKey, UIDataType t, int x, int y, int w, int h, ButtonAction ba, std::string bLabel)
	{
		UIData::alloc(uiKey, t, x, y, w, h);
		buttonAction = ba;
		buttonLabel = bLabel;
		return true;
	}
};

class TextUIData : public UIData {
public:
	std::string textValue;
	std::string fontKey;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    virtual std::shared_ptr<cugl::Node> dataToNode(std::shared_ptr<GenericAssetManager> assets) override;
    
	static std::shared_ptr<TextUIData> alloc(std::string uiKey, UIDataType t, int x, int y, int w, int h, std::string tv, std::string fKey) {
		std::shared_ptr<TextUIData> result = std::make_shared<TextUIData>();
		return (result->init(uiKey, t, x, y, w, h, tv, fKey) ? result : nullptr);
	}
	bool init(std::string uiKey, UIDataType t, int x, int y, int w, int h, std::string tv, std::string fKey)
	{
		UIData::alloc(uiKey, t, x, y, w, h);
		textValue = tv;
		fontKey = fKey;
		return true;
	}
};

class ImageUIData : public UIData {
public:
	std::string textureKey;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    virtual std::shared_ptr<cugl::Node> dataToNode(std::shared_ptr<GenericAssetManager> assets) override;
    
	static std::shared_ptr<ImageUIData> alloc(std::string uiKey, UIDataType t, int x, int y, int w, int h, std::string tKey) {
		std::shared_ptr<ImageUIData> result = std::make_shared<ImageUIData>();
		return (result->init(uiKey, t, x, y, w, h, tKey) ? result : nullptr);
	}
	bool init(std::string uiKey, UIDataType t, int x, int y, int w, int h, std::string tKey)
	{
		UIData::alloc(uiKey, t, x, y, w, h);
		textureKey = tKey;
		return true;
	}
};



#endif /* UIData_hpp */
