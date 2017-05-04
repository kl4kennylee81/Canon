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
#include "InputController.hpp"
#include "UIData.hpp"
#include "UIDataLoader.hpp"
#include "Util.hpp"
#include <memory>
#include <iostream>
#include <fstream>

// Add support for simple random number generation
#include <cstdlib>
#include <ctime>

// This keeps us from having to write cugl:: all the time
using namespace cugl;

// The number of frames before moving the logo to a new position
#define TIME_STEP 60
// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

bool GameEngine::_touch;



void loadFilesApple(const std::shared_ptr<GenericAssetManager> &assets, std::string dirToRead, std::string path) {
#ifdef __APPLE__
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dirToRead.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] != '.') {
				std::string pathToLoad = path;
				pathToLoad.append(ent->d_name);
				assets->loadDirectory(pathToLoad);
			}
		}
		closedir(dir);
	}
#endif
}

void loadFilesWindows(const std::shared_ptr<GenericAssetManager> &assets, std::string templateDir, std::string path) {
#ifdef _WIN32
	WIN32_FIND_DATA file;
	HANDLE hndl = FindFirstFile((templateDir + "*").c_str(), &file);

	if (hndl != INVALID_HANDLE_VALUE) {
		do {
			std::string filename = file.cFileName;
			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// directory logic
			}
			else {
				std::string templatePath = path;
				templatePath.append(filename);
				assets->loadDirectory(templatePath);
			}
		} while (FindNextFile(hndl, &file));	
	}
	FindClose(hndl);
	
#endif
}

void GameEngine::attachLoaders(std::shared_ptr<GenericAssetManager> assets){
    // You have to attach the individual loaders for each asset type
    assets->attach<SaveGameData>(GenericLoader<SaveGameData>::alloc()->getHook());
    assets->attach<Texture>(TextureLoader::alloc()->getHook());
    assets->attach<Font>(FontLoader::alloc()->getHook());
    assets->attach<Sound>(SoundLoader::alloc()->getHook());
    assets->attach<Music>(MusicLoader::alloc()->getHook());
    assets->attach<LevelData>(GenericLoader<LevelData>::alloc()->getHook());
    assets->attach<WaveData>(GenericLoader<WaveData>::alloc()->getHook());
    assets->attach<ObjectData>(GenericLoader<ObjectData>::alloc()->getHook());
    assets->attach<ShapeData>(GenericLoader<ShapeData>::alloc()->getHook());
    assets->attach<AnimationData>(GenericLoader<AnimationData>::alloc()->getHook());
    assets->attach<MenuScreenData>(GenericLoader<MenuScreenData>::alloc()->getHook());
    assets->attach<UIData>(UIDataLoader::alloc()->getHook());
    assets->attach<AIData>(AILoader::alloc()->getHook());
    assets->attach<ZoneData>(ZoneLoader::alloc()->getHook());
    assets->attach<SoundData>(GenericLoader<SoundData>::alloc()->getHook());
    assets->attach<TemplateWaveEntry>(GenericLoader<TemplateWaveEntry>::alloc()->getHook());
}

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
    
    _scene = Scene::alloc(size);
    
    // Create a sprite batch (and background color) to render the scene
    _batch = SpriteBatch::alloc();
    
    // Create an asset manager to load all assets
    _assets = GenericAssetManager::alloc();
    
    attachLoaders(_assets);
    
    _loading = LoadController::alloc(_scene,_assets);
    _loading->activate();
    
    // create the menuGraph
    _menuGraph = MenuGraph::alloc();
    
    // Initialize Audio Engine
    AudioEngine::start();
    
    
    // This reads the given JSON file and uses it to load all other assets

    _assets->loadDirectory("json/assets.json");
    //_assets->loadDirectory("json/animations.json");
    _assets->loadDirectory("json/sounds.json");
	//_assets->loadDirectory("json/ai.json");
	_assets->loadDirectory("json/menu.json");
	_assets->loadDirectory("json/save.json");
    //_assets->loadDirectory("json/level.json");
    
    /** THIS IS TEMPORARY CODE TO SHOWCASE EXAMPLE */
    
    std::string assetDir = Application::get()->getAssetDirectory();
    std::string templateDir = assetDir + TEMPLATE_PATH;
    std::string levelDir = assetDir + LEVEL_PATH;
    
    // std::cout << "current file directory: "<< __FILE__ << std::endl;
    std::vector<std::string> vec = Util::split(__FILE__, '/');
    
    // std::cout << "Retrieve Key "<< Util::join(vec,vec.size()-2,'/') << std::endl;
    
    //load all template wave entries

	#ifdef _WIN32
	loadFilesWindows(_assets, templateDir, TEMPLATE_PATH);
    loadFilesWindows(_assets, levelDir, LEVEL_PATH);
	#elif __APPLE__
	loadFilesApple(_assets, templateDir, TEMPLATE_PATH);
    loadFilesApple(_assets, levelDir, LEVEL_PATH);
	#endif

    
    /** END OF TEMPORARY CODE TO WIPE **/


    
    // Activate mouse or touch screen input as appropriate
    // We have to do this BEFORE the scene, because the scene has a button
#if defined (CU_TOUCH_SCREEN)
	_touch = true;
    InputController::setTouch(true);
	Input::activate<Touchscreen>();
#else
	_touch = false;
    Input::activate<Mouse>();
	Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::DRAG);
