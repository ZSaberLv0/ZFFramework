/**
 * @file ZFIOBufferByCacheFile.h
 * @brief common impl for #ZFIOBuffer
 */

#ifndef _ZFI_ZFIOBufferByCacheFile_h_
#define _ZFI_ZFIOBufferByCacheFile_h_

#include "ZFIOBuffer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief #ZFIOBuffer using cache file */
zfclass ZF_ENV_EXPORT ZFIOBufferByCacheFile : zfextends ZFIOBuffer
{
    ZFOBJECT_DECLARE(ZFIOBufferByCacheFile, ZFIOBuffer)

protected:
    zfoverride
    virtual ZFInput implInput(void);
    zfoverride
    virtual ZFOutput implOutput(void);
    zfoverride
    virtual void implRemoveAll(void);

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    zfautoObject iOwner;
    zfautoObject oOwner;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOBufferByCacheFile_h_

