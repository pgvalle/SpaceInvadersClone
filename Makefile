FLAGS := $(shell pkg-config --cflags --libs sdl2 SDL2_image)\
	-O2 -std=c99 -lm

space_invaders.out: main.c stb_ds.h
	$(CC) main.c stb_ds.h $(FLAGS) -o $@
