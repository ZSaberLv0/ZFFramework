/**
 * @file ZFSerializableData.h
 * @brief serializable data for #ZFSerializable
 */

#ifndef _ZFI_ZFSerializableData_h_
#define _ZFI_ZFSerializableData_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief whether enable resolve check during serialization, true by default
 *
 * resolve check may consume more CPU but useful during develop
 * to find ill-formed serializable data\n
 * when on, check would be performed and an error output would be printed,
 * further more, assert fail if ZFFramework was built ad DEBUG mode\n
 * when off, no check would be performed,
 * also, no resolve state would be saved,
 * and all resolve related method would return false as resolved state\n
 * \n
 * note, you must not change this config if some serialization logic is processing,
 * usually, change it in app entry
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataResolveCheckEnable;

/**
 * @brief keyword for #ZFSerializable to hold a no name node,
 *   which means the data doesn't care about its class name
 *
 * @code
 *   <node key="value" />
 * @endcode
 */
#define ZFSerializableKeyword_node "node"
/**
 * @brief keyword for #ZFSerializable to hold attribute or property's name,
 *   see #ZFSerializableData::propertyName
 */
#define ZFSerializableKeyword_name "name"
/**
 * @brief keyword for #ZFSerializable to hold attribute or property's value,
 *   see #ZFSerializableData::propertyValue
 */
#define ZFSerializableKeyword_value "value"
/**
 * @brief keyword for #ZFSerializable's to hold category,
 *   see #ZFSerializableData::category
 */
#define ZFSerializableKeyword_category "category"
/**
 * @brief keyword for #ZFSerializable's to hold whether it's under editMode,
 *   see #ZFSerializable::editModeData
 */
#define ZFSerializableKeyword_editMode "editMode"
/**
 * @brief used to declare a null object
 *
 * @code
 *   <null>
 * @endcode
 * serializing this would result a null with success state\n
 * the actual type for the null is determined by the owner
 */
#define ZFSerializableKeyword_null "null"

// ============================================================
zfclassFwd ZFSerializable;
zfclassFwd _ZFP_ZFSerializableDataPrivate;
/**
 * @brief ZFSerializable's data container, see #ZFSerializable
 *
 * this is a data structure similar to xml,
 * designed to reduce xml dependency
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFSerializableData
{
public:
    /** @cond ZFPrivateDoc */
    ZFSerializableData(ZF_IN _ZFP_ZFSerializableDataPrivate *d);
    /** @endcond */
    /**
     * @brief construct an empty data
     */
    ZFSerializableData(void);
    /**
     * @brief retain from another ref
     */
    ZFSerializableData(ZF_IN const ZFSerializableData &ref);
    /**
     * @brief retain from another ref
     */
    ZFSerializableData &operator = (ZF_IN const ZFSerializableData &ref);
    /**
     * @brief true if same ref
     */
    zfbool operator == (ZF_IN const ZFSerializableData &ref) const;
    /**
     * @brief true if not same ref
     */
    inline zfbool operator != (ZF_IN const ZFSerializableData &ref) const {return !this->operator == (ref);}
    ~ZFSerializableData(void);

public:
    /**
     * @brief copy all the contents
     */
    zffinal void copyFrom(ZF_IN const ZFSerializableData &ref);
    /**
     * @brief return a copy of this data
     */
    inline ZFSerializableData copy(void) const
    {
        ZFSerializableData ret;
        ret.copyFrom(*this);
        return ret;
    }

    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const;

    // ============================================================
    // local path logic
public:
    /**
     * @brief used for impl to store path related info, see #ZFPathInfo
     */
    zffinal const ZFPathInfo *pathInfo(void) const;
    /** @brief see #pathInfo */
    zffinal void pathInfo(ZF_IN const ZFPathInfo *pathInfo);
    /** @brief see #pathInfo */
    zffinal void pathInfo(ZF_IN const zfchar *pathType, ZF_IN const zfchar *pathData);
    /**
     * @brief recursively check path info from child to parent
     */
    zffinal const ZFPathInfo *pathInfoCheck(void) const;

    // ============================================================
    // parent
public:
    /**
     * @brief access parent of this node, return true if parent exists
     */
    zffinal zfbool serializableDataParent(ZF_OUT ZFSerializableData &ret) const;

    // ============================================================
    // class
public:
    /** @brief see #ZFSerializable */
    zffinal void itemClass(ZF_IN const zfchar *classNameFull);
    /** @brief see #ZFSerializable */
    zffinal const zfchar *itemClass(void) const;

    // ============================================================
    // other types
