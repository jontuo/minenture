
#ifndef __SETTING_HPP_INCLUDED__
#define __SETTING_HPP_INCLUDED__

#include <cstdint>
#include <string>




/*
    TODO: Separate computer and world settings.
    TODO: Separate default and user settings.
*/



// Window
const std::string WINDOW_TITLE = "Minenture";
const bool WINDOW_VERTICAL_SYNCHRONIZATION = true; // If 'false', WINDOW_FRAMERATE_LIMIT will be used instead.
const unsigned int WINDOW_FRAMERATE_LIMIT = 60; // WINDOW_VERTICAL_SYNCHRONIZATION has to be 'false' to activate this.
// WINDOW_MODE
// WINDOW_SIZE? fullscreen parameter

// Context
// TODO: Change "CONTEXT to "OPENGL"?
const signed int CONTEXT_DEPTH_BIT = 0;
const signed int CONTEXT_STENCIL_BIT = 0;
const signed int CONTEXT_ANTIALIASING = 0;
const signed int CONTEXT_MAJOR_VERSION = 3;
const signed int CONTEXT_MINOR_VERSION = 3;
const float OPENGL_CLEAR_COLOR_RED = 0.5f;
const float OPENGL_CLEAR_COLOR_GREEN = 0.2f;
const float OPENGL_CLEAR_COLOR_BLUE = 0.2f;
const float OPENGL_CLEAR_COLOR_ALPHA = 1.0f;

// Space
const unsigned int SPACE_ITERATION = 3;
const double SPACE_DAMPING = 1.0f;
const double SPACE_IDLE_SPEED_THRESHOLD = 0.1;
const double SPACE_SLEEP_TIME_THRESHOLD = 0.1;
const double SPACE_COLLISION_BIAS = 0.2;
const double SPACE_GRAVITY = 9.8;
const double SPACE_COLLISION_SLOP = 0.1;
/*
    Decides for how many steps (or is it iterations?) the collision impulses/solutions should be retained for.
    Makes stacked objects more stable and less mushy with less CPU power.
    Prevents jittering contacts from getting worse. Spiral out of control?
    If its stored over iterations instead of steps, then iteration count affects this a lot.
    A fixed timestep is crucial for this to work properly.

    Minenture default: ?
    Chipmunk default: 3
*/
const unsigned int SPACE_COLLISION_PERSISTENCE = 3;
const bool SPACE_SPATIAL_HASH_ENABLED = false;
const double SPACE_SPATIAL_HASH_DIMENSION = 24.0;
const unsigned int SPACE_SPATIAL_HASH_COUNT = 1000;





const std::uint64_t TARGET_UPDATE_TIME = 33333; // Common values: 16666, 33333, 66666



const unsigned int HISTORY_WIDTH = 100;









/*
    Default settings
*/





/*
1/15    66666
1/30    33333
1/60    16666
1/120   8333
*/
//const std::uint64_t TARGET_UPDATE_TIME = 33333;
const double SPACE_TIME_STEP = 1.0 / 30.0;







#endif
