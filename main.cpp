#include "blit.h"

char str[256];

int main(int argc, char** argv){
   if(argc>1&&argv[1][0]=='d')while(1)parser_test();
   else gui();
   return 0;
}
