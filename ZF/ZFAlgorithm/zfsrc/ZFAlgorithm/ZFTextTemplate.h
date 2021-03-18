/**
 * @file ZFTextTemplate.h
 * @brief text template util
 */

#ifndef _ZFI_ZFTextTemplate_h_
#define _ZFI_ZFTextTemplate_h_

#include "ZFTextTemplateType.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util method to apply template to plain text
 *
 * data would be applied according to #ZFTextTemplateParam,
 * and result would be written to output,
 * return newly size written if success,
 * or zfindexMax() if failed,
 * you may pass an invalid output callback to calculate result size\n
 * \n
 * methods:
 * -  replace\n
 *   replace the content with specified string\n
 *   assume we have a <"myKey", "myValue"> map,
 *   then the text would be applied like:
 *   @code
 *     // original:
 *     this is {ZFTT_R_myKey}, and this is {ZFTT_R_not_set}
 *
 *     // applied:
 *     this is myValue, and this is {ZFTT_R_not_set}
 *   @endcode
 *   \n
 *   params:
 *   -  #ZFTextTemplateParam::replaceData:\n
 *     a <string, string> map
 * -  enable\n
 *   whether to enable text block\n
 *   assume we have a <"myCond0", true> map and a <"myCond1", false> map,
 *   then the text would be applied like:
 *   @code
 *     // original:
 *     aaa {ZFTT_C_myCond0}text in myCond0{ZFTT_CE} bbb
 *     ccc {ZFTT_C_myCond1}text in myCond1{ZFTT_CE} ddd
 *
 *     // result:
 *     aaa text in myCond0 bbb
 *     ccc  ddd
 *   @endcode
 *   \n
 *   params:
 *   -  #ZFTextTemplateParam::enableData:\n
 *     a <string, bool> map
 *   -  #ZFTextTemplateParam::enableDataDefault:\n
 *     whether we should enable the entries that not exist in #ZFTextTemplateParam::enableData,
 *     false by default
 * -  index\n
 *   replace the content with specified index value\n
 *   the text would be applied like:
 *   @code
 *     // original:
 *     this is {ZFTT_I_myKey} item
 *     this is {ZFTT_I_myKey} item
 *
 *     // applied:
 *     this is 0 item
 *     this is 1 item
 *   @endcode
 *   \n
 *   params:
 *   -  #ZFTextTemplateParam::indexData:\n
 *     a <string, ZFTextTemplateIndexData> map
 *   -  #ZFTextTemplateParam::indexDataDefault:\n
 *     default setting if not set in #ZFTextTemplateParam::indexData
 *
 *   \n
 *   addition:
 *   -  "{ZFTT_IR_myKey}":\n
 *     reset index state for "myKey"
 */
ZFMETHOD_FUNC_DECLARE_4(zfindex, ZFTextTemplateApply,
                        ZFMP_IN(const ZFTextTemplateParam &, param),
                        ZFMP_IN(const ZFOutput &, output),
                        ZFMP_IN(const zfchar *, data),
                        ZFMP_IN_OPT(zfindex, dataSize, zfindexMax()))
/** @brief see #ZFTextTemplateApply */
ZFMETHOD_FUNC_DECLARE_3(zfindex, ZFTextTemplateApply,
                        ZFMP_IN(const ZFTextTemplateParam &, param),
                        ZFMP_IN(const ZFOutput &, output),
                        ZFMP_IN(const ZFInput &, input))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTextTemplate_h_

