#include "TemplateWaveEntry.hpp"

using namespace cugl;

#define SPAWN_FRAMES 180.0f

bool TemplateWaveEntry::updateFile() {
	return true;
}


bool TemplateWaveEntry::init(const std::shared_ptr<cugl::JsonValue>& json) {
	return true;
}


bool TemplateWaveEntry::init(std::string name, std::string obKey,
     std::vector<std::string> aiKeys, std::vector<std::string> zoneKeys,float spawnTime)
{
	this->name = name;
    this->objectKey = obKey;
	this->aiKeys = aiKeys;
	this->zoneKeys = zoneKeys;
    this->_spawnTime = spawnTime;
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
    float sTime = json->getFloat("spawnTime",SPAWN_FRAMES);
    init(name, ob, ais, zones,sTime);
	Data::preload(json);
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

std::vector<std::string> TemplateWaveEntry::getZoneKeys(){
    return zoneKeys;
}

float TemplateWaveEntry::getSpawnTime(){
    return _spawnTime;
}
