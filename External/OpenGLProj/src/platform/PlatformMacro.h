//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_PLATFORMMACRO_H
#define OPENGLPROJ_PLATFORMMACRO_H
#ifdef __ANDROID__
#include <android/log.h>
    #include <jni.h>
    #define  LOG_TAG                    "game"
    #define  pf_log(...)                __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
    #define  pf_vprint(format, ap)      __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, (format), (ap))
#else
#define pf_log                      printf
#define pf_vprint                   vprintf
#endif

#endif //OPENGLPROJ_PLATFORMMACRO_H
