
# ======================================================================
# app template for ZFFramework
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
ZF_ROOT_PATH = $$clean_path($${_PRO_FILE_PWD_}/../../../../../ZFFramework)
ZF_TOOLS_PATH = $${ZF_ROOT_PATH}/tools

# name of your project
ZF_PROJ_NAME = ZFFramework_test

# build path
ZF_BUILD_PATH = $${_PRO_FILE_PWD_}/../../../../_tmp

# src path of your project
# can hold one or more paths, separated by space
ZF_PROJ_SRC_PATH =
ZF_PROJ_SRC_PATH += $${_PRO_FILE_PWD_}/../../../zfsrc

# extra source files, ensured no unity build
ZF_PROJ_SRC_EXT_PATH =
ZF_PROJ_SRC_EXT_PATH += $${_PRO_FILE_PWD_}/../../../zfsrc_ext

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
    _ZF_DESTDIR = $${ZF_BUILD_PATH}/$${_ZF_QT_TYPE}/$${ZF_PROJ_NAME}/$${_ZF_BUILD_TYPE}
    _ZF_RES_DEPLOY_PATH=$$system_path($$clean_path($${_ZF_DESTDIR}/zfres))
    _ZF_LIB_DEPLOY_PATH=$$system_path($$clean_path($${_ZF_DESTDIR}/.))
}
unix:!macx {
    _ZF_QT_TYPE=Qt_Posix
    _ZF_SCRIPT_CALL=sh
    _ZF_SCRIPT_EXT=sh
    _ZF_DESTDIR = $${ZF_BUILD_PATH}/$${_ZF_QT_TYPE}/$${ZF_PROJ_NAME}/$${_ZF_BUILD_TYPE}
    _ZF_RES_DEPLOY_PATH=$$system_path($$clean_path($${_ZF_DESTDIR}/zfres))
    _ZF_LIB_DEPLOY_PATH=$$system_path($$clean_path($${_ZF_DESTDIR}/.))
}
macx {
    _ZF_QT_TYPE=Qt_MacOS
    _ZF_SCRIPT_CALL=sh
    _ZF_SCRIPT_EXT=sh
    _ZF_DESTDIR = $${ZF_BUILD_PATH}/$${_ZF_QT_TYPE}/$${ZF_PROJ_NAME}/$${_ZF_BUILD_TYPE}
    _ZF_RES_DEPLOY_PATH=$$system_path($$clean_path($${_ZF_DESTDIR}/"$${TARGET}".app/Contents/Resources/zfres))
    _ZF_LIB_DEPLOY_PATH=$$system_path($$clean_path($${_ZF_DESTDIR}/"$${TARGET}".app/Contents/Frameworks))
}

defineReplace(ZFAddLib) {
    _ZF_MODULE_PATH=$$1
    _ZF_IS_IMPL=$$2
    _ZF_LIBNAME=$$3
    equals(_ZF_IS_IMPL, 1) {
        ZF_PROJ_SRC_EXT_PATH += $${_ZF_MODULE_PATH}/ZF/$${_ZF_LIBNAME}/zfsrc
        ZF_PROJ_SRC_EXT_PATH += $${_ZF_MODULE_PATH}/ZF/$${_ZF_LIBNAME}/zfsrc_ext
        export(ZF_PROJ_SRC_EXT_PATH)
    } else {
        ZF_PROJ_SRC_PATH += $${_ZF_MODULE_PATH}/ZF/$${_ZF_LIBNAME}/zfsrc
        export(ZF_PROJ_SRC_PATH)
        ZF_PROJ_SRC_EXT_PATH += $${_ZF_MODULE_PATH}/ZF/$${_ZF_LIBNAME}/zfsrc_ext
        export(ZF_PROJ_SRC_EXT_PATH)
    }
    INCLUDEPATH += $${_ZF_MODULE_PATH}/ZF/$${_ZF_LIBNAME}/zfsrc
    export(INCLUDEPATH)
    QMAKE_POST_LINK += $${_ZF_SCRIPT_CALL} \
        $$system_path($$clean_path($${ZF_TOOLS_PATH}/util/copy_res.$${_ZF_SCRIPT_EXT})) \
        $$system_path($$clean_path($${_ZF_MODULE_PATH}/ZF/$${_ZF_LIBNAME}/zfres)) \
        $${_ZF_RES_DEPLOY_PATH} \
        $$escape_expand(\\n\\t)
    export(QMAKE_POST_LINK)
    return (true)
}

