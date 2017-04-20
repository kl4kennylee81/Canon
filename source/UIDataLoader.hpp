#ifndef UIElementLoader_hpp
#define UIElementLoader_hpp

#include <cugl/cugl.h>
#include <cugl/assets/CULoader.h>
#include <cugl/assets/CUAsset.h>
#include "UIData.hpp"

class UIDataLoader : public cugl::GenericLoader<UIData> {
public:

	UIDataLoader() : GenericLoader<UIData>() {}

	static std::shared_ptr<UIDataLoader> alloc() {
		std::shared_ptr<UIDataLoader> result = std::make_shared<UIDataLoader>();
		return (result->init() ? result : nullptr);
	}

	virtual bool read(const std::shared_ptr<cugl::JsonValue>& json,
		cugl::LoaderCallback callback, bool async) override {
		std::string key = json->key();
		if (_assets.find(key) != _assets.end() || _queue.find(key) != _queue.end()) {
			return false;
		}

		_queue.emplace(key);

		if (json->getString("type") == "BUTTON") {
			return readButtonEntry(json, callback, async, key);
		}
		if (json->getString("type") == "TEXT") {
			return readTextEntry(json, callback, async, key);
		}
		if (json->getString("type") == "IMAGE") {
			return readImageEntry(json, callback, async, key);
		}

		return false;
	}

	bool readButtonEntry(const std::shared_ptr<cugl::JsonValue>& json,
		cugl::LoaderCallback callback, bool async, std::string key) {
		bool success = false;

		if (_loader == nullptr || !async) {
			std::shared_ptr<ButtonUIData> asset = std::make_shared<ButtonUIData>();
			if (asset->preload(json)) {
				success = materialize(key, asset, callback);
			}
		}
		else {
			_loader->addTask([=](void) {
				std::shared_ptr<ButtonUIData> asset = std::make_shared<ButtonUIData>();
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

	bool readTextEntry(const std::shared_ptr<cugl::JsonValue>& json,
		cugl::LoaderCallback callback, bool async, std::string key) {
		bool success = false;

		if (_loader == nullptr || !async) {
			std::shared_ptr<TextUIData> asset = std::make_shared<TextUIData>();
			if (asset->preload(json)) {
				success = materialize(key, asset, callback);
			}
		}
		else {
			_loader->addTask([=](void) {
				std::shared_ptr<TextUIData> asset = std::make_shared<TextUIData>();
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

	bool readImageEntry(const std::shared_ptr<cugl::JsonValue>& json,
		cugl::LoaderCallback callback, bool async, std::string key) {
		bool success = false;

		if (_loader == nullptr || !async) {
			std::shared_ptr<ImageUIData> asset = std::make_shared<ImageUIData>();
			if (asset->preload(json)) {
				success = materialize(key, asset, callback);
			}
		}
		else {
			_loader->addTask([=](void) {
				std::shared_ptr<ImageUIData> asset = std::make_shared<ImageUIData>();
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

#endif /* ZoneLoader_hpp */
