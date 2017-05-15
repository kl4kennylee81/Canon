//
//  TexturedPathNode.cpp
//  Canon
//
//  Created by Kyle Sampson on 5/1/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TexturedPathNode.hpp"
#include "GameState.hpp"


using namespace cugl;

#pragma mark Constructors
/**
 * Creates an empty path node.
 *
 * You must initialize this PathNode before use.
 *
 * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
 * the heap, use one of the static constructors instead.
 */
TexturedPathNode::TexturedPathNode() : TexturedNode(),
_stroke(1.0f),
_closed(true),
_joint(PathJoint::NONE),
_endcap(PathCap::NONE) {
    _classname = "TexturedPathNode";
}

/**
 * Intializes a path with the given vertices and stroke width.
 *
 * You do not need to set the texture; rendering this into a SpriteBatch
 * will simply use the blank texture. Hence the wireframe will have a solid
 * color.
 *
 * The polygon will be extruded using the given sequence of vertices.
 * First it will traverse the vertices using either a closed or open
 * traveral.  Then it will extrude that polygon with the given joint
 * and cap. TexturedPathNode objects share a single extruder, so this initializer
 * is not thread safe.
 *
 * @param vertices  The vertices to texture (expressed in image space)
 * @param stroke    The stroke width of the extruded path.
 * @param joint     The joint between extrusion line segments.
 * @param cap       The end caps of the extruded paths.
 * @param closed    The whether the vertex path is open or closed.
 *
 * @return  true if the path node is initialized properly, false otherwise.
 */
bool TexturedPathNode::initWithVertices(const std::vector<Vec2>& vertices, float stroke,
                                PathJoint joint, PathCap cap, bool closed) {
    _joint = joint;
    _endcap = cap;
    _closed = true;
    _stroke = stroke;
    return init(vertices);
}

/**
 * Intializes a path node with the given polygon and stroke width.
 *
 * You do not need to set the texture; rendering this into a SpriteBatch
 * will simply use the blank texture. Hence the wireframe will have a solid
 * color.
 *
 * The polygon will be extruded using the given polygon, assuming that it
 * is a (connected) path. It will extrude that polygon with the given joint
 * and cap.  It will assume the polygon is closed if the number of indices
 * is twice the number of vertices. TexturedPathNode objects share a single extruder,
 * so this initializer is not thread safe.
 *
 * @param poly      The polygon to texture (expressed in image space)
 * @param stroke    The stroke width of the extruded path.
 * @param joint     The joint between extrusion line segments.
 * @param cap       The end caps of the extruded paths.
 *
 * @return  true if the path node is initialized properly, false otherwise.
 */
bool TexturedPathNode::initWithPoly(const Poly2& poly, float stroke, PathJoint joint, PathCap cap) {
    _joint = joint;
    _endcap = cap;
    _closed = (poly.getVertices().size() * 2 == poly.getIndices().size());
    _stroke = stroke;
    return init(poly);
}

#pragma mark -
#pragma mark Attributes
/**
 * Updates the extrusion polygon, based on the current settings.
 */
void TexturedPathNode::updateExtrusion() {
    if (_stroke > 0) {
        _extruder.set(_polygon.getVertices(), _closed);
        _extruder.calculate(_stroke, _joint, _endcap);
        _extruder.getPolygon(&_extrusion);
        _extrbounds = _extrusion.getBounds();
        _extrbounds.origin -= _polygon.getBounds().origin;
    }
    else {
        _extrbounds.set(Vec2::ZERO, getContentSize());
    }
}


/**
 * Sets the stroke width of the path.
 *
 * This method affects the extruded polygon, but not the original path
 * polygon.
 *
 * @param stroke    The stroke width of the path
 */
void TexturedPathNode::setStroke(float stroke) {
    CUAssertLog(stroke >= 0, "Stroke width is invalid");
    bool changed = (stroke != _stroke);
    _stroke = stroke;
    
    if (changed) {
        clearRenderData();
        _extrusion.clear();
        updateExtrusion();
    }
}

/**
 * Sets whether the path is closed.
 *
 * This method affects both the extruded polygon and the original path
 * polygon.
 *
 * @param closed    Whether the path is closed.
 */
void TexturedPathNode::setClosed(bool closed) {
    bool changed = (closed != _closed);
    _closed = closed;
    
    if (changed) {
        clearRenderData();
        _extrusion.clear();
        
        _outliner.set(_polygon.getVertices());
        _outliner.calculate(_closed ? PathTraversal::CLOSED : PathTraversal::OPEN);
        _outliner.getPath(_polygon.getIndices());
        
        updateExtrusion();
    }
}

/**
 * Sets the joint type between path segments.
 *
 * This method affects the extruded polygon, but not the original path
 * polygon.
 *
 * @param joint The joint type between path segments
 */
void TexturedPathNode::setJoint(PathJoint joint) {
    bool changed = (joint != _joint);
    _joint = joint;
    
    if (changed && _stroke > 0) {
        clearRenderData();
        _extrusion.clear();
        updateExtrusion();
    }
}

/**
 * Sets the cap shape at the ends of the path.
 *
 * This method affects the extruded polygon, but not the original path
 * polygon.
 *
 * @param cap   The cap shape at the ends of the path.
 */
void TexturedPathNode::setCap(PathCap cap) {
    bool changed = (cap != _endcap);
    _endcap = cap;
    
    if (changed && _stroke > 0) {
        clearRenderData();
        _extrusion.clear();
        updateExtrusion();
    }
}


