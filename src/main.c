#include <SDL.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <time.h>

// HANDY DEFINES HERE //

#define FRAMERATE (1000 / 60)

#define TILE 8
// These are the dimensions of the canvas. NOT of the window.
#define WIDTH  (28 * TILE) // 240
#define HEIGHT (32 * TILE) // 224

// GAME CONTEXT HERE //

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* atlas;

SDL_Event event;
Uint32 delta;

enum {
  SCREEN_MENU = 0,
  SCREEN_PLAY,
  SCREEN_PAUSE,
  SCREEN_OVER,
  SCREEN_EXIT
} screen;
int score, high_score;
int credits;

// UTILITIES //

void increase_score(int value) {
  score += value;
  if (score > high_score) {
    high_score = score;
  }
}

void add_credit() { credits += credits < 99; }
void remove_credit() { credits -= credits > 0; }

void render_clip(const SDL_Rect* clip, int x, int y);
void render_text(const char* text, int len, int x, int y);

// MENU //
struct {
  enum {
    MENU_ANIMATION = 0,
    MENU_WAITING,
    MENU_BLINK_HIDING,
    MENU_BLINK_SHOWING
  } state;
  int i;
  int timer;
} menu;

void reset_menu_state() {
  menu.state = 0;
  menu.i = 0;
  menu.timer = 0;
}

void menu_process_event();
void menu_update();
void menu_render();

// PLAY //

struct {
  enum {
    PLAY_PLAYING,
    PLAY_RESTARTING
  } state;

  Uint32 timer;
} play;

void play_process_event();
void play_update();
void play_render();

SDL_Point useless_bar[WIDTH];

// PLAY.EXPLOSIONS //

struct explosion_t {
  int x, y;
  SDL_Rect clip;
  int32_t lifetime;
}* explosions;

void update_explosions();
void render_explosions();

// PLAY.PLAYER //

struct {
  enum {
    PLAYER_STARTING,
    PLAYER_ALIVE,
    PLAYER_DYING,
    PLAYER_DEAD
  } state;
  int x;
  int lives;
  int dying_clip_swaps;
  SDL_Point* shots;
  Uint32 timer;
} player;

void update_player_shots();
void render_player_shots();
void update_player();
void render_player();

// PLAY.HORDE //

struct {
  enum {
    HORDE_STARTING,
    HORDE_MOVING,
    HORDE_WAITING
  } state;
  int xrel, yrel;

  struct invader_t {
    int x, y;
    SDL_Rect clip;
  }* invaders;

  struct horde_shot_t {
    int x, y;
    SDL_Rect clip;
    Uint32 timer;
  }* shots;

  Uint32 timer, shot_timer, shot_timeout;
} horde;

void update_horde_shots();
void render_horde_shots();
void update_horde();
void render_horde();

// PLAY (AGAIN) //

void reset_play_state() {
  score = 0;

  play.timer = 0;
  play.state = PLAY_PLAYING;

  explosions = NULL;

  player.state = PLAYER_STARTING;
  player.lives = 3;
  player.dying_clip_swaps = 0;
  player.shots = NULL;
  player.timer = 0;

  for (int i = 0; i < WIDTH; i++) {
    useless_bar[i] = (SDL_Point){ i, 239 };
  }
}

// MAIN PROGRAM //

void load_resources();
void free_resources();
void loop();


int main(int argc, char** args)
{
  // INITIALIZATION //

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_ShowCursor(SDL_FALSE); // retro games don't have a cursor showing up
  // Mix_Init(MIX_INIT_MP3);
  srand(time(NULL));

  reset_menu_state();
  
  // resolution independent rendering
  window = SDL_CreateWindow("Space Invaders Clone",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2*WIDTH, 2*HEIGHT, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetScale(renderer, 2, 2);

  screen = 0;
  score = high_score = 0;
  credits = 1;

  load_resources();
  
  loop(); // EXECUTION //

  // TERMINATION //

  free_resources();
  SDL_Quit();

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
// UTILITIES //
///////////////////////////////////////////////////////////////////////////////

void render_clip(const SDL_Rect* clip, int x, int y) {
  const SDL_Rect scale = { x, y, clip->w, clip->h };
  SDL_RenderCopy(renderer, atlas, clip, &scale);
}

void render_text(const char* text, int len, int x, int y) {
  static const char CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*?-<>=";
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < sizeof(CHARS) - 1; j++) {
      if (text[i] == CHARS[j]) {
        // % 6 because it's 6 characters per line in atlas
        const SDL_Rect clip = { TILE * (j % 6),
          40 + TILE * (j / 6), TILE, TILE };

        render_clip(&clip, x, y);
        break;
      }
    }

    x += TILE;
  }
}

