#include "keyboard.h"
char str[256];
bool shift,alpha,alock=0;
int pos;
int main(){
	char keycode;
	shift=alpha=alock=0;
	str[0]='|';
	str[1]=0;
	while(1){
		scanf("%c",&keycode);
		if(!keycode)break;
		int i;
		for(i=0;i<strlen(str);i++)
		if(str[i]=='|'){
		pos=i;break;
		}
		switch(keycode){
			case KEY_SHIFT:
			shift=!shift;
			alpha=alock=0;
			break;
			case KEY_ALPHA:
			alpha=!(alpha||alock);
			alock=shift=0;
			case KEY_1:
			
		}
		parser_test();
	}
}