#include "ZFInputForLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFInputForLuaOwner : zfextends ZFObject
{
public:
    ZFOBJECT_DECLARE(_ZFP_I_ZFInputForLuaOwner, ZFObject)

    ZFALLOC_CACHE_RELEASE({
        cache->_bufCache->zfv.removeAll();
        cache->_countCache->zfv = 0;
        cache->luaCallback.callbackClear();
        cache->userData = zfnull;
    })

public:
    ZFListener luaCallback;
    zfautoObject userData;

private:
    v_zfstring *_bufCache;
    v_zfindex *_countCache;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->_bufCache = zfAlloc(v_zfstring);
        this->_countCache = zfAlloc(v_zfindex);
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(this->_bufCache);
        zfRelease(this->_countCache);
        zfsuper::objectOnDealloc();
    }

protected:
    ZFMETHOD_DECLARE_2(zfindex, onInput,
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, count))
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFInputForLuaOwner, zfindex, onInput,
                  ZFMP_IN(void *, buf),
                  ZFMP_IN(zfindex, count))
{
    if(this->luaCallback.callbackIsValid())
    {
        this->_bufCache->zfv.removeAll();
        this->_countCache->zfv = ((buf == zfnull) ? zfindexMax() : count);
        this->luaCallback.execute(ZFListenerData(
                zfidentityInvalid(),
                zfnull,
                this->_bufCache,
                this->_countCache
            ), this->userData);
        if(buf == zfnull || count == zfindexMax())
        {
            count = this->_countCache->zfv;
        }
        else
        {
            count = zfmMin(this->_bufCache->zfv.length(), this->_countCache->zfv);
        }
        zfmemcpy(buf, this->_bufCache->zfv.cString(), count * sizeof(zfchar));
        return count;
    }
    else
    {
        return 0;
    }
}

ZFMETHOD_FUNC_DEFINE_2(ZFInput, ZFInputForLua,
                       ZFMP_IN(const ZFListener &, luaCallback),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    if(!luaCallback.callbackIsValid())
    {
        return ZFCallbackNull();
    }
    zfblockedAllocWithCache(_ZFP_I_ZFInputForLuaOwner, owner);
    owner->luaCallback = luaCallback;
    owner->userData = userData;
    ZFInput ret = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFInputForLuaOwner, onInput));
    ret.callbackOwnerObjectRetain();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

