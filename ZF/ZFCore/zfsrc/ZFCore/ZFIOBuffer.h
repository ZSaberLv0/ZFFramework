/**
 * @file ZFIOBuffer.h
 * @brief util to hold and connect #ZFInput and #ZFOutput
 */

#ifndef _ZFI_ZFIOBuffer_h_
#define _ZFI_ZFIOBuffer_h_

#include "ZFObject.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to hold and connect #ZFInput and #ZFOutput
 *
 * usage:
 * @code
 *   void outputFunc(ZF_IN const ZFOutput &callback) {...}
 *   void inputFunc(ZF_IN const ZFInput &callback) {...}
 *
 *   zfblockedAlloc(ZFIOBufferByXxx, io);
 *   outputFunc(io->output()); // output data to io's internal buffer
 *   inputFunc(io->input()); // input data from io's internal buffer
 * @endcode
 *
 * once created, the #ZFIOBuffer would be retained by the
 * returned #ZFInput/#ZFOutput by #ZFCallbackTagKeyword_ZFIOBuffer,
 * it's ensured safe to release the owner #ZFIOBuffer's reference
 */
zfabstract ZF_ENV_EXPORT ZFIOBuffer : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFIOBuffer, ZFObject)

public:
    /**
     * @brief get input callback
     */
    ZFMETHOD_DECLARE_0(ZFInput, input)
    /**
     * @brief get output callback
     */
    ZFMETHOD_DECLARE_0(ZFOutput, output)
    /**
     * @brief remove all of contents,
     *   so next output would write from beginning
     *   and next input would read from beginning
     */
    ZFMETHOD_DECLARE_0(void, removeAll)

protected:
    /** @brief see #input */
    virtual ZFInput implInput(void) zfpurevirtual;
    /** @brief see #output */
    virtual ZFOutput implOutput(void) zfpurevirtual;
    /** @brief see #removeAll */
    virtual void implRemoveAll(void) zfpurevirtual;
};

/**
 * @brief store #ZFIOBuffer as #ZFCallback::callbackTag
 *   for #ZFIOBuffer::input and #ZFIOBuffer::output
 */
#define ZFCallbackTagKeyword_ZFIOBuffer "ZFCallbackTagKeyword_ZFIOBuffer"

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOBuffer_h_

