// In World Coordinates
#define MIN_DISTANCE_WORLD 20
#define AI_SPEED_WORLD 1
#define VELOCITY_WORLD 14
#define RADIUS_WORLD 9
#define TOUCH_RADIUS_WORLD 100

//All in Physics Coordinates
#define MIN_DISTANCE 0.625 //per frame, must be greater than velocity ?
#define AI_SPEED 0.03125 //per frame
#define VELOCITY 0.4375 //per frame
#define RADIUS 0.28125 //(radius * 2) > velocity + 3 ? Basically it needs to be slightly more than half the velocity
#define TOUCH_RADIUS 3.125
