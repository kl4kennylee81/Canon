//
//  LevelEditorController.hpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelEditorController_hpp
#define LevelEditorController_hpp

#include <stdio.h>
#include "BaseController.hpp"
#include "MenuGraph.hpp"
#include "World.hpp"
#include "WaveEditorController.hpp"
#include "LevelData.hpp"

enum class LevelEditorState : int {
	MAIN,
	SWITCH_TO_WAVE,
	ADD_NEW_WAVE,
	WAVE,
};

class LevelEditorController : public BaseController {
private:
    std::shared_ptr<World> _world;
    
    /** the main node for the level editor
      * connect all related nodes to this node not the scene */
    std::shared_ptr<cugl::Node> _levelEditNode;

	LevelEditorState _state;

	std::shared_ptr<WaveEditorController> _waveEditorController;

	std::shared_ptr<LevelData> _levelData;

public:
    
    LevelEditorController(): BaseController() {}
    
    ~LevelEditorController() { dispose(); };
    
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<MenuGraph> menuGraph);
    
    virtual bool init(std::shared_ptr<cugl::Scene> scene, std::shared_ptr<GenericAssetManager> assets);
    
    virtual void dispose();
    
	void setSceneGraph();
    
    /** to add it to the scene graph node */
    void activate(std::shared_ptr<cugl::Scene> scene);
    
    /** to remove it from hte scene graph node */
    void deactivate(std::shared_ptr<cugl::Scene> scene);

	void addNewWave();

	void updateWaveNodes();
    
    void saveLevel();
    
    void loadLevel(std::string file);

    static std::shared_ptr<LevelEditorController> alloc(std::shared_ptr<cugl::Scene> scene, std::shared_ptr<GenericAssetManager> assets) {
        std::shared_ptr<LevelEditorController> result = std::make_shared<LevelEditorController>();
        return (result->init(scene,assets) ? result : nullptr);
    }
    
    std::shared_ptr<World> getWorld(){
        return _world;
    }
    
    std::shared_ptr<LevelData> getCurrentLevelData(){
        return _levelData;
    }

};

#endif /* LevelEditorController_hpp */
