#ifndef _ZFI_ZFMainEntry_sys_Android_h_
#define _ZFI_ZFMainEntry_sys_Android_h_

#include "ZFCore.h"
#include "ZFImpl_sys_Android_ZF_impl.h"

/*
 * main entry steps:
 * -# prepare your loader Activity
 * -# load all so files in your Activity as static init step
 *   @code
 *     public class Loader extends Activity {
 *         static {
 *             System.loadLibrary("ZF_lib_name");
 *         }
 *     }
 *   @endcode
 * -# start ZFMainEntry as a Activity
 * -# finish your loader Activity
 *
 * see ZFMainEntry.java for more info
 */
#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// return android.content.Context
extern ZF_ENV_EXPORT JNIObjectHolder ZFImpl_sys_Android_appContext(void);
// return android.content.res.AssetManager
extern ZF_ENV_EXPORT JNIObjectHolder ZFImpl_sys_Android_assetManager(void);
// return android.app.Activity
extern ZF_ENV_EXPORT JNIObjectHolder ZFImpl_sys_Android_mainEntryActivity(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFMainEntry_sys_Android_h_

