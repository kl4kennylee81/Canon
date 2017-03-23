//
//  ObjectData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ObjectData.hpp"

using namespace cugl;

bool ObjectData::init(int uid,int shape_id,int animation_id,int speed,int acceleration,Element element,std::vector<int> zone_ids){
    this->_uid = uid;
    this->shape_id = shape_id;
    this->animation_id = animation_id;
    this->speed = speed;
    this->acceleration = acceleration;
    this->element = element;
    this->zone_ids = zone_ids;
    return true;
};


std::string ObjectData::serialize(){
    return "";
}

bool ObjectData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool ObjectData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    int id = json->getInt("id");
	int sid = json->getInt("shape_id");
    int aid = json->getInt("animation_id");
	float spd = json->getFloat("speed");
	float acc = json->getFloat("acceleration");
	auto el = json->getString("element").compare("BLUE") ? Element::BLUE : Element::GOLD;
    std::vector<int> zids;
    if (json->has("zone_ids")) {
        zids = json->get("zone_ids")->asIntArray();
    }
	init(id, sid, aid, spd, acc, el, zids);
    return true;
}

bool ObjectData::materialize(){
    return true;
}
