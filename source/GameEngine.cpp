//
//  GameEngine.cpp
//  Canon
//
//  Created by Jonathan Chen on 2/24/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifdef _WIN32
#define CURRENT_OS "windows"
#define TEMPLATE_PATH "json\\templates\\"
#define LEVEL_PATH "json\\fullLevels\\"
#define MENU_PATH "json\\menus\\"
#define CHAPTER_PATH "json\\chapters\\"
#define TUTORIAL_PATH "json\\tutorials\\"
#elif __APPLE__
#define CURRENT_OS "apple"
#define TEMPLATE_PATH "json/templates/"
#define LEVEL_PATH "json/fullLevels/"
#define MENU_PATH "json/menus/"
#define CHAPTER_PATH "json/chapters/"
#define TUTORIAL_PATH "json/tutorials/"
#include <dirent.h>
#endif

#include "GameEngine.hpp"
#include <cugl/base/CUBase.h>
#include "SaveData.hpp";
#include "SaveLevelData.hpp"
#include "SaveChapterData.hpp"
#include "ChapterSelectData.hpp"
#include "ChapterListData.hpp"
#include "LevelSelectData.hpp"
#include "MenuScreenData.hpp"
#include "MenuListData.hpp"
#include "MenuEvent.hpp"
#include "ZoneLoader.hpp"
#include "AILoader.hpp"
#include "InputController.hpp"
#include "UIData.hpp"
#include "UIDataLoader.hpp"
#include "Util.hpp"
#include <memory>
#include <iostream>
#include <fstream>
#include "BulletData.hpp"
#include "ParticleStateData.hpp"
#include "TutorialLevelData.hpp"
#include "TutorialStepData.hpp"

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
    assets->attach<SaveLevelData>(GenericLoader<SaveLevelData>::alloc()->getHook());
	assets->attach<SaveChapterData>(GenericLoader<SaveChapterData>::alloc()->getHook());
	assets->attach<SaveData>(GenericLoader<SaveData>::alloc()->getHook());
	assets->attach<ChapterListData>(GenericLoader<ChapterListData>::alloc()->getHook());
	assets->attach<ChapterSelectData>(GenericLoader<ChapterSelectData>::alloc()->getHook());
	assets->attach<LevelSelectData>(GenericLoader<LevelSelectData>::alloc()->getHook());
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
	assets->attach<MenuListData>(GenericLoader<MenuListData>::alloc()->getHook());
    assets->attach<BulletData>(GenericLoader<BulletData>::alloc()->getHook());
    assets->attach<ParticleStateData>(GenericLoader<ParticleStateData>::alloc()->getHook());
    assets->attach<TutorialLevelData>(GenericLoader<TutorialLevelData>::alloc()->getHook());
    assets->attach<TutorialStepData>(GenericLoader<TutorialStepData>::alloc()->getHook());
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
    _assets->loadDirectory("json/sounds.json");
	_assets->loadDirectory("json/menuList.json");
	_assets->loadDirectory("json/chapterList.json");
	_assets->loadDirectory("json/save.json");
    _assets->loadDirectory("json/particleStates.json");
    _assets->loadDirectory("json/uianimations.json");
    
    std::string assetDir = Application::get()->getAssetDirectory();
    std::string templateDir = assetDir + TEMPLATE_PATH;
    std::string levelDir = assetDir + LEVEL_PATH;
	std::string menuDir = assetDir + MENU_PATH;
	std::string chapterDir = assetDir + CHAPTER_PATH;
    std::string tutorialDir = assetDir + TUTORIAL_PATH;
    
    //load all template wave entries

	#ifdef _WIN32
	loadFilesWindows(_assets, templateDir, TEMPLATE_PATH);
    loadFilesWindows(_assets, levelDir, LEVEL_PATH);
	loadFilesWindows(_assets, menuDir, MENU_PATH);
	loadFilesWindows(_assets, chapterDir, CHAPTER_PATH);
    loadFilesWindows(_assets, tutorialDir, TUTORIAL_PATH);
	#elif __APPLE__
	loadFilesApple(_assets, templateDir, TEMPLATE_PATH);
    loadFilesApple(_assets, levelDir, LEVEL_PATH);
	loadFilesApple(_assets, menuDir, MENU_PATH);
	loadFilesApple(_assets, chapterDir, CHAPTER_PATH);
    loadFilesApple(_assets, tutorialDir, TUTORIAL_PATH);
	#endif
    
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
    
	#ifdef _WIN32
	std::vector<std::string> vec = Util::split(__FILE__, '\\');
	std::string saveDir = Util::join(vec, vec.size() - 2, '\\') + "\\assets";
	std::string filePath = saveDir + "\\suspend.json";

	#elif __APPLE__
	std::vector<std::string> vec = Util::split(__FILE__, '/');
	std::string saveDir = Util::join(vec, vec.size() - 2, '/') + "/assets";
	std::string filePath = "/" + saveDir + "/suspend.json";

	#endif
    
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
   
	#ifdef _WIN32
	std::vector<std::string> vec = Util::split(__FILE__, '\\');
	std::string saveDir = Util::join(vec, vec.size() - 2, '\\') + "\\assets";
	std::string filePath = saveDir + "\\suspend.json";

	#elif __APPLE__
	std::vector<std::string> vec = Util::split(__FILE__, '/');
	std::string saveDir = Util::join(vec, vec.size() - 2, '/') + "/assets";
	std::string filePath = "/" + saveDir + "/suspend.json";

	#endif

    auto reader = JsonReader::alloc(filePath);
    if(reader == nullptr) {
        return;
    }
    std::shared_ptr<JsonValue> resumeJson = reader->readJson();
    
    // have to reload all data files
    this->onStartup();
    
    // poll while loading
    while (_assets->progress() < 1.0){
    }
    
    // this is to clear the outdated screen when resuming
    if (_menu!=nullptr){
        _menu->deactivate();
        _menu = nullptr;
    }
    if (_gameplay != nullptr){
        _gameplay->deactivate();
        _gameplay = nullptr;
    }
    // possibly call all the load methods again
    std::shared_ptr<World> levelWorld = World::alloc(_assets);
    
    // TODO replace with allocing the menuGraph and also reloading in all the files
    _gameplay = GameplayController::alloc(_scene,levelWorld,nullptr,"");
    
    _gameplay->onResume(resumeJson->get("gameplay"));
    
    _menuGraph->populate(_assets);
    _menuGraph->initAfterResume(resumeJson->get("menuGraph"));
    
    _menu = MenuController::alloc(_scene,_menuGraph, _assets);
    
    if (_gameplay != nullptr){
        // this is so that the menu can interact with the game screen
        _menu->attach(_gameplay.get());
        _gameplay->attach(_menu.get());
    }
    
    if (_levelEditor != nullptr){
        _levelEditor = LevelEditorController::alloc(_scene, _assets);
        _levelEditor->attach(_menu.get());
    }
    
    // send an event to gameplayController to pause the game
    std::shared_ptr<Event> pauseEvent = PauseGameEvent::alloc(true);
    _menu->notify(pauseEvent.get());
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
            std::shared_ptr<LevelSelectData> lsData = _assets->get<LevelSelectData>(_menu->getSelectedLevel());
            std::shared_ptr<LevelData> level = _assets->get<LevelData>(lsData->levelKey);
            AudioEngine::get()->playMusic(_assets->get<Music>(lsData->songKey),true);
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
            std::string curChap = _menuGraph->getCurrentChapter();
            std::shared_ptr<ChapterSelectData> chapterData = _assets->get<ChapterSelectData>(curChap);
            _gameplay = GameplayController::alloc(_scene, levelWorld,chapterData, _menu->getSelectedLevel());
            break;
        }
        case Mode::MAIN_MENU:
        {
//            _menu = MenuController::alloc(_scene,_menuGraph);
//            //TODO replace hard coded populate with menus loaded from data file
            AudioEngine::get()->playMusic(_assets->get<Music>("menu"),true);
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
    
    if (_menuGraph->needsUpdate()){
        initializeNextMode();
        cleanPreviousMode();
        
        _menu = MenuController::alloc(_scene,_menuGraph, _assets);
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
            // TODO loadController should also holds onto the next mode
            // so it can transition after loading to other screens when needed
            // ex. useful in loading before a level
            _menuGraph->populate(_assets);
            _menuGraph->setNextMode(Mode::MAIN_MENU);
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
