#ifndef UI_RENDER_H
#define UI_RENDER_H

#include <stdbool.h>
#include <stdint.h>

// x and y are in-game coordinates
// character set: ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>*?
// All other characters are interpreted as spaces.
// So, for example: "i$love$u" -> "i love u"
void RenderText(int x, int y, const char* text, bool red);
// x and y are in-game coordinates.
void RenderUnsignedInt(int x, int y, uint64_t value, bool red);


#endif // UI_RENDER_H
