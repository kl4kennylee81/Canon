//
//  ParticleStateData.cpp
//  Canon
//
//  Created by Jonathan Chen on 5/13/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ParticleStateData.hpp"


using namespace cugl;

bool ParticleStateData::init(){
    return true;
}

std::shared_ptr<JsonValue> ParticleStateData::toJsonValue(){
    return nullptr;
}

bool ParticleStateData::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool ParticleStateData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    init();
    auto statemapjson = json->get("statemap");
    for (int i = 0; i < statemapjson->size(); i++) {
        auto statejson = statemapjson->get(i);
        
        std::string animationActionString = statejson->key();
        AnimationAction animAction = stringToAction(animationActionString);
        
        auto particleActionStrings = statejson->asStringArray();
        std::vector<ParticleAction> particleActions;
        for (std::string s : particleActionStrings) {
            particleActions.push_back(particleStringToAction(s));
        }
        
        stateMap.insert({animAction,particleActions});
    }
    Data::preload(json);
    return true;
}

bool ParticleStateData::materialize(){
    return true;
}

bool ParticleStateData::hasAnimationAction(AnimationAction aa){
    return stateMap.count(aa);
}

std::vector<ParticleAction> ParticleStateData::getParticleActions(AnimationAction aa){
    return stateMap.at(aa);
}
