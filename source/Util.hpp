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
#include <algorithm>
#include <typeinfo>


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
    
    static std::string join(const std::vector<std::string>& v,size_t n, const char& c)
    {
        std::stringstream ss;
        
        for(size_t i = 0; i < std::min(n,v.size()); ++i)
        {
            if(i != 0)
            ss << c;
            ss << v[i];
        }
        std::string s = ss.str();
        return s;
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
    
    #pragma mark Coordinate Conversions
    
    static float getGameSceneHeight()
    {
        cugl::Size size = cugl::Application::get()->getDisplaySize();
        return size.height / size.width * GAME_SCENE_WIDTH;
    }
    
    static float getScreenToSceneScale()
    {
        cugl::Size size = cugl::Application::get()->getDisplaySize();
        return GAME_SCENE_WIDTH/size.width;
    }
    
    /** Helper function to calculate the y translate needed to go from scene to world **/
    static float getSceneToWorldTranslateY(){
        float sceneYMax = getGameSceneHeight();
        float scene_yPos = (sceneYMax - (GAME_PHYSICS_HEIGHT * GAME_PHYSICS_SCALE))/2.f;
        return scene_yPos;
    }
    
    /** Physics Conversion **/
    static cugl::Vec2& physicsToSceneCoords(cugl::Vec2 physicsCoords,cugl::Vec2& sceneCoords){
        cugl::Vec2::scale(physicsCoords, GAME_PHYSICS_SCALE, &sceneCoords);
        sceneCoords.y += getSceneToWorldTranslateY();
        return sceneCoords;
    }
    
    static cugl::Vec2& physicsToScreenCoords(cugl::Vec2 physicsCoords, cugl::Vec2& screenCoords){
        physicsToSceneCoords(physicsCoords,screenCoords);
        sceneToScreenCoords(screenCoords,screenCoords);
        return screenCoords;
    }
    
    /** Screen conversions */
    static cugl::Vec2& screenToSceneCoords(cugl::Vec2 screenCoords, cugl::Vec2& sceneCoords){
        cugl::Size size = cugl::Application::get()->getDisplaySize();
        cugl::Vec2 flippedCoords =  cugl::Vec2(screenCoords.x,size.height - screenCoords.y);
        cugl::Vec2::scale(flippedCoords,getScreenToSceneScale(),&sceneCoords);
        return sceneCoords;
    }
    
    static cugl::Vec2& screenToPhysicsCoords(cugl::Vec2 screenCoords, cugl::Vec2& physicsCoords){
        screenToSceneCoords(screenCoords,physicsCoords);
        sceneToPhysicsCoords(physicsCoords, physicsCoords);
        return physicsCoords;
    }
    
    /** Scene conversions */
    static cugl::Vec2& sceneToScreenCoords(cugl::Vec2 sceneCoords, cugl::Vec2& screenCoords){
        cugl::Vec2 flippedCoords = cugl::Vec2(sceneCoords.x,getGameSceneHeight() - screenCoords.y);
        cugl::Vec2::divide(flippedCoords,getScreenToSceneScale(),&screenCoords);
        return screenCoords;
    }
    
    static cugl::Vec2& sceneToPhysicsCoords(cugl::Vec2 sceneCoords, cugl::Vec2& physicsCoords){
        physicsCoords.set(sceneCoords);
        physicsCoords.y -= getSceneToWorldTranslateY();
        cugl::Vec2::divide(physicsCoords,GAME_PHYSICS_SCALE,&physicsCoords);
        return physicsCoords;
    }

};


#endif /* Util_hpp */
