#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>

using namespace std;
int main () {
    typedef char* (*FN)(char*, char*);
    void *handle = NULL;
    FN fn;

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

    char* ret = (*fn)((char*)"foo",(char*)"bar");
    cout << ret << endl;
 
    return 0;
}
