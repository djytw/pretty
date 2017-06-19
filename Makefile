objects = $(patsubst %.cpp,build/%.o,$(wildcard *.cpp))
executable = output/pretty
INCLUDE = . /usr/include/freetype2
CFLAGS = -g -Wall
LIBS = SDL2 freetype
CC = g++

all : $(executable)
build/keyboard.o : keyboard.h

color_gcc = 36
color_log = 35
color_cmd = 32
color_link = 33

font_file = OpenSans-Regular.ttf

$(executable) : $(objects)
	@/usr/bin/printf "\e[0;$(color_link)m"
	@echo "$(CC) -o $(executable) $(objects) $(patsubst %,-l%,$(LIBS))"
	@/usr/bin/printf "\e[0;$(color_log)m\e[K"
	@$(CC) -o $(executable) $(objects) $(patsubst %,-l%,$(LIBS))
	@/usr/bin/printf "\e[0;$(color_cmd)m\e[K"
	cat res/$(font_file) > output/font.ttf
	cat res/SDL2.dll > output/SDL2.dll
	@/usr/bin/printf "\e[0m\e[K"

$(objects) : build/%.o : %.cpp pretty.h
	@if [ ! -d "output" ];then mkdir output; fi
	@if [ ! -d "build" ];then mkdir build; fi
	@/usr/bin/printf "\e[0;$(color_gcc)m"
	@echo "$(CC) -c $< -o $@ $(patsubst %,-I%,$(INCLUDE)) $(CFLAGS)"
	@/usr/bin/printf "\e[0;$(color_log)m\e[K"
	@$(CC) -c $< -o $@ $(patsubst %,-I%,$(INCLUDE)) $(CFLAGS)
	@/usr/bin/printf "\e[0m\e[K"

.PHONY : clean
clean :
	@/usr/bin/printf "\e[0;$(color_cmd)m\e[K"
	-rm -f build/* output/pretty
	@/usr/bin/printf "\e[0m\e[K"
