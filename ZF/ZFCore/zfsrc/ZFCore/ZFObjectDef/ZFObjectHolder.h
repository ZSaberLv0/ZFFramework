/**
 * @file ZFObjectHolder.h
 * @brief utilities for ZFObject
 */

#ifndef _ZFI_ZFObjectHolder_h_
#define _ZFI_ZFObjectHolder_h_

#include "ZFObjectSmartPointer.h"
#include "ZFTypeId.h"
#include "ZFPropertyDeclare.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFObjectHolder
/**
 * @brief util object to hold a ZFObject but don't retain it
 *
 * most of classes and methods use auto-retain logic to hold a ZFObject,
 * so we supply this holder class that won't retain it's content,
 * use only if necessary\n
 * typically, you should use #ZFObject::objectHolder for most case
 */
zffinal zfclass ZF_ENV_EXPORT ZFObjectHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFObjectHolder, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->objectHolded(zfnull);
    })

public:
    /**
     * @brief set the holded object
     */
    inline void objectHolded(ZF_IN ZFObject *obj)
    {
        this->_ZFP_objectHolded = obj;
    }
    /**
     * @brief set the holded object
     */
    template<typename T_ZFObject>
    inline void objectHolded(ZF_IN T_ZFObject obj)
    {
        this->_ZFP_objectHolded = obj;
    }

protected:
    /** @brief see #ZFObject::objectOnInit */
    virtual void objectOnInit(ZF_IN ZFObject *obj)
    {
        this->objectOnInit();
        this->_ZFP_objectHolded = obj;
    }
    /** @brief see #ZFObject::objectOnInit */
    template<typename T_ZFObject>
    void objectOnInit(ZF_IN T_ZFObject const &obj)
    {
        this->objectOnInit();
        this->_ZFP_objectHolded = obj;
    }

    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }

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

public:
    // override for performance
    /** @cond ZFPrivateDoc */
    zfoverride
    virtual inline ZFAny objectHolded(void)
    {
        return this->_ZFP_objectHolded;
    }
    template<typename T_ZFObject>
    T_ZFObject objectHolded(void)
    {
        return this->_ZFP_objectHolded;
    }
    /** @endcond */

public:
    ZFAny _ZFP_objectHolded;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectHolder_h_

