//
//  CUTextureLoader.cpp
//  Cornell University Game Library (CUGL)
//
//  This module provides a specific implementation of the Loader class to load
//  textures. A texture asset is identified by both its source file and its
//  texture parameters.  Hence you may wish to load a texture asset multiple
//  times, though this is potentially wasteful regarding memory.
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
#include <cugl/assets/CUTextureLoader.h>
#include <cugl/base/CUApplication.h>
#include <SDL/SDL_image.h>

using namespace cugl;

#pragma mark Support Functions
/** What the source name is if we do not know it */
#define UNKNOWN_SOURCE  "<unknown>"
/** The default min filter */
#define UNKNOWN_MINFLT  "nearest"
/** The default mage filter */
#define UNKNOWN_MAGFLT  "linear"
/** The default wrap rule */
#define UNKNOWN_WRAP    "clamp"

/**
 * Returns the OpenGL enum for the given min filter name
 *
 * This function converts JSON directory entries into OpenGL values. If the
 * name is invalid, it returns GL_NEAREST.
 *
 * @param name  The JSON name for the min filter
 *
 * @return the OpenGL enum for the given min filter name
 */
GLuint decodeMinFilter(const std::string& name) {
    if (name == "nearest") {
        return GL_NEAREST;
    } else if (name == "linear") {
        return GL_LINEAR;
    } else if (name == "nearest-nearest") {
        return GL_NEAREST_MIPMAP_NEAREST;
    } else if (name == "linear-nearest") {
        return GL_LINEAR_MIPMAP_NEAREST;
    } else if (name == "nearest-linear") {
        return GL_NEAREST_MIPMAP_LINEAR;
    } else if (name == "linear-linear") {
        return GL_LINEAR_MIPMAP_LINEAR;
    }
    return GL_NEAREST;
}

/**
 * Returns the OpenGL enum for the given mag filter name
 *
 * This function converts JSON directory entries into OpenGL values. If the
 * name is invalid, it returns GL_LINEAR.
 *
 * @param name  The JSON name for the mag filter
 *
 * @return the OpenGL enum for the given mag filter name
 */
GLuint decodeMagFilter(const std::string& name) {
    if (name == "nearest") {
        return GL_NEAREST;
    }
    return GL_LINEAR;
}

/**
 * Returns the OpenGL enum for the given texture wrap name
 *
 * This function converts JSON directory entries into OpenGL values. If the
 * name is invalid, it returns GL_CLAMP_TO_EDGE.
 *
 * @param name  The JSON name for the texture wrap
 *
 * @return the OpenGL enum for the given texture wrap name
 */
GLuint decodeWrap(const std::string& name) {
    if (name == "clamp") {
        return GL_CLAMP_TO_EDGE;
    } else if (name == "repeat") {
        return GL_REPEAT;
    } else if (name == "mirrored") {
        return GL_MIRRORED_REPEAT;
    }
    return GL_CLAMP_TO_EDGE;
}

#pragma mark -
#pragma mark Constructor

/**
 * Creates a new, uninitialized texture loader
 *
 * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate a loader on
 * the heap, use one of the static constructors instead.
 */
TextureLoader::TextureLoader() : Loader<Texture>(),
_minfilter(GL_LINEAR),
_magfilter(GL_LINEAR),
_wraps(GL_CLAMP_TO_EDGE),
_wrapt(GL_CLAMP_TO_EDGE),
_mipmaps(false) {
}


#pragma mark -
#pragma mark Asset Loading
/**
 * Loads the portion of this asset that is safe to load outside the main thread.
 *
 * It is not safe to create an OpenGL texture in a separate thread.  However,
 * it is safe to create an SDL_surface, which contains all of the data that
 * we need to create an OpenGL texture.  Hence this method does the maximum
 * amount of work that can be done in asynchronous texture loading.
 *
 * @param source    The pathname to the asset
 *
 * @return the SDL_Surface with the texture information
 */
SDL_Surface* TextureLoader::preload(const std::string& source) {
    SDL_Surface* surface = IMG_Load(source.c_str());
    if (surface == nullptr) {
        return nullptr;
    }
    
    SDL_Surface* normal;
#if CU_MEMORY_ORDER == CU_ORDER_REVERSED
    normal = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_ABGR8888,0);
