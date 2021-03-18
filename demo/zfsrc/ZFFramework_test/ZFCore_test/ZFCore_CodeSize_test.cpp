#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// 34249070
#define _ZFP_ZFCore_CodeSize_test_Dummy_ENABLE                  0 // x~x,     106960, 34356030
#define _ZFP_ZFCore_CodeSize_test_StaticRegister_ENABLE         0 // 1000~7N, 74256, 34323326
#define _ZFP_ZFCore_CodeSize_test_StaticInit_ENABLE             0 // 10~10,   308336, 34557406
#define _ZFP_ZFCore_CodeSize_test_GlobalInit_ENABLE             0 // 150~N,   151248, 34400318
#define _ZFP_ZFCore_CodeSize_test_Object_ENABLE                 0 // 250~N,   559856, 34808926
#define _ZFP_ZFCore_CodeSize_test_Method_ENABLE                 0 // 800~5N,  318592, 34567662
#define _ZFP_ZFCore_CodeSize_test_MethodFunc_ENABLE             0 // 300~N,   336080, 34585150
#define _ZFP_ZFCore_CodeSize_test_MethodUserRegister_ENABLE     0 // 600~600, 210960, 34460030
#define _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_ENABLE 0 // 350~350, 217680, 34466750
#define _ZFP_ZFCore_CodeSize_test_Property_ENABLE               0 // 500~3N,  1151888, 35400958
#define _ZFP_ZFCore_CodeSize_test_PropertyUserRegister_ENABLE   0 // 10~10,   453856, 34702926
#define _ZFP_ZFCore_CodeSize_test_PropertyType_ENABLE           0 // 90~N,    2227632, 36476702

// ============================================================
// dummy object
#if _ZFP_ZFCore_CodeSize_test_Dummy_ENABLE
#define _ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(N) \
    zfclassNotPOD _ZFP_ZFCore_CodeSize_test_Dummy_Obj##N \
    { \
    public: \
        _ZFP_ZFCore_CodeSize_test_Dummy_Obj##N(void) \
        { \
        } \
        ~_ZFP_ZFCore_CodeSize_test_Dummy_Obj##N(void) \
        { \
        } \
    }; \
    static _ZFP_ZFCore_CodeSize_test_Dummy_Obj##N _ZFP_ZFCore_CodeSize_test_Dummy_obj##N;
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(0)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(1)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(2)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(3)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(4)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(5)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(6)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(7)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(8)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(9)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(10)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(11)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(12)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(13)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(14)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(15)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(16)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(17)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(18)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(19)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(20)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(21)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(22)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(23)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(24)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(25)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(26)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(27)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(28)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(29)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(30)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(31)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(32)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(33)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(34)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(35)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(36)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(37)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(38)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(39)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(40)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(41)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(42)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(43)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(44)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(45)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(46)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(47)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(48)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(49)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(50)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(51)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(52)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(53)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(54)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(55)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(56)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(57)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(58)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(59)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(60)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(61)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(62)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(63)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(64)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(65)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(66)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(67)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(68)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(69)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(70)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(71)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(72)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(73)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(74)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(75)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(76)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(77)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(78)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(79)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(80)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(81)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(82)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(83)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(84)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(85)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(86)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(87)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(88)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(89)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(90)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(91)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(92)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(93)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(94)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(95)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(96)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(97)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(98)
_ZFP_ZFCore_CodeSize_test_Dummy_EXPAND(99)
#endif