public:
    /** @brief see #ZFSerializable, same as set value of attribute "name" */
    inline void propertyName(ZF_IN const zfchar *name)
    {
        this->attributeForName(ZFSerializableKeyword_name, name);
    }
    /** @brief see #propertyName */
    inline const zfchar *propertyName(void) const
    {
        return this->attributeForName(ZFSerializableKeyword_name);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "value" */
    inline void propertyValue(ZF_IN const zfchar *value)
    {
        this->attributeForName(ZFSerializableKeyword_value, value);
    }
    /** @brief see #propertyValue */
    inline const zfchar *propertyValue(void) const
    {
        return this->attributeForName(ZFSerializableKeyword_value);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "category" */
    inline void category(ZF_IN const zfchar *category)
    {
        this->attributeForName(ZFSerializableKeyword_category, category);
    }
    /** @brief see #category */
    inline const zfchar *category(void) const
    {
        return this->attributeForName(ZFSerializableKeyword_category);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "editMode" */
    inline void editMode(ZF_IN zfbool editMode)
    {
        this->attributeForName(ZFSerializableKeyword_editMode, editMode ? ZFTOKEN_zfbool_zftrue : zfnull);
    }
    /** @brief see #category */
    inline zfbool editMode(void) const
    {
        return (this->attributeForName(ZFSerializableKeyword_editMode) != zfnull);
    }

    // ============================================================
public:
    /**
     * @brief clear all the contents
     */
    zffinal void removeAll(void);

    // ============================================================
public:
    /**
     * @brief used to store extra state for this serializable data
     *
     * typically used by impl to achieve additional features
     */
    zffinal void serializableDataTag(ZF_IN const zfchar *key,
                                     ZF_IN ZFObject *tag);
    /**
     * @brief see #serializableDataTag
     */
    zffinal ZFObject *serializableDataTag(ZF_IN const zfchar *key) const;
    /** @brief see #serializableDataTag */
    template<typename T_ZFObject>
    T_ZFObject serializableDataTag(ZF_IN const zfchar *key) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->serializableDataTag(key));
    }
    /** @brief see #serializableDataTag */
    zffinal void serializableDataTagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                                   ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue) const;
    /**
     * @brief remove tag, same as set tag to null
     */
    zffinal void serializableDataTagRemove(ZF_IN const zfchar *key);
    /**
     * @brief remove tag, return removed tag or null if not exist
     */
    zffinal zfautoObject serializableDataTagRemoveAndGet(ZF_IN const zfchar *key);
    /**
     * @brief see #serializableDataTag
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void serializableDataTagRemoveAll(void);

    // ============================================================
    // attribute
public:
    /**
     * @brief see #ZFSerializable, remove if value is null (same as #attributeRemove)
     *
     * note that order of attributes is not ensured
     * after you add, remove or modify attributes
     */
    zffinal void attributeForName(ZF_IN const zfchar *name,
                                  ZF_IN const zfchar *value);
    /** @brief see #attributeForName */
    zffinal const zfchar *attributeForName(ZF_IN const zfchar *name) const;

    /** @brief see #attributeForName */
    zffinal zfindex attributeCount(void) const;

    /**
     * @brief remove value with name, see #attributeForName
     */
    zffinal void attributeRemove(ZF_IN const zfchar *name);

    /**
     * @brief remove all attribute
     */
    zffinal void attributeRemoveAll(void);

    /** @brief see #zfiterator */
    zffinal zfiterator attributeIteratorForName(ZF_IN const zfchar *name) const;
    /** @brief see #zfiterator */
    zffinal zfiterator attributeIterator(void) const;
    /** @brief see #zfiterator */
    zffinal zfbool attributeIteratorIsValid(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal zfbool attributeIteratorIsEqual(ZF_IN const zfiterator &it0,
                                            ZF_IN const zfiterator &it1) const;
    /** @brief see #zfiterator */
    zffinal void attributeIteratorValue(ZF_IN_OUT zfiterator &it,
                                        ZF_IN const zfchar *value);
    /** @brief see #zfiterator */
    zffinal void attributeIteratorRemove(ZF_IN_OUT zfiterator &it);
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorKey(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorValue(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorNextKey(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorNextValue(ZF_IN_OUT zfiterator &it) const;

    /** @brief see #zfiterator */
    zffinal zfbool attributeIteratorResolved(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal void attributeIteratorResolveMark(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal void attributeIteratorResolveUnmark(ZF_IN const zfiterator &it) const;

    // ============================================================
    // element
public:
    /**
     * @brief add child element
     *
     * note this method won't check duplicate data with same name
     */
    zffinal void elementAdd(ZF_IN const ZFSerializableData &element);
    /**
     * @brief add child element
     *
     * note this method won't check duplicate data with same name
     */
    zffinal void elementAdd(ZF_IN const ZFSerializableData &element,
                            ZF_IN zfindex atIndex);

    /**
     * @brief replace existing element at index or assert fail if index out of range
     */
    zffinal void elementSetAtIndex(ZF_IN zfindex index,
                                   ZF_IN const ZFSerializableData &element);

    /**
     * @brief find element with name or zfindexMax() if not found
     */
    zffinal zfindex elementFindByName(ZF_IN const zfchar *name) const;
    /**
     * @brief see #elementFindByName
     */
    zffinal zfindex elementFindByCategory(ZF_IN const zfchar *name) const;

    /** @brief see #ZFSerializable */
    zffinal zfindex elementCount(void) const;
    /** @brief see #ZFSerializable */
    zffinal const ZFSerializableData &elementAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFSerializable */
    zffinal ZFSerializableData &elementAtIndex(ZF_IN zfindex index);

    /**
     * @brief remove element at index
     */
    zffinal void elementRemoveAtIndex(ZF_IN zfindex index);
    /**
     * @brief remove all element
     */
    zffinal void elementRemoveAll(void);

    // ============================================================
    // resolved state
public:
    /**
     * @brief used to mark whether the data is resolved
     *
     * useful while serializing some type from serializable,
     * after operation's done,
     * you may check whether all data resolved
     * to see whether there's extra unrecognized data
     */
    zffinal zfbool resolved(void) const;
    /** @brief see #resolved */
    zffinal void resolveMark(void) const;
    /** @brief see #resolved */
    zffinal void resolveUnmark(void) const;

    /** @brief see #resolved */
    zffinal zfbool resolvedAttribute(ZF_IN const zfchar *name) const;
    /** @brief see #resolved */
    zffinal void resolveAttributeMark(ZF_IN const zfchar *name) const;
    /** @brief see #resolved */
    zffinal void resolveAttributeUnmark(ZF_IN const zfchar *name) const;

    /** @brief see #resolved */
    zffinal zfbool resolvedPropertyName(void) const
    {
        return this->resolvedAttribute(ZFSerializableKeyword_name);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyNameMark(void) const
    {
        this->resolveAttributeMark(ZFSerializableKeyword_name);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyNameUnmark(void) const
    {
        this->resolveAttributeUnmark(ZFSerializableKeyword_name);
    }

    /** @brief see #resolved */
    zffinal zfbool resolvedPropertyValue(void) const
    {
        return this->resolvedAttribute(ZFSerializableKeyword_value);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyValueMark(void) const
    {
        this->resolveAttributeMark(ZFSerializableKeyword_value);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyValueUnmark(void) const
    {
        this->resolveAttributeUnmark(ZFSerializableKeyword_value);
    }

    /** @brief see #resolved */
    zffinal zfbool resolvedCategory(void) const
    {
        return this->resolvedAttribute(ZFSerializableKeyword_category);
    }
    /** @brief see #resolved */
    zffinal void resolveCategoryMark(void) const
    {
        this->resolveAttributeMark(ZFSerializableKeyword_category);
    }
    /** @brief see #resolved */
    zffinal void resolveCategoryUnmark(void) const
    {
        this->resolveAttributeUnmark(ZFSerializableKeyword_category);
    }

    /**
     * @brief see #resolved, check whether self and all attribute and child element resolved
     */
    zffinal zfbool resolvedAll(ZF_OUT_OPT const ZFSerializableData **firstNotResolvedElement = zfnull,
                               ZF_OUT_OPT zfstring *firstNotResolvedAttribute = zfnull) const;
    /**
     * @brief see #resolved, mark self and all attribute and child element
     */
    zffinal void resolveMarkAll(void) const;
    /**
     * @brief see #resolved, unmark self and all attribute and child element
     */
    zffinal void resolveUnmarkAll(void) const;
    /**
     * @brief see #resolved, mark all attribute
     */
    zffinal void resolveAttributeMarkAll(void) const;
    /**
     * @brief see #resolved, unmark all attribute
     */
    zffinal void resolveAttributeUnmarkAll(void) const;

    // ============================================================
    // other functions
public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /**
     * @brief get a short info
     */
    zffinal zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
    /**
     * @brief compare another serializable data
     *
     * if two data considered as same,
     * attribute's order have no need to be same,
     * but child element's order must be same
     */
    zffinal ZFCompareResult objectCompare(ZF_IN const ZFSerializableData &another) const;

public:
    /**
     * @brief true if the data contains nothing
     */
    zffinal zfbool isEmpty(void) const;

private:
    _ZFP_ZFSerializableDataPrivate *d;
    friend zfclassFwd _ZFP_ZFSerializableDataPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableData_h_

