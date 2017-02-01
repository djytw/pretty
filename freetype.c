#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "SDL2/SDL.h"
#include <stdio.h>

FT_Library  library;
int image[100][100];
void draw_bitmap(FT_Bitmap* bitmap,FT_Int x,FT_Int y){
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;
  for ( i = x, p = 0; i < x_max; i++, p++ ){
    for ( j = y, q = 0; j < y_max; j++, q++ ){
      if ( i < 0 || j < 0 || i >= 100 || j >= 100 )
        continue;
      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}
int main(){
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window *w = SDL_CreateWindow("ft test",0,0,1536,768, 0);
   SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
   SDL_Surface *s= SDL_CreateRGBSurface(0,1536,768,32,0,0,0,0);
   SDL_FillRect(s,NULL,0xFF888888);
   SDL_Rect re;re.w=5;re.h=5;
   FT_Init_FreeType( &library );
   FT_Face face;
   FT_New_Face( library,"OpenSans-Regular.ttf",0,&face);
   //FT_Set_Char_Size(face,0,16,3000,3000 );
   FT_Set_Pixel_Sizes(face,0,20);
   FT_GlyphSlot  slot = face->glyph;
   FT_Load_Char( face, 'g', FT_LOAD_RENDER);
   int i,j;
   for(i=0;i<*(&slot->bitmap.rows);i++){
      for(j=0;j<*(&slot->bitmap.width);j++){
         re.x=j*6;re.y=i*6;
         int color=0xff-slot->bitmap.buffer[i**(&slot->bitmap.width)+j];
         color=0xff000000+color*0x111111;
         printf("draw color:#%08X x:%d y:%d\n",color,j,i);
         SDL_FillRect(s,&re,color);
      }
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
      }
      SDL_Delay(10);
   }
}
