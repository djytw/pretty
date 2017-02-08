#include "keyboard.h"
#include <ctype.h>
#include <SDL2/SDL_keycode.h>

bool shift,alpha,alock=0;
void insert(char c){
	if(str[posi]=='@')
		str[posi]=c;
	else{
		int i;
		str[strlen(str)+1]=0;
		for(i=strlen(str);i>posi;i--){
			str[i]=str[i-1];
		}
		str[posi]=c;
	}
	posi++;
}
void insert(const char* s){
	int l=strlen(s);
	if(str[posi]=='@')l--;
	int i;
	str[strlen(str)+l]=0;
	for(i=strlen(str)-1;i>=posi;i--){
		str[i+l]=str[i];
	}
	l=strlen(s);
	for(i=0;i<l;i++){
		str[posi+i]=s[i];
	}
	posi+=l;
}
void pos_shift(int pos){
	posi+=pos;
}
