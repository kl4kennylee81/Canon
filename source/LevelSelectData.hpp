#ifndef LevelSelectData_hpp
#define LevelSelectData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "MenuScreenData.hpp"
#include "LevelData.hpp"

class LevelSelectData : public Data {
public:
    std::string name;
    std::string levelKey;

	LevelSelectData() {}

    bool init(std::string n, std::string k) {
		this->name = n;
		this->levelKey = k;
		return true;
	}

    static std::shared_ptr<LevelSelectData> alloc(std::string n, std::string k) {
		std::shared_ptr<LevelSelectData> result = std::make_shared<LevelSelectData>();
		return (result->init(n, k) ? result : nullptr);
	}

	virtual bool preload(const std::string& file) override;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

};

#endif /* LevelSelectData_hpp */
