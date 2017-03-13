//
//  Path.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>
#include <cugl/cugl.h>

class Path {
public:
	std::shared_ptr<std::vector<cugl::Vec2>> _coordinates;
	
	void add(cugl::Vec2 vec) { _coordinates->push_back(vec); }

	void add(float x, float y) {
		cugl::Vec2 vec = cugl::Vec2::Vec2(x, y);
		add(vec);
	}

	cugl::Poly2 getPoly() {
		cugl::Poly2 pathPoly = cugl::Poly2::Poly2(*_coordinates);
		pathPoly.setType(cugl::Poly2::Type::PATH);
		return pathPoly;
	}

	std::shared_ptr<Path> clone() {
		std::shared_ptr<Path> newPath = Path::alloc();
		for (auto it = _coordinates->begin(); it != _coordinates->end(); it++) {
			newPath->add(*it);
		}
		return newPath;
	}

	cugl::Vec2 get(int i) { return _coordinates->at(i); }

	cugl::Vec2 getLast() { return _coordinates->back(); }
    
    /* 
     * Returns a clone of the current path object but in physics coordinates instead of the world.
     */
    std::shared_ptr<Path> convertToPhysicsCoords(float physicsScale) {
        std::shared_ptr<Path> newPath = Path::alloc();
        for (auto it = _coordinates->begin(); it != _coordinates->end(); it++) {
            newPath->add(*it / physicsScale);
        }
        return newPath;
    }

	int size() { return _coordinates->size(); }

	void clear() { _coordinates->clear(); }

	bool init() {
		_coordinates = std::make_shared<std::vector<cugl::Vec2>>(std::vector<cugl::Vec2>());
		return true;
	}

	static std::shared_ptr<Path> alloc() {
		std::shared_ptr<Path> result = std::make_shared<Path>();
		return (result->init() ? result : nullptr);
	}
};

#endif /* Path_hpp */
