#include "ZFEnvInfo_ZFUIKit.h"
#include "protocol/ZFProtocolZFEnvInfo_ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFEnvDeviceUIType)

ZF_NAMESPACE_BEGIN(ZFEnvInfo)

ZFMETHOD_FUNC_DEFINE_1(ZFEnvDeviceUITypeEnum, deviceUIType,
                       ZFMP_IN_OPT(ZFEnvDeviceUITypeEnum, defaultValue, ZFEnvDeviceUIType::e_Desktop))
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFEnvInfo_deviceUIInfo) *impl = ZFPROTOCOL_TRY_ACCESS(ZFEnvInfo_deviceUIInfo);
    if(impl != zfnull)
    {
        return impl->deviceUIType();
    }
    else
    {
        return defaultValue;
    }
}

ZF_NAMESPACE_END(ZFEnvInfo)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFEnvSummary_ZFUIKit, ZFLevelZFFrameworkNormal)
{
    ZFEnvInfo::envSummaryCallbackRegister("deviceUIType", zfself::deviceUITypeSummary);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFEnvSummary_ZFUIKit)
{
    ZFEnvInfo::envSummaryCallbackUnregister("deviceUIType");
}
public:
    static void deviceUITypeSummary(ZF_IN_OUT zfstring &ret)
    {
        ret += ZFEnvDeviceUIType::EnumNameForValue(ZFEnvInfo::deviceUIType());
    }
ZF_GLOBAL_INITIALIZER_END(ZFEnvSummary_ZFUIKit)

ZF_NAMESPACE_GLOBAL_END

