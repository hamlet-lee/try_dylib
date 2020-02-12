#include<stdlib.h>
#include<string.h>
#include<string>
#include<iostream>
#include <dlfcn.h>
#include<map>
#include "App.h"
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

struct StringMeta {
  char* bytes;
  jbyteArray stringJbytes;
  JNIEnv* env;
  jsize length;
  StringMeta(JNIEnv *env, jstring str){
    // https://stackoverflow.com/questions/32205446/getting-true-utf-8-characters-in-java-jni
    const jclass stringClass = env->GetObjectClass(str);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");

    const jstring charsetName = env->NewStringUTF("UTF-8");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(str, getBytes, charsetName);
    env->DeleteLocalRef(charsetName);

    const jsize length = env->GetArrayLength(stringJbytes);
    const jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL); 
    this->env = env;
    this->stringJbytes = stringJbytes; 
    this->bytes = (char*)pBytes;
    this->length = length;
  }

  ~StringMeta() {
    env->ReleaseByteArrayElements(stringJbytes, (jbyte*)bytes, JNI_ABORT); 
    env->DeleteLocalRef(stringJbytes);
  }
};

// https://blog.csdn.net/xlxxcc/article/details/51106721
jstring charTojstring(JNIEnv* env, const char* pat) {
    //定义java String类 strClass
    jclass strClass = (env)->FindClass("Ljava/lang/String;");
    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    //建立byte数组
    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
    //将char* 转换为byte数组
    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
    // 设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (env)->NewStringUTF("UTF-8");
    //将byte数组转换为java String,并输出
    return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
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

// https://blog.csdn.net/u011928958/article/details/103576747
JNIEXPORT jstring JNICALL Java_App_jniCall
  (JNIEnv *env, jclass, jstring libName, jstring arg, jstring constantArg){
  StringMeta metaLibName(env,libName), metaArg(env,arg), metaConstantArg(env, constantArg);
  const char* ret = call(metaLibName.bytes, metaArg.bytes, metaConstantArg.bytes);
  // ...转换成jstring...
  jstring jret = charTojstring(env, ret);
  drop(metaLibName.bytes, ret);
  return jret;
}

}
