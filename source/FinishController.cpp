//
//  FinishController.cpp
//  Canon
//
//  Created by Kenneth Lee on 4/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "FinishController.hpp"
#include "AnimationEvent.hpp"
#include "FinishEvent.hpp"

using namespace cugl;

FinishController::FinishController() :
wavesFinished(false),
playerDead(false),
BaseController() {}

void FinishController::attach(Observer* obs) {
    BaseController::attach(obs);
}
void FinishController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void FinishController::notify(Event* e) {
    BaseController::notify(e);
}

void FinishController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::ANIMATION:
        {
            AnimationEvent* animEvent = (AnimationEvent*) e;
            switch (animEvent->_animationType){
                case AnimationEvent::AnimationEventType::PLAYER_REMOVED:
                {
                    playerDead = true;
                }
            }
        }
    }
}

void FinishController::update(float timestep, std::shared_ptr<GameState> state){
    if (playerDead)
    {
        // notify the gameplay controller the game is lost
        std::shared_ptr<Event> fEvent = GameLostEvent::alloc();
        this->notify(fEvent.get());
    }
    
    if (wavesFinished && state->getNumberEnemyCharacters() == 0) //all enemies are dead
    {
        // notify the gameplayController that the game is won
        // notify the gameplay controller the game is lost
        std::shared_ptr<Event> fEvent = GameWonEvent::alloc();
        this->notify(fEvent.get());
    }
}

bool FinishController::init(std::shared_ptr<GameState> state){
    wavesFinished = false;
    playerDead = false;
    return true;
}
