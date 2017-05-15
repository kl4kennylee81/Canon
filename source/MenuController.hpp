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


class MenuController : public BaseController {
protected:
    std::shared_ptr<cugl::Scene>  _scene;
    
    std::shared_ptr<MenuGraph> _menuGraph;
    
    std::string _selectedLevel; // key to the levelSelectData chosen

	std::shared_ptr<GenericAssetManager> _assets;
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
};



#endif /* MenuController_hpp */
