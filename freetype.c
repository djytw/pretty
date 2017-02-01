#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "SDL2/SDL.h"
#include <stdio.h>
#include <string.h>
FT_Library  library;
unsigned char image[128][256];
void draw_bitmap(FT_Bitmap *bitmap,int x, int y){
  int i, j, p, q;
  int x_max = x + bitmap->width;
  int y_max = y + bitmap->rows;
  for(i=x,p=0;i<x_max;i++,p++){
    for (j=y,q=0;j<y_max;j++,q++){
      if(i<0||j<0||i>=256||j>=128)continue;
      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
      //to 2bit gray:
      //00-2A -> 00 ; 2B-7F -> 55 ; 80-D4 -> AA ; D5-FF -> FF
      if(image[j][i]<=0x2A)
         image[j][i]=0;
      else if(image[j][i]<=0x7F)
         image[j][i]=0x55;
      else if(image[j][i]<=0xD4)
         image[j][i]=0xAA;
      else
         image[j][i]=0xFF;
    }
  }
}
int main(){
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window *w = SDL_CreateWindow("ft test",0,0,1536,768, 0);
   SDL_Renderer *r = SDL_CreateRenderer(w, -1,SDL_RENDERER_ACCELERATED);
   SDL_Surface *s= SDL_CreateRGBSurface(0,1536,768,32,0,0,0,0);
   SDL_FillRect(s,NULL,0xFF888888);
   SDL_Rect re;re.w=5;re.h=5;
   FT_Init_FreeType(&library);
   FT_Face face;
   FT_New_Face(library,"OpenSans-Regular.ttf",0,&face);
   //FT_Set_Char_Size(face,0,16,3000,3000 );
   FT_Set_Pixel_Sizes(face,0,12);
   FT_GlyphSlot slot=face->glyph;
   //FT_Render_Glyph( slot,FT_RENDER_MODE_MONO );
   int i,j;
   int penx=0,peny=16;
   //generate
   char str[]="Abgf01";
   for(i=0;i<strlen(str);i++){
      FT_Load_Char(face,str[i],FT_LOAD_RENDER);
      draw_bitmap(&slot->bitmap,slot->bitmap_left+penx,peny-slot->bitmap_top);
      penx += slot->advance.x >> 6;
   }
   //draw picture
   for(i=0;i<128;i++)for(j=0;j<256;j++){
         re.x=j*6;re.y=i*6;
         int color=(255-image[i][j])*0x010101+0xff000000;
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
             return 0;
          }
          if (e.type == SDL_MOUSEBUTTONDOWN){
             printf("x:%d y:%d color:%02X\n",e.button.x/6,e.button.y/6,image[e.button.y/6][e.button.x/6]);
          }
      }
      SDL_Delay(10);
   }
}
