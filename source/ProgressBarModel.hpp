//
//  ProgressBarModel.hpp
//  Canon
//
//  Created by Kenneth Lee on 3/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ProgressBarModel_hpp
#define ProgressBarModel_hpp

#include <stdio.h>
#include <cugl/2d/CUNode.h>
#include <cugl/2d/CUPolygonNode.h>

namespace cugl {
    
    /**
     * This class is a node the represents an (animating) project bar.
     *
     * The progress bar may either be represented via a texture or a simple colored
     * rectangle. If it is a texture, the foreground texture will be sampled left
     * to right and the maximum horizontal texture coordinate will be the percentage
     * of the progress bar.  So if the progress bar is at 50%, the progress bar will
     * draw the left side of the foreground texture.
     *
     * When using textures it is also possible to specify endcap textures.  This
     * allows for progress bars that are not completely rectangular.
     */
    class ProgressBarModel: public Node {
    protected:
        /** The progress percentage of this progress bar (between 0 and 1) */
        float _progress;
        /** The (maximum) size of the foreground texture or node */
        Size _foresize;
        
        /** The background image or rectangle (representing total time) */
        std::shared_ptr<PolygonNode> _background;
        /** The foreground image or rectangle (representing progress) */
        std::shared_ptr<PolygonNode> _foreground;
        /** The starting endcap image */
        std::shared_ptr<PolygonNode> _begincap;
        /** The finishing endcap image */
        std::shared_ptr<PolygonNode> _finalcap;
        
        /** The starting endcap image for background */
        std::shared_ptr<PolygonNode> _begincap_b;
        /** The finishing endcap image for background */
        std::shared_ptr<PolygonNode> _finalcap_b;
        
    public:
#pragma mark -
#pragma mark Constructors
        /**
         * Creates an uninitialized progress bar with no size or texture information.
         *
         * You must initialize this progress bar before use.
         *
         * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate a Node on the
         * heap, use one of the static constructors instead.
         */
        ProgressBarModel() : _progress(1.0f) {}
        
        /**
         * Deletes this progress bar, disposing all resources
         */
        ~ProgressBarModel() { }
        
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
        virtual void dispose() override;
        
        /**
         * Deactivates the default initializer.
         *
         * This initializer may not be used for a progress bar.  A progress bar
         * either needs a texture or a size
         *
         * @return false
         */
        virtual bool init() override {
            CUAssertLog(false,"This node does not support the empty initializer");
            return false;
        }
        
        /**
         * Initializes a texture-less progress bar of the given size.
         *
         * The background will be a white rectangle, while the foreground (measuring
         * progess) will be a red rectangle. You can change these colors with the
         * {@link setBackgroundColor} and {@link setForegroundColor} methods.
         *
         * @param size  The progress bar size
         *
         * @return true if the progress bar is initialized properly, false otherwise.
         */
        bool init(const Size& size) {
            return initWithCaps(nullptr,nullptr,nullptr,nullptr,size);
        }
        
        /**
         * Initializes a progress bar with the given texture.
         *
         * The progress bar will be the size of the texture.  It will use the same
         * texture for the background and foreground.  However, the foreground
         * (measuring progress) will be tinted red.  You can change this color with
         * the {@link setForegroundColor} method.
         *
         * @param background    The progress bar texture
         *
         * @return true if the progress bar is initialized properly, false otherwise.
         */
        bool init(const std::shared_ptr<Texture>& background) {
            return initWithCaps(background,nullptr,nullptr,nullptr);
        }
        
        /**
         * Initializes a progress bar with the given texture and size
         *
         * The progress bar texture will scale to the given size.  It will use the
         * same texture for the background and foreground.  However, the foreground
         * (measuring progress) will be tinted red.  You can change this color with
         * the {@link setForegroundColor} method.
         *
         * @param background    The progress bar texture
         * @param size          The progress bar size
         *
         * @return true if the progress bar is initialized properly, false otherwise.
         */
        bool init(const std::shared_ptr<Texture>& background, const Size& size) {
            return initWithCaps(background,nullptr,nullptr,nullptr,size);
        }
        
        /**
         * Initializes a progress bar with the given background and foreground.
         *
         * The progress bar will be the size of the background texture.  The
         * foreground texture will be scaled to this size.  Neither the background
         * nor the foreground texture will be tinted.
         *
         * @param background    The texture for the background
         * @param foreground    The texture for the animated foreground
         *
         * @return true if the progress bar is initialized properly, false otherwise.
         */
        bool init(const std::shared_ptr<Texture>& background,
                  const std::shared_ptr<Texture>& foreground) {
            return initWithCaps(background,foreground,nullptr,nullptr);
        }
        
