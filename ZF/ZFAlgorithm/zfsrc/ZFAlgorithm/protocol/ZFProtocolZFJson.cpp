#include "ZFProtocolZFJson.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFJson)

void ZFPROTOCOL_INTERFACE_CLASS(ZFJson)::jsonEscapeCharEncode(ZF_OUT const ZFOutput &dst,
                                                              ZF_IN const zfchar *src,
                                                              ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    for(const zfchar *srcEnd = src + ((count == zfindexMax()) ? zfslen(src) : count); src != srcEnd; ++src)
    {
        if(*src == '\"')
        {
            dst << "\\\"";
        }
        else if(*src == '\\')
        {
            dst << "\\\\";
        }
        else if(*src == '\n')
        {
            dst << "\\n";
        }
        else if(*src == '\r')
        {
            dst << "\\r";
        }
        else if(*src == '\t')
        {
            dst << "\\t";
        }
        else if(*src == '\f')
        {
            dst << "\\f";
        }
        else if(*src == '\b')
        {
            dst << "\\b";
        }
        else
        {
            dst << *src;
        }
    }
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFJson)::jsonEscapeCharDecode(ZF_OUT const ZFOutput &dst,
                                                              ZF_IN const zfchar *src,
                                                              ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    for(const zfchar *srcEnd = src + ((count == zfindexMax()) ? zfslen(src) : count); src != srcEnd; ++src)
    {
        if(*src == '\\' && src + 1 != srcEnd)
        {
            ++src;
            if(*src == '\"')
            {
                dst << '\"';
            }
            else if(*src == '\\')
            {
                dst << '\\';
            }
            else if(*src == 'n')
            {
                dst << '\n';
            }
            else if(*src == 'r')
            {
                dst << '\r';
            }
            else if(*src == 't')
            {
                dst << '\t';
            }
            else if(*src == 'f')
            {
                dst << '\f';
            }
            else if(*src == 'b')
            {
                dst << '\b';
            }
            else
            {
                dst << '\\';
                dst << *src;
            }
        }
        else
        {
            dst << *src;
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

