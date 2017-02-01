#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "SDL2/SDL.h"
#include <stdio.h>
#include <string.h>
#include "blit.h"
FT_Library  library;
img* font_gen(char c, bool bigfont){
   int size;
   bigfont?size=16:size=12;
   FT_Init_FreeType(&library);
   FT_Face face;
   FT_New_Face(library,"OpenSans-Regular.ttf",0,&face);
   FT_Set_Pixel_Sizes(face,0,size);
   FT_Load_Char(face,c,FT_LOAD_RENDER);
   FT_GlyphSlot slot=face->glyph;
   int i,j,p;
   FT_Bitmap *bitmap=&slot->bitmap;
   img *ret=blit_createimg(slot->advance.x >> 6 ,size);
   printf("char:%c advance.x=%ld bitmap_left=%d bitmap->width=%d\n",c,slot->advance.x>>6,slot->bitmap_left,bitmap->width);
   for(p=0,i=slot->bitmap_left;p<bitmap->width;i++,p++){
      for(j=0;j<bitmap->rows;j++){
         if(i<0||j<0||i>=ret->w||j>=ret->h)continue;
         unsigned char a = bitmap->buffer[j*bitmap->width+p];
         //to 2bit gray:
         //00-2A -> 00 ; 2B-7F -> 55 ; 80-D4 -> AA ; D5-FF -> FF
         if(a<=0x2A)
            ret->data[j][i]=0;
         else if(a<=0x7F)
            ret->data[j][i]=0x55;
         else if(a<=0xD4)
            ret->data[j][i]=0xAA;
         else
            ret->data[j][i]=0xFF;
      }
   }
   ret->base=slot->bitmap_top-12;
   printf("%d\n",ret->base);
   return ret;
}
void gui_draw(img* map){
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window *w = SDL_CreateWindow("ft test",0,0,1536,768, 0);
   SDL_Renderer *r = SDL_CreateRenderer(w, -1,SDL_RENDERER_ACCELERATED);
   SDL_Surface *s= SDL_CreateRGBSurface(0,1536,768,32,0,0,0,0);
   SDL_FillRect(s,NULL,0xFF888888);
   SDL_Rect re;re.w=5;re.h=5;
   int i,j;
   for(i=0;i<map->h;i++)for(j=0;j<map->w;j++){
         re.x=j*6;re.y=i*6;
         int color=(255-map->data[i][j])*0x010101+0xff000000;
         SDL_FillRect(s,&re,color);
   }
   SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
   SDL_RenderCopy(r, t, NULL,NULL);
   SDL_RenderPresent(r);
   SDL_Event e; int a=0;
   while(1){
      while (SDL_PollEvent(&e)) {
          if (e.type == SDL_WINDOWEVENT&&e.window.event==SDL_WINDOWEVENT_CLOSE) {
             SDL_DestroyTexture(t);
             SDL_DestroyRenderer(r);
             SDL_DestroyWindow(w);
             SDL_Quit();
             return;
          }
          if (e.type == SDL_MOUSEBUTTONDOWN){
             printf("x:%d y:%d color:%02X\n",e.button.x/6,e.button.y/6,map->data[e.button.y/6][e.button.x/6]);
          }
      }
      SDL_Delay(10);
   }
}