        /**
         * Initializes a progress bar with the given textures and size.
         *
         * The progress bar will scale both the background and foreground texture.
         * to the given size. Neither the background nor the foreground texture
         * will be tinted.
         *
         * @param background    The texture for the background
         * @param foreground    The texture for the animated foreground
         * @param size          The progress bar size
         *
         * @return true if the progress bar is initialized properly, false otherwise.
         */
        bool init(const std::shared_ptr<Texture>& background,
                  const std::shared_ptr<Texture>& foreground,
                  const Size& size) {
            return initWithCaps(background,foreground,nullptr,nullptr,size);
        }
        
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
        bool initWithCaps(const std::shared_ptr<Texture>& background,
                          const std::shared_ptr<Texture>& foreground,
                          const std::shared_ptr<Texture>& beginCap,
                          const std::shared_ptr<Texture>& finalCap);
        
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
        bool initWithCaps(const std::shared_ptr<Texture>& background,
                          const std::shared_ptr<Texture>& foreground,
                          const std::shared_ptr<Texture>& beginCap,
                          const std::shared_ptr<Texture>& finalCap,
                          const Size& size);
        
        /**
         * Initializes a progress bar with the given textures and size.
         *
         * The progress bar will scale the background texture to the given size.
         * The foreground texture and end caps will be scaled so that they are this
         * size when combined together.  None of the textures will be tinted.
         *
         * @param background    The texture for the background
         * @param beginCap_b    The left end cap of the background
         * @param finalCap_b    The right end cap of the background
         * @param foreground    The texture for the animated foreground
         * @param beginCap_f    The left end cap of the foreground
         * @param finalCap_f    The right end cap of the foreground
         * @param size          The progress bar size
         *
         * @return true if the progress bar is initialized properly, false otherwise.
         */
        bool initWithCaps(const std::shared_ptr<Texture>& background,
                          const std::shared_ptr<Texture>& beginCap_b,
                          const std::shared_ptr<Texture>& finalCap_b,
                          const std::shared_ptr<Texture>& foreground,
                          const std::shared_ptr<Texture>& beginCap_f,
                          const std::shared_ptr<Texture>& finalCap_f,
                          const Size& size);
        
        
#pragma mark -
#pragma mark Static Constructors
        /**
         * Returns a newly allocated texture-less progress bar of the given size.
         *
         * The background will be a white rectangle, while the foreground (measuring
         * progess) will be a red rectangle. You can change these colors with the
         * {@link setBackgroundColor} and {@link setForegroundColor} methods.
         *
         * @param size  The progress bar size
         *
         * @return a newly allocated texture-less progress bar of the given size.
         */
        static std::shared_ptr<ProgressBarModel> alloc(const Size& size) {
            std::shared_ptr<ProgressBarModel> node = std::make_shared<ProgressBarModel>();
            return (node->init(size) ? node : nullptr);
        }
        
        /**
         * Returns a newly allocated progress bar with the given texture.
         *
         * The progress bar will be the size of the texture.  It will use the same
         * texture for the background and foreground.  However, the foreground
         * (measuring progress) will be tinted red.  You can change this color with
         * the {@link setForegroundColor} method.
         *
         * @param background    The progress bar texture
         *
         * @return a newly allocated progress bar with the given texture.
         */
        static std::shared_ptr<ProgressBarModel> alloc(const std::shared_ptr<Texture>& background) {
            std::shared_ptr<ProgressBarModel> node = std::make_shared<ProgressBarModel>();
            return (node->init(background) ? node : nullptr);
        }
        
        /**
         * Returns a newly allocated progress bar with the given texture and size
         *
         * The progress bar texture will scale to the given size.  It will use the
         * same texture for the background and foreground.  However, the foreground
         * (measuring progress) will be tinted red.  You can change this color with
         * the {@link setForegroundColor} method.
         *
         * @param background    The progress bar texture
         * @param size          The progress bar size
         *
         * @return a newly allocated progress bar with the given texture and size
         */
        static std::shared_ptr<ProgressBarModel> alloc(const std::shared_ptr<Texture>& background, const Size& size) {
            std::shared_ptr<ProgressBarModel> node = std::make_shared<ProgressBarModel>();
            return (node->init(background,size) ? node : nullptr);
        }
        
        /**
         * Returns a newly allocated progress bar with the given background and foreground.
         *
         * The progress bar will be the size of the background texture.  The
         * foreground texture will be scaled to this size.  Neither the background
         * nor the foreground texture will be tinted.
         *
         * @param background    The texture for the background
         * @param foreground    The texture for the animated foreground
         *
         * @return a newly allocated progress bar with the given background and foreground.
         */
        static std::shared_ptr<ProgressBarModel> alloc(const std::shared_ptr<Texture>& background,
                                                  const std::shared_ptr<Texture>& foreground) {
            std::shared_ptr<ProgressBarModel> node = std::make_shared<ProgressBarModel>();
            return (node->init(background,foreground) ? node : nullptr);
        }
        
