//
//  GameState.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameObject.hpp"

class GameState {
    
    /** Reference to the physics world root of the scene graph 
     *  All physics objects are in this
     */
    std::shared_ptr<cugl::Node> _worldnode;
    
    /** The root of our scene graph. 
     * Example:
     * Child1 : Background scene node
     * Child2 : is the _worldnode for the physics world root
     * would be used to define a draw order in the z axis for the different layers
     */
    std::shared_ptr<cugl::Scene>  _scene;
    
    std::shared_ptr<GameObject>   _activeCharacter;
    
    /** not sure yet if representing time as an int is the move */
    int time_elapsed;
};

#endif /* GameState_hpp */
