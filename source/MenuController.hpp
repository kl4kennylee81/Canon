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
public:
    
    MenuController();
    
    virtual void attach(std::shared_ptr<Observer> obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep);

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    
    virtual bool init(std::shared_ptr<cugl::Scene> scene);
    
    virtual bool init(std::shared_ptr<cugl::Scene> scene, const std::shared_ptr<GenericAssetManager>& assets);
    
    static std::shared_ptr<MenuController> alloc(std::shared_ptr<cugl::Scene> scene) {
        std::shared_ptr<MenuController> result = std::make_shared<MenuController>();
        return (result->init(scene) ? result : nullptr);
    }
    
    static std::shared_ptr<MenuController> alloc(std::shared_ptr<cugl::Scene> scene, const std::shared_ptr<GenericAssetManager>& assets) {
        std::shared_ptr<MenuController> result = std::make_shared<MenuController>();
        return (result->init(scene, assets) ? result : nullptr);
    }
    
    Mode getMode();
    
    void setMode(Mode m);
    
    void activate();
    
    void deactivate();
};



#endif /* MenuController_hpp */
