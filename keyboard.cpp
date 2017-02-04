#include "keyboard.h"
char str[256];
bool shift,alpha,alock=0;
int pos;
void insert(int posi, char c){
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
}
void insert(int posi, char* s){
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
}
void key(int keycode){
	
}
/*int main(){
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
			insert(pos,'1');
			break;
			case KEY_2:
			insert(pos,'2');
			break;
			case KEY_3:
			insert(pos,'3');
			break;
			case KEY_4:
			insert(pos,'4');
			break;
			case KEY_5:
			insert(pos,'5');
			break;
			case KEY_6:
			insert(pos,'6');
			break;
			case KEY_7:
			insert(pos,'7');
			break;
			case KEY_8:
			insert(pos,'8');
			break;
			case KEY_9:
			insert(pos,'9');
			break;
		}
		parser_test();
	}
}
*/
