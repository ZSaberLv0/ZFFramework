/**
 * @page DocTag_Tutorial_Observer Observer
 *
 * with ZFCallback available,
 * it's easy to use observer mechanism
 * @code
 *   zfclass MyObject : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(MyObject, ZFObject)
 *
 *       ZFOBSERVER_EVENT(MyEvent)
 *
 *   public:
 *       void test(void)
 *       {
 *           zfblockedAlloc(ZFObject, myParam0);
 *           zfblockedAlloc(ZFObject, myParam1);
 *           this->observerNotify(
 *               MyObject::EventMyEvent(), // event id to notify
 *               myParam0, // param0
 *               myParam1); // param1
 *       }
 *   };
 *   static ZFLISTENER_PROTOTYPE_EXPAND(testListener)
 *   {
 *       zfLogT()
 *           << listenerData.sender() // should be MyObject
 *           << listenerData.param0() // should be myParam0 passed from observerNotify
 *           << listenerData.param1() // should be myParam1 passed from observerNotify
 *           << userData // should be the userData passed from observerAdd
 *           ;
 *   }
 * @endcode
 * @code
 *   // the object to test observer
 *   zfblockedAlloc(MyObject, obj);
 *   // the object to test userData passing
 *   zfblockedAlloc(ZFObject, myUserData);
 *
 *   // we use ZFListener as observer callback type,
 *   // which takes two param as ZFListenerData and ZFObject,
 *   // to create it,
 *   // use ZFCallbackForXXX series same as ZFCallback
 *   ZFListener listener1 = ZFCallbackForFunc(testListener);
 *
 *   // add an observer
 *   obj->observerAdd(MyObject::EventMyEvent(), listener1, myUserData);
 *
 *   // optionally, you may declare a lambda-like listener for short
 *   ZFLISTENER_LOCAL(listener2, {
 *       testListener(listenerData, userData);
 *   })
 *   obj->observerAdd(MyObject::EventMyEvent(), listener2, myUserData);
 *
 *   // finlly we will test to notify the event
 *   obj->test();
 *
 *   // or, you may notify it manually
 *   // but it's your responsibility to ensure the passed params suit for the listener
 *   zfblockedAlloc(ZFObject, myParam0);
 *   zfblockedAlloc(ZFObject, myParam1);
 *   obj->observerNotify(MyObject::EventMyEvent(), myParam0, myParam1);
 * @endcode
 */

