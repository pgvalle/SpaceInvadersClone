#include "SIC.h"

enum State {
  TEXT1,
  TEXT12
};

State state;

const std::string text1 = R"(







            PLAY


       SPACE INVADERS
)";

const std::string text2 = R"(














   *SCORE ADVANCES TABLE*

          =? MYSTERY

          =30 POINTS

          =20 POINTS

          =10 POINTS
)";

int i1, i2, i;

void menu_init() {
  state = TEXT1;
  i1 = 0;
  i2 = 0;
  i = 0;
}

void menu_draw() {
  switch (state) {
    case TEXT1: 
      FC_Draw(sic->font, sic->renderer, 0, 0, text1.substr(0, i1).c_str());
      break;
    case TEXT12: {
      FC_Draw(sic->font, sic->renderer, 0, 0, text1.c_str());
      FC_Draw(sic->font, sic->renderer, 0, 0, text2.substr(0, i2).c_str());
      
      if (i2 < 48)
        break;

      SDL_Rect src = { 0, 0, 16, 8 },
               dst = { 64, 136, 16, 8 };
      SDL_RenderCopy(sic->renderer, sic->atlas, &src, &dst);

      src = { 0, 16, 8, 8 };
      dst = { 68, 152, 8, 8 };
      SDL_RenderCopy(sic->renderer, sic->atlas, &src, &dst);

      src = { 0, 24, 11, 8 };
      dst = { 66, 168, 11, 8 };
      SDL_RenderCopy(sic->renderer, sic->atlas, &src, &dst);

      src = { 0, 32, 12, 8 };
      dst = { 66, 184, 12, 8 };
      SDL_RenderCopy(sic->renderer, sic->atlas, &src, &dst);
      break;
    }
  }
}

int menu_updt() {
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  switch (state) {
    case TEXT1:
      i1 += i;
      if (i1 == text1.length())
        state = TEXT12;
      break;
    case TEXT12:
      i2 += i;
      if (i2 == text2.length())
        return EXIT_HOOK;
      break;
  }

  i = !i;

  if (keys[SDL_SCANCODE_SPACE]) {
    state = TEXT12;
    i1 = text1.length();
    i2 = text2.length();
  }

  if (keys[SDL_SCANCODE_Q])
    return EXIT_HOOK;

  return KEEP_SCREEN;
}
