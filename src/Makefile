objects = $(patsubst %.cpp,build/%.o,$(wildcard *.cpp))
SRC_BASE = ..
executable = $(SRC_BASE)/$(exec)

all : $(executable)
build/keyboard.o : keyboard.h

$(executable) : $(objects)
	@$(PRINTF) "\e[0;$(color_link)m"
	@echo "$(CC) -o $(executable) $(objects) $(patsubst %,-l%,$(LIBS))"
	@$(PRINTF) "\e[0;$(color_log)m\e[K"
	@$(CC) -o $(executable) $(objects) $(patsubst %,-l%,$(LIBS))
	@$(PRINTF) "\e[0m\e[K"

$(objects) : build/%.o : %.cpp pretty.h
	@$(PRINTF) "\e[0;$(color_gcc)m"
	@echo "$(CC) -c $< -o $@ $(patsubst %,-I%,$(INCLUDE)) $(CFLAGS)"
	@$(PRINTF) "\e[0;$(color_log)m\e[K"
	@$(CC) -c $< -o $@ $(patsubst %,-I%,$(INCLUDE)) $(CFLAGS)
	@$(PRINTF) "\e[0m\e[K"
