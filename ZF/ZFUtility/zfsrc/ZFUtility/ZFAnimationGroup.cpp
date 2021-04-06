#include "ZFAnimationGroup.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFAnimationGroupChildData
ZFOBJECT_REGISTER(ZFAnimationGroupChildData)

zfidentity ZFAnimationGroupChildData::objectHash(void)
{
    return zfidentityHash(ZFObjectHash(this->childAni())
        , this->childDelayBeforeStart()
        , this->childAutoCopyTarget()
    );
}
ZFCompareResult ZFAnimationGroupChildData::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(ZFObjectCompare(this->childAni(), another->childAni()) == ZFCompareTheSame
        && this->childDelayBeforeStart() == another->childDelayBeforeStart()
        && this->childAutoCopyTarget() == another->childAutoCopyTarget())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

// ============================================================
// _ZFP_ZFAnimationGroupPrivate
zfclass _ZFP_ZFAnimationGroupPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFAnimationGroupPrivate, ZFObject)

public:
    ZFAnimationGroup *pimplOwner;
    ZFArray *childAnis; // ZFAnimationGroupChildData *
private:
    /*
     * ZFAnimationGroupChildData *
     * for parallel group, stores all running child
     * for serial group, stores running child and all child to run
     */
    ZFArray *childBuf;
    zfbool cachedParallel;
    ZFListener cachedOnStartListener;
    ZFListener cachedOnStopListener;
    ZFCoreArrayPOD<zfidentity> childDelayTaskIds;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->childAnis = zfAlloc(ZFArray);
        this->childBuf = zfAlloc(ZFArray);
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(this->childAnis);
        zfRelease(this->childBuf);
        zfsuper::objectOnDealloc();
    }

public:
    void doStart(void)
    {
        if(this->childAnis->isEmpty())
        {
            this->pimplOwner->aniImplNotifyStop();
            return ;
        }
        if(this->cachedParallel != this->pimplOwner->aniParallel() || !this->cachedOnStartListener.callbackIsValid())
        {
            this->cachedParallel = this->pimplOwner->aniParallel();
            if(this->cachedParallel)
            {
                this->cachedOnStartListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(_ZFP_ZFAnimationGroupPrivate, onChildStart_parallel));
                this->cachedOnStopListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(_ZFP_ZFAnimationGroupPrivate, onChildStop_parallel));
            }
            else
            {
                this->cachedOnStartListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(_ZFP_ZFAnimationGroupPrivate, onChildStart_serial));
                this->cachedOnStopListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(_ZFP_ZFAnimationGroupPrivate, onChildStop_serial));
            }
        }

        if(this->cachedParallel)
        {
            this->doStartParallel();
        }
        else
        {
            this->doStartSerial();
        }
    }
    void doStop(void)
    {
        if(!this->childDelayTaskIds.isEmpty())
        {
            for(zfindex i = 0; i < this->childDelayTaskIds.count(); ++i)
            {
                ZFThreadExecuteCancel(this->childDelayTaskIds[i]);
            }
            this->childDelayTaskIds.removeAll();
        }
        if(!this->childBuf->isEmpty())
        {
            zfblockedAlloc(ZFArray, childToStop, this->childBuf);
            this->childBuf->removeAll();
            if(this->cachedParallel)
            {
                for(zfindex i = 0; i < childToStop->count(); ++i)
                {
                    ZFAnimationGroupChildData *childData = childToStop->get<ZFAnimationGroupChildData *>(i);
                    this->childCleanup(childData);
                    childData->childAni()->aniStop();
                }
            }
            else
            {
                for(zfindex i = 0; i < childToStop->count(); ++i)
                {
                    ZFAnimationGroupChildData *childData = childToStop->get<ZFAnimationGroupChildData *>(i);
                    this->childCleanup(childData);
                }
                childToStop->getFirst<ZFAnimationGroupChildData *>()->childAni()->aniStop();
            }
        }
    }

private:
    void doStartParallel(void)
    {
        zfidentity aniId = this->pimplOwner->aniId();
        zfblockedAlloc(ZFArray, tmpArray, this->childAnis);
        this->childBuf->addFrom(this->childAnis);
        for(zfindex i = 0; aniId == this->pimplOwner->aniId() && i < tmpArray->count(); ++i)
        {
            ZFAnimationGroupChildData *childData = tmpArray->get<ZFAnimationGroupChildData *>(i);
            this->childSetup(childData);
            this->childStart(childData, aniId);
        }
    }
    void doStartSerial(void)
    {
        this->childBuf->addFrom(this->childAnis);
        ZFAnimationGroupChildData *childData = this->childBuf->getFirst<ZFAnimationGroupChildData *>();
        this->childSetup(childData);
        this->childStart(childData, this->pimplOwner->aniId());
    }

