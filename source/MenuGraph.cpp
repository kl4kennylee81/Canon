//
//  MenuGraph.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuGraph.hpp"

using namespace cugl;

MenuGraph::MenuGraph(){
    _currentMode = Mode::LOADING;
}

void MenuGraph::setMode(Mode mode){
    _currentMode = mode;
}

Mode MenuGraph::getMode(){
    return _currentMode;
}
