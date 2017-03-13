#include "HomingAI.hpp"
#include "MoveController.hpp"
#include "PathParameters.h"

void HomingAI::update(std::shared_ptr<GameState> state) {
	Element e = _object->getPhysicsComponent()->getElementType();

	// HACK we should check the actual active player list
	// and then iterate through to find closest one
	// no assumption on only 2
	int playerIndex = e == Element::BLUE ? 0 : 1;

	// HACK MUST FIX to keep from null pointering
	if (state->getPlayerCharacters().size() == 2) {
		auto player = state->getPlayerCharacters().at(playerIndex);
		cugl::Vec2 playerPos = player->getPosition();
		cugl::Vec2 enemyPos = _object->getPosition();
        
		//We should get speed from object data file
		cugl::Vec2 direction = MoveController::getVelocityVector(enemyPos, playerPos, AI_SPEED);
		_object->getPhysicsComponent()->getBody()->setLinearVelocity(direction);	
	}
}

bool HomingAI::garbageCollect(GameObject* obj) {
	return true;;
}
