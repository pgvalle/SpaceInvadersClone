#include "SIC.h"
#include "UI.h"

///////////////////////////////////////////////////////////
/// VARIABLES
///////////////////////////////////////////////////////////

const std::string text = R"(
         PLAY


    SPACE INVADERS

 

*SCORE ADVANCES TABLE*

       =? MYSTERY

       =30 POINTS

       =20 POINTS

       =10 POINTS      
)";

int state, ticks, i;

///////////////////////////////////////////////////////////

void splash_init() {
  state = 0;
  ticks = 0;
  i = 0;
}

void splash_draw() {
  ui_draw();

  sic.render_text(24, 56, text.substr(0, i).c_str());
  
  if (i <= text.find('*'))
    return;

  sic.render_clip({ 0, 0, 16, 8 }, { 64, 136, 16, 8 });
  sic.render_clip({ 0, 16, 8, 8 }, { 68, 152, 8, 8 });
  sic.render_clip({ 0, 24, 11, 8 }, { 66, 168, 11, 8 });
  sic.render_clip({ 0, 32, 12, 8 }, { 66, 184, 12, 8 });
}

///////////////////////////////////////////////////////////
/// UPDATE
///////////////////////////////////////////////////////////

void update_text_display() {
  // draw one more character each 4 ticks
  if (ticks++ % 4)
    return;

  // skip whitespaces and new lines
  do i++;
  while (i < text.length() && (text[i] == ' ' || text[i] == '\n'));
}

int splash_update(const SDL_Event &event) {
  ui_update(event);

  bool pressed_no_repeat = (event.type == SDL_KEYDOWN && event.key.repeat == 0);
  SDL_Keycode key = event.key.keysym.sym;

  // pressing space skips text typewriting
  if (pressed_no_repeat && key == SDLK_SPACE) {
    state = (state < 4 ? 4 : 5);
    ticks = 0;
    i = text.length();
  }

  if (pressed_no_repeat && key == SDLK_q)
    return SCREEN_EXIT_HOOK;

  switch (state) {
    case 0: // wait 3 seconds before start displaying text
      if (ticks++ == 90) // 3 seconds
        state = 1;
      break;
    case 1: // typewrite but only a part of it (until "SPACE INVADERS")
      update_text_display();
      if (i == text.find('*')) {
        state = 2;
        ticks = 0;
        i--;
      }
      break;
    case 2: // wait 2 seconds
      if (ticks++ == 60) {
        state = 3;
        ticks = 0;
        i += 23;
      }
      break;
    case 3: // typewrite remaining text
      update_text_display();
      if (i == text.length()) {
        state = 4;
        ticks = 0;
      }
      break;
    case 4: // wait 2 seconds again, then done
      if (ticks++ == 60)
        state = 5;
      break;
  }

  if (sic.credits)
    return 1;
  
  // for now keep same screen (0), later add a demo screen
  return (state == 5 ? 0 : SCREEN_UNCHANGED);
}
