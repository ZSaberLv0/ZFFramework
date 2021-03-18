/**
 * @file ZFPointerHolder.h
 * @brief utilities for ZFObject
 */

#ifndef _ZFI_ZFPointerHolder_h_
#define _ZFI_ZFPointerHolder_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFPointerHolder
/**
 * @brief used to hold a non ZFObject type for performance
 */
zffinal zfclass ZF_ENV_EXPORT ZFPointerHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFPointerHolder, ZFObject)

protected:
    ZFPointerHolder(void) : holdedData(zfnull) {}

protected:
    /**
     * @brief init with pointer
     */
    virtual void objectOnInit(ZF_IN const void *holdedData)
    {
        this->objectOnInit();
        this->holdedData = holdedData;
    }
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }

public:
    /**
     * @brief used to hold the raw pointer,
     *   you must ensure it's valid during the holder object's life cycle
     */
    const void *holdedData;

    ZFALLOC_CACHE_RELEASE({
        cache->holdedData = zfnull;
    })

public:
    /** @brief util method to cast #holdedData */
    template<typename T_RawType>
    inline T_RawType holdedDataPointer(void)
    {
        return ZFCastStatic(T_RawType, this->holdedData);
    }
    /** @brief util method to cast #holdedData */
    template<typename T_RawType>
    inline T_RawType holdedDataRef(void)
    {
        typedef typename zftTraits<T_RawType>::TrConstPtr T_RawTypeConstPointer;
        return *ZFCastStatic(T_RawTypeConstPointer, this->holdedData);
    }

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    zfoverride
    virtual zfbool objectIsPrivate(void)
    {
        return zftrue;
    }
    zfoverride
    virtual zfbool objectIsInternal(void)
    {
        return zftrue;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPointerHolder_h_

