/**
 * @file ZFUIGlobalStyle.h
 * @brief global style for ui elements
 */

#ifndef _ZFI_ZFUIGlobalStyle_h_
#define _ZFI_ZFUIGlobalStyle_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief global style for ui elements
 */
zfclass ZF_ENV_EXPORT ZFUIGlobalStyle : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIGlobalStyle, ZFStyleableObject)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIGlobalStyle)

public:
    // ============================================================
    // image scale
    /**
     * @brief resource image's base scale, 2 by default to suit most case,
     *   see #ZFUIImage::imageScale for more info
     *
     * scale 2 means, 80 pixel size image would be treated as 40 size in #ZFUIView tree,
     * which should suit for most physical screen,
     * if you want to support higher screen resolution,
     * supply larger pixel image and increase this scale value
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, imageScale, 2.0f)

    // ============================================================
    // common size hints
    /**
     * @brief space between inner items
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSpace, 8)
    /**
     * @brief space to outer containers
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemMargin, 8)

    // ============================================================
    // item size hints
    /**
     * @brief text item size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeText, 21)
    /**
     * @brief small control item size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeControl, 32)
    /**
     * @brief button size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeButton, 44)
    /**
     * @brief button size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeButtonWidth, 68)
    /**
     * @brief icon size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeIcon, 40)
    /**
     * @brief icon size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeIconSmall, 32)
    /**
     * @brief list cell size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeListCell, 48)
    /**
     * @brief dialog item size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeDialog, 80)
    /**
     * @brief dialog item size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, itemSizeDialogWidth, 200)

    // ============================================================
    // text size hints
    /**
     * @brief extra tiny text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeExtraTiny, 8)
    /**
     * @brief tiny text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeTiny, 10)
    /**
     * @brief small text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeSmall, 12)
    /**
     * @brief normal text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeNormal, 14)
    /**
     * @brief big text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeBig, 16)
    /**
     * @brief large text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeLarge, 18)
    /**
     * @brief extra large text size hint
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeExtraLarge, 20)

    // ============================================================
    // text settings
    /**
     * @brief default text appearance
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextAppearanceEnum, textAppearance, ZFUITextAppearance::EnumDefault())
    /**
     * @brief default text align
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, textAlign, ZFUIAlign::e_LeftInner)
    /**
     * @brief default text truncate mode
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextTruncateModeEnum, textTruncateMode, ZFUITextTruncateMode::EnumDefault())

    // ============================================================
    // generanl colors
    /**
     * @brief default background color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorBgDefault, ZFUIColorGrayLight())
    /**
     * @brief item background color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorBgItem, ZFUIColorWhite())
    /**
     * @brief separator color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorSeparator, ZFUIColorGrayLight())
    /**
     * @brief shadow color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorShadow, ZFUIColorWithA(ZFUIColorBlack(), 0.5f))
    /**
     * @brief mask color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorMask, ZFUIColorWithA(ZFUIColorBlack(), 0.5f))
    /**
     * @brief tint color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, colorTint, ZFUIColorGrayLight())

    // ============================================================
    // text colors
    /**
     * @brief default text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorDefault, ZFUIColorBlack())
    /**
     * @brief secondary default text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorSecondary, ZFUIColorGray())
    /**
     * @brief hint text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorHint, ZFUIColorGrayLight())
    /**
     * @brief link text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorLink, ZFUIColorBlue())
    /**
     * @brief visited link text color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColorLinkVisited, ZFUIColorMagenta())

    // ============================================================
    // control
    /**
     * @brief normal control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorNormal, ZFUIColorBlack())
    /**
     * @brief highlighted control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorHighlighted, ZFUIColorBlack())
    /**
     * @brief checked control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorChecked, ZFUIColorBlack())
    /**
     * @brief checked highlighted control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorCheckedHighlighted, ZFUIColorBlack())
    /**
     * @brief disabled control color
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, controlColorDisabled, ZFUIColorGray())
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIGlobalStyle_h_

