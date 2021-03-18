cmake_minimum_required(VERSION 3.4.1)

set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../../../ZFFramework)

if(WIN32)
    execute_process(COMMAND ${ZF_ROOT_PATH}/zfsetup.bat)
else()
    execute_process(COMMAND ${ZF_ROOT_PATH}/zfsetup.sh)
endif()

file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../../../zfsrc/*.c
    ${PROJECT_SOURCE_DIR}/../../../../zfsrc/*.cpp
    ${PROJECT_SOURCE_DIR}/../../../../zfsrc_ext/*.c
    ${PROJECT_SOURCE_DIR}/../../../../zfsrc_ext/*.cpp
    )
add_library({ZFTT_R_proj_name} SHARED ${srcfiles})
set_target_properties({ZFTT_R_proj_name} PROPERTIES LINKER_LANGUAGE CXX)

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries({ZFTT_R_proj_name} ${android-lib} ${log-lib})

include_directories(${PROJECT_SOURCE_DIR}/../../../../zfsrc)
include_directories(${ZF_ROOT_PATH}/_release/Android/all/src/main/cpp/include)

function(ZFAddLib ZF_LIB_NAME_)
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION ${ZF_ROOT_PATH}/_release/Android/all/src/main/jniLibs/${ANDROID_ABI}/lib${ZF_LIB_NAME_}.so)
    target_link_libraries({ZFTT_R_proj_name} ${ZF_LIB_NAME_})
endfunction(ZFAddLib)

{ZFTT_C_lib_require_0}
ZFAddLib({ZFTT_R_lib_name_0}){ZFTT_CE}{ZFTT_C_lib_require_1}
ZFAddLib({ZFTT_R_lib_name_1}){ZFTT_CE}{ZFTT_C_lib_require_2}
ZFAddLib({ZFTT_R_lib_name_2}){ZFTT_CE}{ZFTT_C_lib_require_3}
ZFAddLib({ZFTT_R_lib_name_3}){ZFTT_CE}{ZFTT_C_lib_require_4}
ZFAddLib({ZFTT_R_lib_name_4}){ZFTT_CE}{ZFTT_C_lib_require_5}
ZFAddLib({ZFTT_R_lib_name_5}){ZFTT_CE}{ZFTT_C_lib_require_6}
ZFAddLib({ZFTT_R_lib_name_6}){ZFTT_CE}{ZFTT_C_lib_require_7}
ZFAddLib({ZFTT_R_lib_name_7}){ZFTT_CE}{ZFTT_C_lib_require_8}
ZFAddLib({ZFTT_R_lib_name_8}){ZFTT_CE}{ZFTT_C_lib_require_9}
ZFAddLib({ZFTT_R_lib_name_9}){ZFTT_CE}{ZFTT_C_lib_require_10}
ZFAddLib({ZFTT_R_lib_name_10}){ZFTT_CE}{ZFTT_C_lib_require_11}
ZFAddLib({ZFTT_R_lib_name_11}){ZFTT_CE}{ZFTT_C_lib_require_12}
ZFAddLib({ZFTT_R_lib_name_12}){ZFTT_CE}{ZFTT_C_lib_require_13}
ZFAddLib({ZFTT_R_lib_name_13}){ZFTT_CE}{ZFTT_C_lib_require_14}
ZFAddLib({ZFTT_R_lib_name_14}){ZFTT_CE}{ZFTT_C_lib_require_15}
ZFAddLib({ZFTT_R_lib_name_15}){ZFTT_CE}{ZFTT_C_lib_require_16}
ZFAddLib({ZFTT_R_lib_name_16}){ZFTT_CE}{ZFTT_C_lib_require_17}
ZFAddLib({ZFTT_R_lib_name_17}){ZFTT_CE}{ZFTT_C_lib_require_18}
ZFAddLib({ZFTT_R_lib_name_18}){ZFTT_CE}{ZFTT_C_lib_require_19}
ZFAddLib({ZFTT_R_lib_name_19}){ZFTT_CE}{ZFTT_C_lib_require_20}
ZFAddLib({ZFTT_R_lib_name_20}){ZFTT_CE}{ZFTT_C_lib_require_21}
ZFAddLib({ZFTT_R_lib_name_21}){ZFTT_CE}{ZFTT_C_lib_require_22}
ZFAddLib({ZFTT_R_lib_name_22}){ZFTT_CE}{ZFTT_C_lib_require_23}
ZFAddLib({ZFTT_R_lib_name_23}){ZFTT_CE}{ZFTT_C_lib_require_24}
ZFAddLib({ZFTT_R_lib_name_24}){ZFTT_CE}{ZFTT_C_lib_require_25}
ZFAddLib({ZFTT_R_lib_name_25}){ZFTT_CE}{ZFTT_C_lib_require_26}
ZFAddLib({ZFTT_R_lib_name_26}){ZFTT_CE}{ZFTT_C_lib_require_27}
ZFAddLib({ZFTT_R_lib_name_27}){ZFTT_CE}{ZFTT_C_lib_require_28}
ZFAddLib({ZFTT_R_lib_name_28}){ZFTT_CE}{ZFTT_C_lib_require_29}
ZFAddLib({ZFTT_R_lib_name_29}){ZFTT_CE}{ZFTT_C_lib_require_30}
ZFAddLib({ZFTT_R_lib_name_30}){ZFTT_CE}{ZFTT_C_lib_require_31}
ZFAddLib({ZFTT_R_lib_name_31}){ZFTT_CE}{ZFTT_C_impl_require_0}
ZFAddLib({ZFTT_R_impl_name_0}){ZFTT_CE}{ZFTT_C_impl_require_1}
ZFAddLib({ZFTT_R_impl_name_1}){ZFTT_CE}{ZFTT_C_impl_require_2}
ZFAddLib({ZFTT_R_impl_name_2}){ZFTT_CE}{ZFTT_C_impl_require_3}
ZFAddLib({ZFTT_R_impl_name_3}){ZFTT_CE}{ZFTT_C_impl_require_4}
ZFAddLib({ZFTT_R_impl_name_4}){ZFTT_CE}{ZFTT_C_impl_require_5}
ZFAddLib({ZFTT_R_impl_name_5}){ZFTT_CE}{ZFTT_C_impl_require_6}
ZFAddLib({ZFTT_R_impl_name_6}){ZFTT_CE}{ZFTT_C_impl_require_7}
ZFAddLib({ZFTT_R_impl_name_7}){ZFTT_CE}{ZFTT_C_impl_require_8}
ZFAddLib({ZFTT_R_impl_name_8}){ZFTT_CE}{ZFTT_C_impl_require_9}
ZFAddLib({ZFTT_R_impl_name_9}){ZFTT_CE}{ZFTT_C_impl_require_10}
ZFAddLib({ZFTT_R_impl_name_10}){ZFTT_CE}{ZFTT_C_impl_require_11}
ZFAddLib({ZFTT_R_impl_name_11}){ZFTT_CE}{ZFTT_C_impl_require_12}
ZFAddLib({ZFTT_R_impl_name_12}){ZFTT_CE}{ZFTT_C_impl_require_13}
ZFAddLib({ZFTT_R_impl_name_13}){ZFTT_CE}{ZFTT_C_impl_require_14}
ZFAddLib({ZFTT_R_impl_name_14}){ZFTT_CE}{ZFTT_C_impl_require_15}
ZFAddLib({ZFTT_R_impl_name_15}){ZFTT_CE}{ZFTT_C_impl_require_16}
ZFAddLib({ZFTT_R_impl_name_16}){ZFTT_CE}{ZFTT_C_impl_require_17}
ZFAddLib({ZFTT_R_impl_name_17}){ZFTT_CE}{ZFTT_C_impl_require_18}
ZFAddLib({ZFTT_R_impl_name_18}){ZFTT_CE}{ZFTT_C_impl_require_19}
ZFAddLib({ZFTT_R_impl_name_19}){ZFTT_CE}{ZFTT_C_impl_require_20}
ZFAddLib({ZFTT_R_impl_name_20}){ZFTT_CE}{ZFTT_C_impl_require_21}
ZFAddLib({ZFTT_R_impl_name_21}){ZFTT_CE}{ZFTT_C_impl_require_22}
ZFAddLib({ZFTT_R_impl_name_22}){ZFTT_CE}{ZFTT_C_impl_require_23}
ZFAddLib({ZFTT_R_impl_name_23}){ZFTT_CE}{ZFTT_C_impl_require_24}
ZFAddLib({ZFTT_R_impl_name_24}){ZFTT_CE}{ZFTT_C_impl_require_25}
ZFAddLib({ZFTT_R_impl_name_25}){ZFTT_CE}{ZFTT_C_impl_require_26}
ZFAddLib({ZFTT_R_impl_name_26}){ZFTT_CE}{ZFTT_C_impl_require_27}
ZFAddLib({ZFTT_R_impl_name_27}){ZFTT_CE}{ZFTT_C_impl_require_28}
ZFAddLib({ZFTT_R_impl_name_28}){ZFTT_CE}{ZFTT_C_impl_require_29}
ZFAddLib({ZFTT_R_impl_name_29}){ZFTT_CE}{ZFTT_C_impl_require_30}
ZFAddLib({ZFTT_R_impl_name_30}){ZFTT_CE}{ZFTT_C_impl_require_31}
ZFAddLib({ZFTT_R_impl_name_31}){ZFTT_CE}{ZFTT_C_lib_ext_require_0}
ZFAddLib({ZFTT_R_lib_ext_name_0}){ZFTT_CE}{ZFTT_C_lib_ext_require_1}
ZFAddLib({ZFTT_R_lib_ext_name_1}){ZFTT_CE}{ZFTT_C_lib_ext_require_2}
ZFAddLib({ZFTT_R_lib_ext_name_2}){ZFTT_CE}{ZFTT_C_lib_ext_require_3}
ZFAddLib({ZFTT_R_lib_ext_name_3}){ZFTT_CE}{ZFTT_C_lib_ext_require_4}
ZFAddLib({ZFTT_R_lib_ext_name_4}){ZFTT_CE}{ZFTT_C_lib_ext_require_5}
ZFAddLib({ZFTT_R_lib_ext_name_5}){ZFTT_CE}{ZFTT_C_lib_ext_require_6}
ZFAddLib({ZFTT_R_lib_ext_name_6}){ZFTT_CE}{ZFTT_C_lib_ext_require_7}
ZFAddLib({ZFTT_R_lib_ext_name_7}){ZFTT_CE}{ZFTT_C_lib_ext_require_8}
ZFAddLib({ZFTT_R_lib_ext_name_8}){ZFTT_CE}{ZFTT_C_lib_ext_require_9}
ZFAddLib({ZFTT_R_lib_ext_name_9}){ZFTT_CE}{ZFTT_C_lib_ext_require_10}
ZFAddLib({ZFTT_R_lib_ext_name_10}){ZFTT_CE}{ZFTT_C_lib_ext_require_11}
ZFAddLib({ZFTT_R_lib_ext_name_11}){ZFTT_CE}{ZFTT_C_lib_ext_require_12}
ZFAddLib({ZFTT_R_lib_ext_name_12}){ZFTT_CE}{ZFTT_C_lib_ext_require_13}
ZFAddLib({ZFTT_R_lib_ext_name_13}){ZFTT_CE}{ZFTT_C_lib_ext_require_14}
ZFAddLib({ZFTT_R_lib_ext_name_14}){ZFTT_CE}{ZFTT_C_lib_ext_require_15}
ZFAddLib({ZFTT_R_lib_ext_name_15}){ZFTT_CE}{ZFTT_C_lib_ext_require_16}
ZFAddLib({ZFTT_R_lib_ext_name_16}){ZFTT_CE}{ZFTT_C_lib_ext_require_17}
ZFAddLib({ZFTT_R_lib_ext_name_17}){ZFTT_CE}{ZFTT_C_lib_ext_require_18}
ZFAddLib({ZFTT_R_lib_ext_name_18}){ZFTT_CE}{ZFTT_C_lib_ext_require_19}
ZFAddLib({ZFTT_R_lib_ext_name_19}){ZFTT_CE}{ZFTT_C_lib_ext_require_20}
ZFAddLib({ZFTT_R_lib_ext_name_20}){ZFTT_CE}{ZFTT_C_lib_ext_require_21}
ZFAddLib({ZFTT_R_lib_ext_name_21}){ZFTT_CE}{ZFTT_C_lib_ext_require_22}
ZFAddLib({ZFTT_R_lib_ext_name_22}){ZFTT_CE}{ZFTT_C_lib_ext_require_23}
ZFAddLib({ZFTT_R_lib_ext_name_23}){ZFTT_CE}{ZFTT_C_lib_ext_require_24}
ZFAddLib({ZFTT_R_lib_ext_name_24}){ZFTT_CE}{ZFTT_C_lib_ext_require_25}
ZFAddLib({ZFTT_R_lib_ext_name_25}){ZFTT_CE}{ZFTT_C_lib_ext_require_26}
ZFAddLib({ZFTT_R_lib_ext_name_26}){ZFTT_CE}{ZFTT_C_lib_ext_require_27}
ZFAddLib({ZFTT_R_lib_ext_name_27}){ZFTT_CE}{ZFTT_C_lib_ext_require_28}
ZFAddLib({ZFTT_R_lib_ext_name_28}){ZFTT_CE}{ZFTT_C_lib_ext_require_29}
ZFAddLib({ZFTT_R_lib_ext_name_29}){ZFTT_CE}{ZFTT_C_lib_ext_require_30}
ZFAddLib({ZFTT_R_lib_ext_name_30}){ZFTT_CE}{ZFTT_C_lib_ext_require_31}
ZFAddLib({ZFTT_R_lib_ext_name_31}){ZFTT_CE}{ZFTT_C_impl_ext_require_0}
ZFAddLib({ZFTT_R_impl_ext_name_0}){ZFTT_CE}{ZFTT_C_impl_ext_require_1}
ZFAddLib({ZFTT_R_impl_ext_name_1}){ZFTT_CE}{ZFTT_C_impl_ext_require_2}
ZFAddLib({ZFTT_R_impl_ext_name_2}){ZFTT_CE}{ZFTT_C_impl_ext_require_3}
ZFAddLib({ZFTT_R_impl_ext_name_3}){ZFTT_CE}{ZFTT_C_impl_ext_require_4}
ZFAddLib({ZFTT_R_impl_ext_name_4}){ZFTT_CE}{ZFTT_C_impl_ext_require_5}
ZFAddLib({ZFTT_R_impl_ext_name_5}){ZFTT_CE}{ZFTT_C_impl_ext_require_6}
ZFAddLib({ZFTT_R_impl_ext_name_6}){ZFTT_CE}{ZFTT_C_impl_ext_require_7}
ZFAddLib({ZFTT_R_impl_ext_name_7}){ZFTT_CE}{ZFTT_C_impl_ext_require_8}
ZFAddLib({ZFTT_R_impl_ext_name_8}){ZFTT_CE}{ZFTT_C_impl_ext_require_9}
ZFAddLib({ZFTT_R_impl_ext_name_9}){ZFTT_CE}{ZFTT_C_impl_ext_require_10}
ZFAddLib({ZFTT_R_impl_ext_name_10}){ZFTT_CE}{ZFTT_C_impl_ext_require_11}
ZFAddLib({ZFTT_R_impl_ext_name_11}){ZFTT_CE}{ZFTT_C_impl_ext_require_12}
ZFAddLib({ZFTT_R_impl_ext_name_12}){ZFTT_CE}{ZFTT_C_impl_ext_require_13}
ZFAddLib({ZFTT_R_impl_ext_name_13}){ZFTT_CE}{ZFTT_C_impl_ext_require_14}
ZFAddLib({ZFTT_R_impl_ext_name_14}){ZFTT_CE}{ZFTT_C_impl_ext_require_15}
ZFAddLib({ZFTT_R_impl_ext_name_15}){ZFTT_CE}{ZFTT_C_impl_ext_require_16}
ZFAddLib({ZFTT_R_impl_ext_name_16}){ZFTT_CE}{ZFTT_C_impl_ext_require_17}
ZFAddLib({ZFTT_R_impl_ext_name_17}){ZFTT_CE}{ZFTT_C_impl_ext_require_18}
ZFAddLib({ZFTT_R_impl_ext_name_18}){ZFTT_CE}{ZFTT_C_impl_ext_require_19}
ZFAddLib({ZFTT_R_impl_ext_name_19}){ZFTT_CE}{ZFTT_C_impl_ext_require_20}
ZFAddLib({ZFTT_R_impl_ext_name_20}){ZFTT_CE}{ZFTT_C_impl_ext_require_21}
ZFAddLib({ZFTT_R_impl_ext_name_21}){ZFTT_CE}{ZFTT_C_impl_ext_require_22}
ZFAddLib({ZFTT_R_impl_ext_name_22}){ZFTT_CE}{ZFTT_C_impl_ext_require_23}
ZFAddLib({ZFTT_R_impl_ext_name_23}){ZFTT_CE}{ZFTT_C_impl_ext_require_24}
ZFAddLib({ZFTT_R_impl_ext_name_24}){ZFTT_CE}{ZFTT_C_impl_ext_require_25}
ZFAddLib({ZFTT_R_impl_ext_name_25}){ZFTT_CE}{ZFTT_C_impl_ext_require_26}
ZFAddLib({ZFTT_R_impl_ext_name_26}){ZFTT_CE}{ZFTT_C_impl_ext_require_27}
ZFAddLib({ZFTT_R_impl_ext_name_27}){ZFTT_CE}{ZFTT_C_impl_ext_require_28}
ZFAddLib({ZFTT_R_impl_ext_name_28}){ZFTT_CE}{ZFTT_C_impl_ext_require_29}
ZFAddLib({ZFTT_R_impl_ext_name_29}){ZFTT_CE}{ZFTT_C_impl_ext_require_30}
ZFAddLib({ZFTT_R_impl_ext_name_30}){ZFTT_CE}{ZFTT_C_impl_ext_require_31}
ZFAddLib({ZFTT_R_impl_ext_name_31}){ZFTT_CE}

