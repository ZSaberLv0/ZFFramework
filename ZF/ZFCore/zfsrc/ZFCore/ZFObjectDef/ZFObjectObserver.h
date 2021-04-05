/**
 * @file ZFObjectObserver.h
 * @brief observer types for #ZFObject
 */

#ifndef _ZFI_ZFObjectObserver_h_
#define _ZFI_ZFObjectObserver_h_

#include "ZFCallback.h"
#include "ZFIdMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFListenerData
/**
 * @brief listener data used by #ZFListener
 *
 * @note this object only hold the necessary datas as pointer,
 *   without auto retain or release logic
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFListenerData
{
    ZFCORE_PARAM_DECLARE_SELF(ZFListenerData)
public:
    /**
     * @brief event id, may be #zfidentityInvalid
     */
    ZFCORE_PARAM_WITH_INIT(zfidentity, eventId, zfidentityInvalid())
    /**
     * @brief who notified the listener event, may be null
     */
    ZFCORE_PARAM(ZFObject *, sender)
    /**
     * @brief params, may be null
     */
    ZFCORE_PARAM(ZFObject *, param0)
    /**
     * @brief params, may be null
     */
    ZFCORE_PARAM(ZFObject *, param1)

    /** @brief util to #sender */
    template<typename T_ZFObject>
    T_ZFObject sender(void) const
    {
        return ZFCastZFObject(T_ZFObject, this->sender());
    }
    /** @brief util to #param0 */
    template<typename T_ZFObject>
    T_ZFObject param0(void) const
    {
        return ZFCastZFObject(T_ZFObject, this->param0());
    }
    /** @brief util to #param1 */
    template<typename T_ZFObject>
    T_ZFObject param1(void) const
    {
        return ZFCastZFObject(T_ZFObject, this->param1());
    }

public:
    // ============================================================
    // event filter logic
    zfbool _ZFP_eventFilteredHolder;
    zfbool *_ZFP_eventFiltered;

    /**
     * @brief used to achieve event filter logic
     *
     * to achieve event filter,
     * you may attach an #ZFObserverHolder::observerAdd with higher #ZFLevel,
     * and set #eventFiltered to true,
     * then the event would not be further dispatched
     */
    inline ZFListenerData const &eventFiltered(ZF_IN zfbool eventFiltered) const {if(this->_ZFP_eventFiltered) {*(this->_ZFP_eventFiltered) = eventFiltered;} return *this;}
    /** @brief see #eventFiltered */
    inline ZFListenerData &eventFiltered(ZF_IN zfbool eventFiltered) {if(this->_ZFP_eventFiltered) {*(this->_ZFP_eventFiltered) = eventFiltered;} return *this;}
    /** @brief see #eventFiltered */
    inline zfbool eventFiltered(void) const {if(this->_ZFP_eventFiltered) {return *(this->_ZFP_eventFiltered);} else {return zffalse;}}

    /** @brief see #eventFiltered */
    inline ZFListenerData &eventFilterEnable(void) {this->_ZFP_eventFiltered = &(this->_ZFP_eventFilteredHolder); return *this;}

    // ============================================================
public:
    /**
     * @brief main constructor
     */
    ZFListenerData(void)
    : _ZFP_eventFilteredHolder(zffalse)
    , _ZFP_eventFiltered(zfnull)
    {
    }
    /**
     * @brief construct with sender and params
     */
    ZFListenerData(ZF_IN zfidentity eventId,
                   ZF_IN ZFObject *sender,
                   ZF_IN_OPT ZFObject *param0 = zfnull,
                   ZF_IN_OPT ZFObject *param1 = zfnull)
    : _ZFP_eventFilteredHolder(zffalse)
    , _ZFP_eventFiltered(zfnull)
    {
        this->eventId(eventId);
        this->sender(sender);
        this->param0(param0);
        this->param1(param1);
    }
    /**
     * @brief construct with another data
     */
    ZFListenerData(ZF_IN const ZFListenerData &ref)
    : _ZFP_eventFilteredHolder(ref._ZFP_eventFilteredHolder)
    , _ZFP_eventFiltered(zfnull)
    {
        this->operator = (ref);
    }

