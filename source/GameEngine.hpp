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
#include "LoadController.hpp"
#include "MenuController.hpp"
#include "GenericAssetManager.hpp"
#include "LevelEditorController.hpp"

/**
 * Class for a simple Hello World style application
 *
 * The application simply moves the CUGL logo across the screen.  It also
 * provides a button to quit the application.
 */
class GameEngine : public cugl::Application {
protected:
    /** The loaders to (synchronously) load in assets */
    std::shared_ptr<GenericAssetManager> _assets;
    /** A scene graph, used to display our 2D scenes */
    std::shared_ptr<cugl::Scene> _scene;
    /** A 3152 style SpriteBatch to render the scene */
    std::shared_ptr<cugl::SpriteBatch>  _batch;
    


    // Player modes
    
    /** the load controller */
    std::shared_ptr<LoadController> _loading;
    
    /** The menu controller */
    std::shared_ptr<MenuController> _menu;
    
    /** menu graph **/
    std::shared_ptr<MenuGraph> _menuGraph;
    
    /** The primary controller for the game world */
    std::shared_ptr<GameplayController> _gameplay;
    
    /** level editor controller */
    std::shared_ptr<LevelEditorController> _levelEditor;
    
public:
	/** Represents whether we are using touch screen or not */
	static bool _touch;

    /**
     * Creates, but does not initialized a new application.
     *
     * This constructor is called by main.cpp.  You will notice that, like
     * most of the classes in CUGL, we do not do any initialization in the
     * constructor.  That is the purpose of the init() method.  Separation
     * of initialization from the constructor allows main.cpp to perform
     * advanced configuration of the application before it starts.
     */
    GameEngine() : Application(){}
    
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
     * The method called when the application is suspended and put in the background.
     *
     * When this method is called, you should store any state that you do not
     * want to be lost.  There is no guarantee that an application will return
     * from the background; it may be terminated instead.
     *
     * If you are using audio, it is critical that you pause it on suspension.
     * Otherwise, the audio thread may persist while the application is in
     * the background.
     */
    virtual void onSuspend() override;
    
    /**
     * The method called when the application resumes and put in the foreground.
     *
     * If you saved any state before going into the background, now is the time
     * to restore it. This guarantees that the application looks the same as
     * when it was suspended.
     *
     * If you are using audio, you should use this method to resume any audio
     * paused before app suspension.
     */
    virtual void onResume()  override;
    
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
    
    void cleanPreviousMode();
    
    void initializeNextMode();
    
    std::shared_ptr<LevelData> getNextLevelData();
    
    std::shared_ptr<World> getNextWorld();
    
};

#endif /* GameEngine_hpp */
