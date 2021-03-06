#include "pretty.h"

char str[256];
unsigned int posi;
int main(int argc, char** argv){

#ifdef WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 4;
    SetConsoleMode(hOut, dwMode);
#endif

   setdebug(4);
	print(5,"START"," ");
   print(5,"START","   --  DJYTW PrettyMath 0.3  --  ");
   print(5,"START","      (c) 2015-2017 djytw     ");
	print(5,"START"," ");
   if(argc>2)setdebug(argv[2][0]-'0');
	if(argc>1){
		switch (argv[1][0]) {
			case 'd':
		      while(1)parser_test();break;
			case 'g':
				gui();break;
			case 'u':
				UI_test();break;
			default:
				print(7,"ERROR","Unrecognized command: %s",argv[1]);
		}
	}else{
		gui();
	}

   return 0;
}
