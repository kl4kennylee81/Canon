//
//  ProgressBarController.cpp
//  Canon
//
//  Created by Kelly Yu on 3/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ProgressBarController.hpp"

using namespace cugl;

// hacks
#define BAR_FOREGROUND       "bar_foreground"
#define BAR_BACKGROUND       "bar_background"
#define RIGHT_CAP	         "right_cap"
#define LEFT_CAP             "left_cap"

ProgressBarController::ProgressBarController():BaseController(){}

void ProgressBarController::attach(std::shared_ptr<Observer> obs) {
	BaseController::attach(obs);
}

void ProgressBarController::detach(Observer* obs) {
	BaseController::detach(obs);
}

void ProgressBarController::notify(Event* e) {
	BaseController::notify(e);
}

void ProgressBarController::eventUpdate(Event* e) {
	switch (e->_eventType) {}
}

void ProgressBarController::update(std::shared_ptr<GameState> state,Level level)
{
    int index = level.getCurrentWave();
    std::shared_ptr<cugl::Node> child = _pBarSceneNode->getChildren().at(index);
    std::shared_ptr<cugl::ProgressBar> waveBar = std::static_pointer_cast<cugl::ProgressBar>(child);
    waveBar->setProgress(level.getProgress());
}

bool ProgressBarController::init(std::shared_ptr<GameState> state, std::shared_ptr<World> world)
{
    cugl::Size cameraSize = state->getScene()->getCamera()->getViewport().size;
    
	// some constants
	float PERCENT_WIDTH = 0.75f;
	float PERCENT_HEIGHT = 0.95f;
	float BAR_WIDTH = cameraSize.getIWidth() *PERCENT_WIDTH;
	float BAR_HEIGHT = 10.f;
	float INTER_BAR_GAP = 20.f;
	
	float barYPos = cameraSize.getIHeight()*PERCENT_HEIGHT;
	float barXPadding = cameraSize.getIWidth() * (1.f- PERCENT_WIDTH) / 2.f;
	
	// parent pbar node, child of scenenode
    if (_pBarSceneNode == nullptr){
        _pBarSceneNode = Node::alloc();
        _pBarSceneNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _pBarSceneNode->setPosition(Vec2::ZERO);
        state->getScene()->addChild(_pBarSceneNode, 3);
    }
    
    std::shared_ptr<LevelData> level = world->getLevelData();

	// not all waves are the same length, so we use the total length of the level to adjust spacing.
	float levelNetTime = 0.f;
	for (int i = 0; i < level->getNumberWaves(); i++) { levelNetTime += level->getTime(i); }

	size_t numberWaves = level->getNumberWaves() - 1;
	float spacePerUnitTime = (BAR_WIDTH - numberWaves*INTER_BAR_GAP) / levelNetTime;

	// creation of actual progress bars
	float nextBarXPos = 0;
	for (int i = 0; i <= numberWaves; i++)
	{
		// make progress bar object
		Size size = Size(level->getTime(i) * spacePerUnitTime, BAR_HEIGHT);
        
		auto manager = world->getAssetManager();

		// current problem - cugl not recognizing color of file texture
		std::shared_ptr<Texture> barForeground = manager->get<Texture>(BAR_FOREGROUND);
		std::shared_ptr<Texture> barBackground = manager->get<Texture>(BAR_BACKGROUND);
		std::shared_ptr<Texture> beginCap = manager->get<Texture>(RIGHT_CAP);
		std::shared_ptr<Texture> finalCap = manager->get<Texture>(LEFT_CAP);

        std::shared_ptr<ProgressBar> progressBar = ProgressBar::allocWithCaps(barBackground,barForeground,beginCap,finalCap,size);
		
		// temp color fix
//		const float transparent_gray[] = { 1., 1., 1., 0.25 };
//		const float less_transparent_gray[] = { 1., 1., 1., 0.75 };
//		progressBar->setBackgroundColor(transparent_gray);
//		progressBar->setForegroundColor(less_transparent_gray);

		// anchor and pos
		progressBar->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
		float barXPos = nextBarXPos + barXPadding;
		progressBar->setPosition(barXPos, barYPos);

		// set children of progressBar to the same position / anchoring
		for (int i = 0; i < progressBar->getChildCount(); i++) 
		{
			std::shared_ptr<cugl::Node> ch = progressBar->getChildren().at(i);
			ch->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
			ch->setPosition(Vec2::ZERO);
		}

		// add to parent node
		_pBarSceneNode->addChild(progressBar, 3);

		// set previous bar position for cumulative reference
		nextBarXPos += INTER_BAR_GAP + level->getTime(i)*spacePerUnitTime;
	}
	return true;
}

void ProgressBarController::cleanup(){
    _pBarSceneNode->removeAllChildren();
}

bool ProgressBarController::reset(std::shared_ptr<GameState> state, std::shared_ptr<World> world){
    cleanup();
    return init(state,world);
}

ProgressBarController::~ProgressBarController()
{
}