#else
    normal = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGBA8888,0);
#endif
    SDL_FreeSurface(surface);
    return normal;
}

/**
 * Creates an OpenGL texture from the SDL_Surface, and assigns it the given key.
 *
 * This method finishes the asset loading started in {@link preload}.  This
 * step is not safe to be done in a separate thread.  Instead, it takes
 * place in the main CUGL thread via {@link Application#schedule}.
 *
 * The loaded texture will have default parameters for scaling and wrap.
 * It will not have any mipmaps.
 *
 * This method supports an optional callback function which reports whether
 * the asset was successfully materialized.
 *
 * @param key       The key to access the asset after loading
 * @param surface   The SDL_Surface to convert
 * @param callback  An optional callback for asynchronous loading
 */
void TextureLoader::materialize(const std::string& key, SDL_Surface* surface, LoaderCallback callback) {
    std::shared_ptr<Texture> texture = Texture::allocWithData(surface->pixels, surface->w, surface->h);
    
    bool success = false;
    if (texture != nullptr) {
        _assets[key] = texture;
        texture->bind();
        if (_mipmaps) { texture->buildMipMaps(); }
        texture->setMinFilter(_minfilter);
        texture->setMagFilter(_magfilter);
        texture->setWrapS(_wraps);
        texture->setWrapT(_wrapt);
        texture->unbind();
        success = true;
    }
    
    if (callback != nullptr) {
        callback(key,success);
    }
    _queue.erase(key);
}

/**
 * Creates an OpenGL texture from the SDL_Surface accoring to the directory entry.
 *
 * This method finishes the asset loading started in {@link preload}.  This
 * step is not safe to be done in a separate thread.  Instead, it takes
 * place in the main CUGL thread via {@link Application#schedule}.
 *
 * This version of read provides support for JSON directories. A texture
 * directory entry has the following values
 *
 *      "file":         The path to the asset
 *      "mipmaps":      Whether to generate mipmaps (bool)
 *      "minfilter":    The name of the min filter ("nearest", "linear";
 *                      with mipmaps, "nearest-nearest", "linear-nearest",
 *                      "nearest-linear", or "linear-linear")
 *      "magfilter":    The name of the min filter ("nearest" or "linear")
 *      "wrapS":        The s-coord wrap rule ("clamp", "repeat", or "mirrored")
 *      "wrapT":        The t-coord wrap rule ("clamp", "repeat", or "mirrored")
 *
 * The asset key is the key for the JSON directory entry
 *
 * This method supports an optional callback function which reports whether
 * the asset was successfully materialized.
 *
 * @param json      The asset directory entry
 * @param surface   The SDL_Surface to convert
 * @param callback  An optional callback for asynchronous loading
 */