void load_resources() {
  #define RESOURCES "./res"

  // ATLAS //

  int width, height;
  void* pixels = stbi_load(RESOURCES "/atlas.png", &width, &height, NULL, 4);
  SDL_assert(pixels);

  SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32,
    4 * width, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000); // abgr format
  stbi_image_free(pixels);

  atlas = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  // ICON //

  SDL_Surface* icon = SDL_LoadBMP(RESOURCES "/icon.bmp");
  SDL_SetWindowIcon(window, icon);
  SDL_FreeSurface(icon);

  #undef RESOURCES
}

void free_resources() {
  SDL_DestroyTexture(atlas);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void render_score_advances_table() {
  render_text("*SCORE ADVANCES TABLE*", 22, 24, 128);

  static const SDL_Rect tour = { 0,  0, 24,  8 };
  render_clip(&tour, 60, 152);
  render_text("=? MYSTERY", 10, 80, 152);

  static const SDL_Rect inv1 = { 0, 16, 12,  8 };
  render_clip(&inv1, 66, 168);
  render_text("=30 POINTS", 10, 80, 168);

  static const SDL_Rect inv2 = { 0, 24, 12,  8 };
  render_clip(&inv2, 66, 184);
  render_text("=20 POINTS", 10, 80, 184);

  static const SDL_Rect inv3 = { 0, 32, 12,  8 };
  render_clip(&inv3, 66, 200);
  render_text("=10 POINTS", 10, 80, 200);
}


///////////////////////////////////////////////////////////////////////////////
// MENU //
///////////////////////////////////////////////////////////////////////////////

void menu_process_event() {
  if (event.type != SDL_KEYDOWN || event.key.repeat) {
    return;
  }
    
  const SDL_Keycode key = event.key.keysym.sym;
  switch (menu.state) {
  case MENU_ANIMATION:
  case MENU_WAITING:
    if (key == SDLK_RETURN) {
      menu.state = MENU_BLINK_SHOWING;
      menu.timer = 0;
    }
    break;
  case MENU_BLINK_HIDING:
  case MENU_BLINK_SHOWING:
    if (key == SDLK_p) {
      screen = SCREEN_PLAY;
      remove_credit();
      reset_play_state();
    } else if (key == SDLK_q) {
      screen = SCREEN_EXIT;
    }
    break;
  }
}

void menu_update() {
  switch (menu.state) {
  case MENU_ANIMATION: // start animation
    menu.timer += delta;
    if (menu.timer >= 160) {
      if (++menu.i == 14)
        menu.state = MENU_WAITING;
      menu.timer = 0;
    }
    break;
  case MENU_WAITING: // on wait
    menu.timer += delta;
    if (menu.timer >= 1008) {
      menu.state = MENU_BLINK_SHOWING;
      menu.timer = 0;
    }
    break;
  case MENU_BLINK_HIDING:
  case MENU_BLINK_SHOWING:
    menu.timer += delta;
    if (menu.timer >= 498) {
      menu.state = (menu.state == MENU_BLINK_HIDING ?
        MENU_BLINK_SHOWING : MENU_BLINK_HIDING);
      menu.timer = 0;
    }
    break;
  }
}

void menu_render() {
  switch (menu.state) {
  case MENU_BLINK_SHOWING:
    render_text("<P> PLAY", 8, 80, 80);
    render_text("<Q> QUIT", 8, 80, 96);
  case MENU_BLINK_HIDING:
    render_text("SPACE INVADERS", 14, 56, 56);
    render_score_advances_table();
    break;
  case MENU_ANIMATION:
  case MENU_WAITING:
    render_text("SPACE INVADERS", menu.i, 56, 56);
    break;
  }
}


///////////////////////////////////////////////////////////////////////////////
// PLAY.EXPLOSIONS //
///////////////////////////////////////////////////////////////////////////////

void update_explosions() {
  for (int i = 0; i < arrlen(explosions); i++) {
    explosions[i].lifetime -= delta;
    if (explosions[i].lifetime <= 0) {
      arrdel(explosions, i);
      i--;
    }
  }
}

void render_explosions() {
  for (int i = 0; i < arrlen(explosions); i++) {
    render_clip(&explosions[i].clip, explosions[i].x, explosions[i].y);
  }
}


///////////////////////////////////////////////////////////////////////////////
// PLAY.PLAYER //
///////////////////////////////////////////////////////////////////////////////

void update_player_shots() {
  for (int i = 0; i < arrlen(player.shots); i++) {
    // reached top of screen
    player.shots[i].y -= 4;
    if (player.shots[i].y <= 32) {
      // add explosion
      const struct explosion_t explosion = {
        .x = player.shots[i].x - 3,
        .y = 34,
        .clip = { 36, 24,  8,  8 },
        .lifetime = 256
      };
      arrput(explosions, explosion);
      arrdel(player.shots, i);
      i--;
    }
	}
}

void render_player_shots() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (int i = 0; i < arrlen(player.shots); i++) {
    const SDL_Rect rect = {player.shots[i].x, player.shots[i].y, 1, 4 };
    SDL_RenderFillRect(renderer, &rect);
  }
}

