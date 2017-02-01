#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
      Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
      *(Uint32 *)target_pixel = pixel;
}
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
      Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
      return *(Uint32 *)target_pixel;
}
int main(){
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window *w = SDL_CreateWindow("ttf test",0,0, 1536, 768, 0);
   SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
   TTF_Init();
   TTF_Font *font = TTF_OpenFont("OpenSans-Regular.ttf",16);
   SDL_Color color={0x7c,0x85,0x77};
   SDL_Surface *message1 = TTF_RenderText_Blended(font, "abcdefghijklmn123456", color);
   set_pixel(message1,0,1,0xffffffff);
   SDL_Rect re;
   re.w=message1->w*6;re.h=message1->h*6;re.x=0;re.y=0;
   SDL_Texture *t = SDL_CreateTextureFromSurface(r, message1);
   SDL_RenderCopy(r, t, NULL,&re);
   SDL_RenderPresent(r);
   SDL_Event e; int a=0;
   while(1){
      while (SDL_PollEvent(&e)) {
          if (e.type == SDL_WINDOWEVENT&&e.window.event==SDL_WINDOWEVENT_CLOSE) {
             TTF_CloseFont(font);
             SDL_DestroyTexture(t);
             SDL_DestroyRenderer(r);
             SDL_DestroyWindow(w);
             SDL_Quit();
             return 0;
          }
          if (e.type == SDL_MOUSEBUTTONDOWN){
             printf("x:%d y:%d color:#%08X\n",e.button.x/6,e.button.y/6,get_pixel(message1,e.button.x/8,e.button.y/8));
          }
      }
      SDL_Delay(10);
   }
   TTF_CloseFont(font);
   SDL_DestroyTexture(t);
   SDL_DestroyRenderer(r);
   SDL_DestroyWindow(w);
   SDL_Quit();
   return 0;
}
