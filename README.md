# SpaceInvaders Clone

This game, as the name suggests, is yet another 1978 Arcade
[SpaceInvaders](https://en.wikipedia.org/wiki/Space_Invaders)' clone.
Obviously It's not 100% the same as the original one. But I guarantee it's good
enough to be called Space Invaders Clone.

# Build instructions

## Linux
**NOTE 1**: If you are completely new to linux, you should at least remember the
command prompt in Windows, right? Good. So, everytime you read *terminal* here,
bring that command prompt window to your mind.

To start, open up a *terminal* window and try out each of these commands to see
which one prompts something other than "command not found" or so:\
**NOTE 2**: Don't copy the $. It's just a symbol people use to mean "that's a
*terminal* window", if you didn't know.

1. `$ sudo apt install make gcc git libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev`
2. `$ sudo pacman -S make gcc git sdl2 sdl2_image sdl2_ttf sdl2_mixer`
3. `$ sudo dnf install make gcc git-all SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel`

When the command runs, whichever it is, it's going to ask for your password.
So you just type it and press enter.\
**NOTE 3**: Don't worry if it seems like your keyboard stopped working because
the password dots don't get printed out. Don't be affraid. No one is going to
hack into your computer.

Then, follow these commands:
```
$ git clone https://github.com/pgvalle/SpaceInvadersClone
(some output showing up here)
$ cd SpaceInvadersClone
$ make
(some output showing up here)
$ cp build/SpaceInvadersClone.out .
$ ./SpaceInvadersClone.out
```
Now you're done. You should have the game executing now. Congrats!

## Windows
To be documented...