// ============================================================
// static register
#if _ZFP_ZFCore_CodeSize_test_StaticRegister_ENABLE
#define _ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(N) \
    ZF_STATIC_REGISTER_INIT(_ZFP_ZFCore_CodeSize_test_StaticRegister_##N) \
    { \
    } \
    ZF_STATIC_REGISTER_END(_ZFP_ZFCore_CodeSize_test_StaticRegister_##N)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(0)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(1)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(2)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(3)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(4)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(5)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(6)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(7)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(8)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(9)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(10)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(11)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(12)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(13)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(14)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(15)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(16)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(17)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(18)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(19)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(20)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(21)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(22)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(23)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(24)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(25)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(26)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(27)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(28)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(29)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(30)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(31)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(32)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(33)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(34)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(35)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(36)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(37)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(38)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(39)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(40)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(41)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(42)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(43)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(44)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(45)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(46)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(47)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(48)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(49)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(50)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(51)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(52)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(53)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(54)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(55)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(56)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(57)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(58)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(59)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(60)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(61)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(62)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(63)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(64)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(65)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(66)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(67)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(68)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(69)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(70)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(71)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(72)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(73)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(74)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(75)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(76)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(77)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(78)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(79)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(80)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(81)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(82)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(83)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(84)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(85)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(86)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(87)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(88)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(89)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(90)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(91)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(92)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(93)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(94)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(95)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(96)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(97)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(98)
_ZFP_ZFCore_CodeSize_test_StaticRegister_EXPAND(99)
#endif

// ============================================================
// static initializer
#if _ZFP_ZFCore_CodeSize_test_StaticInit_ENABLE
#define _ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(N) \
    ZF_STATIC_INITIALIZER_INIT(_ZFP_ZFCore_CodeSize_test_StaticInit_##N) \
    { \
    } \
    ZF_STATIC_INITIALIZER_END(_ZFP_ZFCore_CodeSize_test_StaticInit_##N)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(0)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(1)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(2)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(3)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(4)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(5)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(6)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(7)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(8)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(9)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(10)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(11)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(12)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(13)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(14)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(15)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(16)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(17)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(18)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(19)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(20)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(21)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(22)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(23)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(24)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(25)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(26)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(27)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(28)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(29)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(30)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(31)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(32)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(33)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(34)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(35)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(36)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(37)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(38)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(39)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(40)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(41)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(42)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(43)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(44)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(45)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(46)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(47)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(48)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(49)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(50)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(51)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(52)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(53)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(54)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(55)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(56)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(57)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(58)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(59)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(60)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(61)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(62)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(63)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(64)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(65)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(66)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(67)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(68)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(69)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(70)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(71)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(72)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(73)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(74)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(75)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(76)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(77)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(78)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(79)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(80)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(81)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(82)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(83)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(84)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(85)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(86)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(87)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(88)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(89)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(90)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(91)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(92)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(93)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(94)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(95)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(96)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(97)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(98)
_ZFP_ZFCore_CodeSize_test_StaticInit_EXPAND(99)
#endif

// ============================================================
// global initializer
#if _ZFP_ZFCore_CodeSize_test_GlobalInit_ENABLE
#define _ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(N) \
    ZF_GLOBAL_INITIALIZER_INIT(_ZFP_ZFCore_CodeSize_test_GlobalInit_##N) \
    { \
    } \
    ZF_GLOBAL_INITIALIZER_END(_ZFP_ZFCore_CodeSize_test_GlobalInit_##N)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(0)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(1)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(2)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(3)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(4)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(5)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(6)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(7)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(8)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(9)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(10)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(11)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(12)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(13)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(14)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(15)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(16)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(17)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(18)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(19)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(20)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(21)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(22)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(23)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(24)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(25)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(26)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(27)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(28)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(29)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(30)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(31)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(32)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(33)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(34)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(35)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(36)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(37)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(38)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(39)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(40)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(41)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(42)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(43)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(44)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(45)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(46)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(47)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(48)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(49)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(50)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(51)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(52)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(53)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(54)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(55)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(56)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(57)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(58)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(59)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(60)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(61)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(62)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(63)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(64)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(65)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(66)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(67)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(68)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(69)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(70)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(71)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(72)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(73)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(74)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(75)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(76)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(77)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(78)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(79)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(80)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(81)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(82)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(83)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(84)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(85)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(86)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(87)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(88)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(89)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(90)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(91)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(92)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(93)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(94)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(95)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(96)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(97)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(98)
_ZFP_ZFCore_CodeSize_test_GlobalInit_EXPAND(99)
#endif

// ============================================================
// object
#if _ZFP_ZFCore_CodeSize_test_Object_ENABLE
#define _ZFP_ZFCore_CodeSize_test_Object_EXPAND(N) \
    zfclass _ZFP_ZFCore_CodeSize_test_Object_Obj##N : zfextends ZFObject \
    { \
        ZFOBJECT_DECLARE(_ZFP_ZFCore_CodeSize_test_Object_Obj##N, ZFObject) \
    }; \
    ZFOBJECT_REGISTER(_ZFP_ZFCore_CodeSize_test_Object_Obj##N)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(0)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(1)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(2)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(3)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(4)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(5)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(6)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(7)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(8)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(9)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(10)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(11)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(12)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(13)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(14)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(15)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(16)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(17)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(18)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(19)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(20)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(21)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(22)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(23)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(24)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(25)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(26)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(27)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(28)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(29)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(30)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(31)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(32)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(33)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(34)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(35)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(36)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(37)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(38)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(39)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(40)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(41)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(42)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(43)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(44)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(45)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(46)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(47)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(48)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(49)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(50)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(51)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(52)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(53)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(54)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(55)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(56)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(57)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(58)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(59)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(60)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(61)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(62)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(63)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(64)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(65)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(66)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(67)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(68)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(69)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(70)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(71)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(72)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(73)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(74)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(75)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(76)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(77)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(78)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(79)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(80)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(81)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(82)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(83)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(84)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(85)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(86)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(87)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(88)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(89)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(90)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(91)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(92)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(93)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(94)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(95)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(96)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(97)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(98)
_ZFP_ZFCore_CodeSize_test_Object_EXPAND(99)
#endif

// ============================================================
// method
#if _ZFP_ZFCore_CodeSize_test_Method_ENABLE
zfclass _ZFP_ZFCore_CodeSize_test_Method : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_CodeSize_test_Method, ZFObject)

    ZFMETHOD_INLINE_0(void, f0) {}
    ZFMETHOD_INLINE_0(void, f1) {}
    ZFMETHOD_INLINE_0(void, f2) {}
    ZFMETHOD_INLINE_0(void, f3) {}
    ZFMETHOD_INLINE_0(void, f4) {}
    ZFMETHOD_INLINE_0(void, f5) {}
    ZFMETHOD_INLINE_0(void, f6) {}
    ZFMETHOD_INLINE_0(void, f7) {}
    ZFMETHOD_INLINE_0(void, f8) {}
    ZFMETHOD_INLINE_0(void, f9) {}
    ZFMETHOD_INLINE_0(void, f10) {}
    ZFMETHOD_INLINE_0(void, f11) {}
    ZFMETHOD_INLINE_0(void, f12) {}
    ZFMETHOD_INLINE_0(void, f13) {}
    ZFMETHOD_INLINE_0(void, f14) {}
    ZFMETHOD_INLINE_0(void, f15) {}
    ZFMETHOD_INLINE_0(void, f16) {}
    ZFMETHOD_INLINE_0(void, f17) {}
    ZFMETHOD_INLINE_0(void, f18) {}
    ZFMETHOD_INLINE_0(void, f19) {}
    ZFMETHOD_INLINE_0(void, f20) {}
    ZFMETHOD_INLINE_0(void, f21) {}
    ZFMETHOD_INLINE_0(void, f22) {}
    ZFMETHOD_INLINE_0(void, f23) {}
    ZFMETHOD_INLINE_0(void, f24) {}
    ZFMETHOD_INLINE_0(void, f25) {}
    ZFMETHOD_INLINE_0(void, f26) {}
    ZFMETHOD_INLINE_0(void, f27) {}
    ZFMETHOD_INLINE_0(void, f28) {}
    ZFMETHOD_INLINE_0(void, f29) {}
    ZFMETHOD_INLINE_0(void, f30) {}
    ZFMETHOD_INLINE_0(void, f31) {}
    ZFMETHOD_INLINE_0(void, f32) {}
    ZFMETHOD_INLINE_0(void, f33) {}
    ZFMETHOD_INLINE_0(void, f34) {}
    ZFMETHOD_INLINE_0(void, f35) {}
    ZFMETHOD_INLINE_0(void, f36) {}
    ZFMETHOD_INLINE_0(void, f37) {}
    ZFMETHOD_INLINE_0(void, f38) {}
    ZFMETHOD_INLINE_0(void, f39) {}
    ZFMETHOD_INLINE_0(void, f40) {}
    ZFMETHOD_INLINE_0(void, f41) {}
    ZFMETHOD_INLINE_0(void, f42) {}
    ZFMETHOD_INLINE_0(void, f43) {}
    ZFMETHOD_INLINE_0(void, f44) {}
    ZFMETHOD_INLINE_0(void, f45) {}
    ZFMETHOD_INLINE_0(void, f46) {}
    ZFMETHOD_INLINE_0(void, f47) {}
    ZFMETHOD_INLINE_0(void, f48) {}
    ZFMETHOD_INLINE_0(void, f49) {}
    ZFMETHOD_INLINE_0(void, f50) {}
    ZFMETHOD_INLINE_0(void, f51) {}
    ZFMETHOD_INLINE_0(void, f52) {}
    ZFMETHOD_INLINE_0(void, f53) {}
    ZFMETHOD_INLINE_0(void, f54) {}
    ZFMETHOD_INLINE_0(void, f55) {}
    ZFMETHOD_INLINE_0(void, f56) {}
    ZFMETHOD_INLINE_0(void, f57) {}
    ZFMETHOD_INLINE_0(void, f58) {}
    ZFMETHOD_INLINE_0(void, f59) {}
    ZFMETHOD_INLINE_0(void, f60) {}
    ZFMETHOD_INLINE_0(void, f61) {}
    ZFMETHOD_INLINE_0(void, f62) {}
    ZFMETHOD_INLINE_0(void, f63) {}
    ZFMETHOD_INLINE_0(void, f64) {}
    ZFMETHOD_INLINE_0(void, f65) {}
    ZFMETHOD_INLINE_0(void, f66) {}
    ZFMETHOD_INLINE_0(void, f67) {}
    ZFMETHOD_INLINE_0(void, f68) {}
    ZFMETHOD_INLINE_0(void, f69) {}
    ZFMETHOD_INLINE_0(void, f70) {}
    ZFMETHOD_INLINE_0(void, f71) {}
    ZFMETHOD_INLINE_0(void, f72) {}
    ZFMETHOD_INLINE_0(void, f73) {}
    ZFMETHOD_INLINE_0(void, f74) {}
    ZFMETHOD_INLINE_0(void, f75) {}
    ZFMETHOD_INLINE_0(void, f76) {}
    ZFMETHOD_INLINE_0(void, f77) {}
    ZFMETHOD_INLINE_0(void, f78) {}
    ZFMETHOD_INLINE_0(void, f79) {}
    ZFMETHOD_INLINE_0(void, f80) {}
    ZFMETHOD_INLINE_0(void, f81) {}
    ZFMETHOD_INLINE_0(void, f82) {}
    ZFMETHOD_INLINE_0(void, f83) {}
    ZFMETHOD_INLINE_0(void, f84) {}
    ZFMETHOD_INLINE_0(void, f85) {}
    ZFMETHOD_INLINE_0(void, f86) {}
    ZFMETHOD_INLINE_0(void, f87) {}
    ZFMETHOD_INLINE_0(void, f88) {}
    ZFMETHOD_INLINE_0(void, f89) {}
    ZFMETHOD_INLINE_0(void, f90) {}
    ZFMETHOD_INLINE_0(void, f91) {}
    ZFMETHOD_INLINE_0(void, f92) {}
    ZFMETHOD_INLINE_0(void, f93) {}
    ZFMETHOD_INLINE_0(void, f94) {}
    ZFMETHOD_INLINE_0(void, f95) {}
    ZFMETHOD_INLINE_0(void, f96) {}
    ZFMETHOD_INLINE_0(void, f97) {}
    ZFMETHOD_INLINE_0(void, f98) {}
    ZFMETHOD_INLINE_0(void, f99) {}
};
ZFOBJECT_REGISTER(_ZFP_ZFCore_CodeSize_test_Method)
#endif

// ============================================================
// method func
#if _ZFP_ZFCore_CodeSize_test_MethodFunc_ENABLE
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f0) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f1) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f2) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f3) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f4) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f5) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f6) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f7) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f8) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f9) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f10) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f11) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f12) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f13) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f14) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f15) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f16) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f17) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f18) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f19) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f20) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f21) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f22) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f23) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f24) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f25) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f26) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f27) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f28) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f29) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f30) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f31) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f32) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f33) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f34) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f35) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f36) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f37) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f38) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f39) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f40) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f41) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f42) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f43) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f44) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f45) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f46) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f47) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f48) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f49) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f50) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f51) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f52) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f53) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f54) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f55) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f56) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f57) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f58) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f59) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f60) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f61) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f62) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f63) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f64) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f65) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f66) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f67) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f68) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f69) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f70) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f71) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f72) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f73) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f74) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f75) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f76) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f77) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f78) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f79) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f80) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f81) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f82) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f83) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f84) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f85) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f86) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f87) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f88) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f89) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f90) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f91) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f92) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f93) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f94) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f95) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f96) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f97) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f98) {}
ZFMETHOD_FUNC_DEFINE_0(void, _ZFP_ZFCore_CodeSize_test_f99) {}
#endif

