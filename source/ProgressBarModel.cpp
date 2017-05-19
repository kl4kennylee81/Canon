//
//  ProgressBarModel.cpp
//  Canon
//
//  Created by Kenneth Lee on 3/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ProgressBarModel.hpp"

using namespace cugl;

#pragma mark -
#pragma mark Constructors

/**
 * Initializes a progress bar with the given textures and size.
 *
 * The progress bar will be the size of the background texture.  The
 * foreground texture and end caps will be scaled so that they are this
 * size when combined together.  None of the textures will be tinted.
 *
 * @param background    The texture for the background
 * @param foreground    The texture for the animated foreground
 * @param beginCap      The left end cap of the foreground
 * @param finalCap      The right end cap of the foreground
 *
 * @return true if the progress bar is initialized properly, false otherwise.
 */
bool ProgressBarModel::initWithCaps(const std::shared_ptr<Texture>& background,
                               const std::shared_ptr<Texture>& foreground,
                               const std::shared_ptr<Texture>& beginCap,
                               const std::shared_ptr<Texture>& finalCap) {
    CUAssertLog(background != nullptr, "Background texture cannot be null if there is no specified size");
    return initWithCaps(background, foreground, beginCap, finalCap, background->getSize());
}

/**
 * Initializes a progress bar with the given textures and size.
 *
 * The progress bar will scale the background texture to the given size.
 * The foreground texture and end caps will be scaled so that they are this
 * size when combined together.  None of the textures will be tinted.
 *
 * @param background    The texture for the background
 * @param foreground    The texture for the animated foreground
 * @param beginCap      The left end cap of the foreground
 * @param finalCap      The right end cap of the foreground
 * @param size          The progress bar size
 *
 * @return true if the progress bar is initialized properly, false otherwise.
 */
bool ProgressBarModel::initWithCaps(const std::shared_ptr<Texture>& background,
                               const std::shared_ptr<Texture>& foreground,
                               const std::shared_ptr<Texture>& beginCap,
                               const std::shared_ptr<Texture>& finalCap,
                               const Size& size) {
    if (!Node::initWithBounds(size)) {
        return false;
    }
    
    std::shared_ptr<Texture> temp = (background == nullptr ? SpriteBatch::getBlankTexture() : background);
    Vec2 scale = size/temp->getSize();
    _background = PolygonNode::allocWithTexture(temp);
    _background->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _background->setPosition(0,0);
    _background->setScale(scale);
    addChild(_background);
    
    _progress = 0;
    _foresize = size;
    
    if (beginCap != nullptr) {
        scale = size/beginCap->getSize();
        scale.x = 1.0f;
        _begincap = PolygonNode::allocWithTexture(beginCap);
        _begincap->setScale(scale);
        _begincap->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _begincap->setPosition(0,0);
//        addChild(_begincap);
        _foresize.width -= beginCap->getSize().width;
    }
    
    if (finalCap != nullptr) {
        scale = size/finalCap->getSize();
        scale.x = 1.0f;
        _finalcap = PolygonNode::allocWithTexture(finalCap);
        _finalcap->setScale(scale);
        _finalcap->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        if (_begincap == nullptr) {
            _finalcap->setPosition(-_background->getSize().width/2.0f,0);
        } else {
            _finalcap->setPosition(_begincap->getBoundingBox().getMaxX(),0);
        }
//        addChild(_finalcap);
        _foresize.width -= finalCap->getSize().width;
    }
    
    if (foreground != nullptr) {
        scale = size/foreground->getSize();
        scale.x = 1.0f;
        _foreground = PolygonNode::allocWithTexture(foreground);
        _foreground->setScale(scale);
        _foresize.height /= scale.y;
    } else {
        std::shared_ptr<Texture> temp = SpriteBatch::getBlankTexture();
        scale = size/temp->getSize();
        scale.x = 1.0f;
        _foreground = PolygonNode::allocWithTexture(temp);
        _foreground->setColor(Color4::RED);
        _foreground->setScale(scale);
        _foresize.height /= scale.y;
    }
    
    _foreground->setPolygon(Rect(0,0,0,_foresize.height));
    _foreground->setContentSize(Size(0,_foresize.height));
    
    _foreground->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    if (_begincap == nullptr) {
        _foreground->setPosition(-_background->getSize().width/2.0f,0);
    } else {
        _foreground->setPosition(_begincap->getBoundingBox().getMaxX(),0);
    }
//    addChild(_foreground);
    _foregroundActive = false;
    return true;
}

/**
 * Initializes a progress bar with the given textures and size.
 *
 * The progress bar will scale the background texture to the given size.
 * The foreground texture and end caps will be scaled so that they are this
 * size when combined together.  None of the textures will be tinted.
 *
 * @param background    The texture for the background
 * @param beginCap_b     The left end cap of the background
 * @param finalCap_b     The right end cap of the background
 * @param foreground    The texture for the animated foreground
 * @param beginCap_f      The left end cap of the foreground
 * @param finalCap_f     The right end cap of the foreground
 * @param size          The progress bar size
 *
 * @return true if the progress bar is initialized properly, false otherwise.
 */
