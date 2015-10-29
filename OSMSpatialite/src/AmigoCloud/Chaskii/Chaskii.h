#ifndef CHASKII_DEF
#define CHASKII_DEF 1

namespace ii{

static const unsigned int UnknownDimension = 0xFFFFFFFF;

typedef unsigned int GfxContext;

void frame();

class Object {
    public:
        Object() {}
        virtual ~Object() {}

        virtual const char * className() { return "Object"; }
};

}

#ifdef __ANDROID__
#include <stdio.h>
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_INFO, "log", __VA_ARGS__)
#else
#include <stdio.h>
#endif

#endif
