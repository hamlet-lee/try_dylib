#include<stdlib.h>
#include<string.h>
#include<string>
#include<iostream>
#include <dlfcn.h>
#include<map>
using namespace std;
typedef char* (*FN_PROCESS)(const char*, const char*);
typedef void* (*FN_DELETER)(const char*);

struct LibStruct {
  void* handle;
  FN_PROCESS process;
  FN_DELETER deleter;
  LibStruct (void* handle) {
    this->handle = handle;
    this->process = (FN_PROCESS)dlsym(handle, (char*)"process");
    this->deleter = (FN_DELETER)dlsym(handle, (char*)"deleter");
  }
};

map<string,LibStruct*> lib_map;
LibStruct* getLibStruct(char* lib_name) {
  LibStruct* old = lib_map[lib_name];
  if (old) {
    return old;
  }

  void* handle = NULL;  
  if((handle = dlopen(lib_name, RTLD_NOW)) == NULL) {
        cout << "dlopen" << dlerror();
        exit(-1);
  }
  LibStruct* libStruct = new LibStruct(handle);
  lib_map[lib_name] = libStruct;
  return libStruct;
}



extern "C" {
char* caller(char* name, char* arg) {
  char* buf = new char[1000];
  buf[0] = 0;
  strcat(buf, name);
  strcat(buf, " ");
  strcat(buf, arg);
  return buf; 
}

const char* call(char* libName, char* arg, char* constantArg) {
  LibStruct* libStruct = getLibStruct(libName);
  return libStruct->process(arg, constantArg);
}

void drop(char* libName, const char* str) {
  LibStruct* libStruct = getLibStruct(libName);
  libStruct->deleter(str);
}
}
