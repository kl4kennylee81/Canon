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
    std::string songKey;

	LevelSelectData() {}

    bool init(std::string n, std::string k, std::string s) {
		this->name = n;
		this->levelKey = k;
        this->songKey = s;
		return true;
	}

    static std::shared_ptr<LevelSelectData> alloc(std::string n, std::string k, std::string s) {
		std::shared_ptr<LevelSelectData> result = std::make_shared<LevelSelectData>();
		return (result->init(n, k, s) ? result : nullptr);
	}

	virtual bool preload(const std::string& file) override;

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

};

#endif /* LevelSelectData_hpp */
