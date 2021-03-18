#include "AndroidJNIUtil.h"
#if (defined(ANDROID) || defined(__ANDROID__))

namespace AndroidJNIUtil {

JNIUtil::JNIString &_AndroidLogTagDefault(void)
{
    static JNIUtil::JNIString _s = "JNI log";
    return _s;
}

}
#endif
