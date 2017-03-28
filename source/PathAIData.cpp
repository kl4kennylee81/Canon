#include "PathAIData.hpp"
#include "PathAI.hpp"
#include "GameState.hpp"

using namespace cugl;

bool PathAIData::init(PathType pathType, std::vector<cugl::Vec2> path) {
    _pathType = pathType;
    _path = path;
    
    // scale to physics coordinates
    for (auto vec : _path){
        vec = vec / (GAME_PHYSICS_SCALE);
    }
    
    return true;
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

std::string getStringFromPathType(const PathType pt)
{
	if (pt == PathType::HORIZONTAL) return "HORIZONTAL";
	if (pt == PathType::VERTICAL) return "VERTICAL";
	if (pt == PathType::CUSTOM) return "CUSTOM";
	return "NONE";
}

std::vector<std::string> split(const std::string& s, const char& c)
{
	std::string buff{ "" };
	std::vector<std::string> v;

	for (auto n : s)
	{
		if (n != c) {
			buff += n;
		}
		else if (n == c && buff != "") {
			v.push_back(buff); buff = "";
		}
	}
	if (buff != "") v.push_back(buff);
	return v;
}

Vec2 getTupleFromString(const std::string& s) {
	std::string str = s.substr(1, s.size() - 2);
	auto nums = split(str, ',');
	return Vec2::Vec2(std::stoi(nums.at(0)), std::stoi(nums.at(1)));
}

std::vector<Vec2> getPathFromString(const std::string& str) {
	auto strings = split(str, ' ');
	std::vector<Vec2> path;
	for (auto it : strings) {
		path.push_back(getTupleFromString(it));
	}
	return path;
}

std::string getStringFromPath(std::vector<Vec2> vecPath)
{
	std::string acc = "";
	for (int i = 0; i < vecPath.size(); i++)
	{
		acc += "(" + std::to_string(vecPath.at(i).x) + "," + std::to_string(vecPath.at(i).y) + ")";
		if (i < vecPath.size() - 1) acc += " ";
	}
	return acc;
}

std::string PathAIData::serialize() {
	std::shared_ptr<JsonValue> data = JsonValue::allocObject();
	data->appendChild("type", JsonValue::alloc("PATH"));
	data->appendChild("pathType", JsonValue::alloc(getStringFromPathType(_pathType)));
	data->appendChild("path", JsonValue::alloc(getStringFromPath(_path)));
	return data->toString();
}


bool PathAIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	type = AIType::PATH;
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