// ============================================================
// method user register
#if _ZFP_ZFCore_CodeSize_test_MethodUserRegister_ENABLE
zfclass _ZFP_ZFCore_CodeSize_test_MethodUserRegister : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_CodeSize_test_MethodUserRegister, ZFObject)
};
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f0)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f1)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f2)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f3)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f4)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f5)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f6)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f7)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f8)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f9)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f10)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f11)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f12)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f13)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f14)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f15)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f16)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f17)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f18)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f19)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f20)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f21)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f22)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f23)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f24)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f25)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f26)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f27)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f28)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f29)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f30)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f31)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f32)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f33)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f34)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f35)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f36)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f37)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f38)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f39)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f40)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f41)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f42)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f43)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f44)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f45)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f46)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f47)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f48)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f49)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f50)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f51)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f52)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f53)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f54)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f55)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f56)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f57)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f58)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f59)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f60)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f61)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f62)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f63)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f64)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f65)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f66)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f67)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f68)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f69)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f70)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f71)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f72)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f73)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f74)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f75)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f76)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f77)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f78)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f79)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f80)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f81)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f82)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f83)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f84)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f85)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f86)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f87)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f88)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f89)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f90)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f91)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f92)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f93)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f94)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f95)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f96)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f97)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f98)
ZFMETHOD_USER_REGISTER_0({}, _ZFP_ZFCore_CodeSize_test_MethodUserRegister, void, f99)
#endif

