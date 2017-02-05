#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "SDL2/SDL.h"
#include <stdio.h>
#include <string.h>
#include "blit.h"
FT_Library  library;
img* font_gen(char c, bool bigfont){
   int size,base;
   if(bigfont){size=16;base=12;}
   else{size=12;base=9;}
   FT_Init_FreeType(&library);
   FT_Face face;
   FT_New_Face(library,"OpenSans-Regular.ttf",0,&face);
   FT_Set_Pixel_Sizes(face,0,size);
   FT_Load_Char(face,c,FT_LOAD_RENDER);
   FT_GlyphSlot slot=face->glyph;
   int i,j,p;
   FT_Bitmap *bitmap=&slot->bitmap;
   img *ret=blit_createimg(slot->advance.x >> 6 ,size+slot->bitmap_top-base,slot->bitmap_top-base);
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
   debug(2,"FONTGEN","\e[33m\e[1mCHAR:\e[0m%c \e[33m\e[1mW:\e[0m%d \e[33m\e[1mH:\e[0m%d \e[33m\e[1mBASE:\e[0m%d",c,ret->w,ret->h,ret->base);
   return ret;
}
void gui_draw(img* map){
   int wi=map->w*6;
   int hi=map->h*6;
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window *w = SDL_CreateWindow("PRETTY",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,wi,hi, 0);
   SDL_Renderer *r = SDL_CreateRenderer(w, -1,SDL_RENDERER_ACCELERATED);
   SDL_Surface *s= SDL_CreateRGBSurface(0,wi,hi,32,0,0,0,0);
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
         switch(e.type){
            case SDL_WINDOWEVENT:
            if (e.window.event==SDL_WINDOWEVENT_CLOSE) goto end;
            break;
            case SDL_MOUSEBUTTONDOWN:
            debug(0,"CLICK","x:%d y:%d color:%02X",e.button.x/6,e.button.y/6,map->data[e.button.y/6][e.button.x/6]);
            break;
            case SDL_KEYDOWN:
            debug(0,"KEYDOWN","key %s/0x%02X is pressed.",SDL_GetKeyName(e.key.keysym.sym),e.key.keysym.sym);
            if(e.key.keysym.sym==SDLK_ESCAPE)goto end;
            key(e.key.keysym.sym);
            break;
         }
      }
      SDL_Delay(10);
   }
   end:
   SDL_DestroyTexture(t);
   SDL_DestroyRenderer(r);
   SDL_DestroyWindow(w);
   SDL_Quit();
   return;
}
