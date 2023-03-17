FLAGS := $(shell pkg-config --cflags --libs sdl2 SDL2_mixer) -O2 -std=c99 -lm
SOURCES := $(shell find ./src -type f \( -name "*.sh" -o -name "*.txt" \))

space_invaders.out: $(SOURCES)
	$(CC) $(SOURCES) $(FLAGS) -o $@