public:
    /** @cond ZFPrivateDoc */
    ZFListenerData &operator = (ZF_IN const ZFListenerData &ref)
    {
        this->eventId(ref.eventId());
        this->sender(ref.sender());
        this->param0(ref.param0());
        this->param1(ref.param1());
        this->_ZFP_eventFilteredHolder = ref._ZFP_eventFilteredHolder;
        if(ref._ZFP_eventFiltered)
        {
            this->eventFilterEnable();
        }
        return *this;
    }
    zfbool operator == (ZF_IN const ZFListenerData &ref) const
    {
        return (zftrue
                && this->eventId() == ref.eventId()
                && this->sender() == ref.sender()
                && this->param0() == ref.param0()
                && this->param1() == ref.param1()
                && this->_ZFP_eventFilteredHolder == ref._ZFP_eventFilteredHolder
            );
    }
    inline zfbool operator != (ZF_IN const ZFListenerData &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief get a short info of this object */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
};

// ============================================================
/**
 * @brief listener as ZFCallback, mostly used by #ZFObject::observerNotify
 *
 * listeners usually achieved by observer logic in ZFObject,
 * see #ZFObject::observerNotify for more info\n
 * \n
 * what params means, is determined by invoker of the listener,
 * see each listener's comments before use it
 */
ZFCALLBACK_DECLARE_BEGIN(ZFListener, ZFCallback)
public:
    /** @brief see #ZFListener */
    inline void execute(void) const
    {
        ZFListenerData listenerData;
        ZFCallback::executeExact<void, const ZFListenerData &, ZFObject *>(listenerData, zfnull);
    }
    /** @brief see #ZFListener */
    inline void execute(ZF_IN const ZFListenerData &listenerData,
                        ZF_IN_OPT ZFObject *userData = zfnull) const
    {
        ZFCallback::executeExact<void, const ZFListenerData &, ZFObject *>(listenerData, userData);
    }
    /** @brief see #ZFListener */
    inline void operator () (void) const
    {
        ZFListenerData listenerData;
        ZFCallback::executeExact<void, const ZFListenerData &, ZFObject *>(listenerData, zfnull);
    }
    /** @brief see #ZFListener */
    inline void operator () (ZF_IN const ZFListenerData &listenerData,
                             ZF_IN_OPT ZFObject *userData = zfnull) const
    {
        ZFCallback::executeExact<void, const ZFListenerData &, ZFObject *>(listenerData, userData);
    }
_ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFListener, ZFCallback)

// ============================================================
// ZFObserverHolder
/** @brief see #ZFObject::observerNotify */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFObserverAddParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFObserverAddParam)

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM_WITH_INIT(zfidentity, eventId, zfidentityInvalid())

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM(ZFListener, observer)

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM_WITH_INIT(ZFObject *, userData, zfnull)

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM_WITH_INIT(ZFObject *, owner, zfnull)

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM_WITH_INIT(zfbool, autoRemoveAfterActivate, zffalse)

    /** @brief see #ZFObject::observerNotify */
    ZFCORE_PARAM_WITH_INIT(ZFLevel, observerLevel, ZFLevelAppNormal)

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN ZFObserverAddParam const &ref) const
    {
        return (this->eventId() == ref.eventId()
            && this->observer() == ref.observer()
            && this->userData() == ref.userData()
            && this->owner() == ref.owner()
            && this->autoRemoveAfterActivate() == ref.autoRemoveAfterActivate()
            && this->observerLevel() == ref.observerLevel()
            );
    }
    inline zfbool operator != (ZF_IN ZFObserverAddParam const &ref) const {return !this->operator == (ref);}
    /** @endcond */
};

