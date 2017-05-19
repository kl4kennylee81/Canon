// In World Coordinates
// Modify in world coordinates
#define MIN_DISTANCE_WORLD 20.f
#define AI_SPEED_WORLD 1.f
#define VELOCITY_WORLD 16.5f
#define RADIUS_WORLD 9.f
#define TOUCH_RADIUS_WORLD 100.f

//All in Physics Coordinates
// Used in the game logic
#define MIN_DISTANCE (MIN_DISTANCE_WORLD/GAME_PHYSICS_SCALE) //per frame, must be greater than velocity ?
#define AI_SPEED (AI_SPEED_WORLD/GAME_PHYSICS_SCALE) //per frame
#define VELOCITY (VELOCITY_WORLD/GAME_PHYSICS_SCALE) //per frame
#define RADIUS (RADIUS_WORLD/GAME_PHYSICS_SCALE) //(radius * 2) > velocity + 3 ? Basically it needs to be slightly more than half the velocity
#define TOUCH_RADIUS (TOUCH_RADIUS_WORLD/GAME_PHYSICS_SCALE)
