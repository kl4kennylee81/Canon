//
//  CUSoundLoader.cpp
//  Cornell University Game Library (CUGL)
//
//  This module provides a specific implementation of the Loader class to load
//  sound assets (e.g. in-memory audio files). A sound asset is identified by
//  both its source file and its volume.
//
//  As with all of our loaders, this loader is designed to be attached to an
//  asset manager.  In addition, this class uses our standard shared-pointer
//  architecture.
//
//  1. The constructor does not perform any initialization; it just sets all
//     attributes to their defaults.
//
//  2. All initialization takes place via init methods, which can fail if an
//     object is initialized more than once.
//
//  3. All allocation takes place via static constructors which return a shared
//     pointer.
//
//
//  CUGL zlib License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 1/7/16
//
#include <cugl/assets/CUSoundLoader.h>
#include <cugl/audio/CUAudioEngine.h>
#include <cugl/base/CUApplication.h>

using namespace cugl;

/** What the source name is if we do not know it */
#define UNKNOWN_SOURCE  "<unknown>"
/** The default volume (MAX) */
#define UNKNOWN_VOLUME  1.0f

#pragma mark -
#pragma mark Constructor

/**
 * Creates a new, uninitialized sound loader
 *
 * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate a loader on
 * the heap, use one of the static constructors instead.
 */
SoundLoader::SoundLoader() : Loader<Sound>(),
_volume(UNKNOWN_VOLUME) {
}


#pragma mark -
#pragma mark Asset Loading
/**
 * Finishes loading the sound file, setting its default volume.
 *
 * Allocating a sound asset can be done safely in a separate thread.
 * However, setting the default volume requires the audio engine, and so
 * this step is not safe to be done in a separate thread.  Instead, it
 * takes place in the main CUGL thread via {@link Application#schedule}.
 *
 * This method supports an optional callback function which reports whether
 * the asset was successfully materialized.
 *
 * @param key       The key to access the asset after loading
 * @param sound     The sound asset partially loaded
 * @param volume    The volume to set for the sound asset
 * @param callback  An optional callback for asynchronous loading
 */
void SoundLoader::materialize(const std::string& key, const std::shared_ptr<Sound>& sound,
                              float volume, LoaderCallback callback) {
    bool success = false;
    if (sound != nullptr) {
        _assets[key] = sound;
        sound->setVolume(volume);
        success = true;
    }
    
    if (callback != nullptr) {
        callback(key,success);
    }
    _queue.erase(key);
}

/**
 * Internal method to support asset loading.
 *
 * This method supports either synchronous or asynchronous loading, as
 * specified by the given parameter.  If the loading is asynchronous,
 * the user may specify an optional callback function.
 *
 * This method will split the loading across the {@link Sound#alloc} and
 * the internal {@link materialize} method.  This ensures that asynchronous
 * loading is safe.
 *
 * @param key       The key to access the asset after loading
 * @param source    The pathname to the asset
 * @param callback  An optional callback for asynchronous loading
 * @param async     Whether the asset was loaded asynchronously
 *
 * @return true if the asset was successfully loaded
 */
bool SoundLoader::read(const std::string& key, const std::string& source, LoaderCallback callback, bool async) {
    if (_assets.find(key) != _assets.end() || _queue.find(key) != _queue.end()) {
        return false;
    }
    _queue.emplace(key);
    
    bool success = false;
    if (_loader == nullptr || !async) {
        std::shared_ptr<Sound> sound = Sound::alloc(source);
        success = (sound != nullptr);
        materialize(key,sound,UNKNOWN_VOLUME,callback);
    } else {
        _loader->addTask([=](void) {
            std::shared_ptr<Sound> sound = Sound::alloc(source);
            Application::get()->schedule([=](void){
                this->materialize(key,sound,this->_volume,callback);
                return false;
            });
        });
    }
    
    return success;
}

/**
 * Internal method to support asset loading.
 *
 * This method supports either synchronous or asynchronous loading, as
 * specified by the given parameter.  If the loading is asynchronous,
 * the user may specify an optional callback function.
 *
 * This method will split the loading across the {@link Sound#alloc} and
 * the internal {@link materialize} method.  This ensures that asynchronous
 * loading is safe.
 *
 * This version of read provides support for JSON directories. A soundfx
 * directory entry has the following values
 *
 *      "file":         The path to the asset
 *      "volume":       This default sound volume (float)
 *
 * @param json      The directory entry for the asset
 * @param callback  An optional callback for asynchronous loading
 * @param async     Whether the asset was loaded asynchronously
 *
 * @return true if the asset was successfully loaded
 */
bool SoundLoader::read(const std::shared_ptr<JsonValue>& json, LoaderCallback callback, bool async) {
    std::string key = json->key();
    if (_assets.find(key) != _assets.end() || _queue.find(key) != _queue.end()) {
        return false;
    }
    _queue.emplace(key);
    
    std::string source = json->getString("file",UNKNOWN_SOURCE);
    float volume = json->getFloat("volume",UNKNOWN_VOLUME);
    
    bool success = false;
    if (_loader == nullptr || !async) {
        std::shared_ptr<Sound> sound = Sound::alloc(source);
        success = (sound != nullptr);
        materialize(key,sound,volume,callback);
    } else {
        _loader->addTask([=](void) {
            std::shared_ptr<Sound> sound = Sound::alloc(source);
			Application::get()->schedule([=](void) {
                this->materialize(key,sound,volume,callback);
                return false;
            });
        });
    }
    
    return success;
}
