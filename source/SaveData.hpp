#ifndef SaveData_hpp
#define SaveData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"

class SaveData : public Data {
public:
	std::string currentChapter;
	bool volumeOn;
	bool soundFXOn;

	SaveData() {}

    bool init(std::string cc, bool vo, bool so) {
		currentChapter = cc;
		volumeOn = vo;
		soundFXOn = so;
		return true;
	}

    static std::shared_ptr<SaveData> alloc(std::string cc, bool vo, bool so) {
		std::shared_ptr<SaveData> result = std::make_shared<SaveData>();
		return (result->init(cc, vo, so) ? result : nullptr);
	}

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	virtual bool preload(const std::string& file) override;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

};

#endif /* SaveData_hpp */
