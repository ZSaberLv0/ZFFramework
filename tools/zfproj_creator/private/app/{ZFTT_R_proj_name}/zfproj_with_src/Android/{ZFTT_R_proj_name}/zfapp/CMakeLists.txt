cmake_minimum_required(VERSION 3.4.1)

set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework)

if(WIN32)
    execute_process(COMMAND ${ZF_ROOT_PATH}/zfsetup.bat)
else()
    execute_process(COMMAND ${ZF_ROOT_PATH}/zfsetup.sh)
endif()

function(ZFAddSrc ZF_SRC_FILES_ PROJ_PATH_)
    file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR}
        ${PROJ_PATH_}/zfsrc/*.c
        ${PROJ_PATH_}/zfsrc/*.cpp
        ${PROJ_PATH_}/zfsrc_ext/*.c
        ${PROJ_PATH_}/zfsrc_ext/*.cpp
        )
    set(${ZF_SRC_FILES_} ${${ZF_SRC_FILES_}} ${srcfiles} PARENT_SCOPE)

    include_directories(${PROJ_PATH_}/zfsrc)
endfunction(ZFAddSrc)


ZFAddSrc(ZF_SRC_FILES ${PROJECT_SOURCE_DIR}/../../../..)
{ZFTT_C_lib_require_0}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_0}){ZFTT_CE}{ZFTT_C_lib_require_1}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_1}){ZFTT_CE}{ZFTT_C_lib_require_2}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_2}){ZFTT_CE}{ZFTT_C_lib_require_3}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_3}){ZFTT_CE}{ZFTT_C_lib_require_4}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_4}){ZFTT_CE}{ZFTT_C_lib_require_5}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_5}){ZFTT_CE}{ZFTT_C_lib_require_6}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_6}){ZFTT_CE}{ZFTT_C_lib_require_7}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_7}){ZFTT_CE}{ZFTT_C_lib_require_8}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_8}){ZFTT_CE}{ZFTT_C_lib_require_9}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_9}){ZFTT_CE}{ZFTT_C_lib_require_10}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_10}){ZFTT_CE}{ZFTT_C_lib_require_11}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_11}){ZFTT_CE}{ZFTT_C_lib_require_12}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_12}){ZFTT_CE}{ZFTT_C_lib_require_13}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_13}){ZFTT_CE}{ZFTT_C_lib_require_14}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_14}){ZFTT_CE}{ZFTT_C_lib_require_15}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_15}){ZFTT_CE}{ZFTT_C_lib_require_16}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_16}){ZFTT_CE}{ZFTT_C_lib_require_17}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_17}){ZFTT_CE}{ZFTT_C_lib_require_18}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_18}){ZFTT_CE}{ZFTT_C_lib_require_19}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_19}){ZFTT_CE}{ZFTT_C_lib_require_20}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_20}){ZFTT_CE}{ZFTT_C_lib_require_21}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_21}){ZFTT_CE}{ZFTT_C_lib_require_22}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_22}){ZFTT_CE}{ZFTT_C_lib_require_23}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_23}){ZFTT_CE}{ZFTT_C_lib_require_24}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_24}){ZFTT_CE}{ZFTT_C_lib_require_25}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_25}){ZFTT_CE}{ZFTT_C_lib_require_26}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_26}){ZFTT_CE}{ZFTT_C_lib_require_27}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_27}){ZFTT_CE}{ZFTT_C_lib_require_28}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_28}){ZFTT_CE}{ZFTT_C_lib_require_29}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_29}){ZFTT_CE}{ZFTT_C_lib_require_30}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_30}){ZFTT_CE}{ZFTT_C_lib_require_31}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_31}){ZFTT_CE}{ZFTT_C_impl_require_0}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_0}){ZFTT_CE}{ZFTT_C_impl_require_1}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_1}){ZFTT_CE}{ZFTT_C_impl_require_2}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_2}){ZFTT_CE}{ZFTT_C_impl_require_3}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_3}){ZFTT_CE}{ZFTT_C_impl_require_4}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_4}){ZFTT_CE}{ZFTT_C_impl_require_5}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_5}){ZFTT_CE}{ZFTT_C_impl_require_6}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_6}){ZFTT_CE}{ZFTT_C_impl_require_7}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_7}){ZFTT_CE}{ZFTT_C_impl_require_8}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_8}){ZFTT_CE}{ZFTT_C_impl_require_9}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_9}){ZFTT_CE}{ZFTT_C_impl_require_10}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_10}){ZFTT_CE}{ZFTT_C_impl_require_11}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_11}){ZFTT_CE}{ZFTT_C_impl_require_12}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_12}){ZFTT_CE}{ZFTT_C_impl_require_13}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_13}){ZFTT_CE}{ZFTT_C_impl_require_14}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_14}){ZFTT_CE}{ZFTT_C_impl_require_15}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_15}){ZFTT_CE}{ZFTT_C_impl_require_16}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_16}){ZFTT_CE}{ZFTT_C_impl_require_17}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_17}){ZFTT_CE}{ZFTT_C_impl_require_18}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_18}){ZFTT_CE}{ZFTT_C_impl_require_19}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_19}){ZFTT_CE}{ZFTT_C_impl_require_20}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_20}){ZFTT_CE}{ZFTT_C_impl_require_21}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_21}){ZFTT_CE}{ZFTT_C_impl_require_22}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_22}){ZFTT_CE}{ZFTT_C_impl_require_23}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_23}){ZFTT_CE}{ZFTT_C_impl_require_24}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_24}){ZFTT_CE}{ZFTT_C_impl_require_25}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_25}){ZFTT_CE}{ZFTT_C_impl_require_26}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_26}){ZFTT_CE}{ZFTT_C_impl_require_27}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_27}){ZFTT_CE}{ZFTT_C_impl_require_28}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_28}){ZFTT_CE}{ZFTT_C_impl_require_29}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_29}){ZFTT_CE}{ZFTT_C_impl_require_30}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_30}){ZFTT_CE}{ZFTT_C_impl_require_31}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_31}){ZFTT_CE}{ZFTT_C_lib_ext_require_0}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_0}){ZFTT_CE}{ZFTT_C_lib_ext_require_1}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_1}){ZFTT_CE}{ZFTT_C_lib_ext_require_2}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_2}){ZFTT_CE}{ZFTT_C_lib_ext_require_3}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_3}){ZFTT_CE}{ZFTT_C_lib_ext_require_4}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_4}){ZFTT_CE}{ZFTT_C_lib_ext_require_5}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_5}){ZFTT_CE}{ZFTT_C_lib_ext_require_6}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_6}){ZFTT_CE}{ZFTT_C_lib_ext_require_7}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_7}){ZFTT_CE}{ZFTT_C_lib_ext_require_8}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_8}){ZFTT_CE}{ZFTT_C_lib_ext_require_9}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_9}){ZFTT_CE}{ZFTT_C_lib_ext_require_10}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_10}){ZFTT_CE}{ZFTT_C_lib_ext_require_11}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_11}){ZFTT_CE}{ZFTT_C_lib_ext_require_12}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_12}){ZFTT_CE}{ZFTT_C_lib_ext_require_13}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_13}){ZFTT_CE}{ZFTT_C_lib_ext_require_14}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_14}){ZFTT_CE}{ZFTT_C_lib_ext_require_15}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_15}){ZFTT_CE}{ZFTT_C_lib_ext_require_16}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_16}){ZFTT_CE}{ZFTT_C_lib_ext_require_17}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_17}){ZFTT_CE}{ZFTT_C_lib_ext_require_18}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_18}){ZFTT_CE}{ZFTT_C_lib_ext_require_19}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_19}){ZFTT_CE}{ZFTT_C_lib_ext_require_20}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_20}){ZFTT_CE}{ZFTT_C_lib_ext_require_21}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_21}){ZFTT_CE}{ZFTT_C_lib_ext_require_22}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_22}){ZFTT_CE}{ZFTT_C_lib_ext_require_23}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_23}){ZFTT_CE}{ZFTT_C_lib_ext_require_24}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_24}){ZFTT_CE}{ZFTT_C_lib_ext_require_25}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_25}){ZFTT_CE}{ZFTT_C_lib_ext_require_26}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_26}){ZFTT_CE}{ZFTT_C_lib_ext_require_27}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_27}){ZFTT_CE}{ZFTT_C_lib_ext_require_28}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_28}){ZFTT_CE}{ZFTT_C_lib_ext_require_29}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_29}){ZFTT_CE}{ZFTT_C_lib_ext_require_30}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_30}){ZFTT_CE}{ZFTT_C_lib_ext_require_31}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_31}){ZFTT_CE}{ZFTT_C_impl_ext_require_0}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_0}){ZFTT_CE}{ZFTT_C_impl_ext_require_1}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_1}){ZFTT_CE}{ZFTT_C_impl_ext_require_2}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_2}){ZFTT_CE}{ZFTT_C_impl_ext_require_3}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_3}){ZFTT_CE}{ZFTT_C_impl_ext_require_4}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_4}){ZFTT_CE}{ZFTT_C_impl_ext_require_5}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_5}){ZFTT_CE}{ZFTT_C_impl_ext_require_6}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_6}){ZFTT_CE}{ZFTT_C_impl_ext_require_7}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_7}){ZFTT_CE}{ZFTT_C_impl_ext_require_8}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_8}){ZFTT_CE}{ZFTT_C_impl_ext_require_9}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_9}){ZFTT_CE}{ZFTT_C_impl_ext_require_10}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_10}){ZFTT_CE}{ZFTT_C_impl_ext_require_11}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_11}){ZFTT_CE}{ZFTT_C_impl_ext_require_12}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_12}){ZFTT_CE}{ZFTT_C_impl_ext_require_13}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_13}){ZFTT_CE}{ZFTT_C_impl_ext_require_14}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_14}){ZFTT_CE}{ZFTT_C_impl_ext_require_15}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_15}){ZFTT_CE}{ZFTT_C_impl_ext_require_16}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_16}){ZFTT_CE}{ZFTT_C_impl_ext_require_17}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_17}){ZFTT_CE}{ZFTT_C_impl_ext_require_18}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_18}){ZFTT_CE}{ZFTT_C_impl_ext_require_19}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_19}){ZFTT_CE}{ZFTT_C_impl_ext_require_20}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_20}){ZFTT_CE}{ZFTT_C_impl_ext_require_21}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_21}){ZFTT_CE}{ZFTT_C_impl_ext_require_22}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_22}){ZFTT_CE}{ZFTT_C_impl_ext_require_23}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_23}){ZFTT_CE}{ZFTT_C_impl_ext_require_24}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_24}){ZFTT_CE}{ZFTT_C_impl_ext_require_25}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_25}){ZFTT_CE}{ZFTT_C_impl_ext_require_26}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_26}){ZFTT_CE}{ZFTT_C_impl_ext_require_27}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_27}){ZFTT_CE}{ZFTT_C_impl_ext_require_28}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_28}){ZFTT_CE}{ZFTT_C_impl_ext_require_29}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_29}){ZFTT_CE}{ZFTT_C_impl_ext_require_30}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_30}){ZFTT_CE}{ZFTT_C_impl_ext_require_31}
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_31}){ZFTT_CE}

add_library({ZFTT_R_proj_name} SHARED ${ZF_SRC_FILES})
set_target_properties({ZFTT_R_proj_name} PROPERTIES LINKER_LANGUAGE CXX)
find_library(log-lib log)
find_library(android-lib android)
target_link_libraries({ZFTT_R_proj_name} ${android-lib} ${log-lib})

