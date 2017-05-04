#include <cugl/cugl.h>
#include "GenericAssetManager.hpp"
#include "WaveData.hpp"
#include "LevelData.hpp"
#include "ObjectData.hpp"
#include "ShapeData.hpp"
#include "AnimationData.hpp"
#include "MenuScreenData.hpp"
#include "TemplateWaveEntry.hpp"
#include "SaveGameData.hpp"
#include "AIData.hpp"
#include "ZoneData.hpp"
#include "UIData.hpp"
#include "SoundData.hpp"
using namespace cugl;

#pragma mark -
#pragma mark Constructors
/**
* Initializes a new asset manager with the given number of auxiliary threads.
*
* The asset manager will have a thread pool of the given size, allowing it
* load assets asynchronously.  These threads have no effect on synchronous
* loading and will sleep when no assets are being loaded.  If threads is
* 0, all assets must be loaded synchronously.
*
* This initializer does not attach any loaders.  It simply creates an
* object that is ready to accept loader objects.
*
* @param threads   The number of threads for asynchronous loading
*
* @return true if the asset manager was initialized successfully
*/
bool GenericAssetManager::init(unsigned int threads) {
	_workers = ThreadPool::alloc(threads);
	return true;
}

/**
* Detaches all the attached loaders and deletes all auxiliary threads.
*
* Unlike the destructor, this does not destroy the asset manager.  However,
* you will need to reinitialize the manager (to restart the auxiliary
* threads) and reattach all loaders to use the asset manager again.
*/
void GenericAssetManager::dispose() {
	detachAll();
	_workers = nullptr;
}

#pragma mark -
#pragma mark Internal Asset Loading
/**
* Synchronously reads an asset category from a JSON file
*
* JSON directories provide a robust way for us to load a collection of
* assets. Instead of having to define parameters like asset key, font
* size, or texture wrap in the code, we can specify them in a JSON file.
* This JSON file (called the asset directory) is read by the asset manager,
* and directs the various loaders to load in assets.
*
* Currently JSON loading supports five types of assets, with the following
* names: "textures", "fonts", "music", "soundfx", and "jsons".  See the
* method {@link BaseLoader#read} in each of the individual loaders for a
* description of the suported JSON format. A loader must still be attached
* for the asset manager to read that type of asset.  If the asset directory
* contains an asset for which there is no attached asset manager, those
* specific assets will not be loaded.
*
* @param hash  The hash of the asset type
* @param json  The child of asset directory with these assets
*
* @return true if all assets of this type were successfully loaded.
*/
bool GenericAssetManager::readCategory(size_t hash, const std::shared_ptr<JsonValue>& json) {
	auto it = _handlers.find(hash);
	if (it == _handlers.end()) {
		return false;
	}

	std::shared_ptr<BaseLoader> loader = it->second;
	if (loader == nullptr) {
		CULogError("No loader for hash %zu", hash);
		return false;
	}

	bool success = true;
	for (int ii = 0; ii < json->size(); ii++) {
		std::shared_ptr<JsonValue> child = json->get(ii);
		success = loader->load(child) && success;
	}

	return success;
}

/**
* Asynchronously reads an asset category from a JSON file
*
* JSON directories provide a robust way for us to load a collection of
* assets. Instead of having to define parameters like asset key, font
* size, or texture wrap in the code, we can specify them in a JSON file.
* This JSON file (called the asset directory) is read by the asset manager,
* and directs the various loaders to load in assets.
*
* Currently JSON loading supports five types of assets, with the following
* names: "textures", "fonts", "music", "soundfx", and "jsons".  See the
* method {@link BaseLoader#read} in each of the individual loaders for a
* description of the suported JSON format. A loader must still be attached
* for the asset manager to read that type of asset.  If the asset directory
* contains an asset for which there is no attached asset manager, those
* specific assets will not be loaded.
*
* As an asynchronous read, all asset loading will take place outside of
* the main thread.  However, assets such as fonts and textures will need
* the OpenGL context to complete, so part of their asset loading may take
* place in the main thread via the {@link Application#schedule} interface.
* You may either poll this interface to determine when the assets are
* loaded or use optional callbacks.
*
* The optional callback function will be called each time an individual
* asset loads or fails to load.  However, if the entire category fails
* to load, the callback function will be given the asset category name
* (e.g. "soundfx") as the asset key.
*
* @param hash      The hash of the asset type
* @param json      The child of asset directory with these assets
* @param callback  An optional callback after each asset is loaded
*/
void GenericAssetManager::readCategory(size_t hash, const std::shared_ptr<JsonValue>& json,
	LoaderCallback callback) {
	auto it = _handlers.find(hash);
	std::shared_ptr<BaseLoader> loader = it->second;
	if (loader == nullptr) {
		if (callback) {
			Application::get()->schedule([=] {
				callback(json->key(), false);
				return false;
			});
		}
		return;
	}

	for (int ii = 0; ii < json->size(); ii++) {
		std::shared_ptr<JsonValue> child = json->get(ii);
		loader->loadAsync(child, callback);
	}
}

