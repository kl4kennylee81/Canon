//
//  ParticleNode.hpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ParticleNode_hpp
#define ParticleNode_hpp

#include <stdio.h>
#include "Particle.h"

/**
 * A scene graph node for a list of particles.
 *
 * For this demo, we do not need any physics.  It is fine enough to draw.  We
 * only need one ParticleNode for a collection of particles.
 */
class ParticleNode : public PolygonNode {
private:
    /** Container to store the particles currently allocated */
    std::set<Particle*> _particles;
    
    
public:
    /**
     * Returns a new ParticleNode with the given texture
     *
     * All particles will use the given texture as their representation. The
     * particles will have the natural size of this texture; individual particles
     * cannot be scaled.  For more control, you will need to implement your
     * own particle node.
     *
     * @param texture   A shared pointer to a Texture object.
     *
     * @return a new ParticleNode with the given texture
     */
    static std::shared_ptr<ParticleNode> allocWithTexture(const std::shared_ptr<Texture>& texture) {
        std::shared_ptr<ParticleNode> node = std::make_shared<ParticleNode>();
        return (node->initWithTexture(texture) ? node : nullptr);
    }
    
    /**
     * Returns the number of particles in this node
     *
     * @return the number of particles in this node
     */
    size_t getParticleCount() { return _particles.size(); }
    
    /**
     * Adds a particle to this node
     *
     * If the particle is already in the node, nothing happens
     *
     * @param particle  The particle to add to this node
     */
    void addParticle(Particle* particle) {
        _particles.insert(particle);
    }
    
    /**
     * Removes a particle from this node
     *
     * If the particle is not in the node, nothing happens
     *
     * @param particle  The particle to remove from this node
     */
    void removeParticle(Particle* particle) {
        _particles.erase(particle);
    }
    
    /**
     * Updates all of the particles, moving them one time step
     *
     * If a particle goes out of the bounding box of this node, it is added
     * to the reset set.  This allows the caller a chance to remove and
     * recycle these particles.
     *
     * @param reset The recycle set
     */
    void update(std::set<Particle*>& reset);
    
    Mat4 updateTransformLocal(Mat4 combined, float scale, float angle);
    
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
     * @param transform The global transformation matrix.
     * @param tint      The tint to blend with the Node color.
     */
    virtual void draw(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint) override;
};


#endif /* ParticleNode_hpp */
