#ifndef SaveChapterData_hpp
#define SaveChapterData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"


class SaveChapterData : public Data {
public:

	bool complete;
	bool unlocked;

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;
	virtual bool preload(const std::string& file) override;
	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	bool init(bool c, bool u) {
		complete = c;
		unlocked = u;
		return true;
	}
   

	static std::shared_ptr<SaveChapterData> alloc(bool c, bool u) {
		std::shared_ptr<SaveChapterData> result = std::make_shared<SaveChapterData>();
		return (result->init(c, u) ? result : nullptr);
	}
};

#endif /* SaveChapterData_hpp */
