#include "ZFCoreSPrintf.h"
#include "ZFCoreStringUtil.h"
#include "ZFCoreUtilMacro.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_zfstringAppendPrecisionNone (zfindexMax() - 1)
#define _ZFP_zfstringAppendFlag_alignLeft 1
#define _ZFP_zfstringAppendFlag_leadingZero 2
zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfstringAppendFlag
{
public:
    zfflags flags;
    zfindex width; // zfindexMax() to scan from vaList
    zfindex precision; // zfindexMax() to scan from vaList,
                       // or _ZFP_zfstringAppendPrecisionNone if none
    zfchar positiveToken; // "+" or "-" or " ", or '\0' if none
    zfbool success;

public:
    _ZFP_zfstringAppendFlag(void)
    : flags(0)
    , width(0)
    , precision(_ZFP_zfstringAppendPrecisionNone)
    , positiveToken('\0')
    , success(zftrue)
    {
    }

public:
    zfbool hasFlag(void)
    {
        return (zffalse
                || flags != 0
                || width != 0
                || precision != _ZFP_zfstringAppendPrecisionNone
                || positiveToken != '\0'
            );
    }
};
#define _ZFP_zfstringAppend_action(ret_, flags_, p_, vaList_) \
    do { \
        switch(*p_) \
        { \
            case 'b': \
                ret_ += ((va_arg(vaList_, zft_zfint) != 0) ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse); \
                break; \
            case 'z': \
                if(*(p_+1) == 'i') \
                { \
                    ++p_; \
                    zfsFromIntT(ret_, va_arg(vaList_, zft_zfindex), 10); \
                } \
                else \
                { \
                    flags_.success = zffalse; \
                } \
                break; \
            case 'd': \
            case 'i': \
            { \
                zfint v = va_arg(vaList_, zft_zfint); \
                if(v < 0 && flags_.hasFlag()) \
                { \
                    flags_.positiveToken = '-'; \
                    v = -v; \
                } \
                zfsFromIntT(ret_, v, 10); \
            } \
                break; \
            case 'u': \
                zfsFromIntT(ret_, va_arg(vaList_, zft_zfuint), 10); \
                break; \
            case 'o': \
                zfsFromIntT(ret_, va_arg(vaList_, zft_zfuint), 8); \
                break; \
            case 'x': \
                zfsFromIntT(ret_, va_arg(vaList_, zft_zfuint), 16, zffalse); \
                break; \
            case 'X': \
                zfsFromIntT(ret_, va_arg(vaList_, zft_zfuint), 16); \
                break; \
            case 'f': \
            { \
                zffloat v = (zffloat)va_arg(vaList_, zft_zfdouble); \
                if(v < 0 && flags_.hasFlag()) \
                { \
                    flags_.positiveToken = '-'; \
                    v = -v; \
                } \
                zfsFromFloatT(ret_, v); \
            } \
                break; \
            case 'p': \
                zfsFromPointerT(ret_, va_arg(vaList_, const void *)); \
                break; \
            case 'c': \
            case 'C': \
                ret_ += (zfchar)va_arg(vaList_, zft_zfint); \
                break; \
            case 's': \
            case 'S': \
            { \
                const zfchar *v = va_arg(vaList_, const zfchar *); \
                if(v != zfnull) \
                { \
                    if(flags_.precision != _ZFP_zfstringAppendPrecisionNone) \
                    { \
                        zfindex len = zfslen(v); \
                        if(flags_.precision < len) \
                        { \
                            len = flags_.precision; \
                        } \
                        ret_.append(v, len); \
                    } \
                    else \
                    { \
                        ret_.append(v); \
                    } \
                } \
                else \
                { \
                    ret_ += ZFTOKEN_zfnull; \
                } \
            } \
                break; \
            default: \
                flags_.success = zffalse; \
                break; \
        } \
        ++p_; \
    } while(zffalse)

