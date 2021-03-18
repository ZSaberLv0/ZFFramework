/**
 * @file ZFCoreStaticRegister.h
 * @brief static register util
 */
#ifndef _ZFI_ZFCoreStaticRegister_h_
#define _ZFI_ZFCoreStaticRegister_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreUtilMacro.h" // for ZFM_TOSTRING

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief static register step that ensured won't be stripped by compiler
 *
 * usage:
 * @code
 *   // in cpp files only
 *   ZF_STATIC_REGISTER_INIT(YourName)
 *   {
 *       // your register code
 *   }
 *   ZF_STATIC_REGISTER_DESTROY(YourName) // destroy step is optional
 *   {
 *       // your unregister code
 *   }
 *   ZF_STATIC_REGISTER_END(YourName)
 * @endcode
 * @note for different register step, the order is not ensured
 * @note for internal use only, for app level,
 *   use #ZF_GLOBAL_INITIALIZER_INIT or #ZF_STATIC_INITIALIZER_INIT instead
 *
 *
 * ADVANCED:\n
 * we have these type of initializer:
 * -  #ZF_STATIC_REGISTER_INIT :
 *   typically used to perform static init by macro expansion declared in header file,
 *   no public access available
 * -  #ZF_STATIC_INITIALIZER_INIT :
 *   typically used to hold internal impl data used by #ZF_STATIC_REGISTER_INIT in source file,
 *   can be accessed even if not initialized
 * -  #ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL :
 *   typically used by app or anything that require additional code to run
 */
#define ZF_STATIC_REGISTER_INIT(Name) \
    /** @cond ZFPrivateDoc */ \
    zfclassNotPOD _ZFP_SR_##Name \
    { \
    protected: \
        typedef _ZFP_SR_##Name zfself; \
    public: \
        _ZFP_SR_##Name(void)
/** @brief see ZF_STATIC_REGISTER_INIT */
#define ZF_STATIC_REGISTER_DESTROY(Name) \
        ~_ZFP_SR_##Name(void)
/** @brief see ZF_STATIC_REGISTER_INIT */
#define ZF_STATIC_REGISTER_END(Name) \
    }; \
    static _ZFP_SR_##Name _ZFP_SRI_##Name; \
    /** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreStaticRegister_h_

