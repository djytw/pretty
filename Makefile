objects = $(patsubst %.cpp,build/%.o,$(wildcard *.cpp))
executable = output/pretty
INCLUDE = . /usr/include/freetype2
CFLAGS = -g -Wall
LIBS = SDL2 freetype
CC = g++

all : $(executable)
build/keyboard.o : keyboard.h


$(executable) : $(objects)
	@/usr/bin/printf "\e[0;43m"
	@echo "$(CC) -o $(executable) $(objects) $(patsubst %,-l%,$(LIBS))"
	@/usr/bin/printf "\e[0;45m\e[K"
	@$(CC) -o $(executable) $(objects) $(patsubst %,-l%,$(LIBS))
	@/usr/bin/printf "\e[0;42m\e[K"
	cat res/OpenSans-Regular.ttf > output/font.ttf
	@/usr/bin/printf "\e[0m\e[K"

$(objects) : build/%.o : %.cpp pretty.h
	@if [ ! -d "output" ];then mkdir output; fi
	@if [ ! -d "build" ];then mkdir build; fi
	@/usr/bin/printf "\e[0;46m"
	@echo "$(CC) -c $< -o $@ $(patsubst %,-I%,$(INCLUDE)) $(CFLAGS)"
	@/usr/bin/printf "\e[0;45m\e[K"
	@$(CC) -c $< -o $@ $(patsubst %,-I%,$(INCLUDE)) $(CFLAGS)
	@/usr/bin/printf "\e[0m\e[K"

.PHONY : clean
clean :
	@/usr/bin/printf "\e[0;42m\e[K"
	-rm -f build/* output/pretty
	@/usr/bin/printf "\e[0m\e[K"
