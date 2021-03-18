/**
 * @file ZFCoreUtilMacro.h
 * @brief macro utils
 */
#ifndef _ZFI_ZFCoreUtilMacro_h_
#define _ZFI_ZFCoreUtilMacro_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief macro to detect whether bit is set, zftrue only if all the bits in bit is set
 *
 * note that test by 0 is always zftrue
 */
#define ZFBitTest(var, bit) (((var) & (bit)) == (bit))
/**
 * @brief macro to get the value at bit
 */
#define ZFBitGet(var, bit) ((var) & (bit))
/**
 * @brief macro to set at bit
 */
#define ZFBitSet(var, bit) ((var) |= (bit))
/**
 * @brief clear at bit
 */
#define ZFBitUnset(var, bit) ((var) &= (~(bit)))

// ============================================================
/**
 * @brief dummy macro to show a hint
 *
 * used to make some code more human readable, no actual use\n
 * for example, add to function call's param list to make params more readable:
 * @code
 *   void yourFunc(zfbool clickable, zfbool moveable) {...}
 *   ...
 *   yourFunc(
 *       zfHint("clickable?")zftrue,
 *       zfHint("moveable?")zffalse);
 * @endcode
 */
#define zfHint(hint)

// ============================================================
#define _ZFP_ZFM_REPEAT_0(Type, LeftCommaFix, CenterCommaFix)
#define _ZFP_ZFM_REPEAT_1(Type, LeftCommaFix, CenterCommaFix) LeftCommaFix() Type(0)
#define _ZFP_ZFM_REPEAT_2(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_1(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(1)
#define _ZFP_ZFM_REPEAT_3(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_2(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(2)
#define _ZFP_ZFM_REPEAT_4(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_3(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(3)
#define _ZFP_ZFM_REPEAT_5(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_4(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(4)
#define _ZFP_ZFM_REPEAT_6(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_5(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(5)
#define _ZFP_ZFM_REPEAT_7(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_6(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(6)
#define _ZFP_ZFM_REPEAT_8(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_7(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(7)
#define _ZFP_ZFM_REPEAT_9(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_8(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(8)
#define _ZFP_ZFM_REPEAT_10(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_9(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(9)
#define _ZFP_ZFM_REPEAT_11(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_10(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(10)
#define _ZFP_ZFM_REPEAT_12(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_11(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(11)
#define _ZFP_ZFM_REPEAT_13(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_12(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(12)
#define _ZFP_ZFM_REPEAT_14(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_13(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(13)
#define _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_14(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(14)
#define _ZFP_ZFM_REPEAT_16(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(15)
#define _ZFP_ZFM_REPEAT_17(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(16)
#define _ZFP_ZFM_REPEAT_18(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(17)
#define _ZFP_ZFM_REPEAT_19(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(18)
#define _ZFP_ZFM_REPEAT_20(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(19)
#define _ZFP_ZFM_REPEAT_21(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(20)
#define _ZFP_ZFM_REPEAT_22(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(21)
#define _ZFP_ZFM_REPEAT_23(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(22)
#define _ZFP_ZFM_REPEAT_24(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(23)
#define _ZFP_ZFM_REPEAT_25(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(24)
#define _ZFP_ZFM_REPEAT_26(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(25)
#define _ZFP_ZFM_REPEAT_27(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(26)
#define _ZFP_ZFM_REPEAT_28(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(27)
#define _ZFP_ZFM_REPEAT_29(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(28)
#define _ZFP_ZFM_REPEAT_30(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(29)
#define _ZFP_ZFM_REPEAT_31(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(30)
#define _ZFP_ZFM_REPEAT_32(Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_15(Type, LeftCommaFix, CenterCommaFix) CenterCommaFix() Type(31)
#define _ZFP_ZFM_REPEAT(N, Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT_##N(Type, LeftCommaFix, CenterCommaFix)
/**
 * @brief macro to repeat something
 *
 * for example\n
 *   ZFM_REPEAT(2, ZFM_REPEAT_TEMPLATE, ZFM_EMPTY, ZFM_COMMA)\n
 * would be expand as:\n
 *   typename Type0, typename Type1\n
 * and
 *   ZFM_REPEAT(3, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA)\n
 * would be expand as:\n
 *   , Type0 param0, Type1 param1, Type2 param2
 */