#endif
	Input::activate<Keyboard>();
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
    AudioEngine::stop();
	Input::deactivate<Keyboard>();
    Application::onShutdown();
}

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
void GameEngine::onSuspend() {
    AudioEngine::get()->pauseAll();
    
    std::shared_ptr<JsonValue> suspendJson = JsonValue::allocObject();
    suspendJson->appendChild("menuGraph", _menuGraph->toJsonValue());
    switch(_menuGraph->getMode()){
        case Mode::GAMEPLAY:
        {
            suspendJson->appendChild("gameplay",_gameplay->toJsonValue());
        }
    }
//    // save the suspendJson to a file in the saveDirectory only exists when running the game
//    // its in a temporary directory of the app
//    std::string saveDir = Application::get()->getSaveDirectory();
    
    std::vector<std::string> vec = Util::split(__FILE__, '/');
    std::string saveDir = Util::join(vec,vec.size()-2,'/') + "/assets";
    
    std::string filePath = "/"+saveDir+"/suspend.json";
    
    std::cout << filePath << std::endl;
    std::ofstream newFile;
    newFile.open(filePath);
    newFile << suspendJson->toString();
    newFile.close();
}

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
void GameEngine::onResume() {
    AudioEngine::get()->resumeAll();
    
}

std::shared_ptr<LevelData> GameEngine::getNextLevelData(){
    switch (_menuGraph->getMode()){
        case Mode::LOADING:
        {
            // TODO replace hardcoding it's just for now
            std::shared_ptr<LevelData> level = _assets->get<LevelData>("level0");
        }
        case Mode::GAMEPLAY:
        {
            return _gameplay->getCurrentLevel();
        }
        case Mode::MAIN_MENU:
        {
            std::shared_ptr<LevelData> level = _assets->get<LevelData>(_menu->getSelectedLevel());
            return level;
        }
        case Mode::LEVEL_EDIT:
        {
            std::shared_ptr<LevelData> level = _levelEditor->getCurrentLevelData();
            return level;
        }
        default:
        {
            break;
        }
    }
    return nullptr;
}

std::shared_ptr<World> GameEngine::getNextWorld(){
    switch (_menuGraph->getMode()){
        case Mode::LOADING:
        {
            std::shared_ptr<LevelData> level = getNextLevelData();
            std::shared_ptr<World> levelWorld = World::alloc(_assets,level);
            return levelWorld;
        }
        case Mode::GAMEPLAY:
        {
            return _gameplay->getWorld();
        }
        case Mode::MAIN_MENU:
        {
            std::shared_ptr<LevelData> level = getNextLevelData();
            std::shared_ptr<World> levelWorld = World::alloc(_assets,level);
            return levelWorld;
        }
        case Mode::LEVEL_EDIT:
        {
            std::shared_ptr<World> levelWorld = _levelEditor->getWorld();
            // TODO will replace once we give a default value to the levelStubs
            return levelWorld;
            break;
        }
        default:
        {
            break;
        }
    }
    return nullptr;
}

void GameEngine::cleanPreviousMode(){
    // clean up resources in previous mode
    switch(_menuGraph->getMode()){
        case Mode::LOADING:
        {
            _loading = nullptr;
            break;
        }
        case Mode::GAMEPLAY:
        {
            _gameplay->deactivate();
            _gameplay = nullptr;
            _menu = nullptr;
            break;
        }
        case Mode::MAIN_MENU:
        {
            _menu = nullptr;
            break;
        }
        case Mode::LEVEL_EDIT:
        {
            _menu = nullptr;
            _levelEditor = nullptr;
            break;
        }
        default:
        {
            break;
        }
    }
}

void GameEngine::initializeNextMode(){
    // initialize resources for next mode
    switch(_menuGraph->getNextMode()){
        case Mode::LOADING:
        {
            break;
        }
        case Mode::GAMEPLAY:
        {
            std::shared_ptr<World> levelWorld = getNextWorld();
            _gameplay = GameplayController::alloc(_scene, levelWorld);
            break;
        }
        case Mode::MAIN_MENU:
        {
//            _menu = MenuController::alloc(_scene,_menuGraph);
//            //TODO replace hard coded populate with menus loaded from data file
            break;
        }
        case Mode::LEVEL_EDIT:
        {
            // TODO likely have menuController always active but initialize it with
            // the menus when it is in that mode otherwise it is just there.
            _levelEditor = LevelEditorController::alloc(_scene,_assets);
            break;
        }
        default:
        {
            break;
        }
    }
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
    // update the touch input
    InputController::update();
    
//    std::cout << timestep << std::endl;
    
    if (_menuGraph->needsUpdate()){
        initializeNextMode();
        cleanPreviousMode();
        
        _menu = MenuController::alloc(_scene,_menuGraph);
        if (_gameplay != nullptr){
            // this is so that the menu can interact with the game screen
            _menu->attach(_gameplay.get());
            _gameplay->attach(_menu.get());
        }
        
        if (_levelEditor != nullptr){
            _levelEditor = LevelEditorController::alloc(_scene, _assets);
            _levelEditor->attach(_menu.get());
        }
        
        _menuGraph->updateToNextMode();
    }
    // update the game
    switch(_menuGraph->getMode()){
        case Mode::LOADING:
        {
            if (!_loading->isComplete()){
                _loading->update(0.01f);
            } else {
                // TODO loadController should also holds onto the next mode
                // so it can transition after loading to other screens when needed
                // ex. useful in loading before a level
                _menuGraph->populate(_assets);
                _menuGraph->setNextMode(Mode::MAIN_MENU);
                
                onSuspend();
            }
            break;
        }
        case Mode::GAMEPLAY:
        {
			_menu->update(timestep);
            _gameplay->update(timestep);
            break;
        }
        case Mode::MAIN_MENU:
        {
            _menu->update(timestep);
            break;
        }
        case Mode::LEVEL_EDIT:
        {
            _levelEditor->update(timestep,_menuGraph);
            break;
        }
        default:
        {
            break;
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
    _scene->sortZOrder();
    _scene->render(_batch);
}
