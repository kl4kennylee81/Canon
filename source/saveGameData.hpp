#ifndef SaveGameData_hpp
#define SaveGameData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"

class SaveLevelEntry {
public:
    std::string name;
	bool unlocked;
	bool complete;
	int highScore;
    std::string levelKey;
    std::string levelUrl;

	SaveLevelEntry() {}

    bool init(std::string n, bool unlock, bool com, int high, std::string k, std::string u) {
		this->name = n;
		this->unlocked = unlock;
		this->complete = com;
		this->highScore = high;
		this->levelKey = k;
		this->levelUrl = u;
		return true;
	}

    static std::shared_ptr<SaveLevelEntry> alloc(std::string n, bool unlock, bool com, int high, std::string k, std::string u) {
		std::shared_ptr<SaveLevelEntry> result = std::make_shared<SaveLevelEntry>();
		return (result->init(n, unlock, com, high, k, u) ? result : nullptr);
	}
};


class SaveGameData : Data {
protected:
	std::vector<std::shared_ptr<SaveLevelEntry>> _saveLevelEntries;
public:
	virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	virtual bool preload(const std::string& file) override;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	virtual bool materialize() override;

	bool init() {
		return true;
	}
    
    std::vector<std::shared_ptr<SaveLevelEntry>> getSaveLevelEntries();

	static std::shared_ptr<SaveGameData> alloc() {
		std::shared_ptr<SaveGameData> result = std::make_shared<SaveGameData>();
		return (result->init() ? result : nullptr);
	}
};

#endif /* SaveGameData_hpp */
