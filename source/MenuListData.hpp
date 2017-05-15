#ifndef MenuListData_hpp
#define MenuListData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"

class MenuListData : public Data {
private:
	std::vector<std::string> _menuKeys;

public:
	std::string startMenuKey;
	
	std::vector<std::string> getMenuKeys() { return _menuKeys; }

	bool init(std::string smKey, std::vector<std::string> entries) {
		startMenuKey = smKey;
		_menuKeys = entries;
		return true;
	}

	static std::shared_ptr<MenuListData> alloc(std::string smKey, std::vector<std::string> entries) {
		std::shared_ptr<MenuListData> result = std::make_shared<MenuListData>();
		return (result->init(smKey, entries) ? result : nullptr);
	}

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	virtual bool preload(const std::string& file) override;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	virtual bool materialize() override;

};

#endif /* MenuListData_hpp */