#include "SDL2/SDL.h"
#include <stdio.h>
#include <string.h>
#include "pretty.h"
#define KEY_STATUS_NON -1
#define KEY_STATUS_ESC -2
#define KEY_STATUS_REG 0

int key(SDL_Keysym k);
void gui(){
   int wi=MIN_WIDTH;
   int hi=MIN_HEIGHT;
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window *w = SDL_CreateWindow("PRETTY",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,wi,hi, 0);
   SDL_Renderer *r = SDL_CreateRenderer(w, -1,SDL_RENDERER_ACCELERATED);
   SDL_Surface *s= SDL_CreateRGBSurface(0,wi,hi,32,0,0,0,0);
   SDL_FillRect(s,NULL,0xFF888888);
   img *map=blit_createimg(0,0);
   SDL_Rect re;re.w=5;re.h=5;
   SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
   SDL_RenderCopy(r, t, NULL,NULL);
   SDL_RenderPresent(r);
   SDL_FreeSurface(s);
   SDL_DestroyTexture(t);
   SDL_Event e;
   while(1){
      while (SDL_PollEvent(&e)) {
         switch(e.type){
            case SDL_WINDOWEVENT:
            if (e.window.event==SDL_WINDOWEVENT_CLOSE) goto end;
            break;
            case SDL_MOUSEBUTTONDOWN:
            if(e.button.x/6>map->w||e.button.y/6>map->h)break;
            print(0,"CLICK","x:%d y:%d color:%02X",e.button.x/6,e.button.y/6,map->data[e.button.y/6][e.button.x/6]);
            break;
            case SDL_KEYDOWN:
            int keystatus=key(e.key.keysym);
            if(keystatus==-2)goto end;
            if(keystatus==-1)break;
            blit_freeimg(map);
            cursorimg=0;
            map=parse(0,strlen(str),1);
            if(cursorimg){
               debug(4,"CURSOR","\t\tX:%d Y:%d H:%d",cursorx,cursory,cursorh);
               int i;if(cursorx>0)cursorx--;
         		for(i=max(-cursory,1);i<1+cursorh&&cursory+i<map->h;i++){
         			map->data[cursory+i][cursorx]=0x88;
         		}
            }
            else{debug(4,"CURSOR","No Cursor in current expression.");}
            wi=max(map->w*6,MIN_WIDTH);
            hi=max(map->h*6,MIN_HEIGHT);
            s= SDL_CreateRGBSurface(0,wi,hi,32,0,0,0,0);
            SDL_FillRect(s,NULL,0xFF888888);
            SDL_SetWindowSize(w,wi,hi);
            int i,j;
            for(i=0;i<map->h;i++)for(j=0;j<map->w;j++){
                  re.x=j*6;re.y=i*6;
                  int color=(255-map->data[i][j])*0x010101+0xff000000;
                  SDL_FillRect(s,&re,color);
            }
            t = SDL_CreateTextureFromSurface(r, s);
            SDL_RenderCopy(r, t, NULL,NULL);
            SDL_RenderPresent(r);
            SDL_FreeSurface(s);
            SDL_DestroyTexture(t);
            break;
         }
      }
      SDL_Delay(10);
   }
   end:
   SDL_DestroyWindow(w);
   SDL_Quit();
   return;
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
   SDL_DestroyTexture(t);
   SDL_DestroyRenderer(r);
   SDL_Event e;
   while(1){
      while (SDL_PollEvent(&e)) {
         switch(e.type){
            case SDL_WINDOWEVENT:
            if (e.window.event==SDL_WINDOWEVENT_CLOSE) goto end;
            break;
            case SDL_MOUSEBUTTONDOWN:
            print(0,"CLICK","x:%d y:%d color:%02X",e.button.x/6,e.button.y/6,map->data[e.button.y/6][e.button.x/6]);
            break;
            case SDL_KEYDOWN:
            print(0,"KEYDOWN","key %s/0x%02X is pressed.",SDL_GetKeyName(e.key.keysym.sym),e.key.keysym.sym);
            if(e.key.keysym.sym==SDLK_ESCAPE)goto end;
            if(e.key.keysym.sym==SDLK_LEFT)pos_shift_left();
            if(e.key.keysym.sym==SDLK_RIGHT)pos_shift_right();
            break;
         }
      }
      SDL_Delay(10);
   }
   end:
   SDL_DestroyWindow(w);
   SDL_Quit();
   return;
}
