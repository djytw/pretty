#include "keyboard.h"
#include <ctype.h>
#include <SDL2/SDL_keycode.h>

bool shift,alpha,alock=0;
void backspace_pow();
void backspace_frac();
void backspace_char();
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
void backspace(){
	if(!posi)return;
	if(str[posi-1]==']'){pos_shift_left();}
	else if(str[posi-1]=='['){
		if(posi>1&&str[posi-2]=='^')backspace_pow();
		else if(posi>1&&str[posi-2]=='/')pos_shift_left();
		else backspace_frac();
	}
	else backspace_char();
}
void backspace_pow(){
	// 123^[456] => 123456  posi:5 => 3
	int i;
	posi-=2;
	for(i=posi;;i++){
		if(str[i+2]==']')break;
		str[i]=str[i+2];
	}
	for(;i<=strlen(str)-2;i++){
		str[i]=str[i+3];
	}
}
void backspace_frac(){
	// [123]/[456] => 123456 posi: 1=>0
	int i;
	posi--;
	for(i=posi;;i++){
		if(str[i+1]==']')break;
		str[i]=str[i+1];
	}
	for(;;i++){
		if(str[i+4]==']')break;
		str[i]=str[i+4];
	}
	for(;i<=strlen(str)-4;i++){
		str[i]=str[i+5];
	}
}
void backspace_char(){
	int i;
	for(i=posi;i<=strlen(str);i++){
		str[i-1]=str[i];
	}
	posi--;
}
void pos_shift(int pos){
	//unsafe pos shift
	posi+=pos;
}
void pos_shift_right(){
	switch(str[posi]){//2^[2] // [2]/[2]
		case '@':posi++;pos_shift_right();posi--;break;
		case '^':posi++;break;
		case ']':if(str[posi+1]=='/'&&str[posi+2]=='[')posi+=2;break;
	}
	posi++;
	if(posi>strlen(str))posi=strlen(str);
}
void pos_shift_left(){
	posi--;
	if(posi<0)posi=0;
	if(str[posi]=='['&&posi>0){
		switch(str[posi-1]){
			case '^':posi--;break;
			case '/':if(posi>1&&str[posi-2]==']')posi-=2;break;
		}
	}
	if(str[posi-1]=='@')posi--;
}
