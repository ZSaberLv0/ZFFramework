
# ======================================================================
# lib template to build ZFFramework lib
# ======================================================================

# ======================================================================
# ZF settings
# ======================================================================

# whether to use unity builds
# NOTE: you must ensure no Q_OBJECT used while unity builds enabled
ZF_UNITY_BUILD = 1
win32 {
    # NOTE: for 32-bit MinGW, it's too easy to reach section limit
    #       (too many sections), disable it for Windows by default
    ZF_UNITY_BUILD = 0
}

# ZFFramework's root path
ZF_ROOT_PATH = $$clean_path($${_PRO_FILE_PWD_}/../../../../../../ZFFramework)
ZF_TOOLS_PATH = $${ZF_ROOT_PATH}/tools

# name of your project
ZF_PROJ_NAME = ZFUIWidget

# src path of your project
# can hold one or more paths, separated by space
ZF_PROJ_SRC_PATH =
ZF_PROJ_SRC_PATH += $${_PRO_FILE_PWD_}/../../../zfsrc

# extra source files, ensured no unity build
ZF_PROJ_SRC_EXT_PATH =
ZF_PROJ_SRC_EXT_PATH += $${_PRO_FILE_PWD_}/../../../zfsrc_ext

# header path to copy to output
ZF_PROJ_HEADER_PATH = $${_PRO_FILE_PWD_}/../../../zfsrc

# res path of your project
# can hold one or more paths, separated by space
ZF_PROJ_RES_PATH = $${_PRO_FILE_PWD_}/../../../zfres


# ======================================================================
CONFIG(debug, debug|release) {
    _ZF_BUILD_TYPE=debug
    DEFINES += DEBUG
} else {
    _ZF_BUILD_TYPE=release
}

win32 {
    _ZF_QT_TYPE=Qt_Windows
    _ZF_SCRIPT_CALL=
    _ZF_SCRIPT_EXT=bat
}
unix:!macx {
    _ZF_QT_TYPE=Qt_Posix
    _ZF_SCRIPT_CALL=sh
    _ZF_SCRIPT_EXT=sh
}
macx {
    _ZF_QT_TYPE=Qt_MacOS
    _ZF_SCRIPT_CALL=sh
    _ZF_SCRIPT_EXT=sh
}

defineReplace(ZFAddLib) {
    _ZF_IS_IMPL=$$1
    _ZF_LIBNAME=$$2
    LIBS += -L$${ZF_ROOT_PATH}/_release/$${_ZF_QT_TYPE}/all/lib -l$${_ZF_LIBNAME}
    export(LIBS)
    macx {
        QMAKE_POST_LINK += install_name_tool -change "lib$${_ZF_LIBNAME}.1.dylib" "@rpath/lib$${_ZF_LIBNAME}.dylib" \
            $$system_path($$clean_path($${OUT_PWD}/lib$${ZF_PROJ_NAME}.dylib)) \
            $$escape_expand(\\n\\t)
        export(QMAKE_POST_LINK)
    }
    return (true)
}

# ZF dependency
$$ZFAddLib(0, ZFCore)
$$ZFAddLib(0, ZFAlgorithm)
$$ZFAddLib(0, ZFUtility)
$$ZFAddLib(0, ZFUIKit)


# ======================================================================
# your custom project settings here
# ======================================================================
# Qt modules
# QT += gui widgets
# qtHaveModule(webenginewidgets) {QT += webenginewidgets} else {qtHaveModule(webkitwidgets) : QT += webkitwidgets}


# ======================================================================
# no need to change these
# ======================================================================
INCLUDEPATH += $${_PRO_FILE_PWD_}/../../../zfsrc
INCLUDEPATH += $${ZF_ROOT_PATH}/_release/$${_ZF_QT_TYPE}/all/include

QT += core

TARGET = $${ZF_PROJ_NAME}
TEMPLATE = lib

QMAKE_CXXFLAGS += -Wno-unused-parameter
CONFIG += warn_off

win32 {
    DEFINES += ZF_ENV_EXPORT=__declspec\\(dllexport\\)
}