void update_player() {
	switch (player.state) {
  case PLAYER_STARTING:
    player.timer += delta;
    if (player.timer >= 2000) {
      player.state = PLAYER_ALIVE;
      player.x = 14;
      player.timer = 0;
    }
    break;
	case PLAYER_ALIVE: {
    const uint8_t* keys = SDL_GetKeyboardState(NULL);
    // moving player left only if position is > 14
    if (keys[SDL_SCANCODE_LEFT]) {
      player.x -= (player.x > 14 ? 1 : 0);
    }
    // move player right only if positions is < WIDTH - 31
    if (keys[SDL_SCANCODE_RIGHT]) {
      player.x += (player.x < WIDTH - 31 ? 1 : 0);
    }
    // shooting mechanic
    if (player.timer < 768) {
      player.timer += delta;
    } else if (keys[SDL_SCANCODE_SPACE]) {
      const SDL_Point shot = { player.x + 8, 216 };
      arrput(player.shots, shot);
      player.timer = 0;
    }
    // check if horde has reached player
    if (arrlen(horde.invaders) > 0 && horde.invaders[0].y == 216) {
      player.state = PLAYER_DYING;
      player.timer = 0;
    }
    break; }
  case PLAYER_DYING:
    player.timer += delta;
    if (player.timer >= 112) {
      if (++player.dying_clip_swaps == 9) {
        player.state = PLAYER_DEAD;
        player.lives--;
        player.dying_clip_swaps = 0;
      }
      player.timer = 0;
    }
  break;
	case PLAYER_DEAD:
		player.timer += delta;
		if (player.timer >= 2000) {
      // horde reached player. game over
      if (horde.invaders[0].y == 216) {
        screen = SCREEN_OVER;
        //reset_over();
      } else {
        player.state = PLAYER_ALIVE;
        player.x = 14;
        player.timer = 0;
      }
		}
		break;
	}
}

void render_player() {
  static SDL_Rect clip = { 0, 8, 16, 8 };
  switch (player.state) {
  case PLAYER_DYING:
    clip.x = (player.dying_clip_swaps % 2 + 1) * 16; // death animation update
    render_clip(&clip, player.x, 216);
    break;
  case PLAYER_ALIVE:
    render_clip(&clip, player.x, 216);
    break;
  }
}


///////////////////////////////////////////////////////////////////////////////
// PLAY.HORDE //
///////////////////////////////////////////////////////////////////////////////

