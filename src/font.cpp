#include "pretty.h"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

FT_Library  library;

img* font_gen_char(char c, bool bigfont){
   int size,base;
   if(bigfont){size=16;base=12;}
   else{size=12;base=9;}
   FT_Init_FreeType(&library);
   FT_Face face;
   FT_New_Face(library,"font.ttf",0,&face);
   FT_Set_Pixel_Sizes(face,0,size);
   FT_Load_Char(face,c,FT_LOAD_RENDER);
   FT_GlyphSlot slot=face->glyph;
   int i,j,p;
   FT_Bitmap *bitmap=&slot->bitmap;
   img *ret=blit_createimg(slot->advance.x >> 6 ,(slot->bitmap).rows,slot->bitmap_top-base);
   for(p=0,i=slot->bitmap_left;p<(int)bitmap->width;i++,p++){
      for(j=0;j<(int)bitmap->rows;j++){
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
   FT_Done_Face(face);
   FT_Done_FreeType(library);
   return blit_expand(ret,(bigfont?17:13));
}
img* font_gen(char c, bool bigfont){
   if(c!='@')return font_gen_char(c,bigfont);
   int w,h;
   if(bigfont){w=9;h=12;}
   else{w=7;h=9;}
   img* ret=blit_createimg(w,h,0);
   int i;
   for(i=1;i<w-1;i++){
      ret->data[0][i]=0xff;
      ret->data[h-1][i]=0xff;
   }
   for(i=0;i<h;i++){
      ret->data[i][0]=0xff;
      ret->data[i][w-2]=0xff;
   }
   return blit_expand(ret,(bigfont?17:13));
}
