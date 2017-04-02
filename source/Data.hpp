//
//  Data.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Data_hpp
#define Data_hpp

#include <stdio.h>
#include <cugl/cugl.h>

class Data : cugl::Asset {
public:
    Data():Asset(){};
    
    bool init(){
        return true;
    };
    
	virtual std::shared_ptr<cugl::JsonValue> toJsonValue()
	{
		std::cout << "using default definition of toJsonValue in Data.hpp" << std::endl;
		return cugl::JsonValue::allocNull();
	}

    // setting equal to 0 is a pure virtual function equivalent to an abstract function
	virtual std::string serialize()
	{
		return toJsonValue()->toString();
	}
        
	virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* Data_hpp */
