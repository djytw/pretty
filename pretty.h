#include <stdio.h>
#include <stdlib.h>
#ifndef __PRETTY_H__
#define __PRETTY_H__

/********            STRUCT           ********/
typedef struct img {
	int w;
	int h;
	unsigned char **data;
	int base;
}img;

/********      FUNCTIONS & MACRO      ********/
#define max(a,b) ((a)>(b)?(a):(b))
//blit.cpp
img* blit_createimg(int w,int h);
img* blit_createimg(int w, int h, int base);
void blit_freeimg(img* a);
void blit_freeimg(img* a, img* b, ...);
void blit_blit(img* dst, img* src, int x, int y);
img* blit_con(img* a, img* b);
img* blit_con(img* a, img* b, int offset);
img* blit_con_f(img* a, img *b);
img* blit_con_f(img* a, img* b, int offset);
img* blit_con_f(img* a, img* b, img* c);
img* blit_frac(img* a, img* b);
img* blit_frac_f(img* a, img* b);
img* blit_power(img* a, img* b);
img* blit_power_f(img* a, img* b);
img* blit_bracket_f(img* a, bool bigfont);
img* blit_bracket_f(img* a, bool bigfont, bool norbraket);
//parser.cpp
int parser_test();
int ttype(char c);
img* parse(int start, int end, bool bigfont);
//gui.cpp
void gui();
void gui_draw(img* map);
img* font_gen(char c,bool bigfont);
//debug.cpp
void setdebug(int a);
void debugtitle(int level, const char *title);
#define print(level, title, msg, ...) \
	{debugtitle(level, title);\
	 printf(msg, ##__VA_ARGS__);\
	 printf("\n\e[0m\e[K");}
#define debug(level, title, msg, ...)\
	if(level<=DEBUG){debugtitle(level, title);\
	printf(msg, ##__VA_ARGS__);\
	printf("\n\e[0m\e[K");}
//keyboard.cpp
void insert(char c);
void insert(const char* s);
void pos_shift(int pos);

/********        GLOBAL VARIBLES      ********/
//cursor
extern img* cursorimg;
extern int cursorx,cursory,cursorh;
//global
extern char str[256];
extern int posi;
extern int DEBUG;
#define MIN_WIDTH 400
#define MIN_HEIGHT 200
#define NUMBER 0 //?
#define CONST 1
#define OPER 2
#define OPERM 3//?
#define POWER 4
#define BRACKET 5
#define CBRAC 6





#endif
