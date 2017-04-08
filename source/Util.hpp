//
//  Util.hpp
//  Canon
//
//  Created by Kenneth Lee on 3/23/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Util_hpp
#define Util_hpp

#include <cugl/cugl.h>

class Util {
public:

    static std::vector<std::string> split(const std::string& s, const char& c)
    {
        std::string buff{ "" };
        std::vector<std::string> v;
        
        for (auto n : s)
        {
            if (n != c) {
                buff += n;
            }
            else if (n == c && buff != "") {
                v.push_back(buff); buff = "";
            }
        }
        if (buff != "") v.push_back(buff);
        return v;
    }

	static std::shared_ptr<cugl::Button> makeBoxButton(int x, int y, int width, int height, cugl::Color4 upColor, cugl::Color4 downColor) {
		auto up = cugl::PolygonNode::alloc(cugl::Rect::Rect(0, 0, width, height));
		up->setColor(upColor);
		auto down = cugl::PolygonNode::alloc(cugl::Rect::Rect(0, 0, width, height));
		down->setColor(downColor);
		auto button = cugl::Button::alloc(up, down);
		button->setPosition(cugl::Vec2::Vec2(x, y));
		return button;
	}

};


#endif /* Util_hpp */
