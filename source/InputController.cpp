//
//  InputController.cpp
//  Canon
//
//  Created by Kenneth Lee on 3/29/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include <stdio.h>
#include "InputController.hpp"

using namespace cugl;

bool InputController::_touch;
bool InputController::_isPressed;
Vec2 InputController::_curPos;

bool InputController::_wasPressed;
Vec2 InputController::_prevPos;

void InputController::setTouch(bool touch) {
    InputController::_touch = touch;
}

/**
 * Returns the screen coordinates of the input. If the input is a 
 * touch screen, return the screen coordinates of the first touch.
 */
cugl::Vec2 InputController::getInputVector() {
    if (_touch) {
        return _curPos;
    }
    else {
        return Input::get<Mouse>()->pointerPosition();
    }
}

cugl::Vec2 InputController::getPrevVector(){
    return _prevPos;
}

bool InputController::getIsPressed() {
    return _isPressed;
}

bool InputController::getIsPressedUp(){
    return _wasPressed && !_isPressed;
}

void InputController::update(){
    _wasPressed = _isPressed;
    if (_touch) {
        auto set = Input::get<Touchscreen>()->touchSet();
        _isPressed = set.size() > 0;
        if (_isPressed){
            _prevPos = _curPos;
            _curPos = Input::get<Touchscreen>()->touchPosition(set.at(0));
        }
    }
    else {
        _isPressed = Input::get<Mouse>()->buttonDown().hasLeft();
        if (_isPressed){
            _prevPos = _curPos;
            _curPos = Input::get<Mouse>()->pointerPosition();
        }
    }
}

bool InputController::getDoubleTouch(){
    {
        if (_touch) {
            return Input::get<Touchscreen>()->touchSet().size() > 1;
        }
        else {
            return Input::get<Mouse>()->buttonDown().hasLeft() &&
            Input::get<Mouse>()->buttonDown().hasRight();
        }
    }
}

std::vector<cugl::KeyCode> InputController::getPressedKeys() {
	return Input::get<Keyboard>()->keySet();
}