#pragma mark -
#pragma mark Polygons
/**
 * Sets the polgon to the vertices expressed in texture space.
 *
 * The polygon will be extruded using the given sequence of vertices.
 * First it will traverse the vertices using the current traversal. Then
 * it will extrude that polygon with the current joint and cap. TexturedPathNode
 * objects share a single extruder, so this method is not thread safe.
 *
 * @param vertices  The vertices to texture
 */
void TexturedPathNode::setPolygon(const std::vector<Vec2>& vertices) {
    _polygon.set(vertices);
    _outliner.set(_polygon.getVertices());
    _outliner.calculate(_closed ? PathTraversal::CLOSED : PathTraversal::OPEN);
    _outliner.getPath(_polygon.getIndices());
    setPolygon(_polygon);
}

/**
 * Sets the polygon to the given one in texture space.
 *
 * This method will extrude that polygon with the current joint and cap.
 * The polygon is assumed to be closed if the number of indices is twice
 * the number of vertices. TexturedPathNode objects share a single extruder, so
 * this method is not thread safe.
 *
 * @param poly  The polygon to texture
 */
void TexturedPathNode::setPolygon(const Poly2& poly) {
    _closed = poly.getVertices().size() * 2 == poly.getIndices().size();
    TexturedNode::setPolygon(poly);
    updateExtrusion();
}

/**
 * Sets the texture polygon to one equivalent to the given rect.
 *
 * The rectangle will be converted into a Poly2, using the standard outline.
 * This is the same as passing Poly2(rect,false). It will then be extruded
 * with the current joint and cap. TexturedPathNode objects share a single extruder,
 * so this method is not thread safe.
 *
 * @param rect  The rectangle to texture
 */
void TexturedPathNode::setPolygon(const Rect& rect) {
    setPolygon(Poly2(rect, false));
}


#pragma mark -
#pragma mark Rendering
#pragma mark Rendering
/**
 * Draws this Node via the given SpriteBatch.
 *
 * This method only worries about drawing the current node.  It does not
 * attempt to render the children.
 *
 * This is the method that you should override to implement your custom
 * drawing code.  You are welcome to use any OpenGL commands that you wish.
 * You can even skip use of the SpriteBatch.  However, if you do so, you
 * must flush the SpriteBatch by calling end() at the start of the method.
 * in addition, you should remember to call begin() at the start of the
 * method.
 *
 * This method provides the correct transformation matrix and tint color.
 * You do not need to worry about whether the node uses relative color.
 * This method is called by render() and these values are guaranteed to be
 * correct.  In addition, this method does not need to check for visibility,
 * as it is guaranteed to only be called when the node is visible.
 *
 * @param batch     The SpriteBatch to draw with.
 * @param matrix    The global transformation matrix.
 * @param tint      The tint to blend with the Node color.
 */
void TexturedPathNode::draw(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint) {
    batch->setColor(tint);
    batch->setTexture(_texture);
    batch->setBlendEquation(_blendEquation);
    batch->setBlendFunc(_srcFactor, _dstFactor);
        
    Poly2* source = (_stroke > 0 ? &_extrusion : &_polygon);
    for (int i = 1; i < _path->size(); i++) {
        Vec2 pos1 = Vec2(_path->get(i)).scale(GAME_PHYSICS_WIDTH);
        Vec2 pos2 = Vec2(_path->get(i-1)).scale(GAME_PHYSICS_WIDTH);
        Vec2 pos3 = Vec2(pos1.x - pos2.x, pos1.y - pos2.y);
        float distance = pos1.distance(pos2);
        
        batch->draw(_texture, tint,Rect(0, -(_stroke/2), distance, _stroke),
                    Vec2(0, 0), Vec2(1,1), pos3.getAngle(), Vec2(pos2.x, pos2.y));
        
        batch->draw(_capTexture, tint, Poly2::createEllipse(Vec2(0, 0), Size(_stroke,_stroke), 30), pos2);
    }
    batch->draw(_capTexture, tint, Poly2::createEllipse(Vec2(0, 0), Size(_stroke,_stroke), 30),
                Vec2::Vec2(_path->get(_path->size()-1)).scale(GAME_PHYSICS_WIDTH));
}

/**
 * Allocate the render data necessary to render this node.
 */
void TexturedPathNode::generateRenderData() {
    CUAssertLog(!_rendered, "Render data is already present");
    if (_texture == nullptr) {
        return;
    }
    
    Poly2* source = (_stroke > 0 ? &_extrusion : &_polygon);
    
    Vertex2 temp;
    temp.color = Color4::WHITE;
    
    float w = (float)_texture->getWidth();
    float h = (float)_texture->getHeight();
    Vec2 offset = _polygon.getBounds().origin;
    for (auto it = source->getVertices().begin(); it != source->getVertices().end(); ++it) {
        temp.position = *it - offset;
        temp.texcoord.x = (it->x) / w;
        temp.texcoord.y = (it->y) / h;
        if (_flipHorizontal) {
            temp.texcoord.x = 1 - temp.texcoord.x;
        }
        if (!_flipVertical) {
            temp.texcoord.y = 1 - temp.texcoord.y;
        }
        _vertices.push_back(temp);
    }
    
    _rendered = true;
}

/** An extruder for those incomplete polygons */
PathExtruder TexturedPathNode::_extruder;
/** An outliner for those incomplete polygons */
PathOutliner TexturedPathNode::_outliner;
