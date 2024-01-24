//
// Created by gauravssnl.

#include "jni.h"
#include "hooker.h"
#include "helper.h"
#include "dlfcn.h"

static HookFunType hook_func = nullptr;

jstring (*original)(JNIEnv *env, jobject);

jstring fake(JNIEnv *env, jobject) {
    std::string hello = "Hello from Native Xposed lib:)";
    return env->NewStringUTF(hello.c_str());
}

void on_library_loaded(const char *name, void *handle) {
    // hooks on `libtarget.so`
    if (ends_with(std::string(name), "hellojni.so")) {
        // hook target function
        void *target = dlsym(handle, "Java_com_gauravssnl_hellojni_MainActivity_stringFromJNI");
        hook_func(target, (void *) fake, (void **) &original);
    }
}

// Note : native_init is mandatory;
extern "C" [[gnu::visibility("default")]] [[gnu::used]]
NativeOnModuleLoaded native_init(const NativeAPIEntries *entries) {
    hook_func = entries->hook_func;
    // system hooks; if we need
    hook_func((void *) fopen, (void *) fake_fopen, (void **) &original_fopen);
    return on_library_loaded;
}

jclass (*original_FindClass)(JNIEnv *env, const char *name);

jclass fake_FindClass(JNIEnv *env, const char *name) {
    if (!strcmp(name, "dalvik/system/BaseDexClassLoader"))
        return nullptr;
    return original_FindClass(env, name);
}

// If we need JNI Env hooks
extern "C" [[gnu::visibility("default")]] [[gnu::used]]
jint JNI_OnLoad(JavaVM *jvm, void *) {
    JNIEnv *env = nullptr;
    jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    hook_func((void *) env->functions->FindClass, (void *) fake_FindClass,
              (void **) &original_FindClass);
    return JNI_VERSION_1_6;
}


FILE *fake_fopen(const char *filename, const char *mode) {
    if (strstr(filename, "banned")) return nullptr;
    return original_fopen(filename, mode);
}