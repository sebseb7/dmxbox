UNAME := $(shell uname)

COMPILER = gcc


FLAGS= -DSIMULATOR -I. -I.. -I../libs -Ilibs --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes -flto -lportmidi -lm

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

LIB_SOURCES=$(wildcard storage/*.c libs/*.c ../libs/*.c ../libs/menu/*.c ../libs/mcugui/*.c)
LIB_HEADERS=$(wildcard storage/*.h libs/*.h ../libs/*.h ../libs/menu/*.h ../libs/mcugui/*.h)

#echo "int main(){}" | gcc -o /dev/null -x c - -lftdi1 2>/dev/null
FLAGS+= -DLIBFTDI
LDFLAGS+= -lftdi1

all: sim

clean:
	rm -f sim


sim: $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	@echo "  \033[1;34mCompile sim \033[0m"
	@rm -f sim
	@$(COMPILER) -DFULL=1 $(SOURCES) $(LIB_SOURCES) $(FLAGS) main.c $(LDFLAGS) -o sim 

.PHONY : clean all

