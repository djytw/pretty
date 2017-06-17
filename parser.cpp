#include "pretty.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
img* parse_int(int start, int end, bool bigfont);
int parser_test(){
	print(5,"parser_test","Started. Enter a expression and cursor position.");
	cursorimg=0;
	scanf("%s%d",str,&posi);
	int l=strlen(str);
	img* ans=parse_int(0,l,1);
	if(cursorimg){
		debug(4,"CURSOR","X:%d Y:%d H:%d\tfinal:%p curimg:%p",cursorx,cursory,cursorh,ans,cursorimg);
		int i;
		for(i=0;i<cursorh;i++){
			ans->data[cursory+i][cursorx]=0x88;
		}
	}
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
		default:return CCONST;
	}
}
img* cursorimg=0;
int cursorx,cursory,cursorh;
img* parse_int(int start, int end, bool bigfont){
	int i,j,l,count,it;
	img *final,*buf, *t, *_t;
	final=blit_createimg(0,0);//TODO -- malloc(0)??
	if(start==end)return final;
	buf=blit_createimg(0,0);
	l=end-start;
	char* s=(char*)malloc((l+1)*sizeof(char));
	strncpy(s,&str[start],l);
	s[l]=0;
	debug(3,"PARSE","Parse: str=\e[32m\e[1m%s\e[0m,l=%d, bigfont=%d",s,l,bigfont);
	free(s);
	/****************************************************************************
		case const/num , put it to buf
		case +-/* , put buf to final, then put OPER to final
		case ^ , put power(buf,parse) to final
		case []/[] , put frac(parse1,parse2) to buf

		fraction: []/[]
		power: ^[]

		after all process, change all ^ to ** and [] to ()
	****************************************************************************/
	for(i=start;i<end;i++){
		if(i==posi){
			cursorx=buf->w;
			cursory=buf->base;
			cursorh=bigfont?12:9;
			cursorimg=buf;
			debug(2,"CURSOR","Cursor generated. Initial position: x:%d y:%d h:%d curimg=%p(w%d,h%d,b%d)",cursorx,cursory,cursorh,cursorimg,cursorimg->w,cursorimg->h,cursorimg->base);
		}
		switch(ttype(str[i])){
		case CCONST:
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
			t=parse_int(i+1,j,0);
			final=blit_con_f(final,blit_power_f(buf,t));
			buf=blit_createimg(0,0);
			i=j;
			break;
		case CBRAC:
			// must be []/[] now, and buf=nil?
			count=0;
			it=i;
			for(j=i;;j++){
				if(str[j]=='[')count++;
				if(str[j]==']')count--;
				if(!count)break;
			}//numerator
			t=parse_int(i+1,j,bigfont);
			i=j+2;
			for(j=i;;j++){
				if(str[j]=='[')count++;
				if(str[j]==']')count--;
				if(!count)break;
			}//denominator
			_t=parse_int(i+1,j,bigfont);
			buf=blit_con_f(buf,blit_frac_f(t,_t));
			if(posi==it){
				cursory=buf->base;
			}
			i=j;
			break;
		case BRACKET:
			count=0;it=0;
			//may not paired
			for(j=i;j<end;j++){
				if(str[j]=='[')it++;
				else if(str[j]==']')it--;
				else if(it==0&&str[j]=='(')count++;
				else if(it==0&&str[j]==')')count--;
				if(!count)break;
			}
			// count!=0 -> not paired
			// AND j=i+1 no contents
			if(j==i+1){
				buf=blit_con_f(buf,font_gen(str[i],bigfont));
				break;
			}
			t=parse_int(i+1,j,bigfont);
			if(count)buf=blit_con_f(buf,blit_bracket_f(t,bigfont,1));
			else buf=blit_con_f(buf,blit_bracket_f(t,bigfont));
			i=j;
			break;
		}
	}
	if(i==posi){
		cursorx=buf->w;
		cursory=buf->base;
		cursorh=bigfont?12:9;
		cursorimg=buf;
		debug(2,"CURSOR","Cursor generated. Initial position: x:%d y:%d curimg=%p",cursorx,cursory,cursorimg);
	}
	final=blit_con_f(final,buf);
	return final;
}
img* parse(int start, int end, bool bigfont){
	img* ret;
	ret=parse_int(start,end,bigfont);
	/*img* ans=blit_createimg(ret->w,ret->h+3,ret->base+3);
	blit_blit(ans,ret,0,3);
	blit_freeimg(ret);
	return ans;*/
	return ret;
}