void TextureLoader::materialize(const std::shared_ptr<JsonValue>& json, SDL_Surface* surface, LoaderCallback callback) {
    std::shared_ptr<Texture> texture = Texture::allocWithData(surface->pixels, surface->w, surface->h);
    std::string key = json->key();

    bool success = false;
    if (texture != nullptr) {
        GLuint minflt = decodeMinFilter(json->getString("minfilter",UNKNOWN_MINFLT));
        GLuint magflt = decodeMinFilter(json->getString("magfilter",UNKNOWN_MAGFLT));
        GLuint wrapS = decodeWrap(json->getString("wrapS",UNKNOWN_WRAP));
        GLuint wrapT = decodeWrap(json->getString("wrapT",UNKNOWN_WRAP));
        bool mipmaps = json->getBool("mipmaps",false);

        _assets[key] = texture;
        texture->bind();
        if (mipmaps) { texture->buildMipMaps(); }
        texture->setMinFilter(minflt);
        texture->setMagFilter(magflt);
        texture->setWrapS(wrapS);
        texture->setWrapT(wrapT);
        texture->unbind();

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
 * This method will split the loading across the {@link preload} and
 * {@link materialize} methods.  This ensures that asynchronous loading
 * is safe.
 *
 * @param key       The key to access the asset after loading
 * @param source    The pathname to the asset
 * @param callback  An optional callback for asynchronous loading
 * @param async     Whether the asset was loaded asynchronously
 *
 * @return true if the asset was successfully loaded
 */
bool TextureLoader::read(const std::string& key, const std::string& source, LoaderCallback callback, bool async) {
    if (_assets.find(key) != _assets.end() || _queue.find(key) != _queue.end()) {
        return false;
    }
    _queue.emplace(key);
    
    bool success = false;
    if (_loader == nullptr || !async) {
        std::shared_ptr<Texture> texture = Texture::allocWithFile(source);
        success = (texture != nullptr);
        if (success) { 
			_assets[key] = texture;
		}
        _queue.erase(key);
    } else {
        _loader->addTask([=](void) {
            SDL_Surface* surface = this->preload(source);
            Application::get()->schedule([=](void){
                this->materialize(key,surface,callback);
                return false;
            });
        });
    }

	if (success) {
		std::shared_ptr<Texture> texture = get(key);
		texture->bind();
		if (_mipmaps) { texture->buildMipMaps(); }
		texture->setMinFilter(_minfilter);
		texture->setMagFilter(_magfilter);
		texture->setWrapS(_wraps);
		texture->setWrapT(_wrapt);
		texture->unbind();
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
 * This method will split the loading across the {@link preload} and
 * {@link materialize} methods.  This ensures that asynchronous loading
 * is safe.
 *
 * This version of read provides support for JSON directories. A texture
 * directory entry has the following values
 *
 *      "file":         The path to the asset
 *      "mipmaps":      Whether to generate mipmaps (bool)
 *      "minfilter":    The name of the min filter ("nearest", "linear";
 *                      with mipmaps, "nearest-nearest", "linear-nearest",
 *                      "nearest-linear", or "linear-linear")
 *      "magfilter":    The name of the min filter ("nearest" or "linear")
 *      "wrapS":        The s-coord wrap rule ("clamp", "repeat", or "mirrored")
 *      "wrapT":        The t-coord wrap rule ("clamp", "repeat", or "mirrored")
 *
 * @param json      The directory entry for the asset
 * @param callback  An optional callback for asynchronous loading
 * @param async     Whether the asset was loaded asynchronously
 *
 * @return true if the asset was successfully loaded
 */
bool TextureLoader::read(const std::shared_ptr<JsonValue>& json, LoaderCallback callback, bool async) {
    std::string key = json->key();
    if (_assets.find(key) != _assets.end() || _queue.find(key) != _queue.end()) {
        return false;
    }
    _queue.emplace(key);
    
    std::string source = json->getString("file",UNKNOWN_SOURCE);
    bool success = false;
    if (_loader == nullptr || !async) {
        std::shared_ptr<Texture> texture = Texture::allocWithFile(source);
        success = (texture != nullptr);
        if (success) { 
			_assets[key] = texture;
		}
        _queue.erase(key);
    } else {
        _loader->addTask([=](void) {
            SDL_Surface* surface = this->preload(source);
            Application::get()->schedule([=](void){
                this->materialize(json,surface,callback);
                return false;
            });
        });
    }
    
    if (success) {
        // Get the settings if they exist
        GLuint minflt = decodeMinFilter(json->getString("minfilter",UNKNOWN_MINFLT));
        GLuint magflt = decodeMinFilter(json->getString("magfilter",UNKNOWN_MAGFLT));
        GLuint wrapS = decodeWrap(json->getString("wrapS",UNKNOWN_WRAP));
        GLuint wrapT = decodeWrap(json->getString("wrapT",UNKNOWN_WRAP));
        bool mipmaps = json->getBool("mipmaps",false);
        
        std::shared_ptr<Texture> texture = get(key);
        texture->bind();
        if (mipmaps) { texture->buildMipMaps(); }
        texture->setMinFilter(minflt);
        texture->setMagFilter(magflt);
        texture->setWrapS(wrapS);
        texture->setWrapT(wrapT);
        texture->unbind();
    }
    
    return success;
}