void update_horde_shots() {
  for (int i = 0; i < arrlen(horde.shots); i++) {
    // update shot animation
    horde.shots[i].timer += delta;
    if (horde.shots[i].timer >= 96) {
      horde.shots[i].clip.x = 24 + (horde.shots[i].clip.x - 21) % 12;
      horde.shots[i].timer = 0;
    }

    // reached bottom of screen
    horde.shots[i].y += 2;
    if (horde.shots[i].y >= 232) {
      // take away some bar pixels
      useless_bar[horde.shots[i].x] = (SDL_Point){ -1, -1 };
      useless_bar[horde.shots[i].x + 2] = (SDL_Point){ -1, -1 };
      useless_bar[horde.shots[i].x + 4] = (SDL_Point){ -1, -1 };

      // add explosion
      const struct explosion_t explosion = {
        .x = horde.shots[i].x - 1,
        .y = 232,
        .clip = { 37, 32,  6,  8 },
        .lifetime = 128
      };
      arrput(explosions, explosion);
      arrdel(horde.shots, i);
      i--;
    }
	}
}

void render_horde_shots() {
  for (int i = 0; i < arrlen(horde.shots); i++) {
    const struct horde_shot_t shot = horde.shots[i];
    render_clip(&shot.clip, shot.x, shot.y);
  }
}

void update_horde_start_anim() {
  const size_t len_horde = arrlen(horde.invaders);
  if (len_horde < 55) {
    const int i = (4 - len_horde / 11);
    struct invader_t invader = {
      .clip = { 12, 16, 12,  8 },
      .x = 26 + 16 * (len_horde % 11), // row 4, 3, 2, 1, 0
      .y = 64 + 16 * i      // col 0, 1, 2, ..., 10
    };
    // correct clip in atlas
    if (i > 2) invader.clip.y = 32; // in 4th or 5th row
    else if (i > 0) invader.clip.y = 24; // in 2nd or 3rd row

    arrput(horde.invaders, invader);
  } else {
    // done. Now move right
    horde.state = HORDE_MOVING;
    horde.xrel = 2;
    horde.yrel = 0;
    horde.timer = 0;
    horde.shot_timeout = 500;
  }
}

void update_horde() {
  switch (horde.state) {
  case HORDE_STARTING:
      update_horde_start_anim();
      break;
  case HORDE_MOVING: {
    horde.timer += delta;
    // moving pace accelerates depending on horde size
    const size_t len_horde = arrlen(horde.invaders);
    if (horde.timer >= FRAMERATE * len_horde) {
      for (int i = 0; i < len_horde; i++) {
        horde.invaders[i].x += horde.xrel;
        horde.invaders[i].y += horde.yrel;
        horde.invaders[i].clip.x = horde.invaders[i].clip.x == 0 ? 12 : 0; // animate
      }

      horde.timer = 0;
    }
    break; }
  case HORDE_WAITING:
    horde.timer += delta;
    if (horde.timer >= 256) {
      horde.state = HORDE_MOVING;
    }
    break;
  }
}

void render_horde() {
  for (int i = 0; i < arrlen(horde.invaders); i++) {
    const struct invader_t invader = horde.invaders[i];
    render_clip(&invader.clip, invader.x, invader.y);
  }
}


///////////////////////////////////////////////////////////////////////////////
// PLAY //
///////////////////////////////////////////////////////////////////////////////

void play_process_event() {
  if (event.type == SDL_KEYDOWN && !event.key.repeat &&
    event.key.keysym.sym == SDLK_ESCAPE)
  {
    screen = SCREEN_PAUSE;
    //reset_pause();
  }
}

