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
		for(i=strlen(str);i>(int)posi;i--){
			str[i]=str[i-1];
		}
		str[posi]=c;
	}
	posi++;
}
void insert(const char* s){
	unsigned l=strlen(s);
	int i;
	if(str[posi]=='@')l--;
	str[strlen(str)+l]=0;
	for(i=strlen(str)-1;i>=(int)posi;i--){
		str[i+l]=str[i];
	}
	l=strlen(s);
	for(i=0;i<(int)l;i++){
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
	}else if(str[posi]==']'&&posi>1&&str[posi-2]=='['){
		str[posi-1]='@';
		posi--;
	}else if(str[posi]=='^'&&posi>1&&(!isdigit(str[posi-2]))&&(!isalpha(str[posi-2]))){
		str[posi-1]='@';
		posi--;
	}else if(str[posi]=='^'&&posi==1){
		str[posi-1]='@';
		posi--;
	}else if(str[posi-1]=='#')return;
	else backspace_char();
}
void backspace_pow(){
	// 1) 123^[456] => 123456  posi:5 => 3  len-=3
	// 2) 123^[@] => 123 		posi:5 => 3  len-=4
	// 3) @^[@]2 => 2				posi:3 => 0  len-=5
	int i;int l=strlen(str);
	if(str[posi]=='@'){
		//mode 2
		posi-=2;
		for(i=posi;i<l-3;i++){
			str[i]=str[i+4];
		}
	}else{
		posi-=2;
		for(i=posi;;i++){
			if(str[i+2]==']')break;
			str[i]=str[i+2];
		}
		for(;i<l-2;i++){
			str[i]=str[i+3];
		}
	}
	if(posi>0 && str[posi-1]=='@')backspace_char();
	if(posi>0 && str[posi-1]=='[' && ( str[posi]==']' || str[posi]=='^' ) ){
		insert('@');
		posi--;
	}
}
void backspace_frac(){
	// 1) [123]/[456] => 123456   posi: 1=>0	len-=5
	// 2) 1[@]/[@]2 => 12         posi: 2=>1	len-=7
	// 3) [@]/[222] => 222        posi: 1=>0  len-=6
	int i;int l=strlen(str);
	if(str[posi]=='@'){
		if(str[posi+4]=='@'){
			posi--;
			for(i=posi;i<l-6;i++){
				str[i]=str[i+7];
			}
		}else{
			posi--;
			for(i=posi;;i++){
				if(str[i+5]==']')break;
				str[i]=str[i+5];
			}
			for(;i<l-5;i++){
				str[i]=str[i+6];
			}
		}
	}else{
		posi--;
		for(i=posi;;i++){
			if(str[i+1]==']')break;
			str[i]=str[i+1];
		}
		for(;;i++){
			if(str[i+4]==']')break;
			str[i]=str[i+4];
		}
		for(;i<l-4;i++){
			str[i]=str[i+5];
		}
	}
	if(posi>0 && str[posi-1]=='[' && ( str[posi]==']' || str[posi]=='^' ) ){
		insert('@');
		posi--;
	}
}
void backspace_char(){
	unsigned int i;
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
		case '#':return;
	}
	posi++;
	if(posi>strlen(str))posi=strlen(str);
}
void pos_shift_left(){
	if(posi>0)posi--;
	else return;
	if(str[posi]=='['&&posi>0){
		switch(str[posi-1]){
			case '^':posi--;break;
			case '/':if(posi>1&&str[posi-2]==']')posi-=2;break;
		}
	}
	else if(str[posi]=='#')
			posi++;
	if(posi>0&&str[posi-1]=='@')posi--;
}