bool ProgressBarModel::initWithCaps(const std::shared_ptr<Texture>& background,
                                    const std::shared_ptr<Texture>& beginCap_b,
                                    const std::shared_ptr<Texture>& finalCap_b,
                                    const std::shared_ptr<Texture>& foreground,
                                    const std::shared_ptr<Texture>& beginCap_f,
                                    const std::shared_ptr<Texture>& finalCap_f,
                                    const Size& size) {
    if (!Node::initWithBounds(size)) {
        return false;
    }
    
    Vec2 scale;
    Size bkgd_barSize = Size(size);
    
    if (beginCap_b != nullptr){
        scale = size/beginCap_f->getSize();
        scale.x = 1.0f;
        _begincap_b = PolygonNode::allocWithTexture(beginCap_b);
        _begincap_b->setScale(scale);
        _begincap_b->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _begincap_b->setPosition(0,0);
        addChild(_begincap_b);
        bkgd_barSize.width -= beginCap_b->getSize().width;
    }
    
    if (finalCap_b != nullptr) {
        scale = size/finalCap_b->getSize();
        scale.x = 1.0f;
        _finalcap_b = PolygonNode::allocWithTexture(finalCap_b);
        _finalcap_b->setScale(scale);
        _finalcap_b->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        addChild(_finalcap_b);
        bkgd_barSize.width -= finalCap_f->getSize().width;
    }
    
    // initialize the background
    std::shared_ptr<Texture> temp = (background == nullptr ? SpriteBatch::getBlankTexture() : background);
    scale = bkgd_barSize/temp->getSize();
    _background = PolygonNode::allocWithTexture(temp);
    _background->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _background->setPosition(0,0);
    _background->setScale(scale);
    
    // the position of the beginning is the end of the begin cap
    _background->setPosition(_begincap_b->getBoundingBox().getMaxX(),0);
    addChild(_background);
    
    // set the position of the final cap to the background bounding box right end
    _finalcap_b->setPosition(_background->getBoundingBox().getMaxX(),0);
    
    // set state variables
    _progress = 0;
    _foregroundActive = false;
    _foresize = size;
    
    if (beginCap_f != nullptr) {
        scale = size/beginCap_f->getSize();
        scale.x = 1.0f;
        _begincap = PolygonNode::allocWithTexture(beginCap_f);
        _begincap->setScale(scale);
        _begincap->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _begincap->setPosition(0,0);
        _foresize.width -= beginCap_f->getSize().width;
    }
    
    if (finalCap_f != nullptr) {
        scale = size/finalCap_f->getSize();
        scale.x = 1.0f;
        _finalcap = PolygonNode::allocWithTexture(finalCap_f);
        _finalcap->setScale(scale);
        _finalcap->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        if (_begincap == nullptr) {
            _finalcap->setPosition(-_background->getSize().width/2.0f,0);
        } else {
            _finalcap->setPosition(_begincap->getBoundingBox().getMaxX(),0);
        }
        _foresize.width -= finalCap_f->getSize().width;
    }
    
    if (foreground != nullptr) {
        scale = size/foreground->getSize();
        scale.x = 1.0f;
        _foreground = PolygonNode::allocWithTexture(foreground);
        _foreground->setScale(scale);
        _foresize.height /= scale.y;
    } else {
        std::shared_ptr<Texture> temp = SpriteBatch::getBlankTexture();
        scale = size/temp->getSize();
        scale.x = 1.0f;
        _foreground = PolygonNode::allocWithTexture(temp);
        _foreground->setColor(Color4::RED);
        _foreground->setScale(scale);
        _foresize.height /= scale.y;
    }
    
    _foreground->setPolygon(Rect(0,0,0,_foresize.height));
    _foreground->setContentSize(Size(0,_foresize.height));
    
    _foreground->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    if (_begincap == nullptr) {
        _foreground->setPosition(-_background->getSize().width/2.0f,0);
    } else {
        _foreground->setPosition(_begincap->getBoundingBox().getMaxX(),0);
    }
    return true;
}

/**
 * Disposes all of the resources used by this node.
 *
 * A disposed progress bar can be safely reinitialized. Any children owned
 * by this node will be released.  They will be deleted if no other object
 * owns them.
 *
 * It is unsafe to call this on a progress bar that is still currently
 * inside of a scene graph.
 */
void ProgressBarModel::dispose() {
    _background = nullptr;
    _foreground = nullptr;
    _begincap = nullptr;
    _finalcap = nullptr;
    _begincap_b = nullptr;
    _finalcap_b = nullptr;
    Node::dispose();
}

#pragma mark -
#pragma mark Properties
/**
 * Sets the percentage progress of this progress bar
 *
 * This value is a float between 0 and 1. Changing this value will alter
 * the size of the progress bar foreground.
 *
 * @param progress  The percentage progress of this progress bar
 */
void ProgressBarModel::setProgress(float progress) {
    _progress = progress;
    
    _foreground->setPolygon(Rect(0,0,progress*_foresize.width,_foresize.height));
    _foreground->setContentSize(Size(progress*_foresize.width,_foresize.height));
    if (_finalcap != nullptr) {
        _finalcap->setPosition(_foreground->getBoundingBox().getMaxX(),0);
    }
}

/**
 * Sets the foreground color or tint of the progress bar
 *
 * This is the color applied to the foreground texture (and end caps) if it
 * exists, or the color of the foreground rectangle. If there is a texture
 * it is white by default.  Otherwise it is red by default.
 *
 * @param color The foreground color or tint of the progress bar
 */
void ProgressBarModel::setForegroundColor(Color4 color) {
    _foreground->setColor(color);
    if (_begincap != nullptr) {
        _begincap->setColor(color);
    }
    if (_finalcap != nullptr) {
        _finalcap->setColor(color);
    }
}

/**
 * Turns the foreground on in the progressBar
 *
 * The foreground is not drawn until it is toggled Active
 */
void ProgressBarModel::toggleActive(){
    if (_foregroundActive){
        return;
    }
    _foregroundActive = true;
    
    if (_begincap != nullptr){
        addChild(_begincap);
    }
    if (_finalcap != nullptr){
        addChild(_finalcap);
    }
    if (_foreground != nullptr){
        addChild(_foreground);
    }
}
