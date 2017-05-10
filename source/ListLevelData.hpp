#ifndef ListLevelData_hpp
#define ListLevelData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"

class ListLevelData : public Data {
public:
    std::string name;
    std::string levelKey;

	ListLevelData() {}

    bool init(std::string n, std::string k) {
		this->name = n;
		this->levelKey = k;
		return true;
	}

    static std::shared_ptr<ListLevelData> alloc(std::string n, std::string k) {
		std::shared_ptr<ListLevelData> result = std::make_shared<ListLevelData>();
		return (result->init(n, k) ? result : nullptr);
	}

	virtual bool preload(const std::string& file) override;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

};

#endif /* ListLevelData_hpp */
