/**
 * @file ZFTypeHolder.h
 * @brief utilities for ZFObject
 */

#ifndef _ZFI_ZFTypeHolder_h_
#define _ZFI_ZFTypeHolder_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFTypeHolder
zfclassFwd ZFTypeHolder;
/**
 * @brief type for #ZFTypeHolder
 *
 * to achieve #ZFTypeHolder, supply a function with following proto type,
 * which cleanup the internal #ZFTypeHolder::holdedData:
 * @code
 *   void type(ZF_IN void *holdedData);
 * @endcode
 */
typedef void (*ZFTypeHolderType)(ZF_IN void *holdedData);
/**
 * @brief used to hold a non ZFObject type for performance
 */
zffinal zfclass ZF_ENV_EXPORT ZFTypeHolder : zfextends ZFObject
{
    // ============================================================
public:
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFTypeHolder, ZFObject)

protected:
    ZFTypeHolder(void) : holdedData(zfnull), holderType(zfnull) {}

    ZFALLOC_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void _cleanup(void)
    {
        if(this->holdedData && this->holderType)
        {
            void *holdedData = this->holdedData;
            ZFTypeHolderType holderType = this->holderType;
            this->holdedData = zfnull;
            this->holderType = zfnull;
            holderType(holdedData);
        }
        else
        {
            this->holdedData = zfnull;
            this->holderType = zfnull;
        }
    }

protected:
    /**
     * @brief init with object
     */
    virtual void objectOnInit(ZF_IN void *holdedData,
                              ZF_IN ZFTypeHolderType holderType)
    {
        this->objectOnInit();
        this->holdedData = holdedData;
        this->holderType = holderType;
    }
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->_cleanup();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    /**
     * @brief used to hold the raw pointer
     *
     * you must ensure it's valid during the holder object's life cycle,
     * would be cleared by #holderType when the holder object deallocated
     */
    void *holdedData;
    /**
     * @brief delete callback to clear the #holdedData
     */
    ZFTypeHolderType holderType;

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
        typedef typename zftTraits<T_RawType>::TrPtr T_RawTypePointer;
        return *ZFCastStatic(T_RawTypePointer, this->holdedData);
    }

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);
};
zfclassNotPOD _ZFP_ZFTypeHolderType
{
public:
    static void TypePointerRef(ZF_IN void *holdedData)
    {
    }
    static void TypePOD(ZF_IN void *holdedData)
    {
        zffree(holdedData);
    }
    template<typename T_Object>
    static void TypeObject(ZF_IN void *holdedData)
    {
        zfdelete(ZFCastStatic(T_Object, holdedData));
    }
};
/** @brief see #ZFTypeHolderType */
#define ZFTypeHolderTypePointerRef _ZFP_ZFTypeHolderType::TypePointerRef
/** @brief see #ZFTypeHolderType */
#define ZFTypeHolderTypePOD _ZFP_ZFTypeHolderType::TypePOD
/** @brief see #ZFTypeHolderType */
#define ZFTypeHolderTypeObject _ZFP_ZFTypeHolderType::TypeObject

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeHolder_h_

