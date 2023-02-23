FLAGS := $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_mixer)\
	-O2 -std=c99 -lm
SOURCES := $(shell find src -name "*.c") $(shell find src -name "*.h")


space_invaders.out: $(SOURCES)
	$(CC) $(SOURCES) $(FLAGS) -o $@
