#ifndef ChapterSelectData_hpp
#define ChapterSelectData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"


class ChapterSelectData : public Data {
protected:
	std::vector<std::string> _levelKeys;
public:

	std::string name;
	std::string cTexture;

	virtual bool preload(const std::string& file) override;
	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	bool init(std::string n, std::vector<std::string> lKeys, std::string cTex) {
		name = n;
		_levelKeys = lKeys;
		cTexture = cTex;
		return true;
	}
    
	std::vector<std::string> getLevelKeys() {
		return _levelKeys;
	}

	static std::shared_ptr<ChapterSelectData> alloc(std::string n, std::vector<std::string> lKeys, std::string cTex) {
		std::shared_ptr<ChapterSelectData> result = std::make_shared<ChapterSelectData>();
		return (result->init(n, lKeys, cTex) ? result : nullptr);
	}
};

#endif /* ChapterSelectData_hpp */
