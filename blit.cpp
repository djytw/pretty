#include "blit.h"

img* blit_createimg(int w, int h){//TODO -- handle 0,0 input for malloc(0) security issues
	return blit_createimg(w, h, 0);
}
img* blit_createimg(int w, int h, int base){
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
		printf("freed\n");
		blit_freeimg(t);
		t=va_arg(ap,img*);
	}
	blit_freeimg(a);
	blit_freeimg(b);
}
void blit_cdraw(img* map){
	int i,j;
	if(map->w==0||map->h==0){
		printf("---no data---\n");
	}
	for(i=0;i<map->h;i++){
		for(j=0;j<map->w;j++){
			map->data[i][j]==1?printf("+"):printf(" ");
		}
		printf("\n");
	}
}
void blit_blit(img* dst, img* src, int x, int y){
	int i,j;
	for(i=0;i<src->w&&i+x<dst->w;i++)
		for(j=0;j<src->h&&j+y<dst->h;j++)
			dst->data[y+j][x+i]=src->data[j][i];
	if(cursorimg==src){
		cursorx+=x;
		cursory+=y;
		cursorimg=dst;
	}
}
img* blit_con(img* a, img* b){
	return blit_con(a,b,0);
}
img* blit_con(img* a, img* b, int offset){
	int base=max(a->base,b->base);
	int h=base+max(a->h-a->base,b->h-b->base);
	int w=a->w+b->w+offset;
	img* ret=blit_createimg(w,h,base);
	blit_blit(ret,a,0,base-a->base);
	blit_blit(ret,b,a->w+offset,base-b->base);
	return ret;
}
img* blit_frac(img* a, img* b){
	img* ret=blit_createimg(max(a->w,b->w)+2,a->h+b->h+2,a->h-6);
	blit_blit(ret,a,(ret->w-a->w)/2,0);
	blit_blit(ret,b,(ret->w-b->w)/2,a->h+3);
	int i;
	for(i=0;i<ret->w;i++)ret->data[a->h][i]=0xff;
	return ret;
}
img* blit_power(img* a, img* b){
	img* ret=blit_createimg(a->w+b->w,a->h+b->h-4,b->h-4+a->base);
	blit_blit(ret,a,0,b->h-4);
	blit_blit(ret,b,a->w,0);
	return ret;
}