#define ZFM_REPEAT(N, Type, LeftCommaFix, CenterCommaFix) _ZFP_ZFM_REPEAT(N, Type, LeftCommaFix, CenterCommaFix)

#define _ZFP_ZFM_REPEAT_TEMPLATE(N) typename Type##N
#define _ZFP_ZFM_REPEAT_TYPE(N) Type##N
#define _ZFP_ZFM_REPEAT_TYPE_CONST(N) Type##N const &
#define _ZFP_ZFM_REPEAT_NAME(N) param##N
#define _ZFP_ZFM_REPEAT_PARAM(N) Type##N param##N

/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFM_REPEAT_TEMPLATE(N) _ZFP_ZFM_REPEAT_TEMPLATE(N)
/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFM_REPEAT_TYPE(N) _ZFP_ZFM_REPEAT_TYPE(N)
/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFM_REPEAT_TYPE_CONST(N) _ZFP_ZFM_REPEAT_TYPE_CONST(N)
/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFM_REPEAT_NAME(N) _ZFP_ZFM_REPEAT_NAME(N)
/**
 * @brief usually used by #ZFM_REPEAT
 */
#define ZFM_REPEAT_PARAM(N) _ZFP_ZFM_REPEAT_PARAM(N)

// ============================================================
/**
 * @brief macro to (
 */
#define ZFM_BRACKET_L() (
/**
 * @brief macro to )
 */
#define ZFM_BRACKET_R() )

/**
 * @brief macro to s space
 */
#define ZFM_EMPTY(...)
/**
 * @brief macro to a comma
 */
#define ZFM_COMMA() ,

/**
 * @brief macro to expand a macro
 */
#define ZFM_EXPAND(...) __VA_ARGS__

/**
 * @brief concatenate a and b to ab without expand a or b
 */
#define ZFM_CAT_DIRECT(a, b) a##b
/**
 * @brief expand a and b, then concatenate to ab
 */
#define ZFM_CAT(a, b) ZFM_CAT_DIRECT(a, b)

/**
 * @brief convert macro to string
 */
#define ZFM_TOSTRING_DIRECT(a) #a
/**
 * @brief convert macro to string
 */
#define ZFM_TOSTRING(a) ZFM_TOSTRING_DIRECT(a)

// ============================================================
#define _ZFP_ZFM_N_INC_T(n) _ZFP_ZFM_N_INC_##n
#define _ZFP_ZFM_N_INC_0 1
#define _ZFP_ZFM_N_INC_1 2
#define _ZFP_ZFM_N_INC_2 3
#define _ZFP_ZFM_N_INC_3 4
#define _ZFP_ZFM_N_INC_4 5
#define _ZFP_ZFM_N_INC_5 6
#define _ZFP_ZFM_N_INC_6 7
#define _ZFP_ZFM_N_INC_7 8
#define _ZFP_ZFM_N_INC_8 9
#define _ZFP_ZFM_N_INC_9 10
#define _ZFP_ZFM_N_INC_10 11
#define _ZFP_ZFM_N_INC_11 12
#define _ZFP_ZFM_N_INC_12 13
#define _ZFP_ZFM_N_INC_13 14
#define _ZFP_ZFM_N_INC_14 15
#define _ZFP_ZFM_N_INC_15 16
#define _ZFP_ZFM_N_INC_16 17
#define _ZFP_ZFM_N_INC_17 18
#define _ZFP_ZFM_N_INC_18 19
#define _ZFP_ZFM_N_INC_19 20
#define _ZFP_ZFM_N_INC_20 21
#define _ZFP_ZFM_N_INC_21 22
#define _ZFP_ZFM_N_INC_22 23
#define _ZFP_ZFM_N_INC_23 24
#define _ZFP_ZFM_N_INC_24 25
#define _ZFP_ZFM_N_INC_25 26
#define _ZFP_ZFM_N_INC_26 27
#define _ZFP_ZFM_N_INC_27 28
#define _ZFP_ZFM_N_INC_28 29
#define _ZFP_ZFM_N_INC_29 30
#define _ZFP_ZFM_N_INC_30 31
#define _ZFP_ZFM_N_INC_31 32
/**
 * @brief increase n by macro expand
 */
#define ZFM_N_INC(n) _ZFP_ZFM_N_INC_T(n)

