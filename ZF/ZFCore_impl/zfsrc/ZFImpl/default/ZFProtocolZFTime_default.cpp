#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTime.h"
#include "ZFCore/ZFTime.h"

#include <time.h>
#if ZF_ENV_sys_Windows
    #include <Windows.h>
#elif __APPLE__
    #include <sys/time.h>
    #include <mach/mach.h>
    #include <mach/mach_time.h>
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
    // need -lrt for Posix
    #include <sys/time.h>
#endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// common define
#define _ZFP_ZFTimeImpl_default_leapYear(year) \
    ((((year) % 100 != 0) && ((year) % 4 == 0)) || ((year) % 400 == 0))
// positive num in [1970, year), or negative num in (year, 1970]
static inline zfint _ZFP_ZFTimeImpl_default_calcLeapYearBetween1970(zfint year)
{
    zfint t = year - 1970;
    if(t >= 0)
    {
        return ((zfint)((t + 1) / 4) - (zfint)((t + 69) / 100) + (zfint)((t + 369) / 400));
    }
    else
    {
        return ((zfint)((t - 1) / 4) - (zfint)((t - 29) / 100) + (zfint)((t - 29) / 400));
    }
}
#if 0
static inline zfint _ZFP_ZFTimeImpl_default_calcLeapYearBetween1900(zfint year)
{
    zfint t = year - 1900;
    if(t >= 0)
    {
        return ((zfint)((t - 1) / 4) - (zfint)((t - 1) / 100) + (zfint)((t + 299) / 400));
    }
    else
    {
        return ((zfint)((t + 1) / 4) - (zfint)((t + 1) / 100) + (zfint)((t - 99) / 400));
    }
}
#endif

