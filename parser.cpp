#include <stdio.h>
#include "blit.h"
#include <ctype.h>
#include <string.h>
int main(){
	char str[]="x - x**3/6 + O(x**5)";

	return 0;
}
#define NUMBER 0
#define CONST 1
#define OPERP 2
#define OPERM 3
#define OPERD 4

int ttype(char c){
	if(isdigit(c))return NUMBER;
	if(c=='+'||c=='-')return OPERP;
	if(c=='*')return OPERM;
	if(c=='/')return OPERD;
	return -1;
}
img* parse(char* str){
	int i,l;
	img *final,*tmp, *tn, *t;
	final=blit_createimg(1,1);
	tmp=blit_createimg(1,1);
	l=strlen(str);
	for(i=0;i<l;i++){
		switch(ttype(str[i])){
			case NUMBER:
			case CONST:
			t=blit_gen(5,9);
			tn=blit_con(tmp,t);
			blit_freeimg(t,tmp,0);
			tmp=tn;
			break;
			case OPERP:
			tn=blit_con(final,tmp);
			blit_freeimg(final,tmp,0);
			t=blit_gen(5,9);
			final=blit_con(tn,t);
			blit_freeimg(tn,t,0);
			tmp=blit_createimg(1,1);	
			break;
			case OPERD:
			break;
		}
	}
	tn=blit_con(final,tmp);
	blit_freeimg(final,tmp,0);
	final=tn;
	return final;
}