/**
 * @file ZFListenerHolder.h
 * @brief utilities for ZFObject
 */

#ifndef _ZFI_ZFListenerHolder_h_
#define _ZFI_ZFListenerHolder_h_

#include "ZFObjectSmartPointer.h"
#include "ZFTypeId.h"
#include "ZFPropertyDeclare.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util class to hold #ZFListener
 */
zfclass ZF_ENV_EXPORT ZFListenerHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFListenerHolder, ZFObject)

public:
    /** @brief see #ZFListener */
    ZFListener runnable;
    /** @brief see #ZFListener */
    ZFListenerData listenerData;
    /** @brief see #ZFListener */
    ZFPROPERTY_RETAIN(ZFObject *, userData)

protected:
    /** @brief see #ZFObject::objectOnInit */
    virtual void objectOnInit(ZF_IN const ZFListener &runnable,
                              ZF_IN const ZFListenerData &listenerData,
                              ZF_IN_OPT ZFObject *userData = zfnull)
    {
        this->objectOnInit();
        this->runnable = runnable;
        this->listenerData = listenerData;
        this->userData(userData);
    }

    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfsuper::objectOnDealloc();
    }

protected:
    zfoverride
    virtual inline void objectInfoT(ZF_IN_OUT zfstring &ret)
    {
        this->runnable.objectInfoT(ret);
    }
public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief execute the runnable or do nothing if callback invalid
     */
    virtual inline void runnableExecute(void)
    {
        if(this->runnable.callbackIsValid())
        {
            this->runnable.execute(this->listenerData, this->userData());
        }
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFListenerHolder_h_

