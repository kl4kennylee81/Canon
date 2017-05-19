#include "TemplateWaveEntry.hpp"

using namespace cugl;

#define SPAWN_FRAMES 180.0f

bool TemplateWaveEntry::updateFile() {
	return true;
}


bool TemplateWaveEntry::init(const std::shared_ptr<cugl::JsonValue>& json) {
	return preload(json);
}


bool TemplateWaveEntry::init(std::string name, std::string obKey, std::vector<std::string> aiKeys,
    std::vector<std::string> zoneKeys,float spawnTime, std::string bullet)
{
	this->name = name;
    this->objectKey = obKey;
	this->aiKeys = aiKeys;
	this->zoneKeys = zoneKeys;
    this->_spawnTime = spawnTime;
    this->bulletKey = bullet;
	return true;
}

std::shared_ptr<JsonValue> TemplateWaveEntry::toJsonValue()
{
    std::shared_ptr<JsonValue> object = Data::toJsonValue();
    object->appendChild("name", JsonValue::alloc(name));
    
    std::shared_ptr<JsonValue> ais = JsonValue::allocArray();
    for(int i = 0; i < aiKeys.size(); i++){
        ais->appendChild(JsonValue::alloc(aiKeys.at(i)));
    }
    
    object->appendChild("aiKeys", ais);
    object->appendChild("objectKey", JsonValue::alloc(objectKey));
    object->appendChild("spawnTime",JsonValue::alloc(_spawnTime));

    std::shared_ptr<JsonValue> zones = JsonValue::allocArray();
    for (int i = 0; i < zoneKeys.size(); i++)
    {
        zones->appendChild(JsonValue::alloc(zoneKeys.at(i)));
    }
    if (zoneKeys.size() > 0) object->appendChild("zoneKeys", zones);
	
    std::shared_ptr<JsonValue> allZones = JsonValue::allocArray();
    for (int i = 0; i < allZoneKeys.size(); i++){
        std::vector<std::string> zone = allZoneKeys.at(i);
        std::shared_ptr<JsonValue> zoneJson = JsonValue::allocArray();
        for(int j = 0; j < zone.size(); j++){
            zoneJson->appendChild(JsonValue::alloc(zone.at(j)));
        }
        allZones->appendChild(zoneJson);
    }
    
    if(allZoneKeys.size() > 0) object->appendChild("zones", allZones);
    return object;
}


bool TemplateWaveEntry::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool TemplateWaveEntry::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string name = json->getString("name");
    
    std::vector<std::string> ais;
    if(json->has("aiKeys")){
        ais = json->get("aiKeys")->asStringArray();
    }
    std::string ob = json->getString("objectKey");
    std::vector<std::string> zones = {};
    if (json->has("zoneKeys")) {
        zones = json->get("zoneKeys")->asStringArray();
    }
    
    std::string bk = "";
    if (json->has("bulletKey")) {
        bk = json->getString("bulletKey");
    }
    
    float sTime = json->getFloat("spawnTime",SPAWN_FRAMES);
	Data::preload(json);
    init(name, ob, ais, zones, sTime, bk);
    
    if(json->has("zones")) {
        std::shared_ptr<JsonValue> jzone = json->get("zones");
        for(int i = 0; i < jzone->size(); i++){
            std::vector<std::string> zs = jzone->get(i)->asStringArray();
            addZoneKeys(zs);
        }
    }
    return true;
}

bool TemplateWaveEntry::materialize(){
    return true;
}

std::string TemplateWaveEntry::getName(){
    return name;
}

std::string TemplateWaveEntry::getObjectKey(){
    return objectKey;
}

std::string TemplateWaveEntry::getNextAIKey(std::string currentKey){
    for(int i = 0; i < aiKeys.size() - 1; i++) {
        if(currentKey.compare(aiKeys.at(i)) == 0){
            std::cout << "switching to " << aiKeys.at(i+1) <<std::endl;
            return aiKeys.at(i+1);
        }
    }
    std::cout << "switching to " << aiKeys.at(0) <<std::endl;

    return aiKeys.at(0);
}

bool vectorEqual(std::vector<std::string> v1, std::vector<std::string> v2){
    if(v1.size() != v2.size()) return false;
    for(int i = 0; i < v1.size(); i++){
        if(v1.at(i).compare(v2.at(i)) != 0){
            return false;
        }
    }
    return true;
}

std::vector<std::string> TemplateWaveEntry::getNextZoneKeys(std::vector<std::string> currentKeys){
    for(int i = 0; i < allZoneKeys.size() - 1; i++) {
        if(vectorEqual(currentKeys, allZoneKeys.at(i))){
            return allZoneKeys.at(i+1);
        }
    }    
    return allZoneKeys.at(0);
}


std::vector<std::string> TemplateWaveEntry::getZoneKeys(){
    return zoneKeys;
}

float TemplateWaveEntry::getSpawnTime(){
    return _spawnTime;
}
