#include "ZFLua_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFLua_metatable_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_metatable_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        {
            ZFCoreStatisticInvokeTimeLoggerOneTime("run lua code");
            ZFLuaExecute(
                    "print('\\n__add: (5 5 5)')\n"
                    "print(zfint(2) + zfint(3))\n"
                    "print(zfint(2) + 3)\n"
                    "print(2 + zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__sub: (-1 -1 -1)')\n"
                    "print(zfint(2) - zfint(3))\n"
                    "print(zfint(2) - 3)\n"
                    "print(2 - zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__mul: (6 6 6)')\n"
                    "print(zfint(2) * zfint(3))\n"
                    "print(zfint(2) * 3)\n"
                    "print(2 * zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__div: (0.66 0.66 0.66)')\n"
                    "print(zfint(2) / zfint(3))\n"
                    "print(zfint(2) / 3)\n"
                    "print(2 / zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__mod: (2 2 2)')\n"
                    "print(zfint(2) % zfint(3))\n"
                    "print(zfint(2) % 3)\n"
                    "print(2 % zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__unm: (-2)')\n"
                    "print(-zfint(2))\n"
                );
            ZFLuaExecute(
                    "print('\\n__band: (2 2 2)')\n"
                    "print(zfint(2) & zfint(3))\n"
                    "print(zfint(2) & 3)\n"
                    "print(2 & zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__bor: (3 3 3)')\n"
                    "print(zfint(2) | zfint(3))\n"
                    "print(zfint(2) | 3)\n"
                    "print(2 | zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__bxor: (1 1 1)')\n"
                    "print(zfint(2) ~ zfint(3))\n"
                    "print(zfint(2) ~ 3)\n"
                    "print(2 ~ zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__bnot: (4294967292)')\n"
                    "print(~zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__shl: (16 16 16)')\n"
                    "print(zfint(2) << zfint(3))\n"
                    "print(zfint(2) << 3)\n"
                    "print(2 << zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__shr: (0 0 0)')\n"
                    "print(zfint(2) >> zfint(3))\n"
                    "print(zfint(2) >> 3)\n"
                    "print(2 >> zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__concat: (23 23 23)')\n"
                    "print(zfstring(2) .. zfstring(3))\n"
                    "print(zfstring(2) .. 3)\n"
                    "print(2 .. zfstring(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__len: (3)')\n"
                    "print(#zfstring('123'))\n"
                );
            ZFLuaExecute(
                    "print('\\n__eq: (true false false)')\n"
                    "print(zfint(3) == zfint(3))\n"
                    "print(zfint(3) == 3)\n" // not compared by __eq
                    "print(3 == zfint(3))\n" // not compared by __eq
                );
            ZFLuaExecute(
                    "print('\\n__lt: (true true true)')\n"
                    "print(zfint(2) < zfint(3))\n"
                    "print(zfint(2) < 3)\n"
                    "print(2 < zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__le: (true true true)')\n"
                    "print(zfint(2) <= zfint(3))\n"
                    "print(zfint(2) <= 3)\n"
                    "print(2 <= zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__rt: (true true true)')\n"
                    "print(zfint(4) > zfint(3))\n"
                    "print(zfint(4) > 3)\n"
                    "print(4 > zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__re: (true true true)')\n"
                    "print(zfint(4) >= zfint(3))\n"
                    "print(zfint(4) >= 3)\n"
                    "print(4 >= zfint(3))\n"
                );
            ZFLuaExecute(
                    "print('\\n__neq: (false true true)')\n"
                    "print(zfint(3) ~= zfint(3))\n"
                    "print(zfint(3) ~= 3)\n" // not compared by __eq
                    "print(3 ~= zfint(3))\n" // not compared by __eq
                );
            ZFLuaExecute(
                    "print('\\nbool: (true true)')\n"
                    "if(zfbool('true')) then\n"
                    "    print('bool true')\n"
                    "else\n"
                    "    print('bool false')\n"
                    "end\n"
                    "if(zfbool('false')) then\n" // lua has no implicit bool compare, only nil check
                    "    print('bool true')\n"
                    "else\n"
                    "    print('bool false')\n"
                    "end\n"
                );
        }

        {
            ZFCoreStatisticInvokeTimeLoggerOneTime("lua gc");
            ZFLuaGC();
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFLua_metatable_test)

ZF_NAMESPACE_GLOBAL_END

