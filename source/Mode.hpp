//
//  Mode.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Mode_hpp
#define Mode_hpp

#include <stdio.h>
#include <cugl/cugl.h>

enum class Mode : int {
    MAIN_MENU,
    GAMEPLAY,
    LOADING,
    LEVEL_EDIT
};

static Mode stringToMode(std::string mode) {
	if (mode == "MAIN_MENU") {
		return Mode::MAIN_MENU;
	}
	if (mode == "GAMEPLAY") {
		return Mode::GAMEPLAY;
	}
	if (mode == "LOADING") {
		return Mode::LOADING;
	}
	if (mode == "LEVEL_EDIT") {
		return Mode::LEVEL_EDIT;
	}
	// default to main menu
	return Mode::MAIN_MENU;
}

static std::string modeToString(Mode mode) {
	switch (mode) {
	case Mode::GAMEPLAY:
		return "GAMEPLAY";
	case Mode::MAIN_MENU:
		return "MAIN_MENU";
	case Mode::LOADING:
		return "LOADING";
	case Mode::LEVEL_EDIT:
		return "LEVEL_EDIT";
	default:
		return "MAIN_MENU";
	}
}

#endif /* Mode_hpp */
