/**
 * @file ZFIdentityUtil.h
 * @brief identity generator utility
 */

#ifndef _ZFI_ZFIdentityUtil_h_
#define _ZFI_ZFIdentityUtil_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_Element>
zfclassFwd ZFCoreArray;
zfclassFwd _ZFP_ZFIdentityGeneratorPrivate;
/**
 * @brief identity generator utility
 *
 * used to generate an unused identity
 * @warning this class is not thread-safe,
 *   you must achieve thread-safe manually,
 *   such as #zfCoreMutexLock
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFIdentityGenerator
{
public:
    /** @cond ZFPrivateDoc */
    ZFIdentityGenerator(void);
    ZFIdentityGenerator(ZF_IN ZFIdentityGenerator const &ref);
    ZFIdentityGenerator &operator = (ZF_IN ZFIdentityGenerator const &ref);
    zfbool operator == (ZF_IN ZFIdentityGenerator const &ref) const;
    inline zfbool operator != (ZF_IN ZFIdentityGenerator const &ref) const {return !this->operator == (ref);}
    ~ZFIdentityGenerator(void);
    /** @endcond */

public:
    /**
     * @brief get next identity,
     *   ensured valid and ensured unused,
     *   and auto mark it as used
     */
    zfidentity idAcquire(void);
    /**
     * @brief mark the identity unused
     */
    void idRelease(ZF_IN zfidentity identity);

    /**
     * @brief true if the identity is not used
     *
     * #zfidentityInvalid is always treated as used
     */
    zfbool idUsed(ZF_IN zfidentity identity) const;

    /**
     * @brief get all identity currently used, for debug use only
     */
    void idUsedGetAll(ZF_IN_OUT ZFCoreArray<zfidentity> &ret) const;

private:
    _ZFP_ZFIdentityGeneratorPrivate *d;
};

// ============================================================
/**
 * @brief calculate identity from string
 */
extern ZF_ENV_EXPORT zfidentity zfidentityCalcString(ZF_IN const zfchar *src, ZF_IN_OPT zfindex srcLen = zfindexMax());
/**
 * @brief calculate identity from raw buffer
 */
extern ZF_ENV_EXPORT zfidentity zfidentityCalcBuf(ZF_IN const void *src, ZF_IN zfindex srcLen);
/**
 * @brief calculate identity from POD object
 */
template<typename T_POD>
zfidentity zfidentityCalcPOD(ZF_IN T_POD const &value)
{
    return zfidentityCalcBuf(&value, sizeof(T_POD));
}

/**
 * @brief make a identity from pointer value
 */
extern ZF_ENV_EXPORT zfidentity zfidentityCalcPointer(ZF_IN const void *p);
/**
 * @brief make a identity from pointer value
 */
extern ZF_ENV_EXPORT zfidentity zfidentityCalcPointer(ZF_IN ZFFuncAddrType p);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFIdentityUtil_h_

