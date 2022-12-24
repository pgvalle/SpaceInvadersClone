# SpaceInvaders Clone

This game, as the name suggests, is yet another 1978 Arcade
[SpaceInvaders](https://en.wikipedia.org/wiki/Space_Invaders)' clone.
Obviously It's not 100% the same as the original one. But I guarantee it's good
enough to be called Space Invaders Clone.

# Build instructions

## Linux
**NOTE**: These instructions are meant for real beginner poeple.
First open up a terminal window and try out each of these commands (without the $)
to see which one gives you something other than "command not found" or so:

1. `$ sudo apt install make gcc git libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev`
2. `$ sudo pacman -S make gcc git sdl2 sdl2_image sdl2_ttf sdl2_mixer`
3. `$ sudo dnf install make gcc git-all SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel`

When the command runs, whichever it is, it's going to ask for your password.
So you just type it and press enter. Don't worry if it seems like your keyboard
stopped working because nothing gets printed out. Trust me. Just do it.\
After all that, follow these steps:

1. `$ git clone https://github.com/pgvalle/SpaceInvadersClone`
2. `$ cd SpaceInvadersClone`
3. `$ make`
4. `$ cp build/SpaceInvadersClone.out .`
5. `$ ./SpaceInvadersClone.out`

Now you're done. You should have the game executing now. Congrats!

## Windows
To be documented...