static const zftimet _ZFP_ZFTimeImpl_default_MonthToSec[] =
{
    (zftimet)(0 * zftimetOneDay),   // 1
    (zftimet)(31 * zftimetOneDay),  // 2
    (zftimet)(59 * zftimetOneDay),  // 3
    (zftimet)(90 * zftimetOneDay),  // 4
    (zftimet)(120 * zftimetOneDay), // 5
    (zftimet)(151 * zftimetOneDay), // 6
    (zftimet)(181 * zftimetOneDay), // 7
    (zftimet)(212 * zftimetOneDay), // 8
    (zftimet)(243 * zftimetOneDay), // 9
    (zftimet)(273 * zftimetOneDay), // 10
    (zftimet)(304 * zftimetOneDay), // 11
    (zftimet)(334 * zftimetOneDay), // 12
};
static const zfuint _ZFP_ZFTimeImpl_default_MonthToDay[] =
{
    (zfuint)(0),      // 1
    (zfuint)(31),     // 2
    (zfuint)(59),     // 3
    (zfuint)(90),     // 4
    (zfuint)(120),    // 5
    (zfuint)(151),    // 6
    (zfuint)(181),    // 7
    (zfuint)(212),    // 8
    (zfuint)(243),    // 9
    (zfuint)(273),    // 10
    (zfuint)(304),    // 11
    (zfuint)(334),    // 12
};
static const zfuint _ZFP_ZFTimeImpl_default_DayNumOfMonth[] =
{
    (zfuint)(31), // 1
    (zfuint)(28), // 2
    (zfuint)(31), // 3
    (zfuint)(30), // 4
    (zfuint)(31), // 5
    (zfuint)(30), // 6
    (zfuint)(31), // 7
    (zfuint)(31), // 8
    (zfuint)(30), // 9
    (zfuint)(31), // 10
    (zfuint)(30), // 11
    (zfuint)(31), // 12
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimeImpl_default, ZFTime, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("nativeAPI")
public:
    virtual zfbool leapYear(ZF_IN zfint year)
    {
        return _ZFP_ZFTimeImpl_default_leapYear(year);
    }
    virtual zfint leapYearBetween(ZF_IN zfint year1, ZF_IN zfint year2)
    {
        if(year1 == year2)
        {
            if(_ZFP_ZFTimeImpl_default_leapYear(year1))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        zfint t1 = _ZFP_ZFTimeImpl_default_calcLeapYearBetween1970(year1);
        zfbool leapYear1 = _ZFP_ZFTimeImpl_default_leapYear(year1);
        if(leapYear1)
        {
            if(year1 < 1970) {--t1;}
            else {++t1;}
        }
        zfint t2 = _ZFP_ZFTimeImpl_default_calcLeapYearBetween1970(year2);
        zfbool leapYear2 = _ZFP_ZFTimeImpl_default_leapYear(year2);
        if(leapYear2)
        {
            if(year2 < 1970) {--t2;}
            else {++t2;}
        }
        zfint ret = t2 - t1;
        if(year1 >= 1970 && year2 >= 1970)
        {
            if(year1 <= year2 && leapYear1)
            {
                ++ret;
            }
            else if(year1 >= year2 && leapYear2)
            {
                --ret;
            }
        }
        else if(year1 < 1970 && year2 < 1970)
        {
            if(year1 <= year2 && leapYear2)
            {
                ++ret;
            }
            else if(year1 >= year2 && leapYear1)
            {
                --ret;
            }
        }
        return ret;
    }
    virtual zftimet timestamp(void)
    {
        #if ZF_ENV_sys_Windows
            return (zftimet)GetTickCount();
        #elif __APPLE__
            static mach_timebase_info_data_t _timebaseInfo;
            if(_timebaseInfo.denom == 0)
            {
                (void)mach_timebase_info(&_timebaseInfo);
            }
            return (zftimet)(((mach_absolute_time() / 1000000) * _timebaseInfo.numer) / _timebaseInfo.denom);
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            return (zftimet)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    }
    virtual void currentTimeValue(ZF_OUT ZFTimeValue &tv)
    {
        #if ZF_ENV_sys_Windows
            #if ZF_ENV_sys_WindowsCE
                SYSTEMTIME stTmp;
                GetLocalTime(&stTmp);
                this->timeInfoToTimeValue(
                    tv,
                    ZFTimeInfoMake(
                        stTmp.wYear,
                        stTmp.wMonth - 1,
                        stTmp.wDay - 1,
                        stTmp.wHour,
                        stTmp.wMinute,
                        stTmp.wSecond,
                        stTmp.wMilliseconds,
                        0));
            #else
                union
                {
                    zft_zfuint16 ns100;
                    FILETIME ft;
                } now;
                GetSystemTimeAsFileTime(&now.ft);
                tv.sec = (time_t)((now.ns100 - 116444736000000000LL) / 10000000LL);
                tv.usec = (time_t)((now.ns100 / 10LL) % 1000000LL);
            #endif
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown // #if ZF_ENV_sys_Windows
            struct timeval tvTmp;
            gettimeofday(&tvTmp, zfnull);
            tv.sec = tvTmp.tv_sec;
            tv.usec = tvTmp.tv_usec;
        #endif // #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown

        // offset from 0 to 1970
        static const zftimet secondsBetween1970 = (zftimet)(1970 * zftimetOneYear + (-_ZFP_ZFTimeImpl_default_calcLeapYearBetween1970(0) + 1) * zftimetOneDay);
        tv.sec += secondsBetween1970;
    }
    virtual zfbool timeInfoFromTimeValue(ZF_OUT ZFTimeInfo &ti, ZF_IN const ZFTimeValue &tv)
    {
        zfmemset(&ti, 0, sizeof(ZFTimeInfo));

        zftimet secTmp = tv.sec;
        zfbool leapYear = zffalse;

        // year
        ti.year = (zfint)(secTmp / zftimetOneYear);
        zfint leapYearNum = ((ti.year == 0) ? 0 : ZFTime::leapYearBetween(-1, ti.year));
        ti.year -= (zfint)(leapYearNum / 365);
        leapYearNum = ((ti.year == 0) ? 0 : ZFTime::leapYearBetween(-1, ti.year));
        secTmp -= (ti.year * zftimetOneYear + leapYearNum * zftimetOneDay);
        if(secTmp < 0)
        {
            while(secTmp < 0)
            {
                secTmp += zftimetOneYear;
                --ti.year;
                if(_ZFP_ZFTimeImpl_default_leapYear(ti.year))
                {
                    secTmp += zftimetOneDay;
                }
            }
        }
        leapYear = _ZFP_ZFTimeImpl_default_leapYear(ti.year);

        // month
        for(ti.month = 0; ti.month < 11; ++(ti.month))
        {
            if(_ZFP_ZFTimeImpl_default_MonthToSec[ti.month] <= secTmp
               && _ZFP_ZFTimeImpl_default_MonthToSec[ti.month + 1] > secTmp)
            {
                break;
            }
        }
        secTmp -= _ZFP_ZFTimeImpl_default_MonthToSec[ti.month];

        // day
        ti.day = (zfuint)(secTmp / zftimetOneDay);
        secTmp -= ti.day * zftimetOneDay;
        if(leapYear && ti.month > 1)
        {
            if(ti.day == 0)
            {
                --ti.month;
                if(ti.month + 1 == 2)
                {
                    ti.day = 28;
                }
                else
                {
                    ti.day = _ZFP_ZFTimeImpl_default_DayNumOfMonth[ti.month] - 1;
                }
            }
            else
            {
                --ti.day;
            }
        }

        // hour
        ti.hour = (zfuint)(secTmp / zftimetOneHour);
        secTmp -= ti.hour * zftimetOneHour;

        // minute
        ti.minute = (zfuint)(secTmp / zftimetOneMinute);
        secTmp -= ti.minute * zftimetOneMinute;

        // second
        ti.second = (zfuint)secTmp;
        zftimet usecTmp = tv.usec;
        if(usecTmp < 0)
        {
            --ti.second;
            usecTmp += 1000000LL;
        }

        // miliSecond
        ti.miliSecond = (zfuint)(usecTmp / 1000);

        // microSecond
        ti.microSecond = (zfuint)(usecTmp % 1000);

        return zftrue;
    }
    virtual zfbool timeInfoToTimeValue(ZF_OUT ZFTimeValue &tv, ZF_IN const ZFTimeInfo &ti)
    {
        zfmemset(&tv, 0, sizeof(ZFTimeValue));
        if(!this->timeInfoIsValid(ti))
        {
            return zffalse;
        }

        tv.sec =
            ti.year * zftimetOneYear + ((ti.year == 0) ? 0 : ZFTime::leapYearBetween(-1, ti.year)) * zftimetOneDay
            + _ZFP_ZFTimeImpl_default_MonthToSec[ti.month]
            + ti.day * zftimetOneDay
            + ti.hour * zftimetOneHour
            + ti.minute * zftimetOneMinute
            + ti.second;
        if(ti.year != 0 && _ZFP_ZFTimeImpl_default_leapYear(ti.year))
        {
            if(ti.year > 0 && ti.month <= 1)
            {
                tv.sec -= zftimetOneDay;
            }
            else if(ti.year < 0 && (ti.month > 1 || (ti.month == 1 && ti.day == 28)))
            {
                tv.sec += zftimetOneDay;
            }
        }

        if(tv.sec >= 0)
        {
            tv.usec = ti.miliSecond * 1000LL + ti.microSecond;
        }
        else
        {
            if(ti.miliSecond != 0 || ti.microSecond != 0)
            {
                ++(tv.sec);
                tv.usec = ti.miliSecond * 1000LL + ti.microSecond - 1000000LL;
            }
            else
            {
                tv.usec = 0;
            }
        }

        return zftrue;
    }
    virtual const ZFTimeValue &timeZoneLocal(void)
    {
        static ZFTimeValue _tv = ZFTimeValueZero();
        #if ZF_ENV_sys_Windows
            TIME_ZONE_INFORMATION tzInfo;
            if(GetTimeZoneInformation(&tzInfo) != TIME_ZONE_ID_INVALID)
            {
                _tv.sec = -tzInfo.Bias * zftimetOneMinute;
                _tv.usec = 0;
            }
        #elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
            struct timeval tvDummy;
            struct timezone tz;
            if(gettimeofday(&tvDummy,&tz) == 0)
            {
                _tv.sec = -tz.tz_minuteswest * zftimetOneMinute;
                _tv.usec = 0;
            }
        #endif
        return _tv;
    }
    virtual zfuint timeInfoDayOfWeek(ZF_IN const ZFTimeValue &tv, ZF_IN const ZFTimeInfo &ti)
    {
        // dayOfWeek of 0000-01-01
        #define _ZFP_ZFTimeImpl_default_baseDayOfWeek 6
        return ((
                 (zfuint)((_ZFP_ZFTimeImpl_default_baseDayOfWeek + (tv.sec / zftimetOneDay)) % 7)
                 + 7) % 7);
    }
    virtual zfuint timeInfoDayOfYear(ZF_IN const ZFTimeValue &tv, ZF_IN const ZFTimeInfo &ti)
    {
        zfuint dayOfYear = _ZFP_ZFTimeImpl_default_MonthToDay[ti.month] + ti.day;
        if(ti.month > 1 && _ZFP_ZFTimeImpl_default_leapYear(ti.year))
        {
            ++dayOfYear;
        }
        return dayOfYear;
    }
    virtual zfbool timeInfoIsValid(ZF_IN const ZFTimeInfo &ti)
    {
        return this->timeInfoIsValid(
            ti.year,
            ti.month,
            ti.day,
            ti.hour,
            ti.minute,
            ti.second,
            ti.miliSecond,
            ti.microSecond);
    }
    virtual zfbool timeInfoIsValid(ZF_IN zfint year,
                                   ZF_IN zfuint month,
                                   ZF_IN zfuint day,
                                   ZF_IN zfuint hour,
                                   ZF_IN zfuint minute,
                                   ZF_IN zfuint second,
                                   ZF_IN zfuint miliSecond,
                                   ZF_IN zfuint microSecond)
    {
        return (month <= 11
            && !(
                    (_ZFP_ZFTimeImpl_default_leapYear(year) && month == 1 && day >= 29)
                    || (!_ZFP_ZFTimeImpl_default_leapYear(year) && day >= _ZFP_ZFTimeImpl_default_DayNumOfMonth[month])
                )
            && hour <= 23
            && minute <= 59
            && second <= 59
            && miliSecond <= 999
            && microSecond <= 999);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimeImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFTimeImpl_default)

ZF_NAMESPACE_GLOBAL_END

