#include "defs.h"
#include "app.h"
#include "screens/screens.h"
#include "stb_image.h"
#include <time.h>

void reset_menu();

void load_resources();
void free_resources();
void loop();

int main(int argc, char** args)
{
  const int INITIAL_SCALE = 2;

  // INITIALIZATION //

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_ShowCursor(SDL_FALSE); // retro games don't have a cursor showing up
  // Mix_Init(MIX_INIT_MP3);
  srand(time(NULL));

  ctx.screen = SCREEN_MENU;
  reset_menu();
  ctx.win = SDL_CreateWindow("Space Invaders Clone", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, INITIAL_SCALE * WIDTH, INITIAL_SCALE * HEIGHT, 0);
  ctx.ren = SDL_CreateRenderer(ctx.win, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetScale(ctx.ren, INITIAL_SCALE, INITIAL_SCALE);
  ctx.score = ctx.high_score = 0;
  ctx.credits = 1;

  load_resources();
  
  loop(); // EXECUTION //

  // TERMINATION //

  free_resources();

  SDL_DestroyRenderer(ctx.ren);
  SDL_DestroyWindow(ctx.win);

  SDL_Quit();

  return 0;
}

void load_resources()
{
  #define RESOURCES "./res"

  // ATLAS //

  int width, height;
  void* pixels = stbi_load(RESOURCES "/atlas.png", &width, &height, NULL, 4);
  if (pixels == NULL) {
    const SDL_Rect rect = { 0, 0, width, height };
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 24, 0xff0000,
      0x00ff00, 0x0000ff, 0);
    
    SDL_FillRect(surface, &rect, 0xffffff);
    ctx.atlas = SDL_CreateTextureFromSurface(ctx.ren, surface);
    SDL_FreeSurface(surface);
  } else {
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32,
      4*width, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    ctx.atlas = SDL_CreateTextureFromSurface(ctx.ren, surface);
    stbi_image_free(pixels);
    SDL_FreeSurface(surface);
  }

  // ICON //

  SDL_Surface* icon = SDL_LoadBMP(RESOURCES "/icon.bmp");
  SDL_SetWindowIcon(ctx.win, icon);
  SDL_FreeSurface(icon);

  #undef RESOURCES
}

void free_resources()
{
  SDL_DestroyTexture(ctx.atlas);
}

void process_event(const SDL_Event* event)
{
  if (event->type == SDL_QUIT) {
    ctx.screen = SCREEN_EXIT;
  } else {
    if (event->type == SDL_KEYDOWN) {
      if (event->key.keysym.sym == SDLK_PLUS) {
        add_credit();
      } else if (event->key.keysym.sym == SDLK_MINUS) {
        remove_credit();
      }
    }
    switch (ctx.screen) {
    case SCREEN_MENU:
      process_menu_event(event);
      break;
    case SCREEN_PAUSE:
      process_pause_event(event);
      break;
    case SCREEN_PLAY:
      process_play_event(event);
      break;
    case SCREEN_OVER:
      process_over_event(event);
      break;
    }
  }
}

void update(Uint32 delta)
{
  switch (ctx.screen) {
  case SCREEN_PAUSE:
    update_pause(delta);
    break;
  case SCREEN_MENU:
    update_menu(delta);
    break;
  case SCREEN_PLAY:
    update_play(delta);
    break;
  case SCREEN_OVER:
    update_over(delta);
    break;
  }
}

void render()
{
  SDL_SetRenderDrawColor(ctx.ren, 0, 0, 0, 255);
  SDL_RenderClear(ctx.ren);
          
  switch (ctx.screen) {
  case SCREEN_MENU:
    render_menu();
    break;
  case SCREEN_PAUSE:
    render_play();
    render_pause();
    break;
  case SCREEN_PLAY:
    render_play();
    break;
  case SCREEN_OVER:
    render_play();
    render_over();
    break;
  }

  // SCORES //

  static char scores_fmt[7];
  // score
  render_text("YOUR SCORE", 10, 8, 8);
  sprintf(scores_fmt, "%06d", ctx.score);
  render_text(scores_fmt, 6, 24, 24);
  // high-score
  render_text("HIGH-SCORE", 10, WIDTH - 88, 8);
  sprintf(scores_fmt, "%06d", ctx.high_score);
  render_text(scores_fmt, 6, WIDTH - 72, 24);

  // COIN COUNTER //

  static char credits_fmt[10];
  sprintf(credits_fmt, "CREDIT %02d", ctx.credits);
  render_text(credits_fmt, 9, WIDTH - 80, HEIGHT - 16);

  SDL_RenderPresent(ctx.ren);
}

void loop()
{
  const Uint32 FPS = 60;
  Uint32 start = 0, delta = 0, event_start = 0, event_wait = 1000 / FPS;

  while (ctx.screen != SCREEN_EXIT) {
    SDL_Event event;
    if (SDL_WaitEventTimeout(&event, event_wait)) {
      process_event(&event);

      const Uint32 event_delta = SDL_GetTicks() - event_start;
      event_start += event_delta;
      // wait less next time
      event_wait -= event_delta < event_wait ? event_delta : event_wait;
    } else {
      update(delta);
      render();
  
      delta = SDL_GetTicks() - start;
      start += delta;
      // reset event timing
      event_start = start;
      event_wait = 1000 / FPS;
    }
  }
}
