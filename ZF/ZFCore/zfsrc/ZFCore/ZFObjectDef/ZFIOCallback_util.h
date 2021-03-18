/**
 * @file ZFIOCallback_util.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_util_h_
#define _ZFI_ZFIOCallback_util_h_

#include "ZFIOCallback_input.h"
#include "ZFIOCallback_output.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util method to read all contents of input to output
 *
 * return size already written to output even if error occurred
 */
extern ZF_ENV_EXPORT zfindex ZFInputReadToOutput(ZF_IN_OUT const ZFOutput &output,
                                                 ZF_IN_OUT const ZFInput &input);

// ============================================================
// ZFIOBufferedCallback
/**
 * @brief a util class to make a buffer to bridge input and output
 *
 * usage:
 * @code
 *   void outputFunc(ZF_IN const ZFOutput &callback) {...}
 *   void inputFunc(ZF_IN const ZFInput &callback) {...}
 *
 *   ZFIOBufferedCallbackXXX bridge;
 *   outputFunc(bridge); // output data to bridge's internal buffer
 *   inputFunc(bridge); // input data from bridge's internal buffer
 * @endcode
 */
zfclassLikePOD ZF_ENV_EXPORT ZFIOBufferedCallback
{
public:
    /** @cond ZFPrivateDoc */
    ZFIOBufferedCallback(void) {}
    ZFIOBufferedCallback(ZF_IN const ZFIOBufferedCallback &ref) {}
    /** @endcond */
    virtual ~ZFIOBufferedCallback(void) {}

public:
    /**
     * @brief implicit convert to input callback
     */
    virtual operator ZFInput (void)
    {
        return this->inputCallback();
    }
    /**
     * @brief implicit convert to output callback
     */
    virtual operator ZFOutput (void)
    {
        return this->outputCallback();
    }

public:
    /**
     * @brief get input callback
     */
    virtual ZFInput inputCallback(void) zfpurevirtual;
    /**
     * @brief get input callback
     */
    virtual ZFOutput outputCallback(void) zfpurevirtual;

public:
    /**
     * @brief reset input so that next input operation would read from begin
     */
    virtual void resetInput(void) zfpurevirtual;
    /**
     * @brief clear all outputed data so that next output operation would write from begin,
     *   note that input would also be reset
     */
    virtual void resetOutput(void) zfpurevirtual;
};

// ============================================================
// ZFIOBufferedCallbackUsingBuffer
zfclassFwd _ZFP_ZFIOBufferedCallbackUsingBufferPrivate;
/**
 * @brief see #ZFIOBufferedCallback
 */
zfclassLikePOD ZF_ENV_EXPORT ZFIOBufferedCallbackUsingBuffer : zfextendsLikePOD ZFIOBufferedCallback
{
public:
    /** @cond ZFPrivateDoc */
    ZFIOBufferedCallbackUsingBuffer(void);
    ZFIOBufferedCallbackUsingBuffer(ZF_IN const ZFIOBufferedCallbackUsingBuffer &ref);
    ZFIOBufferedCallbackUsingBuffer &operator = (ZF_IN const ZFIOBufferedCallbackUsingBuffer &ref);
    /** @endcond */
    virtual ~ZFIOBufferedCallbackUsingBuffer(void);

public:
    zfoverride
    virtual ZFInput inputCallback(void);
    zfoverride
    virtual ZFOutput outputCallback(void);

public:
    zfoverride
    virtual void resetInput(void);
    zfoverride
    virtual void resetOutput(void);

private:
    _ZFP_ZFIOBufferedCallbackUsingBufferPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_util_h_

