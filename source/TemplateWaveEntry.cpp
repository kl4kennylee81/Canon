#include "TemplateWaveEntry.hpp"

using namespace cugl;

#define SPAWN_FRAMES 90.0f

bool TemplateWaveEntry::updateFile() {
	return true;
}


bool TemplateWaveEntry::init(const std::shared_ptr<cugl::JsonValue>& json) {
	return true;
}


bool TemplateWaveEntry::init(std::string name, std::string obKey,
    std::string aiKey, std::vector<std::string> zoneKeys,float spawnTime)
{
	this->name = name;
    this->objectKey = obKey;
	this->aiKey = aiKey;
	this->zoneKeys = zoneKeys;
    this->_spawnTime = spawnTime;
	return true;
}

std::shared_ptr<JsonValue> TemplateWaveEntry::toJsonValue()
{
    std::shared_ptr<JsonValue> object = JsonValue::allocObject();
    object->appendChild("name", JsonValue::alloc(name));
    object->appendChild("aiKey", JsonValue::alloc(aiKey));
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
    std::string ai = json->getString("aiKey");
    std::string ob = json->getString("objectKey");
    std::vector<std::string> zones = {};
    if (json->has("zoneKeys")) {
        zones = json->get("zoneKeys")->asStringArray();
    }
    float sTime = json->getFloat("spawnTime",SPAWN_FRAMES);
    init(name, ob, ai, zones,sTime);
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

std::string TemplateWaveEntry::getAIKey(){
    return aiKey;
}

std::vector<std::string> TemplateWaveEntry::getZoneKeys(){
    return zoneKeys;
}

float TemplateWaveEntry::getSpawnTime(){
    return _spawnTime;
}
