/**
 * @file ZFCallerInfo.h
 * @brief caller info utils
 */
#ifndef _ZFI_ZFCallerInfo_h_
#define _ZFI_ZFCallerInfo_h_

#include "ZFCoreStringUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT void _ZFP_ZF_CALLER_FILE_TO_NAME(ZF_IN_OUT zfstring &ret,
                                                      ZF_IN const zfchar *filePath);
extern ZF_ENV_EXPORT zfstring _ZFP_ZF_CALLER_FILE_TO_NAME(ZF_IN const zfchar *filePath);

/**
 * @brief convert file path to file name
 *
 * used by ZF_CALLER_FILE_NAME, you may use this function directly if you want\n
 * the return value would be deleted after line end,
 * you should not save the value for future use,
 * store it in a std::string if necessary
 */
#define ZF_CALLER_FILE_TO_NAME(path) (_ZFP_ZF_CALLER_FILE_TO_NAME(path).cString())
/**
 * @brief see #ZF_CALLER_FILE_TO_NAME
 */
#define ZF_CALLER_FILE_TO_NAME_REF(ret, path) _ZFP_ZF_CALLER_FILE_TO_NAME(ret, path)
/**
 * @brief similar to __FILE__ in C++ world, except that don't include full path
 *
 * note that this macro contains string convertion, take care of performance if necessary\n
 * the return value would be deleted after line end,
 * you should not save the value for future use,
 * store it in a zfstring if necessary
 */
#define ZF_CALLER_FILE_NAME ZF_CALLER_FILE_TO_NAME(__FILE__)

/**
 * @brief same as __FILE__ in C++ world
 *
 * ensured in zfchar format, you should convert to zfchar type if necessary
 */
#define ZF_CALLER_FILE __FILE__
/**
 * @brief same as __FUNCTION__ in C++ world
 *
 * ensured in zfchar format, you should convert to zfchar type if necessary
 */
#define ZF_CALLER_FUNCTION __FUNCTION__
/**
 * @brief same as __LINE__ in C++ world
 */
#define ZF_CALLER_LINE __LINE__

// ============================================================
// ZFCallerInfo
/**
 * @brief util class to hold caller info
 */
zfclassLikePOD ZF_ENV_EXPORT ZFCallerInfo
{
public:
    /**
     * @brief main constructor
     */
    ZFCallerInfo(void)
    : _callerFileH(zfnull)
    , _callerFuncH(zfnull)
    , _callerFile(zfnull)
    , _callerFunc(zfnull)
    , _callerLine(0)
    {
    }
    /**
     * @brief main constructor
     */
    ZFCallerInfo(ZF_IN const zfchar *callerFile,
                 ZF_IN const zfchar *callerFunc,
                 ZF_IN zfuint callerLine)
    : _callerFileH(zfsCopy(callerFile))
    , _callerFuncH(zfsCopy(callerFunc))
    , _callerFile(_callerFileH)
    , _callerFunc(_callerFuncH)
    , _callerLine(callerLine)
    {
    }
    /** @cond ZFPrivateDoc */
    ZFCallerInfo(ZF_IN const ZFCallerInfo &ref)
    : _callerFileH(zfsCopy(ref._callerFileH))
    , _callerFuncH(zfsCopy(ref._callerFuncH))
    , _callerFile(_callerFileH)
    , _callerFunc(_callerFuncH)
    , _callerLine(ref._callerLine)
    {
    }
    ~ZFCallerInfo(void)
    {
        zfsChange(this->_callerFileH, zfnull);
        zfsChange(this->_callerFuncH, zfnull);
    }
    virtual ZFCallerInfo &operator = (ZF_IN const ZFCallerInfo &ref)
    {
        zfsChange(this->_callerFileH, ref._callerFileH);
        zfsChange(this->_callerFuncH, ref._callerFuncH);
        this->_callerFile = this->_callerFileH;
        this->_callerFunc = this->_callerFuncH;
        this->_callerLine = ref._callerLine;
        return *this;
    }
    zfbool operator == (ZF_IN const ZFCallerInfo &ref) const
    {
        return (zfscmpTheSame(this->callerFile(), ref.callerFile())
            && zfscmpTheSame(this->callerFunc(), ref.callerFunc())
            && this->callerLine() == ref.callerLine());
    }
    inline zfbool operator != (ZF_IN const ZFCallerInfo &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #ZF_CALLER_FILE */
    inline const zfchar *callerFile(void) const {return _callerFile;}
    /** @brief see #ZF_CALLER_FUNCTION */
    inline const zfchar *callerFunc(void) const {return _callerFunc;}
    /** @brief see #ZF_CALLER_LINE */
    inline zfuint callerLine(void) const {return _callerLine;}

public:
    /**
     * @brief set the caller info
     */
    virtual void callerInfo(ZF_IN const zfchar *callerFile,
                            ZF_IN const zfchar *callerFunc,
                            ZF_IN zfuint callerLine)
    {
        zfsChange(this->_callerFileH, callerFile);
        zfsChange(this->_callerFuncH, callerFunc);
        this->_callerFile = this->_callerFileH;
        this->_callerFunc = this->_callerFuncH;
        this->_callerLine = callerLine;
    }
    /** @brief return caller info looks like "[File function (line)]" */
    inline zfbool callerInfoT(ZF_IN_OUT zfstring &ret) const
    {
        if(this->callerFile() != zfnull)
        {
            ret += "[";
            ZF_CALLER_FILE_TO_NAME_REF(ret, this->callerFile());
            ret += " ";
            ret += this->callerFunc();
            ret += " (";
            zfsFromIntT(ret, this->callerLine());
            ret += ")]";
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }
    /** @brief return caller info looks like "[File function (line)]" */
    inline zfstring callerInfo(void) const
    {
        zfstring ret;
        this->callerInfoT(ret);
        return ret;
    }
public:
    /** @brief see #objectInfo */
    inline void objectInfoT(ZF_IN_OUT zfstring &ret) const
    {
        this->callerInfoT(ret);
    }
    /** @brief return object info */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

private:
    zfchar *_callerFileH;
    zfchar *_callerFuncH;
    const zfchar *_callerFile;
    const zfchar *_callerFunc;
    zfuint _callerLine;
public:
    ZFCallerInfo &_ZFP_callerInfo(ZF_IN const zfchar *callerFile,
                                  ZF_IN const zfchar *callerFunc,
                                  ZF_IN zfuint callerLine)
    {
        this->_callerFile = callerFile;
        this->_callerFunc = callerFunc;
        this->_callerLine = callerLine;
        return *this;
    }
};

/**
 * @brief util macro to make #ZFCallerInfo
 */
#define ZFCallerInfoMake() \
    ZFCallerInfo()._ZFP_callerInfo(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

extern ZF_ENV_EXPORT const ZFCallerInfo &_ZFP_ZFCallerInfoEmpty(void);
/**
 * @brief empty caller info
 */
#define ZFCallerInfoEmpty() _ZFP_ZFCallerInfoEmpty()

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallerInfo_h_

