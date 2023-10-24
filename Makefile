FLAGS := $(shell pkg-config --cflags --libs sdl2) -O2 -std=c99 -lm -Iext/ -Isrc/
SOURCES := $(shell find ./src -name "*.c")

space_invaders.out: $(SOURCES)
	$(CC) $(SOURCES) $(FLAGS) -o $@