// ============================================================
// method func user register
#if _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_ENABLE
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f0)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f1)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f2)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f3)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f4)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f5)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f6)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f7)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f8)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f9)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f10)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f11)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f12)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f13)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f14)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f15)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f16)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f17)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f18)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f19)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f20)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f21)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f22)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f23)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f24)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f25)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f26)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f27)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f28)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f29)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f30)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f31)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f32)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f33)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f34)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f35)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f36)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f37)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f38)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f39)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f40)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f41)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f42)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f43)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f44)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f45)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f46)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f47)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f48)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f49)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f50)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f51)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f52)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f53)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f54)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f55)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f56)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f57)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f58)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f59)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f60)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f61)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f62)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f63)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f64)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f65)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f66)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f67)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f68)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f69)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f70)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f71)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f72)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f73)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f74)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f75)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f76)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f77)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f78)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f79)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f80)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f81)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f82)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f83)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f84)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f85)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f86)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f87)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f88)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f89)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f90)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f91)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f92)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f93)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f94)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f95)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f96)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f97)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f98)
ZFMETHOD_FUNC_USER_REGISTER_0({}, void, _ZFP_ZFCore_CodeSize_test_MethodFuncUserRegister_f99)
#endif

// ============================================================
// property
#if _ZFP_ZFCore_CodeSize_test_Property_ENABLE
zfclass _ZFP_ZFCore_CodeSize_test_Property : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_CodeSize_test_Property, ZFObject)

    ZFPROPERTY_ASSIGN(zfstring, v0)
    ZFPROPERTY_ASSIGN(zfstring, v1)
    ZFPROPERTY_ASSIGN(zfstring, v2)
    ZFPROPERTY_ASSIGN(zfstring, v3)
    ZFPROPERTY_ASSIGN(zfstring, v4)
    ZFPROPERTY_ASSIGN(zfstring, v5)
    ZFPROPERTY_ASSIGN(zfstring, v6)
    ZFPROPERTY_ASSIGN(zfstring, v7)
    ZFPROPERTY_ASSIGN(zfstring, v8)
    ZFPROPERTY_ASSIGN(zfstring, v9)
    ZFPROPERTY_ASSIGN(zfstring, v10)
    ZFPROPERTY_ASSIGN(zfstring, v11)
    ZFPROPERTY_ASSIGN(zfstring, v12)
    ZFPROPERTY_ASSIGN(zfstring, v13)
    ZFPROPERTY_ASSIGN(zfstring, v14)
    ZFPROPERTY_ASSIGN(zfstring, v15)
    ZFPROPERTY_ASSIGN(zfstring, v16)
    ZFPROPERTY_ASSIGN(zfstring, v17)
    ZFPROPERTY_ASSIGN(zfstring, v18)
    ZFPROPERTY_ASSIGN(zfstring, v19)
    ZFPROPERTY_ASSIGN(zfstring, v20)
    ZFPROPERTY_ASSIGN(zfstring, v21)
    ZFPROPERTY_ASSIGN(zfstring, v22)
    ZFPROPERTY_ASSIGN(zfstring, v23)
    ZFPROPERTY_ASSIGN(zfstring, v24)
    ZFPROPERTY_ASSIGN(zfstring, v25)
    ZFPROPERTY_ASSIGN(zfstring, v26)
    ZFPROPERTY_ASSIGN(zfstring, v27)
    ZFPROPERTY_ASSIGN(zfstring, v28)
    ZFPROPERTY_ASSIGN(zfstring, v29)
    ZFPROPERTY_ASSIGN(zfstring, v30)
    ZFPROPERTY_ASSIGN(zfstring, v31)
    ZFPROPERTY_ASSIGN(zfstring, v32)
    ZFPROPERTY_ASSIGN(zfstring, v33)
    ZFPROPERTY_ASSIGN(zfstring, v34)
    ZFPROPERTY_ASSIGN(zfstring, v35)
    ZFPROPERTY_ASSIGN(zfstring, v36)
    ZFPROPERTY_ASSIGN(zfstring, v37)
    ZFPROPERTY_ASSIGN(zfstring, v38)
    ZFPROPERTY_ASSIGN(zfstring, v39)
    ZFPROPERTY_ASSIGN(zfstring, v40)
    ZFPROPERTY_ASSIGN(zfstring, v41)
    ZFPROPERTY_ASSIGN(zfstring, v42)
    ZFPROPERTY_ASSIGN(zfstring, v43)
    ZFPROPERTY_ASSIGN(zfstring, v44)
    ZFPROPERTY_ASSIGN(zfstring, v45)
    ZFPROPERTY_ASSIGN(zfstring, v46)
    ZFPROPERTY_ASSIGN(zfstring, v47)
    ZFPROPERTY_ASSIGN(zfstring, v48)
    ZFPROPERTY_ASSIGN(zfstring, v49)
    ZFPROPERTY_ASSIGN(zfstring, v50)
    ZFPROPERTY_ASSIGN(zfstring, v51)
    ZFPROPERTY_ASSIGN(zfstring, v52)
    ZFPROPERTY_ASSIGN(zfstring, v53)
    ZFPROPERTY_ASSIGN(zfstring, v54)
    ZFPROPERTY_ASSIGN(zfstring, v55)
    ZFPROPERTY_ASSIGN(zfstring, v56)
    ZFPROPERTY_ASSIGN(zfstring, v57)
    ZFPROPERTY_ASSIGN(zfstring, v58)
    ZFPROPERTY_ASSIGN(zfstring, v59)
    ZFPROPERTY_ASSIGN(zfstring, v60)
    ZFPROPERTY_ASSIGN(zfstring, v61)
    ZFPROPERTY_ASSIGN(zfstring, v62)
    ZFPROPERTY_ASSIGN(zfstring, v63)
    ZFPROPERTY_ASSIGN(zfstring, v64)
    ZFPROPERTY_ASSIGN(zfstring, v65)
    ZFPROPERTY_ASSIGN(zfstring, v66)
    ZFPROPERTY_ASSIGN(zfstring, v67)
    ZFPROPERTY_ASSIGN(zfstring, v68)
    ZFPROPERTY_ASSIGN(zfstring, v69)
    ZFPROPERTY_ASSIGN(zfstring, v70)
    ZFPROPERTY_ASSIGN(zfstring, v71)
    ZFPROPERTY_ASSIGN(zfstring, v72)
    ZFPROPERTY_ASSIGN(zfstring, v73)
    ZFPROPERTY_ASSIGN(zfstring, v74)
    ZFPROPERTY_ASSIGN(zfstring, v75)
    ZFPROPERTY_ASSIGN(zfstring, v76)
    ZFPROPERTY_ASSIGN(zfstring, v77)
    ZFPROPERTY_ASSIGN(zfstring, v78)
    ZFPROPERTY_ASSIGN(zfstring, v79)
    ZFPROPERTY_ASSIGN(zfstring, v80)
    ZFPROPERTY_ASSIGN(zfstring, v81)
    ZFPROPERTY_ASSIGN(zfstring, v82)
    ZFPROPERTY_ASSIGN(zfstring, v83)
    ZFPROPERTY_ASSIGN(zfstring, v84)
    ZFPROPERTY_ASSIGN(zfstring, v85)
    ZFPROPERTY_ASSIGN(zfstring, v86)
    ZFPROPERTY_ASSIGN(zfstring, v87)
    ZFPROPERTY_ASSIGN(zfstring, v88)
    ZFPROPERTY_ASSIGN(zfstring, v89)
    ZFPROPERTY_ASSIGN(zfstring, v90)
    ZFPROPERTY_ASSIGN(zfstring, v91)
    ZFPROPERTY_ASSIGN(zfstring, v92)
    ZFPROPERTY_ASSIGN(zfstring, v93)
    ZFPROPERTY_ASSIGN(zfstring, v94)
    ZFPROPERTY_ASSIGN(zfstring, v95)
    ZFPROPERTY_ASSIGN(zfstring, v96)
    ZFPROPERTY_ASSIGN(zfstring, v97)
    ZFPROPERTY_ASSIGN(zfstring, v98)
    ZFPROPERTY_ASSIGN(zfstring, v99)
};
ZFOBJECT_REGISTER(_ZFP_ZFCore_CodeSize_test_Property)
#endif

