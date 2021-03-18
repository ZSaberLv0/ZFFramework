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
add_library(ZFLua SHARED ${srcfiles})
set_target_properties(ZFLua PROPERTIES LINKER_LANGUAGE CXX)

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries(ZFLua ${android-lib} ${log-lib})

include_directories(${PROJECT_SOURCE_DIR}/../../../../zfsrc)
include_directories(${ZF_ROOT_PATH}/_release/Android/all/src/main/cpp/include)

function(ZFAddLib ZF_LIB_NAME_)
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION ${ZF_ROOT_PATH}/_release/Android/all/src/main/jniLibs/${ANDROID_ABI}/lib${ZF_LIB_NAME_}.so)
    target_link_libraries(ZFLua ${ZF_LIB_NAME_})
endfunction(ZFAddLib)


ZFAddLib(ZFCore)

