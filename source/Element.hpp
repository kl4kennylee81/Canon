//
//  Element.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/25/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Element_hpp
#define Element_hpp

#include <stdio.h>

enum class Element : int {
    BLUE,
    GOLD
};

enum class ElementDataType : int {
    MATCH,
    OPPOSITE
};

static std::string elementDataTypeToString(ElementDataType elementData){
    if (elementData == ElementDataType::MATCH){
        return "MATCH";
    }
    if (elementData == ElementDataType::OPPOSITE){
        return "OPPOSITE";
    }
    return "MATCH";
}

static ElementDataType stringToElementDataType(std::string element){
    if (element == "MATCH"){
        return ElementDataType::MATCH;
    }
    if (element == "OPPOSITE"){
        return ElementDataType::OPPOSITE;
    }
    return ElementDataType::MATCH;
}

#endif /* Element_hpp */
