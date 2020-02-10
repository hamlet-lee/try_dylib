#include<stdlib.h>
#include<string.h>
extern "C" {
char* caller(char* name, char* arg) {
  char* buf = new char[1000];
  buf[0] = 0;
  strcat(buf, name);
  strcat(buf, " ");
  strcat(buf, arg);
  return buf; 
}
}
