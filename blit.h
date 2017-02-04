#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifndef __BLIT_H__
#define __BLIT_H__
#define max(a,b) ((a)>(b)?(a):(b))
typedef struct img {
	int w;
	int h;
	unsigned char **data;
	int base;
}img;
void blit_test();
img* blit_createimg(int w,int h);
img* blit_createimg(int w, int h, int base);
void blit_freeimg(img* a);
void blit_freeimg(img* a, img* b, ...);
void blit_cdraw(img* map);
void blit_blit(img* dst, img* src, int x, int y);
img* blit_gen(int w, int h);
img* blit_gen(int w, int h, int base);
img* blit_con(img* a, img* b);
img* blit_con(img* a, img* b, int offset);
img* blit_frac(img* a, img* b);
img* blit_power(img* a, img* b);
int parser_test();
img* parse(char* str, bool bigfont);
void gui_draw(img* map);
img* font_gen(char c,bool bigfont);
void setdebug(int a);
#define print(level, title, msg, ...) \
	{debugtitle(level, title);\
	 printf(msg, ##__VA_ARGS__);\
	 printf("\n\e[0m\e[K");}
#define debug(level, title, msg, ...)\
	if(level<=DEBUG){debugtitle(level, title);\
	printf(msg, ##__VA_ARGS__);\
	printf("\n\e[0m\e[K");}
void debugtitle(int level, const char *title);
void key(int keycode);
//cursor
extern img* cursorimg;
extern int cursorx,cursory,cursorh;
//font
extern const char ascii[][16];
//global
extern char str[256];
extern int DEBUG;
#endif
