/**
 * @file ZFUIAutoLayoutMaker.h
 * @brief utilities for #ZFUIAutoLayout
 */

#ifndef _ZFI_ZFUIAutoLayoutMaker_h_
#define _ZFI_ZFUIAutoLayoutMaker_h_

#include "ZFUIAutoLayout.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUIAutoLayoutMakerPrivate;
/**
 * @brief util to make rules for #ZFUIAutoLayout
 *
 * usage:
 * @code
 *   ZFUIAutoLayoutMaker(child).left().toLeft(parent).offset(10).scale(2);
 *   // or
 *   zfal_maker(child).left().toParentLeft();
 * @endcode
 *
 * all available functions of maker:
 * -  zfal_maker(view) :
 *   change the maker's target
 * -  width(int) / height(int) :
 *   explicitly specify the size rule,
 *   same as setting #ZFUIView::layoutParam with sizeHint and fill sizeParam,
 *   would be overrided by other rules if conflict,
 *   set to negative value would reset the rule
 * -  size(int) / size(int, int) / size(ZFUISize) :
 *   util to width(int) / height(int)
 * -  left(), top/right/bottom/width/height, etc :
 *   specify the #ZFUIAutoLayoutRule::pos
 * -  toLeft(target), top/right/bottom/width/height, etc :
 *   specify the #ZFUIAutoLayoutRule::target and #ZFUIAutoLayoutRule::targetPos
 * -  toParentLeft(), top/right/bottom/width/height, etc :
 *   util to toLeft(target) with parent
 * -  to(target) :
 *   util to toLeft(target) with same position of left() series,
 *   can be combined with multiple rules:
 *   @code
 *     maker.left().top().to(target);
 *     // same as:
 *     maker.left().toLeft(target).top().toTop(target);
 *   @endcode
 * -  toParent() :
 *   util to to(target) with parent
 * -  scale(scale) :
 *   specify the #ZFUIAutoLayoutRule::scale
 * -  offset(offset) :
 *   specify the #ZFUIAutoLayoutRule::offset
 * -  biasX(biasX) / biasY(biasY):
 *   specify the #ZFUIAutoLayoutParam::biasX
 * -  done() :
 *   the rule would be made when the maker destroyed,
 *   however, the destroy step may be scheduled later for script files with GC,
 *   so you may use this method to explicitly apply the rule maker
 *
 * all avaialbe functions in global scope for convenient:
 * -  ZFUIAutoLayoutMaker(view)
 * -  zfal_maker(view)
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFUIAutoLayoutMaker
{
public:
    /** @brief main constructor */
    ZFUIAutoLayoutMaker(void);
    /** @brief construct with child */
    ZFUIAutoLayoutMaker(ZF_IN ZFUIView *child);
    /** @brief construct with child and add to parent if necessary */
    ZFUIAutoLayoutMaker(ZF_IN ZFUIView *child,
                        ZF_IN ZFUIAutoLayout *parent);
    /** @cond ZFPrivateDoc */
    ZFUIAutoLayoutMaker(ZF_IN const ZFUIAutoLayoutMaker &ref);
    ~ZFUIAutoLayoutMaker(void);
    ZFUIAutoLayoutMaker &operator = (ZF_IN const ZFUIAutoLayoutMaker &ref);
    zfbool operator == (ZF_IN const ZFUIAutoLayoutMaker &ref) const {return d == ref.d;}
    zfbool operator != (ZF_IN const ZFUIAutoLayoutMaker &ref) const {return d != ref.d;}
    /** @endcond */

    ZFUIAutoLayoutMaker &zfal_maker(ZF_IN ZFUIView *child); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &zfal_maker(ZF_IN ZFUIView *child, ZF_IN ZFUIAutoLayout *parent); /**< @brief see #ZFUIAutoLayoutMaker */

    ZFUIAutoLayoutMaker &width(ZF_IN zfint size); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &height(ZF_IN zfint size); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &size(ZF_IN zfint size); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &size(ZF_IN zfint width, ZF_IN zfint height); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &size(ZF_IN const ZFUISize &size); /**< @brief see #ZFUIAutoLayoutMaker */

    ZFUIAutoLayoutMaker &width(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &height(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &left(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &top(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &right(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &bottom(void); /**< @brief see #ZFUIAutoLayoutMaker */

    ZFUIAutoLayoutMaker &toWidth(ZF_IN ZFUIView *target); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toHeight(ZF_IN ZFUIView *target); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toLeft(ZF_IN ZFUIView *target); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toTop(ZF_IN ZFUIView *target); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toRight(ZF_IN ZFUIView *target); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toBottom(ZF_IN ZFUIView *target); /**< @brief see #ZFUIAutoLayoutMaker */

    ZFUIAutoLayoutMaker &toParentWidth(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toParentHeight(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toParentLeft(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toParentTop(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toParentRight(void); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toParentBottom(void); /**< @brief see #ZFUIAutoLayoutMaker */

    ZFUIAutoLayoutMaker &to(ZF_IN ZFUIView *target); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &toParent(void); /**< @brief see #ZFUIAutoLayoutMaker */

    ZFUIAutoLayoutMaker &scale(ZF_IN zffloat scale); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &offset(ZF_IN zfint offset); /**< @brief see #ZFUIAutoLayoutMaker */

    ZFUIAutoLayoutMaker &biasX(ZF_IN zffloat biasX); /**< @brief see #ZFUIAutoLayoutMaker */
    ZFUIAutoLayoutMaker &biasY(ZF_IN zffloat biasY); /**< @brief see #ZFUIAutoLayoutMaker */

    ZFUIAutoLayoutMaker &done(void); /**< @brief see #ZFUIAutoLayoutMaker */

private:
    _ZFP_ZFUIAutoLayoutMakerPrivate *d;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFUIAutoLayoutMaker, ZFUIAutoLayoutMaker)

/** @brief see #ZFUIAutoLayoutMaker */
ZFMETHOD_FUNC_DECLARE_1(ZFUIAutoLayoutMaker, zfal_maker, ZFMP_IN(ZFUIView *, child))
/** @brief see #ZFUIAutoLayoutMaker */
ZFMETHOD_FUNC_DECLARE_2(ZFUIAutoLayoutMaker, zfal_maker, ZFMP_IN(ZFUIView *, child), ZFMP_IN(ZFUIAutoLayout *, parent))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAutoLayoutMaker_h_

