#include "blit.h"
int main(){
	blit_test();
	return 0;
}
void blit_test(){
	img* a = blit_gen(5,9);
	img* b = blit_gen(3,15,2);
	img* frac = blit_frac(a,b);
	img* ans = blit_con(a,frac);
	frac=blit_frac(a,ans);
	ans=blit_con(frac,ans);
	blit_cdraw(ans);
	blit_freeimg(a,b,ans,frac,0);
}
img* blit_createimg(int w, int h){
	return blit_createimg(w, h, 0);
}
img* blit_createimg(int w, int h, int base){
	img* ret=(img*)malloc(sizeof(img));
	ret->w=w;
	ret->h=h;
	ret->data=(bool**)malloc(h*sizeof(bool*));
	int i;
	for(i=0;i<h;i++)
	ret->data[i]=(bool*)calloc(w,sizeof(bool));
	ret->base=base;
	return ret;
}
void blit_freeimg(img* a){
	int i;
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
}
img* blit_gen(int w, int h){
	return blit_gen(w,h,0);
}
img* blit_gen(int w, int h, int base){
	img* ret=(blit_createimg(w,h,base));
	int i;
	for(i=0;i<h;i++){ret->data[i][0]=ret->data[i][w-1]=1;}
	for(i=0;i<w;i++){ret->data[0][i]=ret->data[h-1][i]=1;}
	return ret;
}
img* blit_con(img* a, img* b){
	int base=max(a->base,b->base);
	int h=base+max(a->h-a->base,b->h-b->base);
	int w=a->w+b->w+1;
	img* ret=blit_createimg(w,h,base);
	blit_blit(ret,a,0,base-a->base);
	blit_blit(ret,b,a->w+1,base-b->base);
	return ret;
}
img* blit_frac(img* a, img* b){
	img* ret=blit_createimg(max(a->w,b->w)+2,a->h+b->h+3,a->h-3);//TODO -- base=a->h+1-fontheight/2
	blit_blit(ret,a,(ret->w-a->w)/2,0);
	blit_blit(ret,b,(ret->w-b->w)/2,a->h+3);
	int i;
	for(i=0;i<ret->w;i++)ret->data[a->h+1][i]=1;
	return ret;
}