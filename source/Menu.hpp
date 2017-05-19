//
//  Menu.hpp
//  Canon
//
//  Created by Kenneth Lee on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "UIComponent.hpp"
#include "ActiveAnimation.hpp"
#include <map>

class Menu {
private:
	std::string _mKey;
    std::shared_ptr<cugl::Node> _menu;
	std::shared_ptr<ActiveAnimation> _menuBackground;
    std::vector<std::shared_ptr<UIComponent>> _uiElements;
    std::map<std::string,std::string> _fontMap;
    
    std::shared_ptr<cugl::PolygonNode> _storyScreen;
    std::shared_ptr<cugl::PolygonNode> _storyScreen2;
public:
    
    Menu():
    _menu(nullptr),
    _menuBackground(nullptr),
    _mKey("")
    {}
    
    ~Menu(){ dispose(); }
    
    void dispose(){
        if (_menu != nullptr){
            _menu->removeFromParent();
        }
        
        if (_menuBackground != nullptr){
            _menuBackground->dispose();
        }
        _menu = nullptr;
        _menuBackground = nullptr;
        _mKey = "";
    }
    
	std::string getMenuKey();
	std::shared_ptr<ActiveAnimation> getBackground();

	void setBackground(std::shared_ptr<ActiveAnimation> node);
    
    void setFontMap(std::map<std::string,std::string> fontMap){
        _fontMap = fontMap;
    }
    
    std::map<std::string,std::string> getFontMap(){
        return _fontMap;
    }

    void addUIElement(std::shared_ptr<UIComponent> element);
    
    std::vector<std::shared_ptr<UIComponent>>& getUIElements();
    
    void attachToScene(std::shared_ptr<cugl::Node> parent);
    
    void detachFromScene();
    
    virtual bool init(std::string mKey);
    
    void populate(std::shared_ptr<GenericAssetManager> assets, std::vector<std::string> uiDataKeys,std::string bgKey,std::map<std::string,std::string> fontMap);
    
    static std::shared_ptr<Menu> alloc(std::string mKey){
        std::shared_ptr<Menu> result = std::make_shared<Menu>();
        return (result->init(mKey) ? result : nullptr);
    }
    
    void updateAnimation();

};

#endif /* Menu_hpp */