private:
    void childSetup(ZF_IN ZFAnimationGroupChildData *childData)
    {
        childData->childAni()->observerAdd(ZFAnimation::EventAniOnStart(), this->cachedOnStartListener);
        childData->childAni()->observerAdd(ZFAnimation::EventAniOnStop(), this->cachedOnStopListener);
    }
    void childCleanup(ZF_IN ZFAnimationGroupChildData *childData)
    {
        childData->childAni()->observerRemove(ZFAnimation::EventAniOnStart(), this->cachedOnStartListener);
        childData->childAni()->observerRemove(ZFAnimation::EventAniOnStop(), this->cachedOnStopListener);
    }
private:
    void childStart(ZF_IN ZFAnimationGroupChildData *childData, ZF_IN zfidentity aniId)
    {
        if(childData->childDelayBeforeStart() <= 0)
        {
            childData->childAni()->aniStart();
        }
        else
        {
            zfidentity childDelayTaskId = ZFThreadExecuteInMainThreadAfterDelay(
                childData->childDelayBeforeStart(),
                ZFCallbackForMemberMethod(this, ZFMethodAccess(_ZFP_ZFAnimationGroupPrivate, onChildStartDelay)),
                zfnull,
                ZFListenerData()
                    .param0(zflineAlloc(v_zfidentity, aniId))
                    .param1(childData)
                );
            if(childDelayTaskId != zfidentityInvalid())
            {
                this->childDelayTaskIds.add(childDelayTaskId);
            }
        }
    }
    ZFMETHOD_INLINE_2(void, onChildStartDelay,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        zfidentity aniId = listenerData.param0<v_zfidentity *>()->zfv;
        ZFAnimationGroupChildData *childData = listenerData.param1<ZFAnimationGroupChildData *>();
        if(aniId == this->pimplOwner->aniId())
        {
            childData->childAni()->aniStart();
        }
    }
private:
    ZFAnimationGroupChildData *checkChild(ZF_IN ZFObject *childAni, ZF_IN zfbool autoRemove = zffalse)
    {
        for(zfindex i = 0; i < this->childBuf->count(); ++i)
        {
            ZFAnimationGroupChildData *childData = this->childBuf->get<ZFAnimationGroupChildData *>(i);
            if(childData->childAni() == childAni)
            {
                if(autoRemove)
                { // retained in childAnis, safe to be removed directly
                    this->childBuf->remove(i);
                }
                return childData;
            }
        }
        return zfnull;
    }
private:
    ZFMETHOD_INLINE_2(void, onChildStart_parallel,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        ZFAnimationGroupChildData *childData = this->checkChild(listenerData.sender());
        if(childData == zfnull)
        {
            return ;
        }
        this->pimplOwner->aniGroupOnChildStart(childData->childAni());
    }
    ZFMETHOD_INLINE_2(void, onChildStop_parallel,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        ZFAnimationGroupChildData *childData = this->checkChild(listenerData.sender(), zfHint("autoRemove")zftrue);
        if(childData == zfnull)
        {
            return ;
        }
        this->childCleanup(childData);
        this->pimplOwner->aniGroupOnChildStop(childData->childAni());

        if(this->childBuf->isEmpty())
        {
            this->pimplOwner->aniImplNotifyStop();
        }
    }
    ZFMETHOD_INLINE_2(void, onChildStart_serial,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        ZFAnimationGroupChildData *childData = this->childBuf->getFirst<ZFAnimationGroupChildData *>();
        if(childData == zfnull || childData->childAni() != listenerData.sender())
        {
            return ;
        }
        this->pimplOwner->aniGroupOnChildStart(childData->childAni());
    }
    ZFMETHOD_INLINE_2(void, onChildStop_serial,
                      ZFMP_IN(const ZFListenerData &, listenerData),
                      ZFMP_IN(ZFObject *, userData))
    {
        ZFAnimationGroupChildData *childData = this->childBuf->getFirst<ZFAnimationGroupChildData *>();
        if(childData == zfnull || childData->childAni() != listenerData.sender())
        {
            return ;
        }
        this->childBuf->removeFirst();
        this->childCleanup(childData);
        this->pimplOwner->aniGroupOnChildStop(childData->childAni());

        if(this->childBuf->isEmpty())
        {
            this->pimplOwner->aniImplNotifyStop();
        }
        else
        {
            childData = this->childBuf->getFirst<ZFAnimationGroupChildData *>();
            this->childSetup(childData);
            this->childStart(childData, this->pimplOwner->aniId());
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimationGroup)

ZFOBSERVER_EVENT_REGISTER(ZFAnimationGroup, AniGroupOnChildStart)
ZFOBSERVER_EVENT_REGISTER(ZFAnimationGroup, AniGroupOnChildStop)

// ============================================================
// serialize
zfbool ZFAnimationGroup::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                         ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->childAniRemoveAll();

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFAnimationGroup_child))
        {
            zfautoObject element;
            if(!ZFObjectFromData(element, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(element == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "null child");
                return zffalse;
            }
            if(!element.toObject()->classData()->classIsTypeOf(ZFAnimationGroupChildData::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "object %s not type of %s", element.toObject()->objectInfoOfInstance().cString(), ZFAnimationGroupChildData::ClassData());
                return zffalse;
            }
            ZFAnimationGroupChildData *childData = ZFCastZFObjectUnchecked(ZFAnimationGroupChildData *, element.toObject());
            if(childData->childAni() == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "null child animation");
                return zffalse;
            }
            this->childAniDataAdd(childData);

            categoryData.resolveMark();
        }
    }
    return zftrue;
}
zfbool ZFAnimationGroup::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                       ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull)
    {
        for(zfindex i = 0; i < d->childAnis->count(); ++i)
        {
            ZFSerializableData elementData;
            if(!ZFObjectToData(elementData, d->childAnis->get(i), outErrorHint))
            {
                return zffalse;
            }
            elementData.category(ZFSerializableKeyword_ZFAnimationGroup_child);
            serializableData.elementAdd(elementData);
        }
    }
    else
    {
        zfbool mismatch = zffalse;
        if(this->childAniCount() != ref->childAniCount())
        {
            mismatch = zftrue;
        }
        else
        {
            for(zfindex i = 0; i < d->childAnis->count(); ++i)
            {
                if(ZFObjectCompare(d->childAnis->get(i), ref->d->childAnis->get(i)) != ZFCompareTheSame)
                {
                    mismatch = zftrue;
                    break;
                }
            }
        }
        if(mismatch)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                "animation group contents mismatch, this: %s, ref: %s",
                d->childAnis->objectInfoOfContent().cString(), ref->d->childAnis->objectInfoOfContent().cString());
            return zffalse;
        }
    }

    return zftrue;
}