#pragma mark -
#pragma mark Directory Support
/**
* Synchronously loads all assets in the given directory.
*
* JSON directories provide a robust way for us to load a collection of
* assets. Instead of having to define parameters like asset key, font
* size, or texture wrap in the code, we can specify them in a JSON file.
* This JSON file (called the asset directory) is read by the asset manager,
* and directs the various loaders to load in assets.
*
* Currently JSON loading supports five types of assets, with the following
* names: "textures", "fonts", "music", "soundfx", and "jsons".  See the
* method {@link BaseLoader#read} in each of the individual loaders for a
* description of the suported JSON format. A loader must still be attached
* for the asset manager to read that type of asset.  If the asset directory
* contains an asset for which there is no attached asset manager, those
* specific assets will not be loaded.
*
* This method will try to load as many assets from the directory as it
* can.  If any asset fails to load, it will return false.  However, some
* assets may still be loaded and safe to access.
*
* @param json  The JSON asset directory
*
* @return true if all assets specified in the directory were successfully loaded.
*/
bool GenericAssetManager::loadDirectory(const std::shared_ptr<JsonValue>& json) {
	bool success = true;
	for (int ii = 0; ii < json->size(); ii++) {
		std::shared_ptr<JsonValue> child = json->get(ii);
		if (child->key() == "textures") {
			success = readCategory(typeid(Texture).hash_code(), child) && success;
		}
		else if (child->key() == "soundfx") {
			success = readCategory(typeid(Sound).hash_code(), child) && success;
		}
		else if (child->key() == "music") {
			success = readCategory(typeid(Music).hash_code(), child) && success;
		}
		else if (child->key() == "fonts") {
			success = readCategory(typeid(Font).hash_code(), child) && success;
		}
		else if (child->key() == "jsons") {
			success = readCategory(typeid(JsonValue).hash_code(), child) && success;
		}
		else if (child->key() == "levels") {
			success = readCategory(typeid(LevelData).hash_code(), child) && success;
		}
		else if (child->key() == "waves") {
			success = readCategory(typeid(WaveData).hash_code(), child) && success;
		}
		else if (child->key() == "objects") {
			success = readCategory(typeid(ObjectData).hash_code(), child) && success;
		}
		else if (child->key() == "shapes") {
			success = readCategory(typeid(ShapeData).hash_code(), child) && success;
		}
		else if (child->key() == "animations") {
			success = readCategory(typeid(AnimationData).hash_code(), child) && success;
		}
		else if (child->key() == "MenuScreenData") {
			success = readCategory(typeid(MenuScreenData).hash_code(), child) && success;
		}
		else if (child->key() == "UIData") {
			success = readCategory(typeid(UIData).hash_code(), child) && success;
		}
		else if (child->key() == "SaveGameData") {
			success = readCategory(typeid(SaveGameData).hash_code(), child) && success;
		}
		else if (child->key() == "ai"){
			success = readCategory(typeid(AIData).hash_code(), child) && success;
		}
        else if (child->key() == "zones"){
            success = readCategory(typeid(ZoneData).hash_code(), child) && success;
        }
        else if (child->key() == "sounds"){
            success = readCategory(typeid(SoundData).hash_code(), child) && success;
        }
        else if (child->key() == "templates") {
            success = readCategory(typeid(TemplateWaveEntry).hash_code(), child) && success;
        }
		else {
			CULogError("Unknown asset category '%s'", child->key().c_str());
			success = false;
		}
	}
	return success;
}

/**
* Synchronously loads all assets in the given directory.
*
* JSON directories provide a robust way for us to load a collection of
* assets. Instead of having to define parameters like asset key, font
* size, or texture wrap in the code, we can specify them in a JSON file.
* This JSON file (called the asset directory) is read by the asset manager,
* and directs the various loaders to load in assets.
*
* Currently JSON loading supports five types of assets, with the following
* names: "textures", "fonts", "music", "soundfx", and "jsons".  See the
* method {@link BaseLoader#read} in each of the individual loaders for a
* description of the suported JSON format. A loader must still be attached
* for the asset manager to read that type of asset.  If the asset directory
* contains an asset for which there is no attached asset manager, those
* specific assets will not be loaded.
*
* This method will try to load as many assets from the directory as it
* can.  If any asset fails to load, it will return false.  However, some
* assets may still be loaded and safe to access.
*
* @param directory The path to the JSON asset directory
*
* @return true if all assets specified in the directory were successfully loaded.
*/
bool GenericAssetManager::loadDirectory(const std::string& directory) {
	std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(directory);
	if (reader == nullptr) {
		CULogError("No asset directory located at '%s'", directory.c_str());
		return false;
	}

	std::shared_ptr<JsonValue> json = reader->readJson();
	return loadDirectory(json);
}