zfclassFwd _ZFP_ZFObserverHolderPrivate;
/**
 * @brief holder object for observer logic, see #ZFObject::observerNotify
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFObserverHolder
{
public:
    /** @cond ZFPrivateDoc */
    ZFObserverHolder(void);
    ZFObserverHolder(ZF_IN ZFObserverHolder const &ref);
    ~ZFObserverHolder(void);
    ZFObserverHolder &operator = (ZF_IN ZFObserverHolder const &ref);
    zfbool operator == (ZF_IN ZFObserverHolder const &ref) const;
    inline zfbool operator != (ZF_IN ZFObserverHolder const &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #ZFObject::observerNotify */
    zffinal zfidentity observerAdd(ZF_IN zfidentity eventId,
                                   ZF_IN const ZFListener &observer,
                                   ZF_IN_OPT ZFObject *userData = zfnull,
                                   ZF_IN_OPT ZFObject *owner = zfnull,
                                   ZF_IN_OPT zfbool autoRemoveAfterActivate = zffalse,
                                   ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal zfidentity observerAdd(ZF_IN const ZFObserverAddParam &param) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal inline zfidentity observerAddForOnce(ZF_IN zfidentity eventId,
                                                 ZF_IN const ZFListener &observer,
                                                 ZF_IN_OPT ZFObject *userData = zfnull,
                                                 ZF_IN_OPT ZFObject *owner = zfnull,
                                                 ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal) const
    {
        return this->observerAdd(eventId, observer, userData, owner, zftrue, observerLevel);
    }
    /** @brief see #ZFObject::observerMoveToFirst */
    zffinal void observerMoveToFirst(ZF_IN zfidentity taskId) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemove(ZF_IN zfidentity eventId,
                                ZF_IN const ZFListener &callback,
                                ZF_IN_OPT ZFObject *userData = zfnull,
                                ZF_IN_OPT ZFComparer<ZFObject *>::Comparer userDataComparer = ZFComparerCheckEqual) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveByTaskId(ZF_IN zfidentity taskId) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveByOwner(ZF_IN ZFObject *owner) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveAll(ZF_IN zfidentity eventId) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerRemoveAll(void) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal zfbool observerHasAdd(void) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal zfbool observerHasAdd(ZF_IN zfidentity eventId) const;
    /** @brief see #ZFObject::observerNotify */
    zffinal inline void observerNotify(ZF_IN zfidentity eventId,
                                       ZF_IN_OPT ZFObject *param0 = zfnull,
                                       ZF_IN_OPT ZFObject *param1 = zfnull) const
    {
        this->observerNotifyWithCustomSender(this->observerOwner(), eventId, param0, param1);
    }
    /** @brief see #ZFObject::observerNotify */
    zffinal void observerNotifyWithCustomSender(ZF_IN ZFObject *customSender,
                                                ZF_IN zfidentity eventId,
                                                ZF_IN_OPT ZFObject *param0 = zfnull,
                                                ZF_IN_OPT ZFObject *param1 = zfnull) const;

public:
    /**
     * @brief attach a state that indicate whether the observer has add,
     *   for performance usage
     *
     * notifying an observer requires much CPU to check and execute,
     * even if no observer has ever added\n
     * to optimize this, you may attach a flag to indicate whether
     * any observer has ever added\n
     * usage:
     * @code
     *   enum {
     *       Flag_MyEvent0,
     *       Flag_MyEvent1,
     *   };
     *   zfuint myFlag = 0;
     *   observer.observerHasAddStateAttach(EventXXX, &myFlag, Flag_MyEvent0);
     *   observer.observerHasAddStateAttach(EventXXX, &myFlag, Flag_MyEvent1);
     *
     *   if(ZFBitTest(myFlag, Flag_MyEvent0))
     *   {
     *       observer.observerNotify(...);
     *   }
     * @endcode
     */
    zffinal void observerHasAddStateAttach(ZF_IN zfidentity eventId,
                                           ZF_IN_OUT zfuint *flag,
                                           ZF_IN_OUT zfuint flagBit);
    /**
     * @brief see #observerHasAddStateAttach
     */
    zffinal void observerHasAddStateDetach(ZF_IN zfidentity eventId,
                                           ZF_IN_OUT zfuint *flag,
                                           ZF_IN_OUT zfuint flagBit);

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_OUT zfstring &ret) const;
    /**
     * @brief return a short string describe the object
     */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief owner object of this observer holder, or null if none */
    inline ZFObject *observerOwner(void) const
    {
        return this->_observerOwner;
    }
    zffinal void _ZFP_ZFObserverHolder_observerOwner(ZF_IN ZFObject *obj) const;

private:
    _ZFP_ZFObserverHolderPrivate *d;
    ZFObject *_observerOwner;
};

