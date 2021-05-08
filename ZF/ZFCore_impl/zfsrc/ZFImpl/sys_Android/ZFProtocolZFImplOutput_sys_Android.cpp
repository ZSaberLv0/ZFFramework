#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFImplOutput.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFImpl_sys_Android_ZFImplOutput_output(fmt, ...) \
    AndroidLogDetail(AndroidLogLevelA, AndroidLogTagDefault(), fmt, ##__VA_ARGS__)

zfclass _ZFP_I_ZFImplOutputImpl_sys_Android_SyncObject : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFImplOutputImpl_sys_Android_SyncObject, ZFObject)
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFImplOutputImpl_sys_Android, ZFImplOutput, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Logcat")
public:
    zfoverride
    virtual void protocolOnInitFinish(void)
    {
        zfsuper::protocolOnInitFinish();
        this->syncObj = zfAlloc(_ZFP_I_ZFImplOutputImpl_sys_Android_SyncObject);
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        if(this->savedString.length() > 0)
        {
            _ZFP_ZFImpl_sys_Android_ZFImplOutput_output("%s", this->savedString.cString());
        }
        zfRelease(this->syncObj);
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual void outputCoreLog(ZF_IN const zfchar *s)
    {
        zfstring tmp = s;
        this->checkOutput(tmp);
    }
    virtual void outputLog(ZF_IN const zfchar *s, ZF_IN_OPT zfindex count = zfindexMax())
    {
        zfsynchronize(this->syncObj);
        if(count == zfindexMax())
        {
            this->savedString += s;
        }
        else
        {
            this->savedString += zfstring(s, count);
        }
        this->checkOutput(this->savedString);
    }
private:
    ZFObject *syncObj;
    zfstring savedString;
    void checkOutput(ZF_IN_OUT zfstring &s)
    {
        if(s.isEmpty())
        {
            return;
        }

        zfindex pL = 0;
        do {
            zfindex p = zfstringFind(s.cString() + pL, '\n');
            if(p == zfindexMax())
            {
                break;
            }
            s[pL + p] = '\0';
            _ZFP_ZFImpl_sys_Android_ZFImplOutput_output("%s", s.cString() + pL);
            pL = pL + p + 1;
        } while(zftrue);
        if(pL != 0)
        {
            s.remove(0, pL);
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFImplOutputImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFImplOutputImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif

