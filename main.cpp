#include "pretty.h"

char str[256];
int posi;
int main(int argc, char** argv){

#ifdef WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 4;
    SetConsoleMode(hOut, dwMode);
#endif

   setdebug(4);
   print(6,"START","DJYTW Pretty (Parser Version 0.2)");
   if(argc>2)setdebug(argv[2][0]-'0');
   if(argc>1&&argv[1][0]=='d'){
      while(1)parser_test();
   }
   else gui();
   return 0;
}
