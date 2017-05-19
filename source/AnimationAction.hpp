//
//  AnimationAction.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/20/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AnimationAction_h
#define AnimationAction_h

enum class AnimationAction : int {
    SPAWNING,
    SPAWN,
    ACTIVE,
    ATTACK,
    RETURN,
    HIT,
    DEATH,
    DEFAULT
};

static AnimationAction stringToAction(std::string action){
    if (action == "SPAWNING") return AnimationAction::SPAWNING;
    if (action == "SPAWN") return AnimationAction::SPAWN;
    if (action == "ACTIVE") return AnimationAction::ACTIVE;
    if (action == "ATTACK") return AnimationAction::ATTACK;
    if (action == "RETURN") return AnimationAction::RETURN;
    if (action == "HIT") return AnimationAction::HIT;
    if (action == "DEATH") return AnimationAction::DEATH;
    std::cout << "stringToAction: AnimationData: default action?\n";
    return AnimationAction::DEFAULT;
}

static std::string actionToString(AnimationAction action)
{
    if (action == AnimationAction::SPAWNING) return "SPAWNING";
    if (action == AnimationAction::SPAWN) return "SPAWN";
    if (action == AnimationAction::ACTIVE) return "ACTIVE";
    if (action == AnimationAction::ATTACK) return "ATTACK";
    if (action == AnimationAction::RETURN) return "RETURN";
    if (action == AnimationAction::HIT) return "HIT";
    if (action == AnimationAction::DEATH) return "DEATH";
    std::cout << "actionToString: AnimationData: default action?\n";
    return "DEFAULT";
}

#endif /* AnimationAction_h */
