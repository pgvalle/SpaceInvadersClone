#ifndef SCREENS_H
#define SCREENS_H

#include <SDL_events.h>
#include <SDL_types.h>

void process_menu_event(const SDL_Event* event);
void update_menu(Uint32 delta);
void render_menu();

void process_pause_event(const SDL_Event* event);
void update_pause(Uint32 delta);
void render_pause();

void process_play_event(const SDL_Event* event);
void update_play(Uint32 delta);
void render_play();

void process_over_event(const SDL_Event* event);
void update_over(Uint32 delta);
void render_over();


#endif // SCREENS_H