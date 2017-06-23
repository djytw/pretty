#include "SDL2/SDL.h"
#include <stdio.h>
#include <string.h>
#include "pretty.h"

void UI_test(){
   int wi=480;
   int hi=1000;
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window *w = SDL_CreateWindow("PRETTY",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,wi,hi, 0);
   SDL_Renderer *r = SDL_CreateRenderer(w, -1,SDL_RENDERER_ACCELERATED);
   SDL_Surface *s= SDL_CreateRGBSurface(0,wi,hi,32,0,0,0,0);
   SDL_FillRect(s,NULL,0xFF888888);
   SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
   SDL_RenderCopy(r, t, NULL,NULL);
   SDL_RenderPresent(r);
   SDL_Event e;
   while(1){
      while (SDL_PollEvent(&e)) {
         switch(e.type){
            case SDL_WINDOWEVENT:
            if (e.window.event==SDL_WINDOWEVENT_CLOSE) goto end;
            break;
            case SDL_MOUSEBUTTONDOWN:
            print(1,"CLICK","x:%d y:%d",e.button.x,e.button.y);
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
