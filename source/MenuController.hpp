//
//  MenuController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef MenuController_hpp
#define MenuController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "MenuGraph.hpp"
#include "Event.hpp"
#include "Mode.hpp"
#include "ChapterListData.hpp"
#include "ChapterSelectData.hpp"
#include "LevelSelectData.hpp"


class MenuController : public BaseController {
protected:
    std::shared_ptr<cugl::Scene>  _scene;
    
    std::shared_ptr<MenuGraph> _menuGraph;
    
    std::string _selectedLevel; // key to the levelSelectData chosen

	std::shared_ptr<GenericAssetManager> _assets;

	std::map<std::string, std::map<std::string, std::string>> _levelNameUltimateMap;
public:
    
    MenuController();
    
    ~MenuController() { dispose(); };
    
    void dispose();
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep);

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    
    /** this is to initialize a container for the menuGraph to receive events. Does not attach to the scene */
    virtual bool init(std::shared_ptr<MenuGraph> menuGraph);
    
    virtual bool init(std::shared_ptr<cugl::Scene> scene, std::shared_ptr<MenuGraph> menuGraph, std::shared_ptr<GenericAssetManager> assets);
    
    static std::shared_ptr<MenuController> alloc(std::shared_ptr<MenuGraph> menuGraph) {
        std::shared_ptr<MenuController> result = std::make_shared<MenuController>();
        return (result->init(menuGraph) ? result : nullptr);
    }
    
    static std::shared_ptr<MenuController> alloc(std::shared_ptr<cugl::Scene> scene,
                                                 std::shared_ptr<MenuGraph> menuGraph,
												std::shared_ptr<GenericAssetManager> assets) {
        std::shared_ptr<MenuController> result = std::make_shared<MenuController>();
        return (result->init(scene,menuGraph,assets) ? result : nullptr);
    }
    
    std::shared_ptr<MenuGraph> getMenuGraph() { return _menuGraph; };
    
    void activate();
    
    void deactivate();
    
    std::string getSelectedLevel();

	static void createLevelNameUltimateMap(std::map<std::string, std::map<std::string, std::string>>& map, std::shared_ptr<GenericAssetManager>& _assets) {
		std::vector<std::string> levelList; // all levels in chronological order
		std::vector<std::string> levelNameList; // all level names ...

		std::vector<std::string> chKeys = _assets->get<ChapterListData>("chapterList")->getChapterKeys();
		for (auto chKey : chKeys) {

			// get chapter index
			int numChaps = chKeys.size();
			auto iter = std::find(chKeys.begin(), chKeys.end(), chKey);
			int chIndex = std::distance(chKeys.begin(), iter);

			std::vector<std::string> lvlKeys = _assets->get<ChapterSelectData>(chKey)->getLevelKeys();

			for (auto lvlKey : lvlKeys) {
				std::shared_ptr<LevelSelectData> lsData = _assets->get<LevelSelectData>(lvlKey);
				levelList.push_back(lsData->key);
				levelNameList.push_back(lsData->levelKey);

				std::map<std::string, std::string> subMap;
				subMap.insert(std::make_pair("key", lsData->key));
				subMap.insert(std::make_pair("num", std::to_string(levelList.size() - 1)));
				subMap.insert(std::make_pair("chapNum", std::to_string(chIndex)));

				map.insert(std::make_pair(lsData->levelKey, subMap));
			}
		}

		// add the "next" fields in the subMap s
		for (auto& item : map) {
			int index = std::stoi(item.second.at("num")) + 1;

			// check index out of bounds for last level
			if (index == levelList.size()) {
				// for the last level, the "next" and "nextName" will not exist
				continue;
			}
			item.second.insert(std::make_pair("nextKey", levelList.at(index)));
			item.second.insert(std::make_pair("nextName", levelNameList.at(index)));
			std::string nextChap = map.at(levelNameList.at(index)).at("chapNum");
			item.second.insert(std::make_pair("nextChap", nextChap));
		}
	}

	// returns (nextlevelname, nextlevelchapterindex)
	static std::pair<std::string, int> getNextLevelInfo(std::string levelName, 
			std::map<std::string, std::map<std::string, std::string>>& theMap, 
			std::shared_ptr<GenericAssetManager>& _assets) {

		if (levelName == "") {
			return std::make_pair("", 0);
		}
		std::map<std::string, std::string> subMap = theMap.at(levelName);
		std::string nextLevelName = subMap.at("nextName");
		int nextChapIndex = std::stoi(subMap.at("nextChap"));
		return std::make_pair(nextLevelName, nextChapIndex);
	}
};



#endif /* MenuController_hpp */
