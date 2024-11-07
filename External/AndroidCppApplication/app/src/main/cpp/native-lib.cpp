#include <jni.h>
#include <android/log.h>
#include <string>
#include <dlfcn.h>
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "androidluaapp::", __VA_ARGS__))

#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, "androidluaapp::", __VA_ARGS__))



extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidcppapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


void RunMono();

//extern "C"
//JNIEXPORT void JNICALL
//Java_com_example_androidcppapplication_MainActivity_loadMono(JNIEnv *env, jobject thiz) {
//
//    LOGI("[Mono] %s","load mono");
//
////    void* handle = dlopen("libmonosgen-2.0.so", RTLD_LAZY);
////    if (!handle) {
////        // 错误处理
////        return;
////    }
////
////
////    handle = dlopen("libSystem.Native.so", RTLD_LAZY);
////    if (!handle) {
////        // 错误处理
////        return;
////    }
////
////
////    handle = dlopen("libSystem.IO.Compression.Native.so", RTLD_LAZY);
////    if (!handle) {
////        // 错误处理
////        return;
////    }
////
////
////
////    handle = dlopen("libSystem.Security.Cryptography.Native.Android.so", RTLD_LAZY);
////    if (!handle) {
////        // 错误处理
////        return;
////    }
////
////
////
////    handle = dlopen("libmono-component-hot_reload.so", RTLD_LAZY);
////    if (!handle) {
////        // 错误处理
////        return;
////    }
////
////
////    handle = dlopen("libmono-component-debugger.so", RTLD_LAZY);
////    if (!handle) {
////        // 错误处理
////        return;
////    }
////    handle = dlopen("libmono-component-diagnostics_tracing.so", RTLD_LAZY);
////    if (!handle) {
////        // 错误处理
////        return;
////    }
//
//
//    RunMono();
//}


#include "mono/utils/mono-publib.h"
#include <mono/jit/jit.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/appdomain.h>

struct invoke_names {
    char *assm_name;
    char *name_space;
    char *name;
    char *meth_name;
};



void RunMono(const char*path){
    LOGI("[Mono] %s","run mono");

    mono_jit_init("");
    MonoDomain *domain = mono_domain_get ();
    if (!domain){
        LOGE("[Mono] Error mono_domain_get");
    }

    char pathBuffer[512];
    snprintf(pathBuffer, sizeof(pathBuffer), "%s/%s", path, "/MyHello.dll");

    LOGI("[Mono] %s","get domain success");
    invoke_names *names=new invoke_names();
    names->assm_name = pathBuffer;
    names->name_space = "MyHello";
    names->name = "Class1";
    names->meth_name = "Hello";


    MonoAssembly *assm = mono_domain_assembly_open (domain, names->assm_name);
    if (assm){
        LOGI("[Mono] %s","get assm success");
    }else{
        LOGE("[Mono] Error mono_domain_assembly_open");
    }
    MonoImage *image = mono_assembly_get_image (assm);
    if (image){
        LOGI("[Mono] %s","get image success");
    }

    MonoClass *klass = mono_class_from_name (image, names->name_space, names->name);
    if (klass){
        LOGI("[Mono] %s","get class success");
    }
    MonoMethod *method = mono_class_get_method_from_name (klass, names->meth_name, -1);
    if (method){
        LOGI("[Mono] %s","get method success");
    }
    MonoObject *args[] = {NULL, };

    mono_runtime_invoke (method, NULL, (void**)args, NULL);
    LOGI("[Mono] %s","invoke success");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidcppapplication_MainActivity_loadMono(JNIEnv *env, jobject thiz,
                                                             jstring assembly_path) {

    const char* path = env->GetStringUTFChars(assembly_path, nullptr);
    LOGI("[Mono] load mono at path %s",path);
    mono_set_assemblies_path(path);
    RunMono(path);
}