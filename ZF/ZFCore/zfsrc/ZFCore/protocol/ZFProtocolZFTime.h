/**
 * @file ZFProtocolZFTime.h
 * @brief protocol for ZFTime
 */

#ifndef _ZFI_ZFProtocolZFTime_h_
#define _ZFI_ZFProtocolZFTime_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFTime.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFTime
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFTime)
public:
    /**
     * @brief see #ZFTime::leapYear
     */
    virtual zfbool leapYear(ZF_IN zfint year) zfpurevirtual;
    /**
     * @brief see #ZFTime::leapYearBetween
     */
    virtual zfint leapYearBetween(ZF_IN zfint year1, ZF_IN zfint year2) zfpurevirtual;
    /**
     * @brief see #ZFTime::timestamp
     */
    virtual zftimet timestamp(void) zfpurevirtual;
    /**
     * @brief see #ZFTime::currentTimeValue
     */
    virtual void currentTimeValue(ZF_OUT ZFTimeValue &tv) zfpurevirtual;
    /**
     * @brief see #ZFTime::timeInfoFromTimeValue
     */
    virtual zfbool timeInfoFromTimeValue(ZF_OUT ZFTimeInfo &ti, ZF_IN const ZFTimeValue &tv) zfpurevirtual;
    /**
     * @brief see #ZFTime::timeInfoToTimeValue
     */
    virtual zfbool timeInfoToTimeValue(ZF_OUT ZFTimeValue &tv, ZF_IN const ZFTimeInfo &ti) zfpurevirtual;
    /**
     * @brief see #ZFTime::timeZoneLocal
     */
    virtual const ZFTimeValue &timeZoneLocal(void) zfpurevirtual;
    /**
     * @brief see #ZFTime::timeInfoDayOfWeek
     */
    virtual zfuint timeInfoDayOfWeek(ZF_IN const ZFTimeValue &tv, ZF_IN const ZFTimeInfo &ti) zfpurevirtual;
    /**
     * @brief see #ZFTime::timeInfoDayOfYear
     */
    virtual zfuint timeInfoDayOfYear(ZF_IN const ZFTimeValue &tv, ZF_IN const ZFTimeInfo &ti) zfpurevirtual;
    /**
     * @brief true if time info is valid
     */
    virtual zfbool timeInfoIsValid(ZF_IN const ZFTimeInfo &ti) zfpurevirtual;
    /**
     * @brief true if time info is valid
     */
    virtual zfbool timeInfoIsValid(ZF_IN zfint year,
                                   ZF_IN zfuint month,
                                   ZF_IN zfuint day,
                                   ZF_IN zfuint hour,
                                   ZF_IN zfuint minute,
                                   ZF_IN zfuint second,
                                   ZF_IN zfuint miliSecond,
                                   ZF_IN zfuint microSecond) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFTime)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFTime_h_

