//
//  GameEngine.hpp
//  Canon
//
//  Created by Jonathan Chen on 2/24/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameEngine_hpp
#define GameEngine_hpp
#include <cugl/cugl.h>
#include "GameplayController.hpp"

/**
 * Class for a simple Hello World style application
 *
 * The application simply moves the CUGL logo across the screen.  It also
 * provides a button to quit the application.
 */
class GameEngine : public cugl::Application {
protected:
    /** The loaders to (synchronously) load in assets */
    std::shared_ptr<cugl::AssetManager> _assets;
    /** A scene graph, used to display our 2D scenes */
    std::shared_ptr<cugl::Scene> _scene;
    /** A 3152 style SpriteBatch to render the scene */
    std::shared_ptr<cugl::SpriteBatch>  _batch;
    /** A reference to the logo, so that we can move it around */
    std::shared_ptr<cugl::Node>  _logo;
    
    // Player modes
    /** The primary controller for the game world */
    GameplayController _gameplay;
    
    /** A countdown used to move the logo */
    int  _countdown;
    
    /**
     * Internal helper to build the scene graph.
     *
     * Scene graphs are not required.  You could manage all scenes just like
     * you do in 3152.  However, they greatly simplify scene management, and
     * have become standard in most game engines.
     */
    void buildScene();
    
public:
    /**
     * Creates, but does not initialized a new application.
     *
     * This constructor is called by main.cpp.  You will notice that, like
     * most of the classes in CUGL, we do not do any initialization in the
     * constructor.  That is the purpose of the init() method.  Separation
     * of initialization from the constructor allows main.cpp to perform
     * advanced configuration of the application before it starts.
     */
    GameEngine() : Application(), _countdown(-1) {}
    
    /**
     * Disposes of this application, releasing all resources.
     *
     * This destructor is called by main.cpp when the application quits.
     * It simply calls the dispose() method in Application.  There is nothing
     * special to do here.
     */
    ~GameEngine() { }
    
    /**
     * The method called after OpenGL is initialized, but before running the application.
     *
     * This is the method in which all user-defined program intialization should
     * take place.  You should not create a new init() method.
     *
     * When overriding this method, you should call the parent method as the
     * very last line.  This ensures that the state will transition to FOREGROUND,
     * causing the application to run.
     */
    virtual void onStartup() override;
    
    /**
     * The method called when the application is ready to quit.
     *
     * This is the method to dispose of all resources allocated by this
     * application.  As a rule of thumb, everything created in onStartup()
     * should be deleted here.
     *
     * When overriding this method, you should call the parent method as the
     * very last line.  This ensures that the state will transition to NONE,
     * causing the application to be deleted.
     */
    virtual void onShutdown() override;
    
    /**
     * The method called to update the application data.
     *
     * This is your core loop and should be replaced with your custom implementation.
     * This method should contain any code that is not an OpenGL call.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    virtual void update(float timestep) override;
    
    /**
     * The method called to draw the application to the screen.
     *
     * This is your core loop and should be replaced with your custom implementation.
     * This method should OpenGL and related drawing calls.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     */
    virtual void draw() override;
    
};

#endif /* GameEngine_hpp */
