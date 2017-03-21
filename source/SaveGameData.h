#ifndef SaveGameData_h
#define SaveGameData_h

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"

class SaveGameData : Data {
protected:
	std::vector<std::shared_ptr<MenuEntry>> _menuEntries;
	std::vector<std::shared_ptr<LevelEntry>> _levelEntries;
public:
	virtual std::string serialize();

	virtual bool preload(const std::string& file);

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);

	virtual bool materialize();

	bool init(int uid) {
		return true;
	}

	static std::shared_ptr<SaveGameData> alloc(int uid) {
		std::shared_ptr<SaveGameData> result = std::make_shared<SaveGameData>();
		return (result->init(uid) ? result : nullptr);
	}
};

#endif /* SaveGameData_h */
