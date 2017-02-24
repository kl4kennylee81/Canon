//
//  CUAnimationNode.cpp
//  Cornell University Game Library (CUGL)
//
//  This module provides a straight-forward filmstrip API, similar to what we
//  use in the intro class.  Note that this class extends PolygonNode, as it
//  simply treats the node texture as a sprite sheet. Therefore, it is possible
//  to animate the filmstrip over polygons.  However, this can have undesirable
//  effects if the polygon coordinates extend beyond a single animation frame.
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
//  Version: 12/1/16
//
#include <cugl/2d/CUAnimationNode.h>


using namespace cugl;


#pragma mark -
#pragma mark Constructors

/**
 * Constructs a ActionNode with no filmstrip
 *
 * You must initialize this object before use.
 *
 * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
 * the heap, use one of the static constructors instead.
 */
AnimationNode::AnimationNode() :
_cols(0),
_size(0),
_frame(0),
_bounds(Rect::ZERO) {
    _name = "AnimationNode";
}

/**
 * Initializes the film strip with the given texture.
 *
 * This initializer assumes that the filmstrip is rectangular, and that
 * there are no unused frames.
 *
 * The size of the node is equal to the size of a single frame in the
 * filmstrip. To resize the node, scale it up or down.  Do NOT change the
 * polygon, as that will interfere with the animation.
 *
 * @param texture   The texture image to use
 * @param rows      The number of rows in the filmstrip
 * @param cols      The number of columns in the filmstrip
 *
 * @return  true if the filmstrip is initialized properly, false otherwise.
 */
bool AnimationNode::initWithFilmstrip(const std::shared_ptr<Texture>& texture,
                                      int rows, int cols, int size) {
    CUAssertLog(size <= rows*cols, "Invalid strip size for %dx%d",rows,cols);
    
    this->_cols = cols;
    this->_size = size;
    _bounds.size = texture->getSize();
    _bounds.size.width /= cols;
    _bounds.size.height /= rows;
    return this->initWithTexture(texture, _bounds);
}


#pragma mark -
#pragma mark Attribute Accessors

/**
 * Sets the active frame as the given index.
 *
 * If the frame index is invalid, an error is raised.
 *
 * @param frame the index to make the active frame
 */
void AnimationNode::setFrame(int frame) {
    CUAssertLog(frame >= 0 && frame < _size, "Invalid animation frame %d", frame);
    
    _frame = frame;
    float x = (frame % _cols)*_bounds.size.width;
    float y = _texture->getSize().height - (1+frame/_cols)*_bounds.size.height;
    shiftPolygon(x-_bounds.origin.x, y-_bounds.origin.y);
    _bounds.origin.set(x,y);
}

