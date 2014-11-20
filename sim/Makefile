UNAME := $(shell uname)

COMPILER = gcc


FLAGS= -I. -I.. -I../libs --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes -flto 

#-fsingle-precision-constant
LDFLAGS=
BINEXT=

ifeq ($(UNAME), Darwin)
	COMPILER = gcc
	FLAGS   += -Qunused-arguments

	FLAGS   += `sdl-config --cflags`
	LDFLAGS += `sdl-config --libs`
endif

ifeq ($(UNAME), Linux)
	FLAGS   += `sdl-config --cflags`
	LDFLAGS += `sdl-config --libs`
endif

ifeq ($(UNAME), MINGW32_NT-6.1)
	FLAGS   += -I/MinGW/msys/1.0/include/
	LDFLAGS += -L/MinGW/msys/1.0/lib -lmingw32 -lSDLmain -lSDL
	BINEXT = .exe
endif

LIB_SOURCES=$(wildcard ../libs/*.c ../libs/menu/*.c ../libs/mcugui/*.c)
LIB_HEADERS=$(wildcard ../libs/*.h ../libs/menu/*.h ../libs/mcugui/*.h)

all: sim

clean:
	rm -f sim


sim: $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	@echo -e "  \033[1;34mCompile sim \033[0m"
	@rm -f sim
	@$(COMPILER) -DFULL=1 $(FLAGS) $(LIB_SOURCES) $(SOURCES) main.c $(LDFLAGS) -o sim 

.PHONY : clean all

