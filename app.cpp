#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>

using namespace std;
int main () {
    typedef char* (*FN)(char*, char*);
    void *handle = NULL;
    FN fn;

    typedef const char* (*FN_CALL) (const char* libName, const char* arg, const char* constArg);
    typedef void (*FN_DROP) (const char* libName, const char* str);
    
    char *myso = (char*)"./caller.so";
 
    if((handle = dlopen(myso, RTLD_NOW)) == NULL) {
        cout << "dlopen" << dlerror();
        exit(-1);
    }
    fn = (FN)dlsym(handle, (char*)"caller");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol superfunctionx: " << dlsym_error << endl;
    }

    if( fn == NULL ) {
        cout << "dlsym fail" << endl;
        exit(-1);
    }

    FN_CALL fnCall = (FN_CALL)dlsym(handle, (char*)"call");
    FN_DROP fnDrop = (FN_DROP)dlsym(handle, (char*)"drop");

    if( fnCall == NULL ) {
        cout << "dlsym 'call' fail" << endl;
        exit(-1);
    }

    if( fnDrop == NULL ) {
        cout << "dlsym 'drop' fail" << endl;
        exit(-1);
    }


    char* ret = (*fn)((char*)"foo",(char*)"bar");
    cout << ret << endl;
 
    while(true) {
      string libName;
      string arg;
      string constantArg;
      
      cout << "libName, arg, constantArg" << endl;
      cin >> libName >> arg >> constantArg;
      cout << "libName: " << libName << endl;
      cout << "arg: " << arg << endl;
      cout << "constantArg: " << constantArg << endl;
      if (libName == "quit") {
          break;
      }
 
      const char* ret = (*fnCall)(libName.c_str(), arg.c_str(), constantArg.c_str());
      cout << "ret: " << ret << endl;
      (*fnDrop)(libName.c_str(), ret);
      cout << "dropped" << endl;
    }
    return 0;
}