# ZF dependency
$$ZFAddLib($${ZF_ROOT_PATH}, 0, ZFCore)
$$ZFAddLib($${ZF_ROOT_PATH}, 0, ZFAlgorithm)
$$ZFAddLib($${ZF_ROOT_PATH}, 0, ZFUtility)
$$ZFAddLib($${ZF_ROOT_PATH}, 0, ZFUIKit)
$$ZFAddLib($${ZF_ROOT_PATH}, 0, ZFUIWidget)
$$ZFAddLib($${ZF_ROOT_PATH}, 0, ZFLua)
$$ZFAddLib($${ZF_ROOT_PATH}, 0, ZFUIWebKit)
$$ZFAddLib($${ZF_ROOT_PATH}, 1, ZF_impl)
$$ZFAddLib($${ZF_ROOT_PATH}, 1, ZFCore_impl)
$$ZFAddLib($${ZF_ROOT_PATH}, 1, ZFAlgorithm_impl)
$$ZFAddLib($${ZF_ROOT_PATH}, 1, ZFUIKit_impl)
$$ZFAddLib($${ZF_ROOT_PATH}, 1, ZFLua_impl)
$$ZFAddLib($${ZF_ROOT_PATH}, 1, ZFUIWebKit_impl)


# ======================================================================
# your custom project settings here
# ======================================================================
# Qt modules
# QT += gui widgets
# qtHaveModule(webenginewidgets) {QT += webenginewidgets} else {qtHaveModule(webkitwidgets) : QT += webkitwidgets}
QT += gui widgets
qtHaveModule(webenginewidgets) {QT += webenginewidgets} else {qtHaveModule(webkitwidgets) : QT += webkitwidgets}


# ======================================================================
# no need to change these
# ======================================================================
INCLUDEPATH += $${_PRO_FILE_PWD_}/../../../zfsrc

QT += core

TARGET = $${ZF_PROJ_NAME}
TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-unused-parameter
CONFIG += warn_off

exists($${ZF_PROJ_NAME}.ico) {
    RC_ICONS = $${ZF_PROJ_NAME}.ico
}
exists($${ZF_PROJ_NAME}.icns) {
    ICON = $${ZF_PROJ_NAME}.icns
}

DESTDIR = $${_ZF_DESTDIR}
OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

# ======================================================================
system($${_ZF_SCRIPT_CALL} $$system_path($$clean_path($${_PRO_FILE_PWD_}/../../../../zfsetup.$${_ZF_SCRIPT_EXT})))

exists(qt_main.cpp) {
    SOURCES += qt_main.cpp
}

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
for(path, ZF_PROJ_RES_PATH) {
    QMAKE_POST_LINK += $${_ZF_SCRIPT_CALL} \
        $$system_path($$clean_path($${ZF_TOOLS_PATH}/util/copy_res.$${_ZF_SCRIPT_EXT})) \
        $$system_path($$clean_path($${path})) \
        $${_ZF_RES_DEPLOY_PATH} \
        $$escape_expand(\\n\\t)
}

unix:!macx {
    QMAKE_LFLAGS += -Wl,--rpath=${ORIGIN}
}
macx {
    CONFIG(release, debug|release) {
        QMAKE_POST_LINK += macdeployqt \
            $$system_path($$clean_path($${_ZF_DESTDIR}/"$${TARGET}".app)) \
            >/dev/null 2>&1 $$escape_expand(\\n\\t)
    }
}

