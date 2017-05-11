#ifndef ChapterListData_hpp
#define ChapterListData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"

class ChapterListData : public Data {
private:
	std::vector<std::string> _chapterKeys;

public:
	
	std::vector<std::string> getChapterKeys() { return _chapterKeys; }

	bool init(std::vector<std::string> entries) {
		_chapterKeys = entries;
		return true;
	}

	static std::shared_ptr<ChapterListData> alloc(std::vector<std::string> entries) {
		std::shared_ptr<ChapterListData> result = std::make_shared<ChapterListData>();
		return (result->init(entries) ? result : nullptr);
	}

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	virtual bool preload(const std::string& file) override;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

};

#endif /* ChapterListData_hpp */