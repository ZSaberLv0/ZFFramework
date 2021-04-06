#include "ZFStyleable.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"
#include "ZFPropertyUtil.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFStyleKeyHolder
{
public:
    zfchar *styleKey;
    zfstlmap<zfstlstringZ, zfstring> stylePropertyKeyMap;
public:
    static void styleOnChange(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData);
    static void stylePropertyOnChange(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData);
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleChangeDataHolder, ZFLevelZFFrameworkEssential)
{
    this->styleOnChangeListener = ZFCallbackForFunc(_ZFP_ZFStyleKeyHolder::styleOnChange);
}
public:
    ZFListener styleOnChangeListener;
ZF_GLOBAL_INITIALIZER_END(ZFStyleChangeDataHolder)

void _ZFP_ZFStyleKeyHolder::styleOnChange(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    zfCoreMutexLocker();
    ZFStyleable *owner = userData->objectHolded();
    zfautoObject style = ZFStyleGet(owner->styleKey());
    if(style != zfnull)
    {
        zfCoreAssertWithMessageTrim(style->classData()->classIsTypeOf(owner->classData())
            || owner->classData()->classIsTypeOf(style->classData()),
            "[ZFStyle] applying style from incorrect type, to object: %s, from style: %s",
            owner->toObject()->objectInfoOfInstance().cString(),
            style->objectInfoOfInstance().cString());

        owner->styleableCopyFrom(style);
    }
    else
    {
        owner->styleKey(zfnull);
    }
}
void ZFStyleable::styleKey(ZF_IN const zfchar *styleKey)
{
    zfCoreMutexLocker();
    if(styleKey == zfnull)
    {
        if(_ZFP_styleKey != zfnull)
        {
            zfsChange(_ZFP_styleKey->styleKey, styleKey);
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleChangeDataHolder)->styleOnChangeListener,
                this->toObject()->objectHolder());
        }
    }
    else
    {
        if(_ZFP_styleKey == zfnull)
        {
            _ZFP_styleKey = zfpoolNew(_ZFP_ZFStyleKeyHolder);
        }
        if(_ZFP_styleKey->styleKey == zfnull)
        {
            ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleChangeDataHolder)->styleOnChangeListener,
                this->toObject()->objectHolder());
        }
        zfsChange(_ZFP_styleKey->styleKey, styleKey);
        ZFListenerData listenerData;
        _ZFP_ZFStyleKeyHolder::styleOnChange(listenerData, this->toObject()->objectHolder());

        if(_ZFP_styleKey->styleKey == zfnull)
        {
            v_zfstring *param0 = zflockfree_zfAllocWithCache(v_zfstring);
            v_zfstring *param1 = zflockfree_zfAllocWithCache(v_zfstring);
            param1->zfv = styleKey;
            ZFGlobalObserver().observerNotifyWithCustomSender(
                this->toObject(),
                ZFGlobalEvent::EventZFStyleOnInvalid(),
                param0,
                param1);
            zflockfree_zfRelease(param0);
            zflockfree_zfRelease(param1);
        }
    }
}
const zfchar *ZFStyleable::styleKey(void)
{
    if(_ZFP_styleKey)
    {
        return _ZFP_styleKey->styleKey;
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStylePropertyChangeDataHolder, ZFLevelZFFrameworkEssential)
{
    this->stylePropertyOnChangeListener = ZFCallbackForFunc(_ZFP_ZFStyleKeyHolder::stylePropertyOnChange);
}
public:
    ZFListener stylePropertyOnChangeListener;
ZF_GLOBAL_INITIALIZER_END(ZFStylePropertyChangeDataHolder)

static zfbool _ZFP_ZFStylePropertyCopy(ZF_IN ZFObject *propertyOwner,
                                       ZF_IN const zfchar *propertyName,
                                       ZF_IN const zfchar *styleKey)
{
    zfautoObject styleValue = ZFStyleGet(styleKey);
    if(styleValue == zfnull)
    {
        return zffalse;
    }
    const ZFMethod *getterMethod = propertyOwner->classData()->propertyGetterForName(propertyName);
    if(getterMethod == zfnull || getterMethod->methodPrivilegeType() != ZFMethodPrivilegeTypePublic)
    {
        return zffalse;
    }
    const ZFMethod *setterMethod = propertyOwner->classData()->propertySetterForName(propertyName);
    if(setterMethod == zfnull || setterMethod->methodPrivilegeType() != ZFMethodPrivilegeTypePublic)
    {
        ZFStyleable *styleable = getterMethod->methodGenericInvoke(propertyOwner);
        if(styleable == zfnull)
        {
            return zffalse;
        }
        styleable->styleableCopyFrom(styleValue);
		return zftrue;
    }
    else
    {
        zfautoObject retDummy;
        zfautoObject paramDummy[ZFMETHOD_MAX_PARAM];
        paramDummy[0] = styleValue;
        return setterMethod->methodGenericInvoker()(setterMethod, propertyOwner, zfnull, retDummy, paramDummy);
    }
}
void _ZFP_ZFStyleKeyHolder::stylePropertyOnChange(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    zfCoreMutexLocker();
    ZFObject *ownerObj = userData->objectHolded();
    ZFStyleable *owner = ownerObj->to<ZFStyleable *>();
    zfstlmap<zfstlstringZ, zfstring> &m = owner->_ZFP_styleKey->stylePropertyKeyMap;
    for(zfstlmap<zfstlstringZ, zfstring>::iterator it = m.begin(); it != m.end(); ++it)
    {
        _ZFP_ZFStylePropertyCopy(ownerObj, it->first.c_str(), it->second);
    }
}
void ZFStyleable::styleKeyForProperty(ZF_IN const zfchar *propertyName, ZF_IN const zfchar *styleKey)
{
    if(propertyName == zfnull)
    {
        return;
    }
    zfCoreMutexLocker();
    if(styleKey == zfnull)
    {
        if(_ZFP_styleKey != zfnull)
        {
            _ZFP_styleKey->stylePropertyKeyMap.erase(propertyName);
            if(_ZFP_styleKey->stylePropertyKeyMap.empty())
            {
                ZFGlobalObserver().observerRemove(
                    ZFGlobalEvent::EventZFStyleOnChange(),
                    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStylePropertyChangeDataHolder)->stylePropertyOnChangeListener,
                    this->toObject()->objectHolder());
            }
        }
    }
    else
    {
        if(_ZFP_styleKey == zfnull)
        {
            _ZFP_styleKey = zfpoolNew(_ZFP_ZFStyleKeyHolder);
        }
        zfbool oldEmpty = _ZFP_styleKey->stylePropertyKeyMap.empty();
        _ZFP_styleKey->stylePropertyKeyMap[propertyName] = styleKey;
        if(oldEmpty)
        {
            ZFGlobalObserver().observerAdd(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStylePropertyChangeDataHolder)->stylePropertyOnChangeListener,
                this->toObject()->objectHolder());
        }
        if(!_ZFP_ZFStylePropertyCopy(this->toObject(), propertyName, styleKey))
        {
            _ZFP_styleKey->stylePropertyKeyMap.erase(propertyName);
            if(_ZFP_styleKey->stylePropertyKeyMap.empty())
            {
                ZFGlobalObserver().observerRemove(
                    ZFGlobalEvent::EventZFStyleOnChange(),
                    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStylePropertyChangeDataHolder)->stylePropertyOnChangeListener,
                    this->toObject()->objectHolder());
            }
            v_zfstring *param0 = zflockfree_zfAllocWithCache(v_zfstring);
            param0->zfv = propertyName;
            v_zfstring *param1 = zflockfree_zfAllocWithCache(v_zfstring);
            param1->zfv = styleKey;
            ZFGlobalObserver().observerNotifyWithCustomSender(
                this->toObject(),
                ZFGlobalEvent::EventZFStyleOnInvalid(),
                param0,
                param1);
            zflockfree_zfRelease(param0);
            zflockfree_zfRelease(param1);
        }
    }
}
const zfchar *ZFStyleable::styleKeyForProperty(ZF_IN const zfchar *propertyName)
{
    if(propertyName == zfnull)
    {
        return zfnull;
    }
    zfCoreMutexLocker();
    if(_ZFP_styleKey != zfnull)
    {
        zfstlmap<zfstlstringZ, zfstring>::iterator it = _ZFP_styleKey->stylePropertyKeyMap.find(propertyName);
        if(it != _ZFP_styleKey->stylePropertyKeyMap.end())
        {
            return it->second;
        }
        else
        {
            return zfnull;
        }
    }
    else
    {
        return zfnull;
    }
}

ZFINTERFACE_ON_DEALLOC_DEFINE(ZFStyleable)
{
    if(_ZFP_styleKey)
    {
        if(_ZFP_styleKey->styleKey != zfnull)
        {
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleChangeDataHolder)->styleOnChangeListener,
                this->toObject()->objectHolder());
            zffree(_ZFP_styleKey->styleKey);
            _ZFP_styleKey->styleKey = zfnull;
        }
        if(!_ZFP_styleKey->stylePropertyKeyMap.empty())
        {
            ZFGlobalObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStylePropertyChangeDataHolder)->stylePropertyOnChangeListener,
                this->toObject()->objectHolder());
        }
        zfpoolDelete(_ZFP_styleKey);
    }
}

ZF_NAMESPACE_GLOBAL_END

