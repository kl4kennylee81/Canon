//
//  ZoneLoader.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/20/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AILoader_hpp
#define AILoader_hpp

#include <cugl/cugl.h>
#include <cugl/assets/CULoader.h>
#include <cugl/assets/CUAsset.h>
#include "AIData.hpp"
#include "StaticAIData.hpp"
#include "PathAIData.hpp"
#include "HomingAIData.hpp"
#include "CompositeAIData.hpp"

class AILoader : public cugl::GenericLoader<AIData> {
public:

	AILoader() : GenericLoader<AIData>() {}

	static std::shared_ptr<AILoader> alloc() {
		std::shared_ptr<AILoader> result = std::make_shared<AILoader>();
		return (result->init() ? result : nullptr);
	}

	virtual bool read(const std::shared_ptr<cugl::JsonValue>& json,
		cugl::LoaderCallback callback, bool async) override {
		std::string key = json->key();
		if (_assets.find(key) != _assets.end() || _queue.find(key) != _queue.end()) {
			return false;
		}

		_queue.emplace(key);

		if (json->getString("type") == "STATIC") {
			return readStaticAI(json, callback, async, key);
		}
		if (json->getString("type") == "HOMING") {
			return readHomingAI(json, callback, async, key);
		}
		if (json->getString("type") == "PATH") {
			return readPathAI(json, callback, async, key);
		}
		if (json->getString("type") == "COMPOSITE") {
			return readCompositeAI(json, callback, async, key);
		}
		return false;
	}

	bool readStaticAI(const std::shared_ptr<cugl::JsonValue>& json,
		cugl::LoaderCallback callback, bool async, std::string key) {
		bool success = false;

		if (_loader == nullptr || !async) {
			std::shared_ptr<StaticAIData> asset = std::make_shared<StaticAIData>();
			if (asset->preload(json)) {
				success = materialize(key, asset, callback);
			}
		}
		else {
			_loader->addTask([=](void) {
				std::shared_ptr<StaticAIData> asset = std::make_shared<StaticAIData>();
				if (!asset->preload(json)) {
					asset = nullptr;
				}
				cugl::Application::get()->schedule([=](void) {
					this->materialize(key, asset, callback);
					return false;
				});
			});
		}

		return success;
	}

	bool readHomingAI(const std::shared_ptr<cugl::JsonValue>& json,
		cugl::LoaderCallback callback, bool async, std::string key) {
		bool success = false;

		if (_loader == nullptr || !async) {
			std::shared_ptr<HomingAIData> asset = std::make_shared<HomingAIData>();
			if (asset->preload(json)) {
				success = materialize(key, asset, callback);
			}
		}
		else {
			_loader->addTask([=](void) {
				std::shared_ptr<HomingAIData> asset = std::make_shared<HomingAIData>();
				if (!asset->preload(json)) {
					asset = nullptr;
				}
				cugl::Application::get()->schedule([=](void) {
					this->materialize(key, asset, callback);
					return false;
				});
			});
		}

		return success;
	}

	bool readPathAI(const std::shared_ptr<cugl::JsonValue>& json,
		cugl::LoaderCallback callback, bool async, std::string key) {
		bool success = false;

		if (_loader == nullptr || !async) {
			std::shared_ptr<PathAIData> asset = std::make_shared<PathAIData>();
			if (asset->preload(json)) {
				success = materialize(key, asset, callback);
			}
		}
		else {
			_loader->addTask([=](void) {
				std::shared_ptr<PathAIData> asset = std::make_shared<PathAIData>();
				if (!asset->preload(json)) {
					asset = nullptr;
				}
				cugl::Application::get()->schedule([=](void) {
					this->materialize(key, asset, callback);
					return false;
				});
			});
		}

		return success;
	}

	bool readCompositeAI(const std::shared_ptr<cugl::JsonValue>& json,
		cugl::LoaderCallback callback, bool async, std::string key) {
		bool success = false;

		if (_loader == nullptr || !async) {
			std::shared_ptr<CompositeAIData> asset = std::make_shared<CompositeAIData>();
			if (asset->preload(json)) {
				success = materialize(key, asset, callback);
			}
		}
		else {
			_loader->addTask([=](void) {
				std::shared_ptr<CompositeAIData> asset = std::make_shared<CompositeAIData>();
				if (!asset->preload(json)) {
					asset = nullptr;
				}
				cugl::Application::get()->schedule([=](void) {
					this->materialize(key, asset, callback);
					return false;
				});
			});
		}

		return success;
	}

};

#endif /* AILoader_hpp */
