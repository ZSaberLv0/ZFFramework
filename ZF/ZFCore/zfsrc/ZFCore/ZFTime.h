/**
 * @file ZFTime.h
 * @brief time utility
 */

#ifndef _ZFI_ZFTime_h_
#define _ZFI_ZFTime_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFTimeValue
/**
 * @brief store a time value
 */
zfclassPOD ZF_ENV_EXPORT ZFTimeValue
{
public:
    zftimet sec;     /**< second */
    zftimet usec;    /**< micro second */
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTimeValue
 *       value="(0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFTimeValue, ZFTimeValue)

ZFOUTPUT_TYPE(ZFTimeValue, {output << ZFTimeValueToString(v);})

ZFCORE_POD_COMPARER_DECLARE(ZFTimeValue)

/**
 * @brief a zero time value
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFTimeValue, ZFTimeValueZero)
/**
 * @brief make a time value
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFTimeValue, ZFTimeValueMake,
                               ZFMP_IN(zftimet, sec),
                               ZFMP_IN(zftimet, usec))
{
    ZFTimeValue ret = {sec, usec};
    return ret;
}

/**
 * @brief convert between #ZFTimeValue and #zftimet (in miliseconds)
 *
 * note, convert from #ZFTimeValue to #zftimet would lose accuracy
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(ZFTimeValue, ZFTimeValueFromMiliSeconds,
                               ZFMP_IN(zftimet, t))
{
    ZFTimeValue ret = {(zftimet)(t / 1000), (zftimet)((t % 1000) * 1000)};
    return ret;
}
/**
 * @brief convert between #ZFTimeValue and #zftimet (in miliseconds)
 *
 * note, convert from #ZFTimeValue to #zftimet would lose accuracy
 */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zftimet, ZFTimeValueToMiliSeconds,
                               ZFMP_IN(const ZFTimeValue &, v))
{
    return (zftimet)(v.sec * 1000 + v.usec / 1000);
}

/**
 * @brief normalize time value
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFTimeValueNormalize,
                        ZFMP_IN_OUT(ZFTimeValue &, v))

/**
 * @brief return tv1 - tv2
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFTimeValueDec,
                               ZFMP_OUT(ZFTimeValue &, result),
                               ZFMP_IN(const ZFTimeValue &, tv1),
                               ZFMP_IN(const ZFTimeValue &, tv2))
{
    result.sec = tv1.sec - tv2.sec;
    result.usec = tv1.usec - tv2.usec;
    ZFTimeValueNormalize(result);
}
/**
 * @brief return tv1 - tv2
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFTimeValue, ZFTimeValueDec,
                               ZFMP_IN(const ZFTimeValue &, tv1),
                               ZFMP_IN(const ZFTimeValue &, tv2))
{
    ZFTimeValue result;
    ZFTimeValueDec(result, tv1, tv2);
    return result;
}

/**
 * @brief return tv1 + tv2
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFTimeValueInc,
                               ZFMP_OUT(ZFTimeValue &, result),
                               ZFMP_IN(const ZFTimeValue &, tv1),
                               ZFMP_IN(const ZFTimeValue &, tv2))
{
    result.sec = tv1.sec + tv2.sec;
    result.usec = tv1.usec + tv2.usec;
    ZFTimeValueNormalize(result);
}
/**
 * @brief return tv1 + tv2
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFTimeValue, ZFTimeValueInc,
                               ZFMP_IN(const ZFTimeValue &, tv1),
                               ZFMP_IN(const ZFTimeValue &, tv2))
{
    ZFTimeValue result;
    ZFTimeValueInc(result, tv1, tv2);
    return result;
}

/**
 * @brief return tv * v
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(void, ZFTimeValueMul,
                               ZFMP_OUT(ZFTimeValue &, result),
                               ZFMP_IN(const ZFTimeValue &, tv),
                               ZFMP_IN(zfindex, v))
{
    result.sec = tv.sec * v;
    result.usec = tv.usec * v;
    ZFTimeValueNormalize(result);
}
/**
 * @brief return tv * v
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFTimeValue, ZFTimeValueMul,
                               ZFMP_IN(const ZFTimeValue &, tv),
                               ZFMP_IN(zfindex, v))
{
    ZFTimeValue result;
    ZFTimeValueMul(result, tv, v);
    return result;
}

/**
 * @brief return tv / v, assert fail if v is 0
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFTimeValueDiv,
                        ZFMP_OUT(ZFTimeValue &, result),
                        ZFMP_IN(const ZFTimeValue &, tv),
                        ZFMP_IN(zfindex, v))
/**
 * @brief return tv / v, assert fail if v is 0
 */
