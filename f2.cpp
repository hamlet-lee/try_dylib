#include<stdlib.h>
#include<string.h>
#include<string>
#include<iostream>
#include<map>
using namespace std;
extern "C" {

char* process(char* arg, char* constantArg) {
  char* buf = new char[1000];
  buf[0] = 0;
  strcat(buf, constantArg);
  strcat(buf, " ");
  strcat(buf, arg);
  return buf; 
}

void deleter(const char* str) {
  delete[] str;
}

}
