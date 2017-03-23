//
//  Util.hpp
//  Canon
//
//  Created by Kenneth Lee on 3/23/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Util_hpp
#define Util_hpp

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
};


#endif /* Util_hpp */
