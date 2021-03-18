#include "ZFProtocolZFXml.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFXml)

static zfindex _ZFP_ZFXml_decimalCharCheck(ZF_IN const zfchar *head,
                                           ZF_IN const zfchar *end)
{ // 0; ~ 65536;
    const zfchar *p = head;
    if(head + 6 < end)
    {
        end = head + 6;
    }
    if(zfcharIsNumber(*p))
    {
        do
        {
            ++p;
            if(zfcharIsNumber(*p))
            {
                continue;
            }
            else if(*p == ';')
            {
                return p - head;
            }
            else
            {
                break;
            }
        } while (p < end);
    }
    return 0;
}
static void _ZFP_ZFXml_decimalCharEscape(ZF_IN_OUT const ZFOutput &dst,
                                         ZF_IN_OUT const zfchar *&p,
                                         ZF_IN zfindex encodedCharLen)
{
    zfcharW s[4] = {0};
    if(zfsToIntT(*(zft_zfuint16 *)s, p, encodedCharLen, 10))
    {
        zfstring t;
        zfstringToUTF8(t, s, ZFStringEncoding::e_UTF16);
        dst.execute(t.cString(), t.length());
    }
    else
    {
        dst.execute("&#");
        dst.execute(p, encodedCharLen);
        dst.execute(";");
    }
    p += encodedCharLen + 1;
}
static zfindex _ZFP_ZFXml_heximalCharCheck(ZF_IN const zfchar *head,
                                           ZF_IN const zfchar *end)
{ // 0; ~ FFFF;
    const zfchar *p = head;
    if(head + 5 < end)
    {
        end = head + 5;
    }
    if(zfcharIsHexNumber(*p))
    {
        do
        {
            ++p;
            if(zfcharIsHexNumber(*p))
            {
                continue;
            }
            else if(*p == ';')
            {
                return p - head;
            }
            else
            {
                break;
            }
        } while (p < end);
    }
    return 0;
}
static void _ZFP_ZFXml_heximalCharEscape(ZF_IN_OUT const ZFOutput &dst,
                                         ZF_IN_OUT const zfchar *&p,
                                         ZF_IN zfindex encodedCharLen)
{
    zfcharW s[4] = {0};
    if(zfsToIntT(*(zft_zfuint16 *)s, p, encodedCharLen, 16))
    {
        zfstring t;
        zfstringToUTF8(t, s, ZFStringEncoding::e_UTF16);
        dst.execute(t.cString(), t.length());
    }
    else
    {
        dst.execute("&#x");
        dst.execute(p, encodedCharLen);
        dst.execute(";");
    }
    p += encodedCharLen + 1;
}

void ZFPROTOCOL_INTERFACE_CLASS(ZFXml)::xmlEscapeCharEncode(ZF_OUT const ZFOutput &dst,
                                                            ZF_IN const zfchar *src,
                                                            ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    const zfchar *p = src;
    const zfchar *pLeft = p;
    const zfchar *pEnd = ((count == zfindexMax()) ? (p + zfslen(src)) : (p + count));
    while(p < pEnd)
    {
        if(zffalse) {}

        else if(*p == '<')
        {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute("&lt;"); ++p; pLeft = p;}
        else if(*p == '>')
        {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute("&gt;"); ++p; pLeft = p;}
        else if(*p == '&')
        {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute("&amp;"); ++p; pLeft = p;}
        else if(*p == '\'')
        {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute("&apos;"); ++p; pLeft = p;}
        else if(*p == '\"')
        {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute("&quot;"); ++p; pLeft = p;}
        else if(*p == '\r')
        {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute("&#x000D;"); ++p; pLeft = p;}
        else if(*p == '\n')
        {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute("&#x000A;"); ++p; pLeft = p;}
        else if(*p == '\t')
        {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute("&#x0009;"); ++p; pLeft = p;}

        else {zfcharMoveNext(p);}
    }
    if(pLeft < pEnd)
    {
        dst.execute(pLeft, pEnd - pLeft);
    }
}
void ZFPROTOCOL_INTERFACE_CLASS(ZFXml)::xmlEscapeCharDecode(ZF_OUT const ZFOutput &dst,
                                                            ZF_IN const zfchar *src,
                                                            ZF_IN_OPT zfindex count /* = zfindexMax() */)
{
    const zfchar *p = src;
    const zfchar *pLeft = p;
    const zfchar *pEnd = ((count == zfindexMax()) ? (p + zfslen(src)) : (p + count));
    zfindex encodedCharLen = 0;
    while(p < pEnd)
    {
        if(*p == '&')
        {
            // &lt;
            if(p+3 < pEnd && *(p+1) == 'l' && *(p+2) == 't' && *(p+3) == ';')
            {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute("<"); p += 4; pLeft = p;}
            // &gt;
            else if(p+3 < pEnd && *(p+1) == 'g' && *(p+2) == 't' && *(p+3) == ';')
            {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute(">"); p += 4; pLeft = p;}
            // &amp;
            else if(p+4 < pEnd && *(p+1) == 'a' && *(p+2) == 'm' && *(p+3) == 'p' && *(p+4) == ';')
            {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute("&"); p += 5; pLeft = p;}
            // &apos;
            else if(p+5 < pEnd && *(p+1) == 'a' && *(p+2) == 'p' && *(p+3) == 'o' && *(p+4) == 's' && *(p+5) == ';')
            {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute("\'"); p += 6; pLeft = p;}
            // &quot;
            else if(p+5 < pEnd && *(p+1) == 'q' && *(p+2) == 'u' && *(p+3) == 'o' && *(p+4) == 't' && *(p+5) == ';')
            {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute("\""); p += 6; pLeft = p;}
            // &#0; ~ &#65536;
            else if(p+3 < pEnd && *(p+1) == '#' && (encodedCharLen = _ZFP_ZFXml_decimalCharCheck(p+2, pEnd)) != 0)
            {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} p += 2; _ZFP_ZFXml_decimalCharEscape(dst, p, encodedCharLen); pLeft = p;}
            // &#x0; ~ &#xFFFF;
            else if(p+4 < pEnd && *(p+1) == '#' && *(p+2) == 'x' && (encodedCharLen = _ZFP_ZFXml_heximalCharCheck(p+3, pEnd)) != 0)
            {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} p += 3; _ZFP_ZFXml_heximalCharEscape(dst, p, encodedCharLen); pLeft = p;}

            else {++p;}
        }
        else
        {
            zfcharMoveNext(p);
        }
    }
    if(pLeft < pEnd)
    {
        dst.execute(pLeft, pEnd - pLeft);
    }
}

ZF_NAMESPACE_GLOBAL_END

