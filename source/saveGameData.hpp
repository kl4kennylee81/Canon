#ifndef SaveGameData_hpp
#define SaveGameData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"

class SaveLevelEntry {
public:
	string name;
	bool unlocked;
	bool complete;
	int highScore;
	string levelKey;
	string levelUrl;

	SaveLevelEntry() {}

	bool init(string n, bool unlock, bool com, int high, string k, string u) {
		this->name = n;
		this->unlocked = unlock;
		this->complete = com;
		this->highScore = high;
		this->levelKey = k;
		this->levelUrl = u;
		return true;
	}

	static std::shared_ptr<SaveLevelEntry> alloc(string n, bool unlock, bool com, int high, string k, string u) {
		std::shared_ptr<SaveLevelEntry> result = std::make_shared<SaveLevelEntry>();
		return (result->init(n, unlock, com, high, k, u) ? result : nullptr);
	}
};


class SaveGameData : Data {
protected:
	std::vector<std::shared_ptr<SaveLevelEntry>> _saveLevelEntries;
public:
	virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	virtual bool preload(const std::string& file);

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);

	virtual bool materialize();

	bool init() {
		return true;
	}

	static std::shared_ptr<SaveGameData> alloc() {
		std::shared_ptr<SaveGameData> result = std::make_shared<SaveGameData>();
		return (result->init() ? result : nullptr);
	}
};

#endif /* SaveGameData_hpp */