        /**
         * Returns a newly allocated progress bar with the given textures and size.
         *
         * The progress bar will scale both the background and foreground texture.
         * to the given size. Neither the background nor the foreground texture
         * will be tinted.
         *
         * @param background    The texture for the background
         * @param foreground    The texture for the animated foreground
         * @param size          The progress bar size
         *
         * @return a newly allocated progress bar with the given textures and size.
         */
        static std::shared_ptr<ProgressBarModel> alloc(const std::shared_ptr<Texture>& background,
                                                  const std::shared_ptr<Texture>& foreground,
                                                  const Size& size) {
            std::shared_ptr<ProgressBarModel> node = std::make_shared<ProgressBarModel>();
            return (node->init(background,foreground,size) ? node : nullptr);
        }
        
        /**
         * Returns a newly allocated progress bar with the given textures and size.
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
         * @return a newly allocated progress bar with the given textures and size.
         */
        static std::shared_ptr<ProgressBarModel> allocWithCaps(const std::shared_ptr<Texture>& background,
                                                          const std::shared_ptr<Texture>& foreground,
                                                          const std::shared_ptr<Texture>& beginCap,
                                                          const std::shared_ptr<Texture>& finalCap) {
            std::shared_ptr<ProgressBarModel> node = std::make_shared<ProgressBarModel>();
            return (node->initWithCaps(background,foreground,beginCap,finalCap) ? node : nullptr);
        }
        
        /**
         * Returns a newly allocated progress bar with the given textures and size.
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
         * @return a newly allocated progress bar with the given textures and size.
         */
        static std::shared_ptr<ProgressBarModel> allocWithCaps(const std::shared_ptr<Texture>& background,
                                                          const std::shared_ptr<Texture>& foreground,
                                                          const std::shared_ptr<Texture>& beginCap,
                                                          const std::shared_ptr<Texture>& finalCap,
                                                          const Size& size) {
            std::shared_ptr<ProgressBarModel> node = std::make_shared<ProgressBarModel>();
            return (node->initWithCaps(background,foreground,beginCap,finalCap,size) ? node : nullptr);
        }
        
        /**
         * Returns a newly allocated progress bar with the given textures and size.
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
         * @return a newly allocated progress bar with the given textures and size.
         */
        static std::shared_ptr<ProgressBarModel> allocWithCaps(const std::shared_ptr<Texture>& background,
                                                               const std::shared_ptr<Texture>& beginCap_b,
                                                               const std::shared_ptr<Texture>& finalCap_b,
                                                               const std::shared_ptr<Texture>& foreground,
                                                               const std::shared_ptr<Texture>& beginCap_f,
                                                               const std::shared_ptr<Texture>& finalCap_f,
                                                               const Size& size)  {
            std::shared_ptr<ProgressBarModel> node = std::make_shared<ProgressBarModel>();
            return (node->initWithCaps(background,beginCap_b,finalCap_b,foreground,beginCap_f,finalCap_f,size) ? node : nullptr);
        }
        
#pragma mark -
#pragma mark Properties
        /**
         * Returns the percentage progress of this progress bar
         *
         * This value is a float between 0 and 1. Changing this value will alter
         * the size of the progress bar foreground.
         *
         * @return the percentage progress of this progress bar
         */
        float getProgress() const { return _progress; }
        
        /**
         * Sets the percentage progress of this progress bar
         *
         * This value is a float between 0 and 1. Changing this value will alter
         * the size of the progress bar foreground.
         *
         * @param progress  The percentage progress of this progress bar
         */
        void setProgress(float progress);
        
        /**
         * Returns the background color or tint of the progress bar
         *
         * This is the color applied to the background texture if it exists, or
         * the color of the background rectangle. It is white by default.
         *
         * @return the background color or tint of the progress bar
         */
        Color4 getBackgroundColor() const { return _background->getColor(); }
        
        /**
         * Sets the background color or tint of the progress bar
         *
         * This is the color applied to the background texture if it exists, or
         * the color of the background rectangle. It is white by default.
         *
         * @param color The background color or tint of the progress bar
         */
        void setBackgroundColor(Color4 color) { _background->setColor(color); }
        
        /**
         * Returns the foreground color or tint of the progress bar
         *
         * This is the color applied to the foreground texture (and end caps) if it 
         * exists, or the color of the foreground rectangle. If there is a texture
         * it is white by default.  Otherwise it is red by default.
         *
         * @return the foreground color or tint of the progress bar
         */
        Color4 getForegroundColor() const { return _foreground->getColor(); }
        
        /**
         * Sets the foreground color or tint of the progress bar
         *
         * This is the color applied to the foreground texture (and end caps) if it
         * exists, or the color of the foreground rectangle. If there is a texture
         * it is white by default.  Otherwise it is red by default.
         *
         * @param color The foreground color or tint of the progress bar
         */
        void setForegroundColor(Color4 color);
    };
    
}

#endif /* ProgressBarModel_hpp */
