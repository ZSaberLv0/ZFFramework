/**
 * @file ZFStyleLoad.h
 * @brief style load utility
 */

#ifndef _ZFI_ZFStyleLoad_h_
#define _ZFI_ZFStyleLoad_h_

#include "ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFStyleListPrivate;
/**
 * @brief see #ZFStyleLoad
 */
zfclass ZF_ENV_EXPORT ZFStyleList : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFStyleList, ZFStyleableObject)

public:
    /** @brief style count */
    ZFMETHOD_DECLARE_0(zfindex, itemCount)
    /** @brief style key */
    ZFMETHOD_DECLARE_1(const zfchar *, itemKeyAtIndex,
                       ZFMP_IN(zfindex, index))
    /** @brief style value */
    ZFMETHOD_DECLARE_1(ZFStyleable *, itemValueAtIndex,
                       ZFMP_IN(zfindex, index))
    /** @brief style value */
    ZFMETHOD_DECLARE_2(void, itemForKey,
                       ZFMP_IN(const zfchar *, key),
                       ZFMP_IN(ZFStyleable *, value))

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

private:
    _ZFP_ZFStyleListPrivate *d;
};

// ============================================================
/**
 * @brief util to load multiple styles from directory
 *
 * pathInfo must points to a directory that contains styles,
 * example:
 * @code
 *   ~/
 *       MyStyle/
 *           button.xml
 *           label.json
 *           image.png
 *           image.xml
 * @endcode
 * \n
 * all styles are loaded by #ZFObjectIOLoad,
 * and the file path name without file ext would be used as styleKey for #ZFStyleSet
 * (`~/path1/path2/mybutton.xml` would result `path1/path2/mybutton` as styleKey),
 * if two item with same name exists,
 * the later one would override the first one
 * (but which one is later, is not ensured, try to prevent that)\n
 * the filters can be supplied to ignore some files by file name,
 * by default, these files or dirs would be ignored:
 * -  files or dirs whose name start with '.' or '_' (e.g. '.ignored.xml')
 * -  files whose name contains '._.' (e.g. 'ignored._.xml')
 *
 * specially, if the loaded object is type of #ZFStyleList,
 * all of its contents would be appended to current style,
 * instead of setting #ZFStyleList itself as a style value
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFStyleLoad,
                        ZFMP_IN(const ZFPathInfo &, pathInfo),
                        ZFMP_IN_OPT(const ZFFilterForString *, fileNameFilter, zfnull),
                        ZFMP_IN_OPT(const ZFFilterForString *, dirNameFilter, zfnull))

/**
 * @brief load multiple style from serializableData
 *
 * data should contains:
 * @code
 *   <ZFStyleList>
 *       <Style1 name="styleKey1" ... />
 *       <Style2 name="styleKey2" ... />
 *   </ZFStyleList>
 * @endcode
 *
 * the #ZFStyleList is a dummy holder to make the serializable data itself serializable
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFStyleLoad,
                        ZFMP_IN(const ZFSerializableData &, serializableData))

/**
 * @brief see #ZFStyleLoad
 */
ZFMETHOD_FUNC_DECLARE_1(zfbool, ZFStyleLoad,
                        ZFMP_IN(ZFStyleList *, styleList))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFStyleLoad_h_

