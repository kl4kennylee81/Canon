//
//  ZoneLoader.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/20/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ZoneLoader_hpp
#define ZoneLoader_hpp

#include <cugl/cugl.h>
#include <cugl/assets/CULoader.h>
#include <cugl/assets/CUAsset.h>
#include "ZoneData.hpp"
#include "StaticZoneData.hpp"
#include "RotateZoneData.hpp"
#include "PulseZoneData.hpp"

class ZoneLoader : public cugl::GenericLoader<ZoneData> {
public:
    
    ZoneLoader() : GenericLoader<ZoneData>() {}
    
    static std::shared_ptr<ZoneLoader> alloc() {
        std::shared_ptr<ZoneLoader> result = std::make_shared<ZoneLoader>();
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
            return readStaticZone(json, callback, async, key);
        }
        if (json->getString("type") == "ROTATE") {
            return readRotateZone(json, callback, async, key);
        }
        if (json->getString("type") == "PULSE") {
            return readPulseZone(json, callback, async, key);
        }
        
        return false;
    }
    
    bool readStaticZone(const std::shared_ptr<cugl::JsonValue>& json,
                        cugl::LoaderCallback callback, bool async, std::string key) {
        bool success = false;
        
        if (_loader == nullptr || !async) {
            std::shared_ptr<StaticZoneData> asset = std::make_shared<StaticZoneData>();
            if (asset->preload(json)) {
                success = materialize(key,asset,callback);
            }
        } else {
            _loader->addTask([=](void) {
                std::shared_ptr<StaticZoneData> asset = std::make_shared<StaticZoneData>();
                if (!asset->preload(json)) {
                    asset = nullptr;
                }
                cugl::Application::get()->schedule([=](void){
                    this->materialize(key,asset,callback);
                    return false;
                });
            });
        }
        
        return success;
    }
    
    bool readRotateZone(const std::shared_ptr<cugl::JsonValue>& json,
                        cugl::LoaderCallback callback, bool async, std::string key) {
        bool success = false;
        
        if (_loader == nullptr || !async) {
            std::shared_ptr<RotateZoneData> asset = std::make_shared<RotateZoneData>();
            if (asset->preload(json)) {
                success = materialize(key,asset,callback);
            }
        } else {
            _loader->addTask([=](void) {
                std::shared_ptr<RotateZoneData> asset = std::make_shared<RotateZoneData>();
                if (!asset->preload(json)) {
                    asset = nullptr;
                }
                cugl::Application::get()->schedule([=](void){
                    this->materialize(key,asset,callback);
                    return false;
                });
            });
        }
        
        return success;
    }
    
    bool readPulseZone(const std::shared_ptr<cugl::JsonValue>& json,
                        cugl::LoaderCallback callback, bool async, std::string key) {
        bool success = false;
        
        if (_loader == nullptr || !async) {
            std::shared_ptr<PulseZoneData> asset = std::make_shared<PulseZoneData>();
            if (asset->preload(json)) {
                success = materialize(key,asset,callback);
            }
        } else {
            _loader->addTask([=](void) {
                std::shared_ptr<PulseZoneData> asset = std::make_shared<PulseZoneData>();
                if (!asset->preload(json)) {
                    asset = nullptr;
                }
                cugl::Application::get()->schedule([=](void){
                    this->materialize(key,asset,callback);
                    return false;
                });
            });
        }
        
        return success;
    }
    
};

#endif /* ZoneLoader_hpp */
