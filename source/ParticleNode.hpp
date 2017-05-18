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
#include "GroupContainer.hpp"

/**
 * A scene graph node for a list of particles.
 *
 * For this demo, we do not need any physics.  It is fine enough to draw.  We
 * only need one ParticleNode for a collection of particles.
 */
class ParticleNode : public PolygonNode {
private:    
    /**
     * Each particle node holds its own memory
     */
    std::shared_ptr<cugl::FreeList<Particle>> _memory;
    
public:
    // expose groups passed down from the generator
    std::shared_ptr<GroupContainer> _groups;
    
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
    size_t getParticleCount() { return _memory->getUsage(); }
    
    /**
     * Initializes the memory inside of this particle node.
     */
    void init_memory(int max_size) {
        _memory = FreeList<Particle>::alloc(max_size);
    }
    
    /**
     * Creates the particle with given particle data template and
     * assigns it to group_num. -1 means no group.
     */
    void addParticle(ParticleData pd, int group_num);
    
    /**
     * No use for this?
     */
    void removeParticle(Particle* particle) {
//        _particles.erase(particle);
    }
    
    /**
     * Goes through the memory in this particle node. Moves each particle one step.
     * Also in charge of freeing the particles that are expired.
     */
    void update();
    
    /**
     * need to peer into the gorup and update the particles based on group settings
     */
    void group_update_particles(Particle* particle);
    
    /**
     * need this for rotating the node with custom transformation matrix in the draw.
     */
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
