#include "App.h"
#include <string>

using namespace App;

namespace App
{
  Screen screen;
  int score, hi_score, credits;

  SDL_Event event;
  Uint32 delta;
  SDL_Renderer *renderer;

  SDL_Window *window;
  SDL_Texture *atlas;
}


// functions in header

bool is_key_pressed(int key)
{
  const Uint8 *keys = SDL_GetKeyboardState(nullptr);
  return keys[key];
}

void render_clip(int x, int y, const SDL_Rect &clip)
{
  const SDL_Rect scale = {x, y, clip.w, clip.h};
  SDL_RenderCopy(renderer, atlas, &clip, &scale);
}

void render_text(int x, int y, const char *text)
{
  static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*?-<>=";
  for (char c : std::string(text))
  {
    for (int i = 0; i < chars.length(); i++)
    {
      if (toupper(c) == chars[i])
      {
        // % 6 because it's 6 characters per line in atlas
        const SDL_Rect clip = {8 * (i % 6), 40 + 8 * (i / 6), 8, 8};
        render_clip(x, y, clip);
        break;
      }
    }
    x += 8;
  }
}


// axuliar functions

void init_context()
{
  screen = MENU;
  score = hi_score = credits = 0;

  event = (SDL_Event){0};
  delta = 0;

  window = SDL_CreateWindow(
      "Space Invaders Clone",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      2 * WIDTH,
      2 * HEIGHT,
      0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetScale(renderer, 2, 2); // resolution independent rendering

  atlas = NULL;
}

void destroy_context()
{
  
}

void load_imgs()
{
#define RESOURCES "./res"
  SDL_Surface *tmp_surfc = IMG_Load(RESOURCES "/atlas.png"); // abgr format
  atlas = SDL_CreateTextureFromSurface(renderer, tmp_surfc);
  SDL_FreeSurface(tmp_surfc);

  tmp_surfc = SDL_LoadBMP(RESOURCES "/icon.bmp");
  SDL_SetWindowIcon(window, tmp_surfc);
  SDL_FreeSurface(tmp_surfc);
}

void load_sfxs()
{
}

bool wait_event_timeout(int timeout)
{
  // only positive time makes sense
  if (timeout < 0)
  {
    timeout = 0;
  }

  event.type = 0;
  return SDL_WaitEventTimeout(&event, timeout);
}

// void add_score(int value) {
//   score += value;
//   if (score > high_score) {
//     high_score = score;
//   }
// }

// void add_credit() {
//   credits += credits < 99;
// }

// void remove_credit() {
//   credits -= credits > 0;
// }

int world_width()
{
  int width = 0;
  SDL_GetWindowSize(window, &width, nullptr);
  float scale = 1.0f;
  SDL_RenderGetScale(renderer, &scale, nullptr);
  return scale * width;
}

int world_height()
{
  int height = 0;
  SDL_GetWindowSize(window, nullptr, &height);
  float scale = 1.0f;
  SDL_RenderGetScale(renderer, nullptr, &scale);
  return scale * height;
}


void process_event()
{
  // TODO: implement process_event
}

void update_n_render()
{
  // TODO: implement process_n_render
}

void main_loop()
{
  int64_t b4 = 0, b4_event = 0, timeout = FRAMERATE;
  while (screen != EXIT)
  {
    if (wait_event_timeout(timeout))
    {
      process_event();

      const int64_t event_delta = SDL_GetTicks() - b4_event;
      b4_event += event_delta;
      // wait less next time
      timeout -= event_delta;
    }
    else
    {
      update_n_render();

      delta = SDL_GetTicks() - b4;
      b4 += delta;
      // reset event timing
      b4_event = b4;
      timeout = FRAMERATE;
    }
  }
}

int main(int argc, char **argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  // Mix_Init(MIX_INIT_MP3);

  SDL_ShowCursor(SDL_FALSE); // retro games don't have a cursor showing up

  srand(time(NULL));

  init_context();
  load_imgs();
  main_loop();

  IMG_Quit();
  SDL_Quit();

  return 0;
}