#ifndef UIData_hpp
#define UIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"

using std::string;


enum class UIDataType : int {
	BUTTON, TEXT, IMAGE
};

class UIData : Data {
public:

	string UIKey;
	UIDataType type;
	int x;
	int y;
	int width;
	int height;

	bool init(string uiKey, UIDataType t, int x, int y, int w, int h)
	{
		UIKey = uiKey;
		type = t;
		this->x = x;
		this->y = y;
		width = w;
		height = h;
        return true;
	}

	UIData() : Data() {}

	static std::shared_ptr<UIData> alloc(string uiKey, UIDataType t, int x, int y, int w, int h) {
		std::shared_ptr<UIData> result = std::make_shared<UIData>();
		return (result->init(uiKey, t, x, y, w, h) ? result : nullptr);
	}

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue();

	virtual std::string serialize();

	virtual bool preload(const std::string& file);

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);

	virtual bool materialize();
};

class ButtonUIData : public UIData {
public:
	string buttonAction;
	string buttonTarget;
	string buttonLabel;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
	static std::shared_ptr<ButtonUIData> alloc(string uiKey, UIDataType t, int x, int y, int w, int h, string bAction, string bTarget, string bLabel) {
		std::shared_ptr<ButtonUIData> result = std::make_shared<ButtonUIData>();
		return (result->init( uiKey,  t,  x,  y,  w,  h,  bAction,  bTarget,  bLabel) ? result : nullptr);
	}
	bool init(string uiKey, UIDataType t, int x, int y, int w, int h, string bAction, string bTarget, string bLabel)
	{
		UIData::alloc(uiKey, t, x, y, w, h);
		buttonAction = bAction;
		buttonTarget = bTarget;
		buttonLabel = bLabel;
		return true;
	}
};

class TextUIData : public UIData {
public:
	string textValue;
	string fontKey;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
	static std::shared_ptr<TextUIData> alloc(string uiKey, UIDataType t, int x, int y, int w, int h, string tv, string fKey) {
		std::shared_ptr<TextUIData> result = std::make_shared<TextUIData>();
		return (result->init(uiKey, t, x, y, w, h, tv, fKey) ? result : nullptr);
	}
	bool init(string uiKey, UIDataType t, int x, int y, int w, int h, string tv, string fKey)
	{
		UIData::alloc(uiKey, t, x, y, w, h);
		textValue = tv;
		fontKey = fKey;
		return true;
	}
};

class ImageUIData : public UIData {
public:
	string textureKey;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
	static std::shared_ptr<ImageUIData> alloc(string uiKey, UIDataType t, int x, int y, int w, int h, string tKey) {
		std::shared_ptr<ImageUIData> result = std::make_shared<ImageUIData>();
		return (result->init(uiKey, t, x, y, w, h, tKey) ? result : nullptr);
	}
	bool init(string uiKey, UIDataType t, int x, int y, int w, int h, string tKey)
	{
		UIData::alloc(uiKey, t, x, y, w, h);
		textureKey = tKey;
		return true;
	}
};



#endif /* UIData_hpp */
