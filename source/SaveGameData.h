#ifndef SaveGameData_h
#define SaveGameData_h

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"

class SaveLevelEntry {
public:
	string name;
	boolean unlocked;
	boolean complete;
	int highScore;
	string levelKey;
	string levelUrl;

	SaveLevelEntry() {}

	bool init(string n, boolean unloc, boolean com, int high, string k, string u) {
		this->name = n;
		this->unlocked = unloc;
		this->complete = com;
		this->highScore = high;
		this->levelKey = k;
		this->levelUrl = u;
		return true;
	}

	static std::shared_ptr<SaveLevelEntry> alloc(string n, boolean unloc, boolean com, int high, string k, string u) {
		std::shared_ptr<SaveLevelEntry> result = std::make_shared<SaveLevelEntry>();
		return (result->init(n, unloc, com, high, k, u) ? result : nullptr);
	}
};


class SaveGameData : Data {
protected:
	std::vector<std::shared_ptr<SaveLevelEntry>> _saveLevelEntries;
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
