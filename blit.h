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
void gui_draw(img* map);
//cursor
extern img* cursorimg;
extern int cursorx,cursory,cursorh;

//font
extern const char ascii[][16];
img* font_gen(char c);

//global
extern char str[256];

#endif
