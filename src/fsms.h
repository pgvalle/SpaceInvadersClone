#ifndef FSMS_H
#define FSMS_H

enum Screens {
  MENU,
  PLAY,
  PAUSE,
  OVER,
  EXIT
};

enum MenuScreenState {
  ANIMATION,
  WAITING,
  BLINK_SHOWING,
  BLINK_HIDING
};

// play screen stuff here //

enum PlayScreenState {
  PLAYING,
  RESTARTING
};

enum PlayerState {
  STARTING,
  ALIVE,
  DYING,
  DEAD
};

enum TouristState {
  VISIBLE,
  HIDDEN,
  DYING, // showing death animation
  DEAD // show its score value
};

enum HordeState {
  STARTING,
  MOVING
};

// remaining screens //

enum PauseScreenState {
  BLINK_SHOWING,
  BLINK_HIDING,
  RESUMING
};

enum OverScreenState {
  WAITING1, // spare time before displaying "you lost"
  ANIMATING, // animate "you lost" text
  WAITING2, // spare time before displaying options
  BLINK_SHOWING,
  BLINK_HIDING
};

#endif // FSMS_H