// ============================================================
// property user register
#if _ZFP_ZFCore_CodeSize_test_PropertyUserRegister_ENABLE
zfclass _ZFP_ZFCore_CodeSize_test_PropertyUserRegister : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, ZFObject)
};
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v0, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v1, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v2, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v3, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v4, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v5, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v6, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v7, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v8, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v9, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v10, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v11, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v12, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v13, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v14, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v15, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v16, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v17, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v18, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v19, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v20, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v21, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v22, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v23, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v24, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v25, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v26, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v27, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v28, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v29, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v30, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v31, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v32, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v33, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v34, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v35, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v36, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v37, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v38, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v39, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v40, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v41, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v42, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v43, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v44, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v45, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v46, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v47, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v48, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v49, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v50, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v51, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v52, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v53, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v54, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v55, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v56, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v57, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v58, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v59, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v60, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v61, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v62, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v63, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v64, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v65, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v66, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v67, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v68, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v69, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v70, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v71, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v72, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v73, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v74, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v75, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v76, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v77, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v78, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v79, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v80, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v81, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v82, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v83, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v84, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v85, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v86, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v87, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v88, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v89, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v90, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v91, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v92, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v93, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v94, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v95, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v96, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v97, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v98, ZFPropertyNoInitValue, public, public)
ZFPROPERTY_USER_REGISTER_ASSIGN(_ZFP_ZFCore_CodeSize_test_PropertyUserRegister, zfstring, v99, ZFPropertyNoInitValue, public, public)
#endif

