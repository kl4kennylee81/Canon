//
//  HomingAI.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef HomingAI_hpp
#define HomingAI_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameState.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"

class HomingAI : public ActiveAI {

public:
    
    HomingAI(){}
    
    ~HomingAI(){ dispose(); }
    
    void dispose(){
        _object = nullptr;
    }
    
    void update(std::shared_ptr<GameState> state);
    
    bool garbageCollect(GameObject* obj);
    
    std::vector<std::shared_ptr<GameObject>> getObjects() {
        std::vector<std::shared_ptr<GameObject>> objects;
        objects.push_back(_object);
        return objects;
    }
    
    bool isActive();
    
    void toggleActive();
    
    bool init(std::shared_ptr<GameObject> object) {
        _object = object;
        _isActive = false;
        return true;
    }
    
    static std::shared_ptr<HomingAI> alloc(std::shared_ptr<GameObject> object) {
        std::shared_ptr<HomingAI> result = std::make_shared<HomingAI>();
        return (result->init(object) ? result : nullptr);
    }

	std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	static std::shared_ptr<HomingAI> allocWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state)
	{
		std::shared_ptr<HomingAI> result = std::make_shared<HomingAI>();
		return (result->initWithJson(json, state) ? result : nullptr);
	}

	bool initWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state);
};

#endif /* HomingAI_hpp */
