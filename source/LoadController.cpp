//
//  LoadController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LoadController.hpp"
#include "InputController.hpp"

using namespace cugl;

#pragma mark -
#pragma mark Loading Assets

/** This is adjusted by screen aspect ratio to get the height */
#define GAME_WIDTH 1024

/** The reference to the play button (which is "inside" the claw)*/
#define PLAY_TEXTURE    "textures/menus/loading/play.png"
#define PLAY_KEY        "play"
#define PLAY_SCALE      1.875f
#define PLAY_VOFF       0.5f

/** The reference to the progress bar textire */
#define PROGRESS_TEXTURE "textures/menus/loading/progressbar.png"
#define PROGRESS_KEY     "progressbar"
#define PROGRESS_VOFF   0.15f

float BACK_RECT[] = {0.0f,  1.0f,  0.0625f, 0.4375f};
float FORE_RECT[] = {0.04f, 0.96f, 0.5625f, 0.9375f};
float LEFT_RECT[] = {0.00f, 0.04f, 0.5625f, 0.9375f};
float RGHT_RECT[] = {0.96f, 1.00f, 0.5625f, 0.9375f};


#pragma mark -
#pragma mark Constructors

/**
 * Initializes the controller contents, making it ready for loading
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool LoadController::init(std::shared_ptr<Scene> scene,const std::shared_ptr<GenericAssetManager>& assets) {
    // IMMEDIATELY load three assets
    _assets = assets;
    assets->load<Texture>(PLAY_KEY,PLAY_TEXTURE);
    assets->load<Texture>(PROGRESS_KEY,PROGRESS_TEXTURE);
    
    _scene = scene;
    
    Rect size = _scene->getCamera()->getViewport();
    
    // The play button
    auto play = PolygonNode::allocWithTexture(assets->get<Texture>(PLAY_KEY));
    _button = Button::alloc(play);
    _button->setName(PLAY_KEY);
    _button->setAnchor(Vec2::ANCHOR_MIDDLE);
    _button->setPosition(Vec2(size.getMaxX()/2.0f,size.getMaxY()*PLAY_VOFF));
    _button->setScale(PLAY_SCALE);
    _button->setListener([=](const std::string& name, bool down) {
        this->_completed = !down;
    });
    _button->setVisible(false);
    
    // The progress bar
    auto texture = assets->get<Texture>(PROGRESS_KEY);
    auto background = texture->getSubTexture(BACK_RECT[0], BACK_RECT[1], BACK_RECT[2], BACK_RECT[3]);
    auto foreground = texture->getSubTexture(FORE_RECT[0], FORE_RECT[1], FORE_RECT[2], FORE_RECT[3]);
    auto begincap   = texture->getSubTexture(LEFT_RECT[0], LEFT_RECT[1], LEFT_RECT[2], LEFT_RECT[3]);
    auto finalcap   = texture->getSubTexture(RGHT_RECT[0], RGHT_RECT[1], RGHT_RECT[2], RGHT_RECT[3]);
    
    _bar = ProgressBar::allocWithCaps(background,foreground,begincap,finalcap);
    _bar->setAnchor(Vec2::ANCHOR_MIDDLE);
    _bar->setPosition(Vec2(size.getMaxX()/2.0f,size.getMaxY()*PROGRESS_VOFF));
    
    
    _loadNode = Node::alloc();
    _loadNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    
    _scene->addChild(_loadNode);
    
    _loadNode->addChild(_bar);
    _loadNode->addChild(_button);
    
    Application::get()->setClearColor(Color4(192,192,192,255));
    return true;
}



/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LoadController::dispose() {
    // Deactivate the button (platform dependent)
    if (isPending()) {
        _button->deactivate();
    }
    _scene->removeChild(_loadNode);
    _button = nullptr;
    _bar = nullptr;
    _scene = nullptr;
    _loadNode = nullptr;
    _assets = nullptr;
    _progress = 0.0f;
    _completed = false;
}


#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void LoadController::update(float progress) {
    if (_progress < 1) {
        _progress = _assets->progress();
        if (_progress >= 1) {
            _progress = 1.0f;
            _button->setVisible(true);
            _button->activate(1);
        }
        _bar->setProgress(_progress);
    }
}

/**
 * The method called to draw the application to the screen.
 *
 * This method should contain OpenGL and related drawing calls.
 */
void LoadController::draw(const std::shared_ptr<cugl::SpriteBatch>& batch) {
    _scene->render(batch);
}

/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool LoadController::isPending( ) const {
    return _button != nullptr && _button->isVisible();
}

void LoadController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}

void LoadController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void LoadController::notify(Event* e) {
    BaseController::notify(e);
}
void LoadController::eventUpdate(Event* e) {}

