#include "pretty.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int parser_test(){
	print(5,"parser_test","Started. Enter a expression.");
	scanf("%s",str);
	int l=strlen(str);
	img* ans=parse(0,l,1);
	if(cursorimg){debug(4,"CURSOR","X:%d Y:%d H:%d\tfinal:%p curimg:%p",cursorx,cursory,cursorh,ans,cursorimg);}
	else{debug(4,"CURSOR","No Cursor in current expression.");}
	gui_draw(ans);
	blit_freeimg(ans);
	return 0;
}

int ttype(char c){
	switch (c) {
		case '+':case '-':case '*':case '/':return OPER;
		case '^':return POWER;
		case '(':return BRACKET;
		case '[':return CBRAC;
		default:return CONST;
	}
}
img* cursorimg=0;
int cursorx,cursory,cursorh;
img* parse(int start, int end, bool bigfont){
	int i,j,l,count;
	img *final,*buf, *t, *_t;
	final=blit_createimg(0,0);//TODO -- malloc(0)??
	if(start==end)return final;
	buf=blit_createimg(0,0);
	l=end-start;
	char* s=(char*)malloc((l+1)*sizeof(char));
	strncpy(s,&str[start],l);
	s[l]=0;
	debug(3,"PARSE","Parse: str=\e[34m\e[1m%s\e[0m,l=%d, bigfont=%d",s,l,bigfont);
	/****************************************************************************
		case const/num , put it to tmp_0
		case /* , put tmp_0 to tmp_1
		case +- , put tmp_0 to tmp_1 ,then put tmp_1 to final
		case ^ , put power(tmp_0,parse) to tmp_1
		case []/[] , put frac(parse1,parse2) to tmp_1

		fraction: []/[]
		power: ^[]

		after all process, change all ^ to ** and [] to ()
	****************************************************************************/
	for(i=start;i<end;i++){
		if(i==posi){
			cursorx=final->w+buf->w;
			cursory=0;
			//cursorh=buf->h;
			cursorh=bigfont?16:12;
			cursorimg=buf;
		}
		switch(ttype(str[i])){
		case CONST:
			buf=blit_con_f(buf,font_gen(str[i],bigfont));
			break;
		case OPER:
			final=blit_con_f(final,buf);
			buf=blit_createimg(0,0);
			final=blit_con_f(final,font_gen(str[i],bigfont),1);
			break;
		case POWER:
			count=0;i++;
			// square brackets was add by system, and must be paired.
			//             and after a power mark, there must be a '['
			for(j=i;;j++){
				if(str[j]=='[')count++;
				if(str[j]==']')count--;
				if(!count)break;
			}
			t=parse(i+1,j,0);
			final=blit_con_f(final,blit_power_f(buf,t));
			buf=blit_createimg(0,0);
			i=j;
			break;
		case CBRAC:
			// must be []/[] now, and buf=nil?
			count=0;
			for(j=i;;j++){
				if(str[j]=='[')count++;
				if(str[j]==']')count--;
				if(!count)break;
			}//numerator
			t=parse(i+1,j,bigfont);
			i=j+2;
			for(j=i;;j++){
				if(str[j]=='[')count++;
				if(str[j]==']')count--;
				if(!count)break;
			}//denominator
			_t=parse(i+1,j,bigfont);
			buf=blit_con_f(buf,blit_frac_f(t,_t));
			i=j;
			break;
		case BRACKET:
			count=0;
			//may not paired
			for(j=i;j<end;j++){
				if(str[j]=='(')count++;
				if(str[j]==')')count--;
				if(!count)break;
			}
			// count!=0 -> not paired
			// AND j=i+1 no contents
			if(j==i+1){
				buf=blit_con_f(buf,font_gen(str[i],bigfont));
				break;
			}
			t=parse(i+1,j,bigfont);
			if(count)buf=blit_con_f(buf,blit_bracket_f(t,bigfont,1));
			else buf=blit_con_f(buf,blit_bracket_f(t,bigfont));
			i=j;
			break;
		}
	}
	final=blit_con_f(final,buf);
	return final;
}
/*
img* _parse(char* str, bool bigfont){
	int i,l;
	img *final,*tmp_0, *tmp_1, *t, *_t;
	final=blit_createimg(0,0);//TODO -- malloc(0)??
	tmp_0=blit_createimg(0,0);
	tmp_1=blit_createimg(0,0);
	l=strlen(str);
	debug(3,"PARSE","parse: str=%s,l=%d, bigfont=%d",str,l,bigfont);
	//case const/num , put it to tmp_0
	//case * , put tmp_0 to tmp_1
	//case +- , put tmp_0 to tmp_1 ,then put tmp_1 to final
	//case / , put tmp_0 to tmp_1, then put frac(tmp1,parse) to tmp_1
	//case ^ , put power(tmp_0,parse) to tmp_1
	for(i=0;i<l;i++){
		switch(ttype(str[i])){
			case CURSOR:
			cursorx=tmp_0->w;
			cursory=0;
			cursorh=tmp_0->h;
			if(cursorh==0)cursorh=bigfont?16:12;
			cursorimg=tmp_0;
			break;
			case NUMBER:
			case CONST:
			t=font_gen(str[i],bigfont);
			_t=blit_con(tmp_0,t);
			blit_freeimg(tmp_0,t,0);
			tmp_0=_t;
			break;
			case OPERP:
			t=blit_con(tmp_1,tmp_0);
			_t=blit_con(final,t);
			blit_freeimg(final,t,tmp_0,tmp_1,0);
			t=font_gen('+',bigfont);
			final=blit_con(_t,t,1);
			blit_freeimg(_t,t,0);
			tmp_0=blit_createimg(0,0);
			tmp_1=blit_createimg(0,0);
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
				t=parse(s,bigfont);
				_t=blit_con(tmp_1,tmp_0);
				blit_freeimg(tmp_1,tmp_0,0);
				tmp_1=blit_frac(_t,t);
				blit_freeimg(t,_t,0);
				tmp_0=blit_createimg(0,0);
				i=j;
			}else{
				int j;
				for(j=i;j<l;j++){
					if(j<l-1&&str[j]=='*'&&str[j+1]=='*'){j++;continue;}
					if(ttype(str[j])!=CONST&&ttype(str[j])!=NUMBER&&ttype(str[j])!=CURSOR)
						break;
				}
				char *s;
				s=(char*)malloc(sizeof(char)*(j-i+1));
				strncpy(s,&str[i],j-i);
				s[j-i]=0;
				t=parse(s,bigfont);
				_t=blit_con(tmp_1,tmp_0);
				blit_freeimg(tmp_1,tmp_0,0);
				tmp_1=blit_frac(_t,t);
				blit_freeimg(t,_t,0);
				tmp_0=blit_createimg(0,0);
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
					t=parse(s,0);
					_t=blit_power(tmp_0,t);
					blit_freeimg(t,tmp_0,0);
					t=blit_con(tmp_1,_t);
					blit_freeimg(tmp_1,_t,0);
					tmp_0=blit_createimg(0,0);
					tmp_1=t;
					i=j;
				}
				else{
					int j;
					for(j=i;j<l;j++)
						if(ttype(str[j])!=CONST&&ttype(str[j])!=NUMBER&&ttype(str[j])!=CURSOR)
							break;
					char *s;
					s=(char*)malloc(sizeof(char)*(j-i+1));
					strncpy(s,&str[i],j-i);
					s[j-i]=0;
					t=parse(s,0);
					_t=blit_power(tmp_0,t);
					blit_freeimg(t,tmp_0,0);
					t=blit_con(tmp_1,_t);
					blit_freeimg(tmp_1,_t,0);
					tmp_0=blit_createimg(0,0);
					tmp_1=t;
					i=j-1;
				}
			}else{
				//mul
				_t=blit_con(tmp_1,tmp_0);
				t=font_gen('*',bigfont);
				tmp_1=blit_con(_t,t,1);
				blit_freeimg(t,_t,tmp_0,0);
				tmp_0=blit_createimg(0,0);
			}
			break;
			case BRACKET:
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
			t=parse(s,bigfont);
			_t=blit_bracket(t,bigfont);
			blit_freeimg(t);
			t=blit_con(tmp_0,_t);
			blit_freeimg(tmp_0,_t,0);
			tmp_0=t;
			i=j;
			break;
		}
	}
	t=blit_con(tmp_1,tmp_0);
	_t=blit_con(final,t);
	blit_freeimg(final,t,tmp_0,tmp_1,0);
	final=_t;
	return final;
}
*/
