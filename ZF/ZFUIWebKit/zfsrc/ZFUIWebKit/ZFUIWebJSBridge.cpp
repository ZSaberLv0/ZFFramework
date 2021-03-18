#include "ZFUIWebJSBridge.h"
#include "protocol/ZFProtocolZFUIWebJSBridge.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIWebJSBridgeSendData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWebJSBridgeSendData, ZFJsonItem, messageSend)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWebJSBridgeSendData, ZFJsonItem, messageResponse)

ZFOBJECT_REGISTER(ZFUIWebJSBridgeRecvData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWebJSBridgeRecvData, ZFJsonItem, messageRecv)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWebJSBridgeRecvData, ZFJsonItem, messageResponse)

// ============================================================
zfclassNotPOD _ZFP_ZFUIWebJSBridgePrivate
{
public:
    void *nativeWebJSBridge;
    ZFUIWebView *webView;

public:
    _ZFP_ZFUIWebJSBridgePrivate(void)
    : nativeWebJSBridge(zfnull)
    , webView(zfnull)
    {
    }
};

// ============================================================
// ZFUIWebJSBridge
ZFOBJECT_REGISTER(ZFUIWebJSBridge)

ZFOBSERVER_EVENT_REGISTER(ZFUIWebJSBridge, WebMessageBeforeSend)
ZFOBSERVER_EVENT_REGISTER(ZFUIWebJSBridge, WebMessageAfterSend)
ZFOBSERVER_EVENT_REGISTER(ZFUIWebJSBridge, WebMessageBeforeRecv)
ZFOBSERVER_EVENT_REGISTER(ZFUIWebJSBridge, WebMessageAfterRecv)

#define _ZFP_ZFUIWebJSBridge_tagKey "_ZFP_ZFUIWebJSBridge_tagKey"
ZFMETHOD_DEFINE_1(ZFUIWebJSBridge, ZFUIWebJSBridge *, instanceForWebView,
                  ZFMP_IN(ZFUIWebView *, webView))
{
    ZFUIWebJSBridge *ret = webView->objectTag<ZFUIWebJSBridge *>(_ZFP_ZFUIWebJSBridge_tagKey);
    if(ret == zfnull)
    {
        zfautoObject tmp = ZFUIWebJSBridge::ClassData()->newInstance();
        ret = tmp;
        ret->d->webView = webView;
        webView->objectTag(_ZFP_ZFUIWebJSBridge_tagKey, ret);
    }
    return ret;
}

ZFMETHOD_DEFINE_0(ZFUIWebJSBridge, ZFUIWebView *, webView)
{
    return d->webView;
}

ZFMETHOD_DEFINE_1(ZFUIWebJSBridge, ZFJsonItem, webMessageSend,
                  ZFMP_IN_OUT(ZFJsonItem &, messageSend))
{
    zfblockedAlloc(ZFUIWebJSBridgeSendData, dataSend);
    dataSend->messageSend = messageSend;
    this->webMessageBeforeSend(dataSend);
    dataSend->messageResponse = ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->webMessageSend(this, messageSend);
    this->webMessageAfterSend(dataSend);
    return dataSend->messageResponse;
}
ZFJsonItem ZFUIWebJSBridge::_ZFP_ZFUIWebJSBridge_notifyWebMessageRecv(ZF_IN_OUT ZFJsonItem &messageRecv)
{
    zfblockedAlloc(ZFUIWebJSBridgeRecvData, dataRecv);
    dataRecv->messageRecv = messageRecv;
    this->webMessageBeforeRecv(dataRecv);
    this->webMessageAfterRecv(dataRecv);
    return dataRecv->messageResponse;
}

void ZFUIWebJSBridge::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIWebJSBridgePrivate);
    d->nativeWebJSBridge = ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->nativeWebJSBridgeCreate(this);
}
void ZFUIWebJSBridge::objectOnDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFUIWebJSBridge)->nativeWebJSBridgeDestroy(this, d->nativeWebJSBridge);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

