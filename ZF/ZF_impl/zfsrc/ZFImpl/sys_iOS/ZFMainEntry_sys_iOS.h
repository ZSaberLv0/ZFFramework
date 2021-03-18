#ifndef _ZFI_ZFMainEntry_sys_iOS_h_
#define _ZFI_ZFMainEntry_sys_iOS_h_

#include "ZFCore.h"
#include "ZFImpl_sys_iOS_ZF_impl.h"

/*
 * main entry steps:
 * -# ensure your UIApplication.keyWindow has been setup properly
 *   during application:didFinishLaunchingWithOptions:
 */
#if ZF_ENV_sys_iOS

#import <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
extern ZF_ENV_EXPORT UIApplication *ZFImpl_sys_iOS_application(void);
extern ZF_ENV_EXPORT UIWindow *ZFImpl_sys_iOS_rootWindow(void);

ZF_NAMESPACE_GLOBAL_END

#endif
#endif // #ifndef _ZFI_ZFMainEntry_sys_iOS_h_

