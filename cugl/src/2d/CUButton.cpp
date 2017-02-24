//
//  CUButton.cpp
//  Cornell University Game Library (CUGL)
//
//  This module provides support for a simple clickable button. The button may
//  either be represented by two images (one up and one down), or a single
//  image and two different color tints.
//
//  The button can track its own state, relieving you of having to manually
//  check mouse presses. However, it can only do this when the button is part
//  of a scene graph, as the scene graph maps mouse coordinates to screen
//  coordinates.
//
//  This class uses our standard shared-pointer architecture.
//
//  1. The constructor does not perform any initialization; it just sets all
//     attributes to their defaults.
//
//  2. All initialization takes place via init methods, which can fail if an
//     object is initialized more than once.
//
//  3. All allocation takes place via static constructors which return a shared
//     pointer.
//
//  CUGL zlib License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 1/8/17
//
#include <cugl/input/cu_input.h>
#include <cugl/2d/CUScene.h>
#include <cugl/2d/CUButton.h>

using namespace cugl;

#pragma mark -
#pragma mark Constructors


/**
 * Initializes a button with the given node and color
 *
 * The button will look exactly like the given node when it is not pressed.
 * When pressed, it will tint the up node by the specified color.
 *
 * @param up    The button when it is not pressed
 * @param down  The button tint when it is pressed
 *
 * @return true if the button is initialized properly, false otherwise.
 */
bool Button::init(const std::shared_ptr<Node>& up, Color4 down) {
    CUAssertLog(up, "Up representation cannot be null");
    if (!Node::init()) {
        return false;
    }

    _upnode = up;
    _downnode = nullptr;
    _upcolor  = up->getColor();
    _downcolor = down;

    _upnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _upnode->setPosition(0,0);
    addChild(_upnode);
    setContentSize(_upnode->getContentSize());

    return true;
}

/**
 * Initializes a button with the given nodes
 *
 * The button will look exactly like the up node when it is not pressed.
 * It will look like the down node when it is pressed.  The size of this
 * button is the size of the larger of the two nodes.
 *
 * @param up    The button when it is not pressed
 * @param down  The button when it is pressed
 *
 * @return true if the button is initialized properly, false otherwise.
 */
bool Button::init(const std::shared_ptr<Node>& up, const std::shared_ptr<Node>& down) {
    CUAssertLog(up, "Up representation cannot be null");
    if (!Node::init()) {
        return false;
    }

    _upnode = up;
    _downnode = down;
    
    _upcolor  = up->getColor();
    _upnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _upnode->setPosition(0,0);
    addChild(_upnode);
    Size size = _upnode->getContentSize();

    if (down != nullptr) {
        _downcolor  = down->getColor();
        _downnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _downnode->setPosition(0,0);
        addChild(_downnode);
        _downnode->setVisible(_upnode == nullptr);
        Size dsize  = _downnode->getContentSize();
        size.width  = dsize.width  > size.width  ? dsize.width  : size.width;
        size.height = dsize.height > size.height ? dsize.height : size.height;
    } else {
        _downcolor = Color4::GRAY*_upcolor;
    }
    setContentSize(size);
    return true;
}

/**
 * Disposes all of the resources used by this node.
 *
 * A disposed button can be safely reinitialized. Any children owned by
 * this node will be released.  They will be deleted if no other object
 * owns them.
 *
 * It is unsafe to call this on a button that is still currently inside
 * of a scene graph.
 */
void Button::dispose() {
    _upnode = nullptr;
    _downnode = nullptr;
    Node::dispose();
}

#pragma mark -
#pragma mark Listeners

/**
 * Activates this button to listen for mouse/touch events.
 *
 * This method attaches a listener to either the {@link Mouse} or
 * {@link Touchscreen} inputs to monitor when the button is pressed and/or
 * released. The button will favor the mouse, but will use the touch screen
 * if no mouse input is active.  If neither input is active, this method
 * will fail.
 *
 * When active, the button will change its state on its own, without
 * requiring the user to use {@link setDown(bool)}.  If there is a
 * {@link ButtonListener} attached, it will call that function upon any
 * state changes.
 *
 * @param key   The listener key for the input device
 *
 * @return true if the button was successfully activated
 */