void zfstringAppendV(ZF_OUT zfstring &s,
                     ZF_IN const zfchar *fmt,
                     ZF_IN va_list vaList)
{
    if(fmt == zfnull) {return ;}

    const zfchar *p = fmt;
    while(*p != '\0')
    {
        if(*p != '%')
        {
            zfcharAppendAndMoveNext(s, p);
            continue;
        }

        ++p;
        if(*p == '%')
        {
            s += '%';
            ++p;
            continue;
        }

        const zfchar *savedPos = p;
        _ZFP_zfstringAppendFlag flags;
        do
        {
            while(*p == '-' || *p == '0' || *p == '+' || *p == ' ')
            {
                if(*p == '-')
                {
                    if(ZFBitTest(flags.flags, _ZFP_zfstringAppendFlag_alignLeft)) {flags.success = zffalse; break;}
                    else {ZFBitSet(flags.flags, _ZFP_zfstringAppendFlag_alignLeft);}
                }
                else if(*p == '0')
                {
                    if(ZFBitTest(flags.flags, _ZFP_zfstringAppendFlag_leadingZero)) {flags.success = zffalse; break;}
                    else {ZFBitSet(flags.flags, _ZFP_zfstringAppendFlag_leadingZero);}
                }
                else if(*p == '+')
                {
                    if(flags.positiveToken != '\0') {flags.success = zffalse; break;}
                    else {flags.positiveToken = '+';}
                }
                else if(*p == ' ')
                {
                    if(flags.positiveToken != '\0') {flags.success = zffalse; break;}
                    else {flags.positiveToken = ' ';}
                }
                ++p;
            }
            if(!flags.success) {break;}

            while(*p >= '0' && *p <= '9')
            {
                flags.width = flags.width * 10 + (*p - '0');
                ++p;
            }
            if(*p == '.')
            {
                ++p;
                flags.precision = 0;
                while(*p >= '0' && *p <= '9')
                {
                    flags.precision = flags.precision * 10 + (*p - '0');
                    ++p;
                }
            }
        } while(zffalse);
        if(!flags.success)
        {
            s += '%';
            p = savedPos;
            continue;
        }
        if(!flags.hasFlag())
        {
            _ZFP_zfstringAppend_action(s, flags, p, vaList);
            continue;
        }
        zfstring tmp;
        _ZFP_zfstringAppend_action(tmp, flags, p, vaList);

        if(flags.precision != _ZFP_zfstringAppendPrecisionNone)
        {
            zfindex pos = zfstringFind(tmp, '.');
            if(pos != zfindexMax())
            {
                zfindex decimalLen = tmp.length() - pos - 1;
                if(decimalLen > flags.precision)
                {
                    zfindex newLen = pos + flags.precision + 1;
                    if(tmp[newLen] >= '5')
                    {
                        tmp[newLen - 1] = tmp[newLen - 1] + 1;
                    }
                    tmp.remove(newLen);
                }
                else if(decimalLen < flags.precision)
                {
                    for(zfindex i = decimalLen; i < flags.precision; ++i)
                    {
                        tmp += '0';
                    }
                }
            }
        }

        if(flags.positiveToken != '\0')
        {
            ++flags.width;
        }

        if(ZFBitTest(flags.flags, _ZFP_zfstringAppendFlag_alignLeft))
        {
            zfindex writtenLen = 0;
            if(flags.positiveToken != '\0')
            {
                s += flags.positiveToken;
                ++writtenLen;
            }
            s += tmp;
            writtenLen += tmp.length();
            for(zfindex i = writtenLen; i < flags.width; ++i)
            {
                s += ' ';
            }
        }
        else
        {
            zfindex leftTokenLen = ((flags.positiveToken == '\0') ? 0 : 1);
            if(flags.width > tmp.length() + leftTokenLen)
            {
                zfindex spaceLen = flags.width - tmp.length() - leftTokenLen;
                if(ZFBitTest(flags.flags, _ZFP_zfstringAppendFlag_leadingZero))
                {
                    if(flags.positiveToken != '\0')
                    {
                        s += flags.positiveToken;
                    }
                    for(zfindex i = 0; i < spaceLen; ++i)
                    {
                        s += '0';
                    }
                }
                else
                {
                    for(zfindex i = 0; i < spaceLen; ++i)
                    {
                        s += ' ';
                    }
                    if(flags.positiveToken != '\0')
                    {
                        s += flags.positiveToken;
                    }
                }
                s += tmp;
            }
            else
            {
                if(flags.positiveToken != '\0')
                {
                    s += flags.positiveToken;
                }
                s += tmp;
            }
        }
    } // while(*p != '\0')
}

ZF_NAMESPACE_GLOBAL_END

