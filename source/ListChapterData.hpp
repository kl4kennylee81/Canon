#ifndef ListChapterData_hpp
#define ListChapterData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"


class ListChapterData : public Data {
protected:
	std::vector<std::string> _levelKeys;
public:

	std::string name;

	virtual bool preload(const std::string& file) override;
	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	bool init(std::string n, std::vector<std::string> lKeys) {
		name = n;
		_levelKeys = lKeys;
		return true;
	}
    
	std::vector<std::string> getLevelKeys() {
		return _levelKeys;
	}

	static std::shared_ptr<ListChapterData> alloc(std::string n, std::vector<std::string> lKeys) {
		std::shared_ptr<ListChapterData> result = std::make_shared<ListChapterData>();
		return (result->init(n, lKeys) ? result : nullptr);
	}
};

#endif /* ListChapterData_hpp */
