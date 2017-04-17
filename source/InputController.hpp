//
//  InputController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef InputController_hpp
#define InputController_hpp

#include <stdio.h>
#include <cugl/cugl.h>

class InputController {
public:
    static bool _touch;
    
    static void setTouch(bool touch);

    static cugl::Vec2 getInputVector();
    
    static bool getIsPressed();

    static bool getDoubleTouch();

	static std::vector<cugl::KeyCode> getPressedKeys();
};

#endif /* InputController_hpp */
