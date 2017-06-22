export exec = output/pretty
export INCLUDE = . /usr/include/freetype2
export CFLAGS = -g -Wall
export LIBS = SDL2 freetype
export CC = g++
export PRINTF = /usr/bin/printf
export font_file = OpenSans-Regular.ttf

SYS := $(shell uname -s)
LINUX := Linux
ifeq ($(SYS),$(LINUX))
  export color_gcc=46
  export color_log=45
  export color_cmd=42
  export color_link=43
else
  export color_gcc=36
  export color_log=35
  export color_cmd=32
  export color_link=33
endif
all :
	@$(PRINTF) "\e[0;$(color_cmd)m\e[K"
	@if [ ! -d "output" ];then\
		$(PRINTF) "mkdir output...\n";\
		mkdir output;\
	fi
	@if [ ! -d "src/build" ];then\
		$(PRINTF) "mkdir src/build...\n";\
		mkdir src/build;\
	fi
	@if [ ! -f "output/font.ttf" ];then\
		$(PRINTF) "copy font file...\n";\
		cat res/$(font_file) > output/font.ttf;\
	fi
ifneq ($(SYS),$(LINUX))
	@if [ ! -f "output/SDL2.dll" ];then\
		$(PRINTF) "copy SDL Library...\n";\
		cat res/SDL2.dll > output/SDL2.dll;\
	fi
endif
	@$(PRINTF) "\e[0m\e[K"
	@cd src; make;
	@make run

.PHONY : clean run
clean :
	@$(PRINTF) "\e[0;$(color_cmd)m\e[K"
	-rm -f -r src/build/ output/
	@$(PRINTF) "\e[0m\e[K"
run :
	@cd output;./pretty