// ============================================================
// property type
#if _ZFP_ZFCore_CodeSize_test_PropertyType_ENABLE
#define _ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(N) \
    zfclassNotPOD _ZFP_ZFCore_CodeSize_test_PropertyType_Class##N {}; \
    ZFTYPEID_ACCESS_ONLY_DECLARE(_ZFP_ZFCore_CodeSize_test_PropertyType_Class##N, _ZFP_ZFCore_CodeSize_test_PropertyType_Class##N) \
    ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(_ZFP_ZFCore_CodeSize_test_PropertyType_Class##N, _ZFP_ZFCore_CodeSize_test_PropertyType_Class##N)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(0)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(1)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(2)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(3)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(4)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(5)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(6)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(7)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(8)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(9)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(10)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(11)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(12)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(13)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(14)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(15)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(16)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(17)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(18)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(19)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(20)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(21)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(22)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(23)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(24)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(25)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(26)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(27)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(28)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(29)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(30)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(31)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(32)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(33)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(34)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(35)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(36)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(37)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(38)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(39)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(40)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(41)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(42)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(43)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(44)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(45)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(46)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(47)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(48)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(49)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(50)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(51)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(52)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(53)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(54)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(55)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(56)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(57)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(58)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(59)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(60)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(61)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(62)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(63)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(64)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(65)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(66)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(67)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(68)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(69)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(70)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(71)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(72)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(73)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(74)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(75)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(76)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(77)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(78)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(79)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(80)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(81)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(82)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(83)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(84)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(85)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(86)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(87)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(88)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(89)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(90)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(91)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(92)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(93)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(94)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(95)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(96)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(97)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(98)
_ZFP_ZFCore_CodeSize_test_PropertyType_EXPAND(99)
#endif

ZF_NAMESPACE_GLOBAL_END