// ============================================================
// object
void ZFAnimationGroup::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_ZFAnimationGroupPrivate);
    d->pimplOwner = this;
}
void ZFAnimationGroup::objectOnDealloc(void)
{
    zfRelease(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAnimationGroup::objectOnDeallocPrepare(void)
{
    this->aniStop();
    this->childAniRemoveAll();
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFAnimationGroup::objectHash(void)
{
    return zfidentityHash(zfsuper::objectHash()
        , d->childAnis->objectHash()
        , this->aniParallel()
    );
}
ZFCompareResult ZFAnimationGroup::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(zfsuper::objectCompare(anotherObj) != ZFCompareTheSame)
    {
        return ZFCompareUncomparable;
    }
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(ZFObjectCompare(d->childAnis, another->d->childAnis) == ZFCompareTheSame
       && this->aniParallel() == another->aniParallel())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

// ============================================================
// child animation control
ZFMETHOD_DEFINE_1(ZFAnimationGroup, void, childAniAdd,
                  ZFMP_IN(ZFAnimation *, ani))
{
    zfblockedAlloc(ZFAnimationGroupChildData, childData);
    childData->childAni(ani);
    this->childAniDataAdd(childData);
}
ZFMETHOD_DEFINE_1(ZFAnimationGroup, void, childAniDataAdd,
                  ZFMP_IN(ZFAnimationGroupChildData *, childData))
{
    zfCoreAssert(childData != zfnull);
    zfCoreAssert(childData->childAni() != zfnull);
    zfCoreAssertWithMessage(!this->aniRunning(), "you must not modify child animation while group is running");
    d->childAnis->add(childData);
}
ZFMETHOD_DEFINE_0(ZFAnimationGroup, zfindex, childAniCount)
{
    return d->childAnis->count();
}
ZFMETHOD_DEFINE_1(ZFAnimationGroup, ZFAnimation *, childAniAtIndex,
                  ZFMP_IN(zfindex, index))
{
    return d->childAnis->get<ZFAnimationGroupChildData *>(index)->childAni();
}
ZFMETHOD_DEFINE_1(ZFAnimationGroup, ZFAnimationGroupChildData *, childAniDataAtIndex,
                  ZFMP_IN(zfindex, index))
{
    return d->childAnis->get<ZFAnimationGroupChildData *>(index);
}
ZFMETHOD_DEFINE_1(ZFAnimationGroup, void, childAniRemove,
                  ZFMP_IN(zfindex, index))
{
    d->childAnis->remove(index);
}
ZFMETHOD_DEFINE_0(ZFAnimationGroup, void, childAniRemoveAll)
{
    d->childAnis->removeAll();
}

// ============================================================
// start stop
zfbool ZFAnimationGroup::aniImplCheckValid(void)
{
    if(!zfsuper::aniImplCheckValid())
    {
        return zffalse;
    }
    for(zfindex i = 0; i < d->childAnis->count(); ++i)
    {
        ZFAnimationGroupChildData *childData = d->childAnis->get<ZFAnimationGroupChildData *>(i);
        if(childData->childAutoCopyTarget() && this->aniTarget() != zfnull)
        {
            childData->childAni()->aniTarget(this->aniTarget());
        }
        if(childData->childAutoCopyDuration())
        {
            childData->childAni()->aniDuration(this->aniDuration());
        }
        if(!childData->childAni()->aniValid())
        {
            return zffalse;
        }
    }
    return zftrue;
}
void ZFAnimationGroup::aniImplStart(void)
{
    zfsuper::aniImplStart();
    d->doStart();
}
void ZFAnimationGroup::aniImplStop(void)
{
    d->doStop();
    zfsuper::aniImplStop();
}

ZF_NAMESPACE_GLOBAL_END

