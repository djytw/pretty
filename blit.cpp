#include "pretty.h"
#include <stdarg.h>
img* blit_createimg(int w, int h){//TODO -- handle 0,0 input for malloc(0) security issues
	return blit_createimg(w, h, 0);
}
img* blit_createimg(int w, int h, int base){
	if(w==0||h==0){print(7,"CREATEIMG","0");}
	img* ret=(img*)malloc(sizeof(img));
	ret->w=w;
	ret->h=h;
	ret->base=base;
	if(w==0||h==0)return ret;
	ret->data=(unsigned char**)malloc(h*sizeof(unsigned char*));
	int i;
	for(i=0;i<h;i++)
	ret->data[i]=(unsigned char*)calloc(w,sizeof(unsigned char));
	return ret;
}
void blit_freeimg(img* a){
	int i;
	if(a->w==0||a->h==0){
		free(a);
		return;
	}
	for(i=0;i<a->h;i++)free(a->data[i]);
	free(a->data);
	free(a);
}
void blit_freeimg(img* a, img* b, ...){
	va_list ap;
	va_start(ap,b);
	img* t=va_arg(ap,img*);
	while(t){
		blit_freeimg(t);
		t=va_arg(ap,img*);
	}
	blit_freeimg(a);
	blit_freeimg(b);
}
void blit_blit(img* dst, img* src, int x, int y){
	if(dst->w==0||dst->h==0||src->w==0||src->h==0)print(7,"BLIT","0");
	int i,j;
	for(i=0;i<src->w&&i+x<dst->w;i++)
		for(j=0;j<src->h&&j+y<dst->h;j++)
			dst->data[y+j][x+i]=src->data[j][i];
	if(cursorimg==src){
		cursorx+=x;
		cursory+=y;
		cursorimg=dst;
		debug(2,"CURSOR","Cursor transferred to new position: x:%d y:%d old=%p new=%p",cursorx,cursory,src,dst);
	}
}
img* blit_con_f(img* a, img *b){
	img* t=blit_con(a,b);
	blit_freeimg(a,b,0);
	return t;
}
img* blit_con_f(img* a, img* b, img* c){
	img *t=blit_con(a,b);
	img *t2=blit_con(t,c);
	blit_freeimg(t,a,b,c,0);
	return t2;
}
img* blit_con_f(img* a, img* b, int offset){
	img* t=blit_con(a,b,offset);
	blit_freeimg(a,b,0);
	return t;
}
img* blit_con(img* a, img* b){
	return blit_con(a,b,0);
}
img* blit_con(img* a, img* b, int offset){
	if(a->w==0||a->h==0){
		int base=b->base;
		int h=b->h;
		int w=b->w+offset*2;
		img* ret=blit_createimg(w,h,base);
		blit_blit(ret,b,0,offset);
		return ret;
	}
	int base=max(a->base,b->base);
	int h=base+max(a->h-a->base,b->h-b->base);
	int w=a->w+b->w+offset*2;
	img* ret=blit_createimg(w,h,base);
	blit_blit(ret,a,0,base-a->base);
	blit_blit(ret,b,a->w+offset,base-b->base);
	return ret;
}
img* blit_frac(img* a, img* b){
	img* ret=blit_createimg(max(a->w,b->w)+2,a->h+b->h+2,a->h-6);
	blit_blit(ret,a,(ret->w-a->w)/2,0);
	blit_blit(ret,b,(ret->w-b->w)/2,a->h+2);
	int i;
	for(i=0;i<ret->w;i++)ret->data[a->h-1][i]=0xff;
	return ret;
}
img* blit_frac_f(img* a, img* b){
	img* ret=blit_frac(a,b);
	blit_freeimg(a,b,0);
	return ret;
}
img* blit_power(img* a, img* b){
	img* ret=blit_createimg(a->w+b->w,a->h+b->h-4,b->h-4+a->base);
	blit_blit(ret,a,0,b->h-4);
	blit_blit(ret,b,a->w,0);
	return ret;
}
img* blit_power_f(img* a, img* b){
	img *t=blit_power(a,b);
	blit_freeimg(a,b,0);
	return t;
}
img* blit_bracket_f(img* a, bool bigfont){
	return blit_bracket_f(a,bigfont,0);
}
img* blit_bracket_f(img* a, bool bigfont, bool norbraket){
	int h=a->h;int w=bigfont?5:4;
	img* br=blit_createimg(w,h,0);
	img* t=font_gen('(',bigfont);
	int i,j;
	if(bigfont){
		for(i=0;i<7;i++)for(j=0;j<5;j++){
			br->data[i][j]=t->data[i][j];
			br->data[h-2-i][j]=t->data[t->h-2-i][j];
		}
		for(i=7;i<h-8;i++){
			br->data[i][0]=0x55;
			br->data[i][1]=0xff;
		}
	}else{
		for(i=0;i<4;i++)for(j=0;j<4;j++){
			br->data[i][j]=t->data[i][j];
			br->data[h-2-i][j]=t->data[t->h-2-i][j];
		}
		for(i=4;i<h-5;i++){
			br->data[i][0]=0x55;
			br->data[i][1]=0xaa;
		}
	}
	blit_freeimg(t);
	img *ret;
	if(norbraket)ret=blit_createimg(a->w+br->w,h,a->base);
	else ret=blit_createimg(a->w+br->w*2,h,a->base);
	blit_blit(ret,br,0,0);
	blit_blit(ret,a,br->w,0);
	if(norbraket)return ret;
	for(i=0;i<br->h;i++){
		for(j=0;j<br->w/2;j++){
			int a=br->data[i][j];
			br->data[i][j]=br->data[i][br->w-1-j];
			br->data[i][br->w-1-j]=a;
		}
	}
	blit_blit(ret,br,br->w+a->w,0);
	blit_freeimg(br);
	blit_freeimg(a);
	return ret;
}
