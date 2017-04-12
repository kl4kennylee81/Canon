#include "TemplateWaveEntry.hpp"

using namespace cugl;



bool TemplateWaveEntry::updateFile() {
	return true;
}


bool TemplateWaveEntry::init(const std::shared_ptr<cugl::JsonValue>& json) {
	return true;
}


bool TemplateWaveEntry::init(std::string name, std::string blue, std::string gold ,
    std::string aiKey, std::vector<std::string> zoneKeys)
{
	this->name = name;
	this->blueObjectKey = blue;
    this->goldObjectKey = gold;
	this->aiKey = aiKey;
	this->zoneKeys = zoneKeys;
	return true;
}
