#include "PathAIData.hpp"
#include "PathAI.hpp"
#include "GameState.hpp"
#include "Util.hpp"

using namespace cugl;

bool PathAIData::init(PathType pathType, std::vector<cugl::Vec2> path, PathDirection direction) {
    _pathType = pathType;
    _path = path;
	_direction = direction;

    // scale to physics coordinates
    for (auto vec : _path){
        vec = vec;
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

PathDirection getDirectionFromString(const std::string& str) {
    if (str.compare("UP") == 0) {
        return PathDirection::UP;
    }
    if (str.compare("DOWN") == 0) {
        return PathDirection::DOWN;
    }
    if (str.compare("LEFT") == 0) {
        return PathDirection::LEFT;
    }
    if (str.compare("RIGHT") == 0) {
        return PathDirection::RIGHT;
    }
    return PathDirection::RANDOM;
}

std::string getStringFromDirection(PathDirection direction){
    switch(direction){
        case PathDirection::UP:
        {
            return "UP";
        }
        case PathDirection::DOWN:
        {
            return "DOWN";
        }
        case PathDirection::LEFT:
        {
            return "LEFT";
        }
        case PathDirection::RIGHT:
        {
            return "RIGHT";
        }
        case PathDirection::RANDOM:
        {
            return "RANDOM";
        }
    }
}

std::shared_ptr<JsonValue> PathAIData::toJsonValue() {
	std::shared_ptr<JsonValue> data = Data::toJsonValue();
	data->appendChild("type", JsonValue::alloc("PATH"));
	data->appendChild("pathType", JsonValue::alloc(PathAI::getStringFromPathType(_pathType)));
    switch (_pathType){
        case PathType::HORIZONTAL:
        {
            data->appendChild("direction",JsonValue::alloc(getStringFromDirection(_direction)));
            break;
        }
        case PathType::VERTICAL:
        {
            data->appendChild("direction",JsonValue::alloc(getStringFromDirection(_direction)));
            break;
        }
        case PathType::CUSTOM:
        {
            data->appendChild("path", JsonValue::alloc(Path::getStringFromPath(_path)));
            break;
        }
        case PathType::NONE:
        {
            break;
        }
    }
	return data;
}

bool PathAIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	type = AIType::PATH;
	PathType pathType = getPathTypeFromString(json->getString("pathType"));
    std::vector<Vec2> path = Path::getPathFromString(json->getString("path"));
	PathDirection direction = getDirectionFromString(json->getString("direction"));
	init(pathType, path, direction);
	Data::preload(json);
	return true;
}

bool PathAIData::materialize() {
	return true;
}

std::shared_ptr<ActiveAI> PathAIData::newActiveAI(std::shared_ptr<GameObject> object) {
	return PathAI::alloc(object, _pathType, _path, _direction);
}
