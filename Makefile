pretty : blit.o keyboard.o main.o parser.o gui.o debug.o
	g++ -o pretty blit.o keyboard.o main.o parser.o gui.o debug.o -lSDL2 -lfreetype
blit.o : blit.cpp blit.h
	g++ -c -g blit.cpp
keyboard.o : keyboard.cpp keyboard.h blit.h
	g++ -c -g keyboard.cpp
main.o : main.cpp blit.h
	g++ -c -g main.cpp
parser.o : parser.cpp blit.h
	g++ -c -g parser.cpp
gui.o : gui.cpp blit.h
	g++ -c -g gui.cpp -I/usr/include/freetype2
debug.o : debug.cpp blit.h
	g++ -c -g debug.cpp
clean :
	rm *.o pretty