/**
* Asynchronously loads all assets in the given directory.
*
* JSON directories provide a robust way for us to load a collection of
* assets. Instead of having to define parameters like asset key, font
* size, or texture wrap in the code, we can specify them in a JSON file.
* This JSON file (called the asset directory) is read by the asset manager,
* and directs the various loaders to load in assets.
*
* Currently JSON loading supports five types of assets, with the following
* names: "textures", "fonts", "music", "soundfx", and "jsons".  See the
* method {@link BaseLoader#read} in each of the individual loaders for a
* description of the suported JSON format. A loader must still be attached
* for the asset manager to read that type of asset.  If the asset directory
* contains an asset for which there is no attached asset manager, those
* specific assets will not be loaded.
*
* As an asynchronous load, all asset loading will take place outside of
* the main thread.  However, assets such as fonts and textures will need
* the OpenGL context to complete, so part of their asset loading may take
* place in the main thread via the {@link Application#schedule} interface.
* You may either poll this interface to determine when the assets are
* loaded or use optional callbacks.
*
* The optional callback function will be called each time an individual
* asset loads or fails to load.  However, if the entire category fails
* to load, the callback function will be given the asset category name
* (e.g. "soundfx") as the asset key.
*
* @param json      The JSON asset directory
* @param callback  An optional callback after each asset is loaded
*/
void GenericAssetManager::loadDirectoryAsync(const std::shared_ptr<JsonValue>& json, LoaderCallback callback) {
	for (int ii = 0; ii < json->size(); ii++) {
		std::shared_ptr<JsonValue> child = json->get(ii);
		if (child->key() == "textures") {
			readCategory(typeid(Texture).hash_code(), child, callback);
		}
		else if (child->key() == "soundfx") {
			readCategory(typeid(Sound).hash_code(), child, callback);
		}
		else if (child->key() == "music") {
			readCategory(typeid(Music).hash_code(), child, callback);
		}
		else if (child->key() == "fonts") {
			readCategory(typeid(Font).hash_code(), child, callback);
		}
		else if (child->key() == "jsons") {
			readCategory(typeid(JsonValue).hash_code(), child, callback);
		}
		else {
			CULogError("Unknown asset category '%s'", child->key().c_str());
		}
	}
}

/**
* Asynchronously loads all assets in the given directory.
*
* JSON directories provide a robust way for us to load a collection of
* assets. Instead of having to define parameters like asset key, font
* size, or texture wrap in the code, we can specify them in a JSON file.
* This JSON file (called the asset directory) is read by the asset manager,
* and directs the various loaders to load in assets.
*
* Currently JSON loading supports five types of assets, with the following
* names: "textures", "fonts", "music", "soundfx", and "jsons".  See the
* method {@link BaseLoader#read} in each of the individual loaders for a
* description of the suported JSON format. A loader must still be attached
* for the asset manager to read that type of asset.  If the asset directory
* contains an asset for which there is no attached asset manager, those
* specific assets will not be loaded.
*
* As an asynchronous load, all asset loading will take place outside of
* the main thread.  However, assets such as fonts and textures will need
* the OpenGL context to complete, so part of their asset loading may take
* place in the main thread via the {@link Application#schedule} interface.
* You may either poll this interface to determine when the assets are
* loaded or use optional callbacks.
*
* The optional callback function will be called each time an individual
* asset loads or fails to load.  However, if the entire category fails
* to load, the callback function will be given the asset category name
* (e.g. "soundfx") as the asset key.
*
* @param directory The path to the JSON asset directory
* @param callback  An optional callback after each asset is loaded
*/
void GenericAssetManager::loadDirectoryAsync(const std::string& directory, LoaderCallback callback) {
	_preload = true;

	std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(directory);
	if (reader == nullptr && callback != nullptr) {
		callback("", false);
		return;
	}

	_workers->addTask([=](void) {
		std::shared_ptr<JsonValue> json = reader->readJson();
		loadDirectoryAsync(json, callback);
		_preload = false;
	});
}


#pragma mark -
#pragma mark Progress Monitoring
/**
* Returns the number of assets currently loaded.
*
* This method is a rough way to determine how many assets have been loaded
* so far. This method counts each asset equally regardless of the memory
* requirements of each asset.
*
* The value returned is the sum of the loadCount for all attached loaders.
*
* @return the number of assets currently loaded.
*/

size_t GenericAssetManager::loadCount() const {
	size_t result = 0;
	for (auto it = _handlers.begin(); it != _handlers.end(); ++it) {
		result += it->second->loadCount();
	}
	return result;

}

/**
* Returns the number of assets waiting to load.
*
* This is a rough way to determine how many assets are still pending.
* An asset is pending if it has been loaded asychronously, and the
* loading process has not yet finished. This method counts each asset
* equally regardless of the memory requirements of each asset.
*
* The value returned is the sum of the waitCount for all attached loaders.
*
* @return the number of assets waiting to load.
*/
size_t GenericAssetManager::waitCount() const {
	size_t result = 0;
	for (auto it = _handlers.begin(); it != _handlers.end(); ++it) {
		result += it->second->waitCount();
	}
	return _preload ? result + 1 : result;
}
