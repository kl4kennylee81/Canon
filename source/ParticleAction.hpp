//
//  ParticleAction.hpp
//  Canon
//
//  Created by Jonathan Chen on 5/13/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ParticleAction_h
#define ParticleAction_h

enum class ParticleAction : int {
    PULSE,
    DEATH,
    TRAIL,
    ZONE,
    DEFAULT
};

static ParticleAction particleStringToAction(std::string action){
    if (action == "PULSE") return ParticleAction::PULSE;
    if (action == "DEATH") return ParticleAction::DEATH;
    if (action == "TRAIL") return ParticleAction::TRAIL;
    if (action == "ZONE") return ParticleAction::ZONE;
    return ParticleAction::DEFAULT;
}

static std::string particleActionToString(ParticleAction action)
{
    if (action == ParticleAction::PULSE) return "PULSE";
    if (action == ParticleAction::DEATH) return "DEATH";
    if (action == ParticleAction::TRAIL) return "TRAIL";
    if (action == ParticleAction::ZONE) return "ZONE";
    return "DEFAULT";
}

#endif /* ParticleAction_h */
