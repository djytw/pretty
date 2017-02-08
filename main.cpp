#include "pretty.h"

char str[256];
int posi;
int main(int argc, char** argv){
   print(6,"START","DJYTW Pretty (Parser Version 0.2)");
   if(argc>1&&argv[1][0]=='d')while(1)parser_test();
   else gui();
   return 0;
}