bool Button::activate(Uint32 key) {
    Mouse* mouse = Input::get<Mouse>();
    Touchscreen* touch = Input::get<Touchscreen>();
    CUAssertLog(mouse || touch,  "Neither mouse nor touch input is enabled");
    
    if (mouse) {
        _mouse = true;
        
        // Add the mouse listeners
        bool down = mouse->addPressListener(key, [=](const MouseEvent& event, Uint8 clicks, bool focus) {
            if (this->containsScreen(event.position)) {
                this->setDown(true);
            }
        });
        
        bool up = false;
        if (down) {
            up = mouse->addReleaseListener(key, [=](const MouseEvent& event, Uint8 clicks, bool focus) {
                if (this->isDown()) {
                    this->setDown(false);
                }
            });
            if (!up) {
                mouse->removePressListener(key);
            }
        }
        
        _active = up & down;
    } else {
        _mouse = false;
        
        // Add the mouse listeners
        bool down = touch->addBeginListener(key, [=](const TouchEvent& event, bool focus) {
            if (this->containsScreen(event.position)) {
                this->setDown(true);
            }
        });
        
        bool up = false;
        if (down) {
            up = touch->addEndListener(key, [=](const TouchEvent& event, bool focus) {
                if (this->isDown()) {
                    this->setDown(false);
                }
            });
            if (!up) {
                touch->removeBeginListener(key);
            }
        }
        
        _active = up & down;
    }
    
    _inputkey = _active ? key : 0;
    return _active;
}

/**
 * Deactivates this button, ignoring future mouse/touch events.
 *
 * This method removes its internal listener from either the {@link Mouse}
 * or {@link Touchscreen} inputs to monitor when the button is pressed and/or
 * released.  The input affected is the one that received the listener
 * upon activation.
 *
 * When deactivated, the button will no longer change its state on its own.
 * However, the user can still change the state manually with the
 * {@link setDown(bool)} method.  In addition, any {@link ButtonListener}
 * attached will still respond to manual state changes.
 *
 * @return true if the button was successfully deactivated
 */
bool Button::deactivate() {
    bool success = false;
    if (_mouse) {
        Mouse* mouse = Input::get<Mouse>();
        CUAssertLog(mouse,  "Mouse input is no longer enabled");
        success = mouse->removePressListener(_inputkey);
        success = mouse->removeReleaseListener(_inputkey) && success;
    } else {
        Touchscreen* touch = Input::get<Touchscreen>();
        CUAssertLog(touch,  "Touch input is no longer enabled");
        success = touch->removeBeginListener(_inputkey);
        success = touch->removeEndListener(_inputkey) && success;
    }

    _active = false;
    _inputkey = 0;
    _mouse = false;

    return success;
}

/**
 * Removes the listener for this button.
 *
 * This listener is invoked when the button state changes (up or down).
 *
 * A button may only have one listener at a time.  If this button does not
 * have a listener, this method will fail.
 *
 * @return true if the listener was succesfully removed
 */
bool Button::removeListener() {
    if (_listener != nullptr) {
        _listener = nullptr;
        return true;
    }
    return false;
}

#pragma mark -
#pragma mark Button State
/**
 * Sets whether this button is currently down.
 *
 * Buttons only have two states: up and down.  The default state is up.
 *
 * Changing this value will change how the button is displayed on the
 * screen.  It will also invoke the {@link ButtonListener} if one is
 * currently attached.
 *
 * @param down  Whether this button is currently down.
 */
void Button::setDown(bool down) {
    if (_down == down) {
        return;
    }
    
    _down = down;
    if (down && _downnode && _upnode) {
        _upnode->setVisible(false);
        _downnode->setVisible(true);
    } else if (down) {
        setColor(_downcolor);
    }
    
    if (!down && _downnode && _upnode) {
        _upnode->setVisible(true);
        _downnode->setVisible(false);
    } else if (!down) {
        setColor(_upcolor);
    }
    
    if (_listener != nullptr) {
        _listener(getName(),down);
    }
}