#define _ZFP_ZFM_N_DEC_T(n) _ZFP_ZFM_N_DEC_##n
#define _ZFP_ZFM_N_DEC_0 -1
#define _ZFP_ZFM_N_DEC_1 0
#define _ZFP_ZFM_N_DEC_2 1
#define _ZFP_ZFM_N_DEC_3 2
#define _ZFP_ZFM_N_DEC_4 3
#define _ZFP_ZFM_N_DEC_5 4
#define _ZFP_ZFM_N_DEC_6 5
#define _ZFP_ZFM_N_DEC_7 6
#define _ZFP_ZFM_N_DEC_8 7
#define _ZFP_ZFM_N_DEC_9 8
#define _ZFP_ZFM_N_DEC_10 9
#define _ZFP_ZFM_N_DEC_11 10
#define _ZFP_ZFM_N_DEC_12 11
#define _ZFP_ZFM_N_DEC_13 12
#define _ZFP_ZFM_N_DEC_14 13
#define _ZFP_ZFM_N_DEC_15 14
#define _ZFP_ZFM_N_DEC_16 15
#define _ZFP_ZFM_N_DEC_17 16
#define _ZFP_ZFM_N_DEC_18 17
#define _ZFP_ZFM_N_DEC_19 18
#define _ZFP_ZFM_N_DEC_20 19
#define _ZFP_ZFM_N_DEC_21 20
#define _ZFP_ZFM_N_DEC_22 21
#define _ZFP_ZFM_N_DEC_23 22
#define _ZFP_ZFM_N_DEC_24 23
#define _ZFP_ZFM_N_DEC_25 24
#define _ZFP_ZFM_N_DEC_26 25
#define _ZFP_ZFM_N_DEC_27 26
#define _ZFP_ZFM_N_DEC_28 27
#define _ZFP_ZFM_N_DEC_29 28
#define _ZFP_ZFM_N_DEC_30 29
#define _ZFP_ZFM_N_DEC_31 30
#define _ZFP_ZFM_N_DEC_32 31
/**
 * @brief decrease n by macro expand
 */
#define ZFM_N_DEC(n) _ZFP_ZFM_N_DEC_T(n)

#define _ZFP_ZFM_PARAM_EXPAND(...) __VA_ARGS__
/**
 * @brief get the param num of a certain sequence
 *
 * usage:
 * @code
 *   #define _EMPTY
 *   zfindex n = 0;
 *   n = ZFM_PARAM_NUM(a, b, c); // n would be 3
 *   n = ZFM_PARAM_NUM(_EMPTY); // n would be 1
 *   n = ZFM_PARAM_NUM(); // result not ensured, may be 0 or 1
 * @endcode
 * this macro could calculate 1 ~ 32 param num
 */
#define ZFM_PARAM_NUM(...) \
    _ZFP_ZFM_PARAM_EXPAND(_ZFP_ZFM_PARAM_NUM_TMP0(__VA_ARGS__, _ZFP_ZFM_PARAM_NUM_TMP2()))
#define _ZFP_ZFM_PARAM_NUM_TMP0(...) \
    _ZFP_ZFM_PARAM_EXPAND(_ZFP_ZFM_PARAM_NUM_TMP1(__VA_ARGS__))
#define _ZFP_ZFM_PARAM_NUM_TMP1(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, N, ...) \
    N
#define _ZFP_ZFM_PARAM_NUM_TMP2() \
    32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

/**
 * @brief fix each param in the __VA_ARGS__
 *
 * usage:
 * @code
 *   // define your own expand method
 *   #define EXPAND(arg) #arg " "
 *
 *   // result is:
 *   //   "1" " " "2" " "  "3" " "
 *   // which is same as:
 *   //   "1 2 3 "
 *   ZFM_FIX_PARAM(EXPAND, ZFM_EMPTY, 1, 2, 3)
 * @endcode
 */
