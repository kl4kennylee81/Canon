//
//  ProgressBarController.cpp
//  Canon
//
//  Created by Kelly Yu on 3/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ProgressBarController.hpp"

using namespace cugl;

// hack progress bar fill
#define BAR_FOREGROUND           "bar_foreground"
#define BEGINCAP_FGRD            "beginCap_foreground"
#define FINALCAP_FGRD            "finalCap_foreground"

// hack progress bar back
#define BAR_BACKGROUND           "bar_background"
#define BEGINCAP_BGRD	         "beginCap_background"
#define FINALCAP_BGRD            "finalCap_background"

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
    std::shared_ptr<cugl::ProgressBarModel> waveBar = std::static_pointer_cast<cugl::ProgressBarModel>(child);
    waveBar->toggleActive();
    waveBar->setProgress(level.getProgress());
}

bool ProgressBarController::init(std::shared_ptr<GameState> state, std::shared_ptr<World> world)
{
    cugl::Size cameraSize = state->getScene()->getCamera()->getViewport().size;
    
	// Constants
	float PERCENT_WIDTH = 0.8f;
	float PERCENT_HEIGHT = 0.96f;
	float BAR_WIDTH = cameraSize.getIWidth() *PERCENT_WIDTH;
    
    // world coordinates are fixed bar will be same height in all screen aspect ratios
	float BAR_HEIGHT = 64.f;
    
    // x coordinate in world coordinate is fixed to 1024 so discrete values is okay
    
    // since kelly's assets have blankspace padding on begin cap and final cap the gap
    // needs to be negative to close the whitespace.
    
    // TODO specify the padding in the UI element data file
	float INTER_BAR_GAP = -20.f;
	
	float barYPos = cameraSize.getIHeight()*PERCENT_HEIGHT;
    
    // padding for the left side to center the bars
	float barXPadding = cameraSize.getIWidth() * (1.f- PERCENT_WIDTH) / 2.f;
	
	// parent pbar node, child of scenenode
    if (_pBarSceneNode == nullptr){
        _pBarSceneNode = Node::alloc();
        _pBarSceneNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _pBarSceneNode->setPosition(Vec2::ZERO);
        state->getScene()->addChild(_pBarSceneNode, 3);
    }
    
    auto manager = world->getAssetManager();
    
    // TODO when UI Element data file encapsulate it in a data file rather than macros
    std::shared_ptr<Texture> barForeground = manager->get<Texture>(BAR_FOREGROUND);
    std::shared_ptr<Texture> beginCap_f = manager->get<Texture>(BEGINCAP_FGRD);
    std::shared_ptr<Texture> finalCap_f = manager->get<Texture>(FINALCAP_FGRD);
    
    std::shared_ptr<Texture> barBackground = manager->get<Texture>(BAR_BACKGROUND);
    std::shared_ptr<Texture> beginCap_b = manager->get<Texture>(BEGINCAP_BGRD);
    std::shared_ptr<Texture> finalCap_b = manager->get<Texture>(FINALCAP_BGRD);

    
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
        
        // since progress bar is anchored in the middle add half the width size;
        nextBarXPos += level->getTime(i)*spacePerUnitTime/2.f;
        
        std::shared_ptr<ProgressBarModel> progressBar = ProgressBarModel::allocWithCaps(barBackground,beginCap_b,finalCap_b,
                                                                                   barForeground,beginCap_f,finalCap_f,size);

		// anchor and pos
		float barXPos = nextBarXPos + barXPadding;
		progressBar->setPosition(barXPos, barYPos);

		// add to parent node
		_pBarSceneNode->addChild(progressBar, 3);

		// after drawing the bar add the latter half and the gap between bars
        nextBarXPos += level->getTime(i)*spacePerUnitTime/2.f + INTER_BAR_GAP;
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
