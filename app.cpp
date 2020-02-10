#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>

using namespace std;
int main () {
    void *handle = NULL;
    char *myso = (char*)"./mylib.so";
 
    if((handle = dlopen(myso, RTLD_NOW)) == NULL) {
        cout << "dlopen" << dlerror();
        exit(-1);
    }
 
    return 0;
}
