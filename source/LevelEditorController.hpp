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

class LevelEditorController : public BaseController {
private:
    std::shared_ptr<World> _world;

public:
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<MenuGraph> menuGraph);
    
    virtual bool init(std::shared_ptr<GenericAssetManager> assets);
    
    static std::shared_ptr<LevelEditorController> alloc(std::shared_ptr<GenericAssetManager> assets) {
        std::shared_ptr<LevelEditorController> result = std::make_shared<LevelEditorController>();
        return (result->init(assets) ? result : nullptr);
    }

};

#endif /* LevelEditorController_hpp */
