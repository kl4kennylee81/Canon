#ifndef SaveGameLevelData_hpp
#define SaveGameLevelData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"

class SaveLevelData : public Data {
public:
	bool complete;
	bool unlocked;
	int highScore;

	SaveLevelData() {}

    bool init(bool c, bool u, int h) {
		complete = c;
		unlocked = u;
		highScore = h;
		return true;
	}

    static std::shared_ptr<SaveLevelData> alloc(bool c, bool u, int h) {
		std::shared_ptr<SaveLevelData> result = std::make_shared<SaveLevelData>();
		return (result->init(c, u, h) ? result : nullptr);
	}

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	virtual bool preload(const std::string& file) override;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

};

#endif /* SaveLevelData_hpp */
