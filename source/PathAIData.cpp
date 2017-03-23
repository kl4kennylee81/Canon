#include "PathAIData.hpp"
#include "PathAI.hpp"
#include "Util.hpp"

using namespace cugl;

std::string PathAIData::serialize() {
	return "";
}

bool PathAIData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

PathType getPathTypeFromString(const std::string& str) {
	if (str.compare("HORIZONTAL") == 0) {
		return PathType::HORIZONTAL;
	}
	if (str.compare("VERTICAL") == 0) {
		return PathType::VERTICAL;
	}
	if (str.compare("CUSTOM") == 0) {
		return PathType::CUSTOM;
	}
	return PathType::NONE;
}


Vec2 getTupleFromString(const std::string& s) {
	std::string str = s.substr(1, s.size() - 2);
	auto nums = Util::split(str, ',');
	return Vec2::Vec2(std::stoi(nums.at(0)), std::stoi(nums.at(1)));
}

std::vector<Vec2> getPathFromString(const std::string& str) {
	auto strings = Util::split(str, ' ');
	std::vector<Vec2> path;
	for (auto it : strings) {
		path.push_back(getTupleFromString(it));
	}
	return path;
}

bool PathAIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	PathType pathType = getPathTypeFromString(json->getString("pathType"));
	std::vector<Vec2> path = getPathFromString(json->getString("path"));
	init(pathType, path);
	return true;
}

bool PathAIData::materialize() {
	return true;
}

std::shared_ptr<ActiveAI> PathAIData::newActiveAI(std::shared_ptr<GameObject> object) {
	return PathAI::alloc(object, _pathType, _path);
}