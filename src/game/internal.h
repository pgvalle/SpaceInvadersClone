#ifndef GAME_INTERNAL_H
#define GAME_INTERNAL_H

#include <SDL.h>

#define RESOURCE_DIR "./res"

#define FONT_PTSIZE 8

#define CHARACTER_SET_MONO " A B C D E F G H  I J K L M N O P Q R S T U V W X"\
	" Y Z 0  1 2 3 4 5 6 7 8 9 ? <  > ="
#define CHARACTER_SET "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?<>="

extern SDL_Texture* atlas, * font_atlas;


#endif // GAME_INTERNAL_H
