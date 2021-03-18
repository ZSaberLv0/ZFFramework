/**
 * @page DocTag_Tutorial_Callback Callback
 *
 * reflectable method is not powerful enough to write high level code,
 * so we introduced ZFCallback to supply more powerful
 * abstract callback logic:
 * @code
 *   zfclass MyObject : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(MyObject, ZFObject)
 *
 *       ZFMETHOD_INLINE_1(zfstring, myMemberMethod, ZFMP_IN(zfindex, param0))
 *       {
 *           return zfstringWithFormat("called myMemberMethod, param: %zi", param0);
 *       }
 *   };
 *   static zfstring myStaticFunc(ZF_IN zfindex param0)
 *   {
 *       return zfstringWithFormat("called myStaticFunc, param: %zi", param0);
 *   }
 * @endcode
 * @code
 *   // prepare an object and a method
 *   zfblockedAlloc(MyObject, obj);
 *   const ZFMethod *method = ZFMethodForName("MyObject", "myMemberMethod");
 *
 *   // create callback from an object's member method
 *   ZFCallback callback1 = ZFCallbackForMemberMethod(obj, method);
 *
 *   // create callback from normal function
 *   ZFCallback callback2 = ZFCallbackForFunc(myStaticFunc);
 *
 *   // safe to assign new value to the callback,
 *   // even if method's proto type is not the same
 *   ZFCallback callbackTmp = callback1;
 *
 *   // now you can invoke the callback,
 *   // without knowing who owns it
 *   callback1.executeExact<zfstring, zfindex>(1);
 *   callback2.executeExact<zfstring, zfindex>(2);
 * @endcode
 *
 * ZFCallback can hold:
 * -  anything that was declared as ZFMethod,
 *   including class member method (static or not),
 *   static functions
 * -  normal function
 *
 * with ZFCallback, you may invoke a class member method
 * without knowing its class type,
 * and now we are free to play with delegate/protocol/listener/observer
 * or whatever-it-called mechanisms
 */

