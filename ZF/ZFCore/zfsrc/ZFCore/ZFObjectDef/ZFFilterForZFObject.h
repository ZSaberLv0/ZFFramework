/**
 * @file ZFFilterForZFObject.h
 * @brief filter define for ZFObject
 */

#ifndef _ZFI_ZFFilterForZFObject_h_
#define _ZFI_ZFFilterForZFObject_h_

#include "ZFObjectCore.h"
#include "ZFFilterForZFClass.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFFilterForZFObject
/**
 * @brief object filter compared by #ZFObject::objectCompare
 */
zfclassLikePOD ZF_ENV_EXPORT ZFFilterForZFObject : zfextendsLikePOD ZFFilterBasic<ZFObject *>
{
    ZFFILTER_DECLARE(ZFFilterForZFObject, ZFFilterBasic<ZFObject *>)

public:
    /**
     * @brief see #ZFFilterBase
     */
    ZFFilterForZFObject(void);
    /**
     * @brief see #ZFFilterBase
     */
    ZFFilterForZFObject(ZF_IN const ZFFilterForZFObject &ref);
    /**
     * @brief see #ZFFilterBase
     */
    virtual ZFFilterForZFObject &operator = (ZF_IN const ZFFilterForZFObject &ref);
    virtual ~ZFFilterForZFObject(void);

    /** @cond ZFPrivateDoc */
    virtual void copyFrom(ZF_IN ZFFilterBase<ZFObject *, ZFObject *> const &ref);
    /** @endcond */

public:
    /**
     * @brief filter the object by class
     */
    ZFFilterForZFClass classFilter;

protected:
    /** @cond ZFPrivateDoc */ // ZFTAG_DOXYGEN_BUG: can't auto detect template specialization
    zfoverride
    virtual void filterOnStore(ZF_IN_OUT ZFObject * &to,
                               ZF_IN ZFObject * const &from) const;
    zfoverride
    virtual void filterOnRemove(ZF_IN_OUT ZFObject * &to) const;
    zfoverride
    virtual zfbool filterOnCheckEqual(ZF_IN ZFObject * const &e1,
                                      ZF_IN ZFObject * const &e2) const;
    zfoverride
    virtual zfbool filterOnCheckActive(ZF_IN ZFObject * const &e) const;
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFilterForZFObject_h_

