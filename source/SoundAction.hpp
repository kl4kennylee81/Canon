//
//  SoundAction.hpp
//  Canon
//
//  Created by Jonathan Chen on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef SoundAction_hpp
#define SoundAction_hpp

enum class SoundAction : int {
    /** General Sounds **/
    SLOW_MOTION_START,
    SLOW_MOTION_END,
    /** Character Sounds **/
    SPAWNING,
    SPAWN,
    ACTIVE,
    DEATH,
    
    /** Default **/
    DEFAULT
};

#endif /* SoundAction_h */