ZFMETHOD_FUNC_INLINE_DECLARE_2(ZFTimeValue, ZFTimeValueDiv,
                               ZFMP_IN(const ZFTimeValue &, tv),
                               ZFMP_IN(zfindex, v))
{
    ZFTimeValue result;
    ZFTimeValueDiv(result, tv, v);
    return result;
}

/**
 * @brief compare two ZFTimeValue
 * @see ZFCompareResult
 */
ZFMETHOD_FUNC_DECLARE_2(ZFCompareResult, ZFTimeValueCompare,
                        ZFMP_IN(const ZFTimeValue &, tv1),
                        ZFMP_IN(const ZFTimeValue &, tv2))

/**
 * @brief convert ZFTimeValue to more readable string, see #ZFTimeValueToString
 *
 * can't be converted back from string, usually for debug use only
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFTimeValueToStringFriendly,
                        ZFMP_IN_OUT(zfstring &, s),
                        ZFMP_IN(ZFTimeValue const &, v))
/** @brief see #ZFTimeValueToStringFriendly */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zfstring, ZFTimeValueToStringFriendly,
                               ZFMP_IN(ZFTimeValue const &, v))
{
    zfstring s;
    ZFTimeValueToStringFriendly(s, v);
    return s;
}

/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT zfbool operator < (ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT zfbool operator <= (ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT zfbool operator > (ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT zfbool operator >= (ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT ZFTimeValue operator + (ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT ZFTimeValue operator - (ZF_IN const ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT void operator += (ZF_IN_OUT ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT void operator -= (ZF_IN_OUT ZFTimeValue &v0, ZF_IN const ZFTimeValue &v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT ZFTimeValue operator * (ZF_IN const ZFTimeValue &v0, ZF_IN zfindex v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT ZFTimeValue operator / (ZF_IN const ZFTimeValue &v0, ZF_IN zfindex v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT void operator *= (ZF_IN_OUT ZFTimeValue &v0, ZF_IN zfindex v1);
/** @brief overrided operator for ZFTimeValue calculate */
extern ZF_ENV_EXPORT void operator /= (ZF_IN_OUT ZFTimeValue &v0, ZF_IN zfindex v1);

// ============================================================
// ZFTimeInfo
/**
 * @brief store a time info, internal use only
 */
zfclassPOD ZF_ENV_EXPORT ZFTimeInfo
{
public:
    zfint year;          /**< e.g. 1999 */
    zfuint month;        /**< [0, 11] */
    zfuint day;          /**< [0, 27...30] */
    zfuint hour;         /**< [0, 23] */
    zfuint minute;       /**< [0, 59] */
    zfuint second;       /**< [0, 59] */
    zfuint miliSecond;   /**< [0, 999] */
    zfuint microSecond;  /**< [0, 999] */
};

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFTimeInfo, ZFTimeInfo)

/**
 * @brief a zero time info (0000-00-00 00:00:00.000 000)
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFTimeInfo, ZFTimeInfoZero)
/**
 * @brief make a time info, you should make sure value is valid
 */
ZFMETHOD_FUNC_INLINE_DECLARE_8(ZFTimeInfo, ZFTimeInfoMake,
                               ZFMP_IN(zfint const &, year),
                               ZFMP_IN(zfuint const &, month),
                               ZFMP_IN(zfuint const &, day),
                               ZFMP_IN(zfuint const &, hour),
                               ZFMP_IN(zfuint const &, minute),
                               ZFMP_IN(zfuint const &, second),
                               ZFMP_IN(zfuint const &, miliSecond),
                               ZFMP_IN(zfuint const &, microSecond))
{
    ZFTimeInfo ret = {year, month, day, hour, minute, second, miliSecond, microSecond};
    return ret;
}

ZFCORE_POD_COMPARER_DECLARE(ZFTimeInfo)

/**
 * @brief convert ZFTimeInfo to string
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFTimeInfoToString,
                        ZFMP_IN_OUT(zfstring &, s),
                        ZFMP_IN(ZFTimeInfo const &, v))
/** @brief see #ZFTimeInfoToString */
ZFMETHOD_FUNC_INLINE_DECLARE_1(zfstring, ZFTimeInfoToString,
                               ZFMP_IN(ZFTimeInfo const &, v))
{
    zfstring s;
    ZFTimeInfoToString(s, v);
    return s;
}
ZFOUTPUT_TYPE(ZFTimeInfo, {output << ZFTimeInfoToString(v);})

// ============================================================
// ZFTime
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTime_timeValue "timeValue"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTime_timeZone "timeZone"

zfclassFwd _ZFP_ZFTimePrivate;
/**
 * @brief time utility
 *
 * use a ZFTimeValue to store time,
 * which means how many seconds had passed since #ZFTimeInfoZero\n
 * a positive value means time that is after #ZFTimeInfoZero,
 * and a negative value means time that is before #ZFTimeInfoZero\n
 * ZFTimeValue as well as ZFTime contains no actual time logic
 * such as time zone, UTC, GMT, etc,
 * it is simply a time unit, what it means is depends on you\n
 * \n
 * for performance, you may want to use ZFTimeValue or zftimet to process times,
 * is enough for most case and is recommended\n
 * \n
 * serializable data:
 * @code
 *   <ZFTime
 *       timeValue="timeValueString" // ZFTime::timeValue, optional, ZFTimeValueZero by default
 *       timeZone="timeZoneString" // ZFTime::timeZone, optional, ZFTimeValueZero by default
 *   />
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFTime : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFTime, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->timeValue(ZFCastZFObjectUnchecked(zfself *, anotherObj)->timeValue());
        this->timeZone(ZFCastZFObjectUnchecked(zfself *, anotherObj)->timeZone());
    }

public:
    /**
     * @brief return true if year is leap year
     */
    ZFMETHOD_DECLARE_STATIC_1(zfbool, leapYear,
                              ZFMP_IN(zfint, year))
    /**
     * @brief return leap year num in range [year1, year2],
     *   may return negative value if (year1 > year2)
     */
    ZFMETHOD_DECLARE_STATIC_2(zfint, leapYearBetween,
                              ZFMP_IN(zfint, year1),
                              ZFMP_IN(zfint, year2))

    /**
     * @brief get timestamp in mili seconds
     *
     * use this instead of #currentTimeValue if you only care about time interval,
     * this value is a relative value, what 0 means is not ensured\n
     * \n
     * typically, this method would have better performance and accuracy than #currentTimeValue
     */
    ZFMETHOD_DECLARE_STATIC_0(zftimet, timestamp)

    /**
     * @brief return time since #ZFTimeInfoZero, negative if before #ZFTimeInfoZero
     */
    ZFMETHOD_DECLARE_STATIC_0(ZFTimeValue, currentTimeValue)
    /**
     * @brief equal to ZFTimeValueToMiliSeconds(currentTimeValue())
     */
    ZFMETHOD_DECLARE_STATIC_0(zftimet, currentTimeMiliSeconds)
    /**
     * @brief return current time info, util method to #currentTimeValue
     */
    ZFMETHOD_DECLARE_STATIC_1(ZFTimeInfo, currentTimeInfo,
                              ZFMP_IN_OPT(const ZFTimeValue &, localTimeZone, ZFTime::timeZoneLocal()))
    /**
     * @brief convert time value to time info
     */
    ZFMETHOD_DECLARE_STATIC_2(zfbool, timeInfoFromTimeValue,
                              ZFMP_OUT(ZFTimeInfo &, ti),
                              ZFMP_IN(const ZFTimeValue &, tv))
    /**
     * @brief convert time info to time value
     */
    ZFMETHOD_DECLARE_STATIC_2(zfbool, timeInfoToTimeValue,
                              ZFMP_OUT(ZFTimeValue &, tv),
                              ZFMP_IN(const ZFTimeInfo &, ti))
    /**
     * @brief convert time info to time value
     */
    static zfbool timeInfoToTimeValue(ZF_OUT ZFTimeValue &tv,
                                      ZF_IN zfint year,
                                      ZF_IN zfuint month,
                                      ZF_IN zfuint day,
                                      ZF_IN zfuint hour,
                                      ZF_IN zfuint minute,
                                      ZF_IN zfuint second,
                                      ZF_IN zfuint miliSecond,
                                      ZF_IN zfuint microSecond);

    /**
     * @brief get current time zone
     *
     * while getting time info from a ZFTime object,
     * time zone would be applied,
     * simply by #ZFTimeValueInc to the time value of ZFTime object
     */
    ZFMETHOD_DECLARE_STATIC_0(const ZFTimeValue &, timeZoneLocal)

protected:
    /**
     * @brief init from another ZFTime
     */
    ZFOBJECT_ON_INIT_DECLARE_1(ZFMP_IN(ZFTime *, time))
    /**
     * @brief init from time value
     */
    ZFOBJECT_ON_INIT_DECLARE_2(ZFMP_IN(const ZFTimeValue &, tv),
                              ZFMP_IN_OPT(const ZFTimeValue &, timeZone, ZFTime::timeZoneLocal)())
    /**
     * @brief init from time info, or zero time if invalid
     */
    ZFOBJECT_ON_INIT_DECLARE_2(ZFMP_IN(const ZFTimeInfo &, ti),
                              ZFMP_IN_OPT(const ZFTimeValue &, timeZone, ZFTime::timeZoneLocal)())

    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    zfoverride
    virtual zfidentity objectHash(void);

    /**
     * @brief compared by #timeValueAppliedTimeZone
     */
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief set time according to time value
     */
    ZFMETHOD_DECLARE_1(zfbool, timeValue,
                       ZFMP_IN(const ZFTimeValue &, tv))
    /**
     * @brief access time value
     */
    ZFMETHOD_DECLARE_0(const ZFTimeValue &, timeValue)

    /**
     * @brief set time zone attched to this ZFTime object
     * @see timeZoneLocal
     */
    ZFMETHOD_DECLARE_1(zfbool, timeZone,
                       ZFMP_IN(const ZFTimeValue &, timeZone))
    /**
     * @brief get time zone attched to this ZFTime object
     */
    ZFMETHOD_DECLARE_0(const ZFTimeValue &, timeZone)

    /**
     * @brief get time value which has been applied by time zone offset
     */
    ZFMETHOD_DECLARE_0(const ZFTimeValue &, timeValueAppliedTimeZone)

    /**
     * @brief set time according to time info
     *
     * set time to #ZFTimeInfoZero if input is invalid
     */
    virtual zfbool timeInfo(ZF_IN zfint year,
                            ZF_IN zfuint month,
                            ZF_IN zfuint day,
                            ZF_IN zfuint hour,
                            ZF_IN zfuint minute,
                            ZF_IN zfuint second,
                            ZF_IN zfuint miliSecond,
                            ZF_IN zfuint microSecond,
                            ZF_IN_OPT const ZFTimeValue &tz = ZFTime::timeZoneLocal());
    /**
     * @brief set time according to time info
     *
     * set time to #ZFTimeInfoZero if input is invalid
     */
    ZFMETHOD_DECLARE_2(zfbool, timeInfo,
                       ZFMP_IN(const ZFTimeInfo &, ti),
                       ZFMP_IN_OPT(const ZFTimeValue &, tz, ZFTime::timeZoneLocal()))
    /**
     * @brief get time info of current time value (by #timeValueAppliedTimeZone)
     */
    ZFMETHOD_DECLARE_0(const ZFTimeInfo &, timeInfo)

    /**
     * @brief year since 0000, e.g. 2012
     */
    ZFMETHOD_DECLARE_0(zfint, timeInfoYear)

    /**
     * @brief month range in [0, 11]
     */
    ZFMETHOD_DECLARE_0(zfuint, timeInfoMonth)

    /**
     * @brief day of month range in [0, 27...30]
     */
    ZFMETHOD_DECLARE_0(zfuint, timeInfoDay)

    /**
     * @brief day of week range in [0, 6], while 0 stands for Sunday
     */
    ZFMETHOD_DECLARE_0(zfuint, timeInfoDayOfWeek)
    /**
     * @brief day of year range in [0, 364/365]
     */
    ZFMETHOD_DECLARE_0(zfuint, timeInfoDayOfYear)

    /**
     * @brief hour range in [0, 23]
     */
    ZFMETHOD_DECLARE_0(zfuint, timeInfoHour)

    /**
     * @brief minute range in [0, 59]
     */
    ZFMETHOD_DECLARE_0(zfuint, timeInfoMinute)

    /**
     * @brief second range in [0, 59]
     */
    ZFMETHOD_DECLARE_0(zfuint, timeInfoSecond)

    /**
     * @brief miliSecond range in [0, 999]
     */
    ZFMETHOD_DECLARE_0(zfuint, timeInfoMiliSecond)

    /**
     * @brief microSecond range in [0, 999]
     */
    ZFMETHOD_DECLARE_0(zfuint, timeInfoMicroSecond)

private:
    _ZFP_ZFTimePrivate *d;
};

// ============================================================
/** @brief in seconds */
#define zftimetOneMinute 60LL
/** @brief in seconds */
#define zftimetOneHour 3600LL // 60 * 60
/** @brief in seconds */
#define zftimetOneDay 86400LL // 60 * 60 * 24
/** @brief in seconds */
#define zftimetOneWeek 604800LL // 60 * 60 * 24 * 7
/** @brief 30 days in seconds */
#define zftimetOneMonth 2592000LL // 60 * 60 * 24 * 30
/** @brief 365 days in seconds */
#define zftimetOneYear 31536000LL // 60 * 60 * 24 * 365

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTime_h_

