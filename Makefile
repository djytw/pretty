pretty : blit.o font.o keyboard.o main.o parser.o
	g++ -o pretty blit.o font.o keyboard.o main.o parser.o
blit.o : blit.cpp blit.h
	g++ -c blit.cpp
font.o : font.cpp blit.h
	g++ -c font.cpp
keyboard.o : keyboard.cpp keyboard.h blit.h
	g++ -c keyboard.cpp
main.o : main.cpp blit.h
	g++ -c main.cpp
parser.o : parser.cpp blit.h
	g++ -c parser.cpp
clean :
	rm *.o pretty
