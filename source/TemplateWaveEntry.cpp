#include "TemplateWaveEntry.hpp"

using namespace cugl;



bool TemplateWaveEntry::updateFile() {
	return true;
}


bool TemplateWaveEntry::init(const std::shared_ptr<cugl::JsonValue>& json) {
	return true;
}


bool TemplateWaveEntry::init(std::string name, std::string objectKey, std::string aiKey, std::vector<std::string> zoneKeys) {
	this->name = name;
	this->objectKey = objectKey;
	this->aiKey = aiKey;
	this->zoneKeys = zoneKeys;
	return true;
}
