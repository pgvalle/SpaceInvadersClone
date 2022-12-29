#ifndef RENDERER_H
#define RENDERER_H

#include <SDL_types.h>
#include <stdbool.h>

// x and y are in-game coordinates
// character set: ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>*?
// All other characters are interpreted as spaces.
// So, for example: "i$love$u" -> "i love u"
void RenderText(int x, int y, const char* text, bool red);
// x and y are in-game coordinates.
void RenderUnsignedInt(int x, int y, Uint64 value, bool red);


#endif // RENDERER_H
