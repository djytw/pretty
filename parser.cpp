#include <stdio.h>
#include "blit.h"
#include <ctype.h>
#include <string.h>
int main(){
	//char str[]="x - x**3/6 + O(x**5)";

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
	return CONST;
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
			i++;
			if(str[i]=='('){
				int j,count=0;
				for(j=i;;j++){
					if(str[j]=='(')count++;
					if(str[j]==')')count--;
					if(!count)break;
				}
				char *s;
				s=(char*)malloc(sizeof(char)*(j-i));
				strncpy(s,&str[i+1],j-i-1);
				s[j-i-1]=0;
				t=parse(s);	
				tn=blit_frac(tmp,t);
				blit_freeimg(t,tmp,0);
				tmp=tn;
				i=j;
			}else{
				int j;
				for(j=i;;j++)
					if(ttype(str[j])!=CONST||ttype(str[j])!=NUMBER)
						break;
					char *s;
					s=(char*)malloc(sizeof(char)*(j-i+1));
					strncpy(s,&str[i],j-i);
					s[j-i]=0;
					t=parse(s);
					tn=blit_frac(tmp,t);
					blit_freeimg(t,tmp,0);
					tmp=tn;
					i=j-1;				
			}
			break;
			case OPERM:
			if(str[i+1]=='*'){
				//power
				i+=2;
				if(str[i]=='('){
					int j,count=0;
					for(j=i;;j++){
						if(str[j]=='(')count++;
						if(str[j]==')')count--;
						if(!count)break;
					}
					char *s;
					s=(char*)malloc(sizeof(char)*(j-i));
					strncpy(s,&str[i+1],j-i-1);
					s[j-i-1]=0;
					t=parse(s);		
					tn=blit_power(tmp,t);
					blit_freeimg(t,tmp,0);
					tmp=tn;	
					i=j;	
				}
				else{
					int j;
					for(j=i;;j++)
						if(ttype(str[j])!=CONST||ttype(str[j])!=NUMBER)
							break;
					char *s;
					s=(char*)malloc(sizeof(char)*(j-i+1));
					strncpy(s,&str[i],j-i);
					s[j-i]=0;
					t=parse(s);
					tn=blit_power(tmp,t);
					blit_freeimg(t,tmp,0);
					tmp=tn;
					i=j-1;
				}
			}else{
				//mul
				t=blit_gen(5,9);
				tn=blit_con(tmp,t);
				blit_freeimg(t,tmp,0);
				tmp=tn;
			}
			break;
		}
	}
	tn=blit_con(final,tmp);
	blit_freeimg(final,tmp,0);
	final=tn;
	return final;
}