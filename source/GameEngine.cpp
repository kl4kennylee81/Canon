//
//  GameEngine.cpp
//  Canon
//
//  Created by Jonathan Chen on 2/24/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameEngine.hpp"
#include <cugl/base/CUBase.h>
#include "MenuScreenData.hpp"
#include "SaveGameData.hpp"
#include "ZoneLoader.hpp"
#include "AILoader.hpp"

// Add support for simple random number generation
#include <cstdlib>
#include <ctime>

// This keeps us from having to write cugl:: all the time
using namespace cugl;

// The number of frames before moving the logo to a new position
#define TIME_STEP 60
// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

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
void GameEngine::onStartup() {
    Size size = getDisplaySize();
    size *= GAME_WIDTH/size.width;
    
    // Create a sprite batch (and background color) to render the scene
    _batch = SpriteBatch::alloc();
    
    // Create an asset manager to load all assets
    _assets = GenericAssetManager::alloc();
    
    // You have to attach the individual loaders for each asset type
	_assets->attach<SaveGameData>(GenericLoader<SaveGameData>::alloc()->getHook());
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Font>(FontLoader::alloc()->getHook());
	_assets->attach<LevelData>(GenericLoader<LevelData>::alloc()->getHook());
	_assets->attach<WaveData>(GenericLoader<WaveData>::alloc()->getHook());
	_assets->attach<ObjectData>(GenericLoader<ObjectData>::alloc()->getHook());
	_assets->attach<PathData>(GenericLoader<PathData>::alloc()->getHook());
	_assets->attach<ShapeData>(GenericLoader<ShapeData>::alloc()->getHook());
	_assets->attach<AnimationData>(GenericLoader<AnimationData>::alloc()->getHook());
	_assets->attach<MenuScreenData>(GenericLoader<MenuScreenData>::alloc()->getHook());
	_assets->attach<AIData>(AILoader::alloc()->getHook());
    _assets->attach<ZoneData>(ZoneLoader::alloc()->getHook());
    
    _loading = LoadController::alloc(_assets);
    
    // have a shell menu controller since it holds the menu graph
    _menu = MenuController::alloc(_assets);
    
    // This reads the given JSON file and uses it to load all other assets

    _assets->loadDirectory("json/assets.json");
	_assets->loadDirectory("json/level.json");
    _assets->loadDirectory("json/animations.json");
	_assets->loadDirectory("json/ai.json");

	_assets->loadDirectory("json/save.json");


    
    // Activate mouse or touch screen input as appropriate
    // We have to do this BEFORE the scene, because the scene has a button
#if defined (CU_TOUCH_SCREEN)
	_touch = true;
    Input::activate<Touchscreen>();
#else
	_touch = false;
    Input::activate<Mouse>();
	Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::DRAG);
#endif
    Application::onStartup();
}

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
void GameEngine::onShutdown() {
    // Delete all smart pointers
    _batch = nullptr;
    _assets = nullptr;
    
    // Deativate input
#if defined CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif
    Application::onShutdown();
}

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
void GameEngine::update(float timestep) {
    if (_menu->getMode() == Mode::LOADING && !_loading->isComplete()) {
        _loading->update(0.01f);
    } else {
        switch (_menu->getMode()){
            case Mode::LOADING:
            {
                _loading->dispose(); // Disables the input listeners in this mode
                std::shared_ptr<World> levelWorld = World::alloc(_assets);
                _gameplay = GameplayController::alloc(levelWorld, _touch);
                _menu->setMode(Mode::MAIN_MENU);
                break;
            }
            case Mode::GAMEPLAY:
            {
                _gameplay->update(timestep);
                break;
            }
            case Mode::MAIN_MENU:
            {
                _menu->update(timestep);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void GameEngine::draw() {
    // This takes care of begin/end
    switch (_menu->getMode()){
        case Mode::LOADING:
            _loading->draw(_batch);
            break;
        case Mode::GAMEPLAY:
            _gameplay->draw(_batch);
            break;
        case Mode::MAIN_MENU:
            _menu->draw(_batch);
            break;
        default:
            break;
    }
}

