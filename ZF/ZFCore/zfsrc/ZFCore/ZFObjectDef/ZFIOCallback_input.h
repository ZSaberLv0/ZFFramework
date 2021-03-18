/**
 * @file ZFIOCallback_input.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_input_h_
#define _ZFI_ZFIOCallback_input_h_

#include "ZFIOCallback_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFInput
/**
 * @brief general input callback
 *
 * proto type:
 * @code
 *   zfindex input(ZF_OUT void *buf,
 *                 ZF_IN zfindex count);
 * @endcode
 *
 * params:
 * -  (void *) buffer to write to
 * -  (zfindex) max byte size to read (excluding '\0')
 *
 * return:
 * -  (zfindex) if buffer is null,
 *   return max byte size in the input or zfindexMax if not available\n
 *   if buffer is not null, return byte size actually read even if error (excluding '\0')
 *
 * ADVANCED:\n
 * for implementations, see #ZFCallbackTagKeyword_ioOwner if need support seek
 */
ZFCALLBACK_DECLARE_BEGIN(ZFInput, ZFIOCallback)
public:
    /** @brief see #ZFInput */
    inline zfindex execute(ZF_OUT void *buf,
                           ZF_IN zfindex count) const
    {
        return ZFCallback::executeExact<zfindex, void *, zfindex>(buf, count);
    }
    /** @brief see #ZFInput */
    inline zfindex operator () (ZF_OUT void *buf,
                                ZF_IN zfindex count) const
    {
        return ZFCallback::executeExact<zfindex, void *, zfindex>(buf, count);
    }
    /** @brief see #ZFInput */
    zfindex input(ZF_OUT zfstring &buf,
                  ZF_IN zfindex count) const
    {
        if(count == zfindexMax())
        {
            count = this->execute(zfnull, zfindexMax());
            if(count == zfindexMax())
            {
                return zfindexMax();
            }
        }
        void *bufTmp = zfmalloc(count);
        zfindex read = this->execute(bufTmp, count);
        if(read > count)
        {
            zffree(bufTmp);
            return zfindexMax();
        }
        buf.append((const zfchar *)bufTmp, read);
        zffree(bufTmp);
        return read;
    }
_ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFInput, ZFIOCallback)

// ============================================================
/**
 * @brief read single logic char to buf, return count of sizoef(zfchar) if success, or 0 if fail
 *
 * since UTF-8 has variable logic char size, use this method to read a single logic char\n
 * note: result may have more than one char for invalid encoding,
 * and buf size must at least 9 byte size
 */
extern ZF_ENV_EXPORT zfindex ZFInputReadChar(ZF_OUT zfchar *buf, ZF_IN_OUT const ZFInput &input);
/**
 * @brief see #ZFInputReadChar
 */
extern ZF_ENV_EXPORT zfindex ZFInputReadChar(ZF_IN_OUT zfstring &buf, ZF_IN_OUT const ZFInput &input);
/**
 * @brief util method to read all contents from input to string
 */
extern ZF_ENV_EXPORT void ZFInputReadToString(ZF_IN_OUT zfstring &ret, ZF_IN_OUT const ZFInput &input);
/**
 * @brief util method to read all contents from input to a buffer
 *
 * returned buffer is ensured null-terminated,
 * and buffer's size shows the content's size excluding tail '\0'\n
 * return null buffer if input invalid or input error
 */
extern ZF_ENV_EXPORT ZFBuffer ZFInputReadToBuffer(ZF_IN_OUT const ZFInput &input);

/**
 * @brief util method to skip chars in set
 *
 * return false if all chars are skipped before end,
 * other wise, buf stores the first logic char (see #ZFInputReadChar)
 */
extern ZF_ENV_EXPORT zfbool ZFInputSkipChars(ZF_OUT zfchar *buf,
                                             ZF_IN_OUT const ZFInput &input,
                                             ZF_IN_OPT const zfchar *charSet = " \t\r\n");

/**
 * @brief read until any char in charSet shows up, or reached maxCount,
 *   return count read so far
 *
 * the first char matched charSet would be read and discarded,
 * and you may check it by firstMatchedChar,
 * if reached end or maxCount before matched charSet,
 * 0 would be returned to firstMatchedChar
 */
extern ZF_ENV_EXPORT zfindex ZFInputReadUntil(ZF_IN_OUT zfstring &ret,
                                              ZF_IN_OUT const ZFInput &input,
                                              ZF_IN_OPT const zfchar *charSet = " \t\r\n",
                                              ZF_IN_OPT zfindex maxCount = zfindexMax(),
                                              ZF_OUT_OPT zfchar *firstMatchedChar = zfnull);

/**
 * @brief util method to check whether the input match the tokens
 *
 * return token's index if match or zfindexMax() if no match or error,
 * header white spaces would be skipped automatically\n
 * if no match, this method would try to restore the callback state by ioSeek to original position
 */
extern ZF_ENV_EXPORT zfindex ZFInputCheckMatch(ZF_IN const zfchar **tokens,
                                               ZF_IN zfindex tokenCount,
                                               ZF_IN_OUT const ZFInput &input);

// ============================================================
// common input callbacks
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       // dummy input has no contents
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputDummy "ZFInputDummy"
/**
 * @brief a dummy input that always read zero byte with success state
 */
extern ZF_ENV_EXPORT ZFInput ZFInputDummy(void);

/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <something category="input" ... />
 *       <zfindex category="start" ... /> // optional, 0 by default
 *       <zfindex category="count" ... /> // optional, zfindexMax() by default
 *       <zfbool category="autoRestorePos" ... /> // optional, zftrue by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputForInputInRange "ZFInputForInputInRange"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputForInputInRange_input "input"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputForInputInRange_start "start"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputForInputInRange_count "count"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputForInputInRange_autoRestorePos "autoRestorePos"
/**
 * @brief create a intput callback from another input callback, specified in range
 *
 * params:
 * -  (const ZFInput &) input callback to use
 * -  (zfindex) src's start index
 * -  (zfindex) src's count or zfindexMax() to use whole
 * -  (zfbool) whether to restore src input callback's position after result callback deleted
 *
 * seeking the result input callback would ensure in range [start, start + count]\n
 * src must support seek, otherwise a null callback would be returned
 */
extern ZF_ENV_EXPORT ZFInput ZFInputForInputInRange(ZF_IN const ZFInput &inputCallback,
                                                    ZF_IN_OPT zfindex start = 0,
                                                    ZF_IN_OPT zfindex count = zfindexMax(),
                                                    ZF_IN_OPT zfbool autoRestorePos = zftrue);

/**
 * @brief create a intput callback input from a const void *,
 *   you must ensure the buffer is alive during the callback's life time
 *
 * params:
 * -  (const void *) src to use
 * -  (zfindex) src's count or zfindexMax() to calculate automatically (treated as const zfchar *),
 *   zfindexMax() by default
 */
extern ZF_ENV_EXPORT ZFInput ZFInputForBufferUnsafe(ZF_IN const void *src,
                                                    ZF_IN_OPT zfindex count = zfindexMax());
/**
 * @brief see #ZFInputForBufferUnsafe,
 *   copy the contents and auto free it
 */
extern ZF_ENV_EXPORT ZFInput ZFInputForBuffer(ZF_IN const void *src,
                                              ZF_IN_OPT zfindex count = zfindexMax());

/**
 * @brief same as #ZFInputForBuffer
 */
extern ZF_ENV_EXPORT ZFInput ZFInputForString(ZF_IN const zfchar *src,
                                              ZF_IN_OPT zfindex count = zfindexMax());

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_input_h_

