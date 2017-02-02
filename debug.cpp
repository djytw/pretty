#include <stdio.h>
#include <string.h>
int DEBUG=7;
void setdebug(int a){
   DEBUG=a;
}
void debugtitle(int level, const char *title){
   switch(level){
      case 0:printf("\e[0m[%s] ",title);break;
      case 1:printf("\e[34m[%s]\e[0m ",title);break;
      case 2:printf("\e[32m[%s]\e[0m ",title);break;
      case 3:printf("\e[33m[%s]\e[0m ",title);break;
      case 4:printf("\e[31m[%s]\e[0m ",title);break;
      case 5:printf("\e[42m[%s] ",title);break;
      case 6:printf("\e[43m[%s] ",title);break;
      case 7:printf("\e[41m[%s] ",title);break;
   }
}
