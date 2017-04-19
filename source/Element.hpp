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
#include <cugl/cugl.h>

class Element {
public:
    enum class ElementType : int {
        BLUE,
        GOLD
    };
    
    enum class ElementDataType : int {
        MATCH,
        OPPOSITE
    };
    
    static std::string elementDataTypeToString(ElementDataType elementData){
        std::string r = "MATCH";
        if (elementData == ElementDataType::MATCH){
            r = "MATCH";
            if (elementData == ElementDataType::OPPOSITE){
                r = "OPPOSITE";
            }
            return r;
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
        
        static ElementType elementDataTypeToElement(ElementDataType dataType, ElementType currentElement){
            switch (dataType){
                case ElementDataType::MATCH:
                {
                    return currentElement;
                }
                case ElementDataType::OPPOSITE:
                {
                    switch(currentElement){
                        case ElementType::BLUE:
                        {
                            return ElementType::GOLD;
                        }
                        case Element::GOLD:
                        {
                            return ElementType::BLUE;
                        }
                    }
                }
            }
        }
};
    
#endif /* Element_hpp */