void play_update() {
  switch (play.state) {
  case PLAY_PLAYING:
    update_explosions();
    update_player();
    if (player.state != PLAYER_DYING && player.state != PLAYER_DEAD) {
      //update_tourist();
      update_horde();
    }

    update_player_shots();
    update_horde_shots();

    // process_shot_collision_with_player();
    // process_shot_collision_with_horde();
    // process_shot_collision_with_tourist();
    // process_collision_between_shots();
    // for (int b = 0; b < 4; b++) {
    //   process_player_shot_collision_with_bunker(b);
    //   process_horde_shot_collision_with_bunker(b);
    // }
    // process_horde_collision_with_bunkers();

    // all invaders killed
    if (player.state == PLAYER_ALIVE && arrlen(horde.invaders) == 0) {
      play.state = PLAY_RESTARTING;
      play.timer = 0;
      arrfree(horde.shots);
      arrfree(player.shots);
    }

    if (player.lives == 0 && player.state == PLAYER_DEAD) {
      screen = SCREEN_OVER;
      //reset_over();
    }
    break;
  case PLAY_RESTARTING:
    update_explosions();
    
    play.timer += delta;
    if (play.timer >= 1504) {
      // keep score and life counter
      const int score_backup = score;
      const int lives_backup = player.lives;
      reset_play_state();
      score = score_backup;
      player.lives = lives_backup;
    }
    break;
  }
}

void render_play() {
  // Just to resemble the original game
  SDL_SetRenderDrawColor(renderer, 32, 255, 32, 255);
  SDL_RenderDrawPoints(renderer, useless_bar, WIDTH);

  // all gameplay elements
  render_player_shots();
  render_horde_shots();
  //render_tourist();
  render_horde();
  render_player();
  //render_bunkers();
  render_explosions();

  // live counter (number)
  char player_lives_text[3];
  sprintf(player_lives_text, "%d", player.lives);
  render_text(player_lives_text, 2, 8, HEIGHT - 16);
  // live counter (cannons)
  const SDL_Rect cannon_clip = { 0, 8, 16, 8 };
  for (int i = 0; i < player.lives - 1; i++) {
    render_clip(&cannon_clip, 24 + i * 16, HEIGHT - 16);
  }
}


///////////////////////////////////////////////////////////////////////////////
// MAIN PROGRAM //
///////////////////////////////////////////////////////////////////////////////

void process_event() {
  switch (event.type) {
  case SDL_QUIT:
    screen = SCREEN_EXIT;
    return;
  case SDL_KEYDOWN: {
    const SDL_Keycode code = event.key.keysym.sym;
    if (code == SDLK_PLUS) {
      add_credit();
      return;
    } else if (code == SDLK_MINUS) {
      remove_credit();
      return;
    }
    break; }
  }

  switch (screen) {
  case SCREEN_MENU:
    menu_process_event();
    break;
  // case SCREEN_PAUSE:
  //   process_pause_event(event);
  //   break;
  case SCREEN_PLAY:
    play_process_event();
    break;
  // case SCREEN_OVER:
  //   process_over_event(event);
  //   break;
  }
}

void update() {
  switch (screen) {
  // case SCREEN_PAUSE:
  //   update_pause(delta);
  //   break;
  case SCREEN_MENU:
    menu_update();
    break;
  case SCREEN_PLAY:
    play_update(delta);
    break;
  // case SCREEN_OVER:
  //   update_over(delta);
  //   break;
  }
}

void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
          
  switch (screen) {
  case SCREEN_MENU:
    menu_render();
    break;
  // case SCREEN_PAUSE:
  //   render_play();
  //   render_pause();
  //   break;
  case SCREEN_PLAY:
    render_play();
    break;
  // case SCREEN_OVER:
  //   render_play();
  //   render_over();
  //   break;
  }

  // SCORES //

  static char scores_str[7];
  // player score
  render_text("YOUR SCORE", 10, 8, 8);
  sprintf(scores_str, "%06d", score);
  render_text(scores_str, 6, 24, 24);
  // high-score
  render_text("HIGH-SCORE", 10, WIDTH - 88, 8);
  sprintf(scores_str, "%06d", high_score);
  render_text(scores_str, 6, WIDTH - 72, 24);

  // COIN COUNTER //

  static char credits_str[10];
  sprintf(credits_str, "CREDIT %02d", credits);
  render_text(credits_str, 9, WIDTH - 80, HEIGHT - 16);

  SDL_RenderPresent(renderer);
}

void loop() {
  Uint32 start = 0, event_start = 0, event_wait = FRAMERATE;

  while (screen != SCREEN_EXIT) {
    event.type = 0; // reset event
    if (SDL_WaitEventTimeout(&event, event_wait)) {
      process_event();

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
      event_wait = FRAMERATE;
    }
  }
}
