#include "keyboard.h"
#include <ctype.h>
#include <SDL2/SDL_keycode.h>

bool shift,alpha,alock=0;
void insert(char c){
	if(str[posi+1]=='@'){
		str[posi]=c;
		str[posi+1]='|';
	}else{
		int i;
		for(i=strlen(str);i>posi;i--){
			str[i]=str[i-1];
		}
		str[posi]=c;
	}
	posi++;
}
void insert(const char* s){
	int l=strlen(s);
	if(str[posi+1]=='@')l--;
	int i;
	for(i=strlen(str)-1;i>posi;i--){
		str[i+l]=str[i];
	}
	l=strlen(s);
	str[posi+l]='|';
	for(i=0;i<l;i++){
		str[posi+i]=s[i];
	}
	posi+=l;
}
void pos_shift(int pos){
	if(!pos)return;
	int i;
	if(pos>0){
		for(i=0;i<pos;i++){
			str[posi]=str[posi+1];
			posi++;
		}
		str[posi]='|';
	}
	else{
		for(i=0;i<-pos;i++){
			str[posi]=str[posi-1];
			posi--;
		}
		str[posi]='|';
	}
}
void key(int keycode){
	if(keycode<0x100&&(isdigit(keycode)||islower(keycode))){
			insert(keycode);
	}else{
		switch(keycode){
			case SDLK_KP_MULTIPLY:
			if(posi&&str[posi-1]=='*'){
				insert("*(@)");
				pos_shift(-2);
			}
			else insert('*');
			break;
			case SDLK_KP_DIVIDE:
			insert("(@/@)");
			pos_shift(-4);
			break;
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
	debug(4,"INPUT","str=%s",str);
}
