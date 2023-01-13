#ifndef GAME_INTERNAL_H
#define GAME_INTERNAL_H

/**
 * CONSTANTS
*/

#define WORLD_WIDTH  224
#define WORLD_HEIGHT 256

#define RESOURCE_DIR "./res"

#define DEFAULT_VOLUME 50
#define DEFAULT_SCALE 2

#define FONT_PTSIZE 8

#define CHARACTER_SET_MONO " A B C D E F G H  I J K L M N O P Q R S T U V W X"\
	" Y Z 0  1 2 3 4 5 6 7 8 9 ? <  > ="

#define ATLAS_INDEX      0
#define FONT_ATLAS_INDEX 1

/**
 * STATES FUNCTIONS
*/

void gameplay_state_init();
void gameplay_state_destroy();
void gameplay_state_update();
void gameplay_state_render();




#endif // GAME_INTERNAL_H