// ============================================================
extern ZF_ENV_EXPORT ZFObserverHolder &_ZFP_ZFObjectGlobalEventObserverRef(void);
/**
 * @brief all event notified by #ZFObject::observerNotify would also be notified to the observers added by this method,
 *   see #ZFObject::observerNotify
 *
 * use only if necessary, which may cause performance issue
 */
#define ZFObjectGlobalEventObserver() (_ZFP_ZFObjectGlobalEventObserverRef())

// ============================================================
// observer
/**
 * @brief see #ZFObject::observerNotify
 *
 * usage:
 * @code
 *   zfclass YourClass ...
 *   {
 *       ZFOBSERVER_EVENT(YourEvent)
 *   };
 *
 *   ZFOBSERVER_EVENT_REGISTER(YourClass, YourEvent)
 * @endcode
 * declare a event for ZFObject's observer logic,
 * declared event name can be accessed by:
 * @code
 *   zfidentity eventId = YourClass::EventYourEvent();
 *   const zfchar *eventName = ZFIdMapNameForId(eventId);
 * @endcode
 * note that subclass may declare a event same as parent,
 * while the final event name is different:\n
 *   ParentClass::EventYourEvent() => "ParentClassEventYourEvent"\n
 *   ChildClass::EventYourEvent() => "ChildClassEventYourEvent"\n
 * \n
 * @note we declare the event id as int types for performance,
 *   it's ensured each event has different event id,
 *   but it's only ensured while app is running,
 *   after relaunching the app,
 *   the event id is not ensured the same,
 *   you should use the name of the event to store or pass between apps,
 *   and you can use #ZFIdMapIdForName or #ZFIdMapNameForId
 *   to convert them easily
 */
#define ZFOBSERVER_EVENT(YourEvent) \
    ZFIDMAP_DETAIL(Event, YourEvent)

/** @brief see #ZFOBSERVER_EVENT */
#define ZFOBSERVER_EVENT_REGISTER(YourClass, YourEvent) \
    ZFIDMAP_REGISTER_DETAIL(YourClass, Event, YourEvent)

// ============================================================
/**
 * @brief declare a observer event in global scope, see #ZFOBSERVER_EVENT
 *
 * usage:
 * @code
 *   // in header files
 *   ZF_NAMESPACE_BEGIN(YourNamespace)
 *   / ** @brief you can add doxygen docs here * /
 *   ZFOBSERVER_EVENT_GLOBAL(YourEvent)
 *   ZF_NAMESPACE_END(YourNamespace)
 *
 *   ZFOBSERVER_EVENT_GLOBAL_REGISTER(YourNamespace, YourEvent)
 *
 *   // use the event
 *   zfidentity eventId = YourNamespace::EventYourEvent();
 * @endcode
 * unlike #ZFOBSERVER_EVENT, this macro would declare event outside of class scope,
 * typically you should use #ZFOBSERVER_EVENT_GLOBAL which have "ZFGlobalEvent" as namespace
 */
#define ZFOBSERVER_EVENT_GLOBAL(YourEvent) \
    ZFIDMAP_GLOBAL_DETAIL(Event, YourEvent)

/** @brief see #ZFOBSERVER_EVENT */
#define ZFOBSERVER_EVENT_GLOBAL_REGISTER(YourEvent) \
    ZFIDMAP_GLOBAL_REGISTER_DETAIL(Event, YourEvent)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectObserver_h_