# ======================================================================
equals(ZF_UNITY_BUILD, 1) {
    for(src_path, ZF_PROJ_SRC_PATH) {
        _ZF_COMPILE_MODULE_NAME = $${src_path}
        _ZF_COMPILE_MODULE_NAME = $$replace(_ZF_COMPILE_MODULE_NAME,[\\/\.:],_)
        _ZF_COMPILE_MODULE_NAME = $$replace(_ZF_COMPILE_MODULE_NAME,__+,_)
        _ZF_UNITY_BUILD_FILE = $${_PRO_FILE_PWD_}/zfgensrc_$${ZF_PROJ_NAME}_$${_ZF_COMPILE_MODULE_NAME}.cpp
        system($${_ZF_SCRIPT_CALL} \
            $$system_path($$clean_path($${ZF_TOOLS_PATH}/common/unity_build.$${_ZF_SCRIPT_EXT})) \
            $$system_path($$clean_path($${_ZF_UNITY_BUILD_FILE})) \
            $$system_path($$clean_path($${src_path})))
        SOURCES += $$system_path($$clean_path($${_ZF_UNITY_BUILD_FILE}))
    }
} else {
    win32 {
        for(path, ZF_PROJ_SRC_PATH) {
            SOURCES += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.c)) 2>nul")
            SOURCES += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.cpp)) 2>nul")
            HEADERS += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.h)) 2>nul")
            HEADERS += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.hh)) 2>nul")
            HEADERS += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.hpp)) 2>nul")
        }
    } else {
        for(path, ZF_PROJ_SRC_PATH) {
            SOURCES += $$system("find $$system_path($$clean_path($${path})) -name \*.c 2>/dev/null")
            SOURCES += $$system("find $$system_path($$clean_path($${path})) -name \*.cpp 2>/dev/null")
            HEADERS += $$system("find $$system_path($$clean_path($${path})) -name \*.h 2>/dev/null")
            HEADERS += $$system("find $$system_path($$clean_path($${path})) -name \*.hh 2>/dev/null")
            HEADERS += $$system("find $$system_path($$clean_path($${path})) -name \*.hpp 2>/dev/null")
        }
    }
}

win32 {
    for(path, ZF_PROJ_SRC_EXT_PATH) {
        SOURCES += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.c)) 2>nul")
        SOURCES += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.cpp)) 2>nul")
        HEADERS += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.h)) 2>nul")
        HEADERS += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.hh)) 2>nul")
        HEADERS += $$system("dir /s /b $$system_path($$clean_path($${path}\\*.hpp)) 2>nul")
    }
} else {
    for(path, ZF_PROJ_SRC_EXT_PATH) {
        SOURCES += $$system("find $$system_path($$clean_path($${path})) -name \*.c 2>/dev/null")
        SOURCES += $$system("find $$system_path($$clean_path($${path})) -name \*.cpp 2>/dev/null")
        HEADERS += $$system("find $$system_path($$clean_path($${path})) -name \*.h 2>/dev/null")
        HEADERS += $$system("find $$system_path($$clean_path($${path})) -name \*.hh 2>/dev/null")
        HEADERS += $$system("find $$system_path($$clean_path($${path})) -name \*.hpp 2>/dev/null")
    }
}

# ======================================================================
win32 {
    _ZF_LIB_SRC_PATH = $$system_path($$clean_path($${OUT_PWD}/$${_ZF_BUILD_TYPE}))
} else {
    _ZF_LIB_SRC_PATH = $$system_path($$clean_path($${OUT_PWD}))
}
QMAKE_POST_LINK += $${_ZF_SCRIPT_CALL} \
    $$system_path($$clean_path($${ZF_TOOLS_PATH}/util/copy_lib.$${_ZF_SCRIPT_EXT})) \
    $${ZF_PROJ_NAME} \
    $$system_path($$clean_path($${_ZF_LIB_SRC_PATH})) \
    $$system_path($$clean_path($${ZF_ROOT_PATH}/_release/$${_ZF_QT_TYPE}/module/$${ZF_PROJ_NAME}/lib)) \
    $$escape_expand(\\n\\t)
for(path, ZF_PROJ_HEADER_PATH) {
    QMAKE_POST_LINK += $${_ZF_SCRIPT_CALL} \
        $$system_path($$clean_path($${ZF_TOOLS_PATH}/util/copy_header.$${_ZF_SCRIPT_EXT})) \
        $$system_path($$clean_path($${path})) \
        $$system_path($$clean_path($${ZF_ROOT_PATH}/_release/$${_ZF_QT_TYPE}/module/$${ZF_PROJ_NAME}/include)) \
        $$escape_expand(\\n\\t)
}
for(path, ZF_PROJ_RES_PATH) {
    QMAKE_POST_LINK += $${_ZF_SCRIPT_CALL} \
        $$system_path($$clean_path($${ZF_TOOLS_PATH}/util/copy_res.$${_ZF_SCRIPT_EXT})) \
        $$system_path($$clean_path($${path})) \
        $$system_path($$clean_path($${ZF_ROOT_PATH}/_release/$${_ZF_QT_TYPE}/module/$${ZF_PROJ_NAME}/zfres)) \
        $$escape_expand(\\n\\t)
}
QMAKE_POST_LINK += $${_ZF_SCRIPT_CALL} \
    $$system_path($$clean_path($${ZF_TOOLS_PATH}/common/copy_check.$${_ZF_SCRIPT_EXT})) \
    $$system_path($$clean_path($${ZF_ROOT_PATH}/_release/$${_ZF_QT_TYPE}/module/$${ZF_PROJ_NAME})) \
    $$system_path($$clean_path($${ZF_ROOT_PATH}/_release/$${_ZF_QT_TYPE}/all)) \
    $$escape_expand(\\n\\t)

