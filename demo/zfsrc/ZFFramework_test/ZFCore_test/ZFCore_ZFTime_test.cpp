#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfindex _ZFP_ZFCore_ZFTime_test_leapYearNumByEnum(zfint year1, zfint year2)
{
    zfint t1 = year1;
    zfint t2 = year2;
    if(year1 > year2)
    {
        t1 = year2;
        t2 = year1;
    }
    zfuint ret = 0;
    for( ; t1 <= t2; ++t1)
    {
        if((t1 % 100 != 0 && t1 % 4 == 0)
           || t1 % 400 == 0)
        {
            ++ret;
        }
    }
    return ret;
}
static zfstring _ZFP_ZFCore_ZFTime_test_printZFTimeValue(const ZFTimeValue &tv)
{
    zfstring s;
    zfstringAppend(s, "\t%s\t%s",
        zfsFromInt(tv.sec).cString(),
        zfsFromInt(tv.usec).cString());
    return s;
}

static zfstring _ZFP_ZFCore_ZFTime_test_printZFTime(ZFTime *t)
{
    zfstring ret;
    zfstringAppend(ret, "  %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(t->timeValue()).cString());
    zfstringAppend(ret, "  %6d-%02d-%02d %02d:%02d:%02d.%03d %03d, dayOfWeek: %d, dayOfYear: %d",
        t->timeInfoYear(),
        t->timeInfoMonth() + 1,
        t->timeInfoDay() + 1,
        t->timeInfoHour(),
        t->timeInfoMinute(),
        t->timeInfoSecond(),
        t->timeInfoMiliSecond(),
        t->timeInfoMicroSecond(),
        t->timeInfoDayOfWeek(),
        t->timeInfoDayOfYear());
    return ret;
}

zfclass ZFCore_ZFTime_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFTime_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFTime");

        srand((zfuint)ZFTime::currentTimeValue().sec);
        this->testCaseOutput("calc leap year between two years, if nothing wrong, none would be printed:");
        for(zfint i = 0; i < 200; ++i)
        {
            zfint y1 = rand() % 30000;
            zfint y2 = rand() % 30000;
            zfint n1 = (zfint)_ZFP_ZFCore_ZFTime_test_leapYearNumByEnum(y1, y2);
            zfint n2 = (zfint)ZFTime::leapYearBetween(y1, y2);
            if(n1 != n2 && n1 != -n2)
            {
                this->testCaseOutput("calc LeapYear num error at loop: %d (y1: %6d, y2: %6d) n1: %6d, n2: %6d",
                    i,
                    y1,
                    y2,
                    n1,
                    n2);
            }
        }
        this->testCaseOutputSeparator();

        zfblockedAlloc(ZFTime, t, ZFTime::currentTimeValue());
        this->testCaseOutput("current time:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t));

        zfblockedAlloc(ZFTime, t2, ZFTimeValueZero());
        this->testCaseOutput("zero time:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t2));

        zfblockedAlloc(ZFTime, t3, ZFTimeInfoMake(
            t->timeInfoYear(),
            t->timeInfoMonth(),
            t->timeInfoDay(),
            t->timeInfoHour(),
            t->timeInfoMinute(),
            t->timeInfoSecond(),
            t->timeInfoMiliSecond(),
            t->timeInfoMicroSecond()));
        this->testCaseOutput("create a ZFTime:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t3));

        zfblockedAlloc(ZFTime, t4, ZFTimeInfoMake(
            t->timeInfoYear() - 300,
            t->timeInfoMonth(),
            t->timeInfoDay(),
            t->timeInfoHour(),
            t->timeInfoMinute(),
            t->timeInfoSecond(),
            t->timeInfoMiliSecond(),
            t->timeInfoMicroSecond()));
        this->testCaseOutput("time before 1970:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t4));

        zfblockedAlloc(ZFTime, t5, ZFTimeInfoMake(
            t->timeInfoYear() - 3000,
            t->timeInfoMonth(),
            t->timeInfoDay(),
            t->timeInfoHour(),
            t->timeInfoMinute(),
            t->timeInfoSecond(),
            t->timeInfoMiliSecond(),
            t->timeInfoMicroSecond()));
        this->testCaseOutput("time before 0000:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t5));

        zfblockedAlloc(ZFTime, t6, ZFTimeInfoMake(
            t->timeInfoYear() + 3000,
            t->timeInfoMonth(),
            t->timeInfoDay(),
            t->timeInfoHour(),
            t->timeInfoMinute(),
            t->timeInfoSecond(),
            t->timeInfoMiliSecond(),
            t->timeInfoMicroSecond()));
        this->testCaseOutput("time after 3000:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t6));

        zfblockedAlloc(ZFTime, t7, t->timeValue());
        this->testCaseOutput("copy a ZFTime:");
        this->testCaseOutput(_ZFP_ZFCore_ZFTime_test_printZFTime(t7));

        // ============================================================
        this->testCaseOutputSeparator();
        this->testCaseOutput("ZFTime performance test");
        zfblockedAlloc(ZFTime, tCur1);
        zfblockedAlloc(ZFTime, tCur2);

        this->testCaseOutput("\naccess none");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess Year");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoYear();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess Month");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoMonth();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess Day");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoDay();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess Hour");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoHour();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess Minute");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoMinute();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess Second");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoSecond();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess MiliSecond");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoMiliSecond();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess MicroSecond");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoMicroSecond();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess DayOfWeek");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoDayOfWeek();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseOutput("\naccess DayOfYear");
        tCur1->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s", _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur1->timeValue()).cString());
        (void)tCur2->timeInfoDayOfYear();
        tCur2->timeValue(ZFTime::currentTimeValue());
        this->testCaseOutput("current timeValue: %s, used: %s",
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(tCur2->timeValue()).cString(),
            _ZFP_ZFCore_ZFTime_test_printZFTimeValue(ZFTimeValueDec(tCur2->timeValue(), tCur1->timeValue())).cString());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTime_test)

ZF_NAMESPACE_GLOBAL_END

