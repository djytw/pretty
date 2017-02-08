#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "SDL2/SDL.h"
#include <stdio.h>
#include <string.h>
#include "pretty.h"
FT_Library  library;
int key(SDL_Keysym k);
#define KEY_STATUS_NON -1
#define KEY_STATUS_ESC -2
#define KEY_STATUS_REG 0

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
void gui(){
   int wi=MIN_WIDTH;
   int hi=MIN_HEIGHT;
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window *w = SDL_CreateWindow("PRETTY",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,wi,hi, 0);
   SDL_Renderer *r = SDL_CreateRenderer(w, -1,SDL_RENDERER_ACCELERATED);
   SDL_Surface *s= SDL_CreateRGBSurface(0,wi,hi,32,0,0,0,0);
   str[0]=0;
   img* map=parse(0,1,1);
   if(cursorimg){debug(4,"CURSOR","X:%d Y:%d H:%d\tfinal:%p curimg:%p",cursorx,cursory,cursorh,map,cursorimg);}
   else{debug(4,"CURSOR","No Cursor in current expression.");}
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
   SDL_FreeSurface(s);
   SDL_DestroyTexture(t);
   SDL_Event e; int a=0;
   while(1){
      while (SDL_PollEvent(&e)) {
         switch(e.type){
            case SDL_WINDOWEVENT:
            if (e.window.event==SDL_WINDOWEVENT_CLOSE) goto end;
            break;
            case SDL_MOUSEBUTTONDOWN:
            if(e.button.x/6>map->w||e.button.y/6>map->h)break;
            debug(0,"CLICK","x:%d y:%d color:%02X",e.button.x/6,e.button.y/6,map->data[e.button.y/6][e.button.x/6]);
            break;
            case SDL_KEYDOWN:
            int keystatus=key(e.key.keysym);
            if(keystatus==-2)goto end;
            if(keystatus==-1)break;
            blit_freeimg(map);
            map=parse(0,strlen(str),1);
            if(cursorimg){
               debug(4,"CURSOR","X:%d Y:%d H:%d\tfinal:%p curimg:%p",cursorx,cursory,cursorh,map,cursorimg);
               int i;
         		for(i=0;i<cursorh;i++){
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
            SDL_Event e; int a=0;
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
int key(SDL_Keysym k){
   int keycode=k.sym;
   int mod=k.mod;
   debug(0,"KEYDOWN","key %s/0x%02X is pressed. Mod=%d ",SDL_GetKeyName(keycode),keycode,mod);
   if(keycode==SDLK_F1){//fraction
      insert("[@]/[@]");
      pos_shift(-6);
      debug(4,"INPUT","str=\e[32m\e[1m%s\e[0m",str);
      return 0;
   }
   if(keycode==SDLK_F2||mod&KMOD_SHIFT&&keycode=='6'){//power
      insert("^[@]");
      pos_shift(-2);
      debug(4,"INPUT","str=\e[32m\e[1m%s\e[0m",str);
      return 0;
   }
   if(keycode==SDLK_LEFT){
      pos_shift(-1);
      return 0;
   }
   if(keycode==SDLK_RIGHT){
      pos_shift(1);
      return 0;
   }
   if(keycode>1<<30)//SDL_SCANCODE_TO_KEYCODE
      return -1;
   if(mod&KMOD_CTRL&&keycode=='c')
      return -2;
   if(keycode==SDLK_ESCAPE)
      return -2;
   if(mod&0xff0)//alt or ctrl
      return -1;
	if(islower(keycode)){
		if((mod&KMOD_SHIFT)||(mod&KMOD_CAPS))insert(keycode-'a'+'A');
      else insert(keycode);
      debug(4,"INPUT","str=\e[32m\e[1m%s\e[0m",str);
      return 0;
	}
   if(isdigit(keycode)){
      char tr[]=")!@#$%^&*(";
      if(mod&KMOD_SHIFT&&(keycode>'7'||keycode=='0'))insert(tr[keycode-'0']);
      else insert(keycode);
      debug(4,"INPUT","str=\e[32m\e[1m%s\e[0m",str);
      return 0;
   }
   if(keycode=='/'||keycode=='.'||keycode=='-'){
      insert(keycode);
      debug(4,"INPUT","str=\e[32m\e[1m%s\e[0m",str);
      return 0;
   }
   if(keycode=='='&&mod&KMOD_SHIFT){
      insert('+');
      debug(4,"INPUT","str=\e[32m\e[1m%s\e[0m",str);
      return 0;
   }
   switch(keycode){
	case '[':
		insert('(');
		break;
	case ']':
		insert(')');
		break;
	case SDLK_LEFT:
		pos_shift(-1);
		break;
	case SDLK_RIGHT:
		pos_shift(1);
		break;
	}
}
