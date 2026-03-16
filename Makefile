SRC   = $(wildcard source/*.c) $(wildcard source/**/*.c)
DEPS  = $(wildcard source/*.h) $(wildcard source/**/*.h)
OBJ   = $(addsuffix .o,$(subst source/,bin/,$(basename ${SRC})))
OUT   = arkas

FLAGS  = -std=c99 -Wall -Wextra -pedantic -Wuninitialized -Llib/ArkasEngine
FLAGS += -Ilib/ArkasEngine/lib

# arkas flags here
FLAGS += -DAE_BACKEND_GL_LEGACY -DAE_AUDIO_PSRC -DAE_USE_SDL2

ifeq ($(PLAT), windows)
	CC     = x86_64-w64-mingw32-gcc
	LIBS   = $(shell x86_64-w64-mingw32-pkg-config sdl2 --libs)
	FLAGS += $(shell x86_64-w64-mingw32-pkg-config sdl2 --cflags)
else
	LIBS = -lSDL2 -lArkasEngine -lGL -lm
endif

ifeq ($(BUILD), release)
	FLAGS += -Ofast -s
else
	FLAGS += -g

	ifeq ($(ASAN),y)
		override FLAGS += -fsanitize=address,undefined
		override LIBS  += -fsanitize=address,undefined
	endif
endif

compile: $(OBJ) $(SRC) $(DEPS)
	$(CC) $(OBJ) $(LIBS) -o $(OUT) $(FLAGS)

./bin:
	mkdir -p bin

bin/%.o: source/%.c $(DEPS)
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm -r bin/* $(OUT)
	if [ -f bin ]; then rm -r bin; fi
	if [ -f $(OUT) ]; then rm $(OUT); fi


