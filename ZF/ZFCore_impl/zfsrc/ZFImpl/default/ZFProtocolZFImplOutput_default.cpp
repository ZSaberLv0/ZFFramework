#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFImplOutput.h"
#include "ZFCore/ZFString.h"
#include <stdio.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFImplOutputImpl_default, ZFImplOutput, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("C:printf")
public:
    virtual void outputCoreLog(ZF_IN const zfchar *s)
    {
        printf("%s", s);
    }
    virtual void outputLog(ZF_IN const zfchar *s, ZF_IN_OPT zfindex count = zfindexMax())
    {
        if(count == zfindexMax())
        {
            printf("%s", s);
        }
        else
        {
            printf("%s", zfstring(s, count).cString());
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFImplOutputImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFImplOutputImpl_default)

ZF_NAMESPACE_GLOBAL_END