#define ZFM_FIX_PARAM(ParamFix, commaFix, ...) \
    _ZFP_ZFM_FIX_PARAM_CALL_01(_ZFP_ZFM_FIX_PARAM_N(_0, ##__VA_ARGS__ \
            , _ZFP_ZFM_FIX_PARAM_E_33, _ZFP_ZFM_FIX_PARAM_E_32, _ZFP_ZFM_FIX_PARAM_E_31, _ZFP_ZFM_FIX_PARAM_E_30 \
            , _ZFP_ZFM_FIX_PARAM_E_29, _ZFP_ZFM_FIX_PARAM_E_28, _ZFP_ZFM_FIX_PARAM_E_27, _ZFP_ZFM_FIX_PARAM_E_26, _ZFP_ZFM_FIX_PARAM_E_25 \
            , _ZFP_ZFM_FIX_PARAM_E_24, _ZFP_ZFM_FIX_PARAM_E_23, _ZFP_ZFM_FIX_PARAM_E_22, _ZFP_ZFM_FIX_PARAM_E_21, _ZFP_ZFM_FIX_PARAM_E_20 \
            , _ZFP_ZFM_FIX_PARAM_E_19, _ZFP_ZFM_FIX_PARAM_E_18, _ZFP_ZFM_FIX_PARAM_E_17, _ZFP_ZFM_FIX_PARAM_E_16, _ZFP_ZFM_FIX_PARAM_E_15 \
            , _ZFP_ZFM_FIX_PARAM_E_14, _ZFP_ZFM_FIX_PARAM_E_13, _ZFP_ZFM_FIX_PARAM_E_12, _ZFP_ZFM_FIX_PARAM_E_11, _ZFP_ZFM_FIX_PARAM_E_10 \
            , _ZFP_ZFM_FIX_PARAM_E_09, _ZFP_ZFM_FIX_PARAM_E_08, _ZFP_ZFM_FIX_PARAM_E_07, _ZFP_ZFM_FIX_PARAM_E_06, _ZFP_ZFM_FIX_PARAM_E_05 \
            , _ZFP_ZFM_FIX_PARAM_E_04, _ZFP_ZFM_FIX_PARAM_E_03, _ZFP_ZFM_FIX_PARAM_E_02, _ZFP_ZFM_FIX_PARAM_E_01, _ZFP_ZFM_FIX_PARAM_E_00 \
        ), (ParamFix, commaFix, ##__VA_ARGS__))

#define _ZFP_ZFM_FIX_PARAM_CALL_01(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_02(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_03(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_04(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_04(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_05(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_06(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_07(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_08(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_09(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_10(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_11(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_12(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_13(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_14(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_15(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_16(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_17(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_18(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_19(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_20(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_21(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_22(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_23(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_24(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_25(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_26(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_27(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_28(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_29(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_30(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_31(C, v) C v
#define _ZFP_ZFM_FIX_PARAM_CALL_32(C, v) C v

#define _ZFP_ZFM_FIX_PARAM_E_00()
#define _ZFP_ZFM_FIX_PARAM_E_01(ParamFix, commaFix, a, ...) ParamFix(a)
#define _ZFP_ZFM_FIX_PARAM_E_02(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_02(_ZFP_ZFM_FIX_PARAM_E_01, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_03(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_03(_ZFP_ZFM_FIX_PARAM_E_02, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_04(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_04(_ZFP_ZFM_FIX_PARAM_E_03, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_05(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_05(_ZFP_ZFM_FIX_PARAM_E_04, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_06(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_06(_ZFP_ZFM_FIX_PARAM_E_05, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_07(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_07(_ZFP_ZFM_FIX_PARAM_E_06, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_08(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_08(_ZFP_ZFM_FIX_PARAM_E_07, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_09(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_09(_ZFP_ZFM_FIX_PARAM_E_08, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_10(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_10(_ZFP_ZFM_FIX_PARAM_E_09, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_11(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_11(_ZFP_ZFM_FIX_PARAM_E_10, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_12(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_12(_ZFP_ZFM_FIX_PARAM_E_11, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_13(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_13(_ZFP_ZFM_FIX_PARAM_E_12, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_14(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_14(_ZFP_ZFM_FIX_PARAM_E_13, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_15(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_15(_ZFP_ZFM_FIX_PARAM_E_14, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_16(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_16(_ZFP_ZFM_FIX_PARAM_E_15, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_17(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_17(_ZFP_ZFM_FIX_PARAM_E_16, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_18(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_18(_ZFP_ZFM_FIX_PARAM_E_17, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_19(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_19(_ZFP_ZFM_FIX_PARAM_E_18, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_20(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_20(_ZFP_ZFM_FIX_PARAM_E_19, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_21(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_21(_ZFP_ZFM_FIX_PARAM_E_20, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_22(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_22(_ZFP_ZFM_FIX_PARAM_E_21, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_23(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_23(_ZFP_ZFM_FIX_PARAM_E_22, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_24(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_24(_ZFP_ZFM_FIX_PARAM_E_23, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_25(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_25(_ZFP_ZFM_FIX_PARAM_E_24, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_26(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_26(_ZFP_ZFM_FIX_PARAM_E_25, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_27(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_27(_ZFP_ZFM_FIX_PARAM_E_26, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_28(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_28(_ZFP_ZFM_FIX_PARAM_E_27, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_29(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_29(_ZFP_ZFM_FIX_PARAM_E_28, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_30(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_30(_ZFP_ZFM_FIX_PARAM_E_29, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_31(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_31(_ZFP_ZFM_FIX_PARAM_E_30, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_32(ParamFix, commaFix, a, ...) ParamFix(a) commaFix() _ZFP_ZFM_FIX_PARAM_CALL_32(_ZFP_ZFM_FIX_PARAM_E_31, (ParamFix, commaFix, ##__VA_ARGS__))
#define _ZFP_ZFM_FIX_PARAM_E_33(...) ERROR: supports up to 32 arguments

#define _ZFP_ZFM_FIX_PARAM_N( \
         _00,_01,_02,_03,_04,_05,_06,_07,_08,_09 \
        ,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19 \
        ,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29 \
        ,_30,_31,_32,_33 \
    , NAME, ...) NAME


// ============================================================
#define _ZFP_zfidentityHash_EXPAND(v) (zft_zfidentity)(v)
#define _ZFP_zfidentityHash_COMMA() ^
/**
 * @brief util method to connect multiple hash value into one hash value
 */
#define zfidentityHash(hash, ...) \
    (zft_zfidentity)(ZFM_FIX_PARAM(_ZFP_zfidentityHash_EXPAND, _ZFP_zfidentityHash_COMMA, hash, ##__VA_ARGS__))

// ============================================================
/**
 * @brief explicit declare v is not used
 */
#define ZFUNUSED(v) ((void)(v))

// ============================================================
/**
 * @brief calculate the size of an array
 */
#define ZFM_ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

// ============================================================
/**
 * @brief declare a unique name
 *
 * typical usage:
 * @code
 *   // assume this line is line 1
 *   int ZFUniqueName(a) = 0; // same as "int _ZFP_zUniqueName_a_2 = 0;"
 *   int ZFUniqueName(a) = 0; // same as "int _ZFP_zUniqueName_a_3 = 0;"
 * @endcode
 * this is usually used to declare a tmp variable, such as:
 * @code
 *   class MyClass
 *   {
 *   public:
 *       zfbool flag;
 *       MyClass() : flag(zftrue) {}
 *   };
 *   #define MyClassBlock for(MyClass ZFUniqueName(cls); \\
 *       ZFUniqueName(cls).flag; \\
 *       ZFUniqueName(cls).flag = zffalse)
 *
 *   // to use the blocked code
 *   MyClassBlock
 *   {
 *       // code block
 *       // there'll be a instance of MyClass named _ZFP_zUniqueName_cls_123
 *   } // and the MyClass's instance will be deconstructed after the brace
 * @endcode
 *
 * REMARKS:\n
 * when used under Windows with embeded macros
 * @code
 *   int ZFM_EXPAND(ZFUniqueName(n)) = 0;
 * @endcode
 * a C2065 "__LINE__Var undeclared identifier" may (or may not) occurred\n
 * this is a compiler bug\n
 * to solve it, turn "Program Database for edit and continue (/ZI)"
 * to "Program Database (/Zi)" in your project setting\n
 * or, prevent use it within other macros
 */
#define ZFUniqueName(name) _ZFP_ZFUniqueName2(name, __LINE__)
#define _ZFP_ZFUniqueName2(name, line) _ZFP_ZFUniqueName3(name, line)
#define _ZFP_ZFUniqueName3(name, line) _ZFP_zfunique_##name##_##line

// ============================================================
/**
 * @brief util macro to check whether class has desired member function,
 *   for advanced use only
 *
 * usage:
 * @code
 *   // declare before use
 *   ZFM_CLASS_HAS_MEMBER_DECLARE(
 *       YourNamespace, // namespace for avoid conflict
 *       yourMemberName, // member method name to check
 *       void (T::*F)(void) // member sig (T and F is reserved keyword for class and member type)
 *   )
 *
 *   // once declared, you may check like this
 *   // value would be true only if "ClassToCheck" has such member method:
 *   // void ClassToCheck::yourMethodName(void); // can be static or virtual
 *   zfbool value = ZFM_CLASS_HAS_MEMBER(YourNamespace, yourMethodName, ClassToCheck);
 *
 *   // typical method sig:
 *   static void func(void)
 *       : void (*F)(void)
 *   virtual void func(void)
 *       : void (T::*F)(void)
 * @endcode
 *
 * limitations:
 * -  for static member methods, if parent have, then child type would also have,
 *   no way to check whether only child have
 * -  for non-public methods, the ZFM_CLASS_HAS_MEMBER_DECLARE must be
 *   declared in class scope to take effect
 */
#define ZFM_CLASS_HAS_MEMBER_DECLARE(NameSpace, memberName, memberSig) \
    /** @cond ZFPrivateDoc */ \
    template<typename T_Owner> \
    zfclassNotPOD _ZFP_ZFM_CLASS_HAS_MEMBER_##NameSpace##_##memberName \
    { \
    private: \
        template <typename T, memberSig> struct _Fix {}; \
        template<typename T> static char _has(_Fix<T, &T::memberName> *); \
        template<typename T> static int _has(...); \
    public: \
        enum { value = (sizeof(_has<T_Owner>(0)) == sizeof(char)) }; \
    }; \
    /** @endcond */
/**
 * @brief see #ZFM_CLASS_HAS_MEMBER_DECLARE
 */
#define ZFM_CLASS_HAS_MEMBER(NameSpace, memberName, ClassToCheck) \
    (_ZFP_ZFM_CLASS_HAS_MEMBER_##NameSpace##_##memberName<ClassToCheck>::value != 0)

// ============================================================
/**
 * @brief see #ZFCORE_PARAM
 */
#define ZFCORE_PARAM_DECLARE_SELF(T_self) \
    protected: \
        /** @cond ZFPrivateDoc */ \
        typedef T_self _ZFP_ZFCoreParam_self; \
        /** @endcond */ \
    public:
/**
 * @brief a util macro to generate setter and getter for POD like object
 *
 * usage:
 * @code
 *   zfclassLikePOD YourType
 *   {
 *   protected:
 *       // declare self is required
 *       ZFCORE_PARAM_DECLARE_SELF(YourType)
 *
 *       / **
 *        * @ brief you can add Doxygen docs here
 *        * /
 *       ZFCORE_PARAM(YourType, YourParamType, paramName)
 *   };
 * @endcode
 * once declared, you can access your param by the generated getter and setter
 * @code
 *   YourType v;
 *   v.paramName(someValue);
 *   YourParamType paramValue = v.paramName();
 * @endcode
 */
#define ZFCORE_PARAM(T_ParamType, paramName) \
    ZFCORE_PARAM_WITH_INIT(T_ParamType, paramName, ZFM_EMPTY())
/**
 * @brief see #ZFCORE_PARAM
 */
#define ZFCORE_PARAM_WITH_INIT(T_ParamType, paramName, initValue) \
    public: \
        inline T_ParamType const &paramName(void) const \
        { \
            return this->_ZFP_##paramName.value; \
        } \
        /** @brief see @ref paramName */ \
        inline T_ParamType &paramName(void) \
        { \
            return this->_ZFP_##paramName.value; \
        } \
    private: \
        /** @cond ZFPrivateDoc */ \
        zfclassLikePOD ZF_ENV_EXPORT _ZFP_T_##paramName \
        { \
        public: \
            _ZFP_T_##paramName(void) \
            : value(initValue) \
            { \
            } \
            _ZFP_T_##paramName(ZF_IN const _ZFP_T_##paramName &ref) \
            : value(ref.value) \
            { \
            } \
            _ZFP_T_##paramName &operator = (ZF_IN const _ZFP_T_##paramName &ref) \
            { \
                this->value = ref.value; \
                return *this; \
            } \
        public: \
            T_ParamType value; \
        }; \
        _ZFP_T_##paramName _ZFP_##paramName; \
        /** @endcond */ \
    public: \
        /** @brief see @ref paramName */ \
        inline _ZFP_ZFCoreParam_self &paramName(ZF_IN T_ParamType const &value) \
        { \
            this->_ZFP_##paramName.value = value; \
            return *this; \
        }

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreUtilMacro_h_

