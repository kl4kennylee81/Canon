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
    
    /** is pressed this frame */
    static bool _isPressed;
    
    /** was pressed last frame */
    static bool _wasPressed;
    
    /** the click position of the current frame */
    static cugl::Vec2 _curPos;

    /** the click position of the last frame */
    static cugl::Vec2 _prevPos;
    
    static void setTouch(bool touch);

    static cugl::Vec2 getInputVector();
    
    /** the vector from the last frame used when letting go and still having the position */
    static cugl::Vec2 getPrevVector();
    
    static bool getIsPressed();
    
    static bool getIsPressedUp();

    static bool getDoubleTouch();
    
    static void update();

	static std::vector<cugl::KeyCode> getPressedKeys();
};

#endif /* InputController_hpp */
