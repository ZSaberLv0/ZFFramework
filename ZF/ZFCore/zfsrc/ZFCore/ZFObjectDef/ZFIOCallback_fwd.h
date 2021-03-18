/**
 * @file ZFIOCallback_fwd.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_fwd_h_
#define _ZFI_ZFIOCallback_fwd_h_

#include "ZFCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief for advance user only
 *
 * for #ZFOutput and #ZFInput to achieve some additional operation\n
 * \n
 * usage:\n
 * set an auto released data by #ZFCallback::callbackTag,
 * whose key is #ZFCallbackTagKeyword_ioOwner and value is a ZFObject
 * that optionally implements those method (as #ZFMethod):
 * -  ioSeek, similar to FILE's #ZFFileFileSeek, proto type:\n
 *   zfbool ioSeek(ZF_IN zfindex byteSize,
 *                 ZF_IN ZFSeekPos pos);\n
 *   return false if the callback doesn't support seek or error occurred
 * -  ioTell, similar to FILE's #ZFFileFileTell, proto type:\n
 *   zfindex ioTell(void);\n
 *   return current's index or zfindexMax() if the callback doesn't support seek
 * -  ioSize, calculate callback's size, proto type:\n
 *   zfindex ioSize(void);\n
 *   return size or zfindexMax() if the callback doesn't support
 *   @note for input callbacks, the size shows the current available size, may change after a ioSeek or execute call
 *   @note for ouput callbacks, the size shows the contents outputed to the output callback
 */
#define ZFCallbackTagKeyword_ioOwner "ZFCallbackTagKeyword_ioOwner"
/**
 * @brief util method for ZFIOCallback::ioSeek's implementations to calculate seek index
 *
 * param:\n
 * -  (zfindex)offset callback's initial offset
 * -  (zfindex)length callback's length including offset (i.e. starting from original 0)
 * -  (zfindex)curPos callback's current position including offset (i.e. start from original 0)
 *
 * return an index start from 0 (offset is included), ensured range in [offset, length]
 */
extern ZF_ENV_EXPORT zfindex ZFIOCallbackCalcFSeek(ZF_IN zfindex offset,
                                                   ZF_IN zfindex length,
                                                   ZF_IN zfindex curPos,
                                                   ZF_IN zfindex seekByteSize,
                                                   ZF_IN ZFSeekPos seekPos);

// ============================================================
/**
 * @brief abstract base class, use #ZFOutput or #ZFInput
 */
ZFCALLBACK_DECLARE_BEGIN(ZFIOCallback, ZFCallback)
public:
    /**
     * @brief similar to FILE's #ZFFileFileSeek
     *
     * return false if the callback doesn't support seek
     */
    virtual zfbool ioSeek(ZF_IN zfindex byteSize,
                          ZF_IN_OPT ZFSeekPos pos = ZFSeekPosBegin) const;
    /**
     * @brief similar to FILE's #ZFFileFileTell, return current's index or zfindexMax() if the callback doesn't support seek
     */
    virtual zfindex ioTell(void) const;
    /**
     * @brief calculate the callback's size or return zfindexMax() if not supported
     */
    virtual zfindex ioSize(void) const;
_ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFIOCallback, ZFCallback)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_fwd_h_

