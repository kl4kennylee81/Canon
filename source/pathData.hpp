//
//  pathData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef pathData_hpp
#define pathData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"

class PathData : public Data {
public:
	std::vector<cugl::Vec2> path;
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();

	bool init() {
		return true;
	}

	static std::shared_ptr<PathData> alloc() {
		std::shared_ptr<PathData> result = std::make_shared<PathData>();
		return (result->init() ? result : nullptr);
	}
};


#endif /* pathData_hpp */
