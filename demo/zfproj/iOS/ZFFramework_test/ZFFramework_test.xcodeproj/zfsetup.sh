WORK_DIR=$(cd "$(dirname "$0")"; pwd)
iOS_SRCROOT=$1
iOS_CONFIGURATION_BUILD_DIR=$2
iOS_CONTENTS_FOLDER_PATH=$3
if test "x-$iOS_SRCROOT" = "x-" || test "x-$iOS_CONFIGURATION_BUILD_DIR" = "x-" || test "x-$iOS_CONFIGURATION_BUILD_DIR" = "x-" ; then
    echo "usage:"
    echo "  zfsetup.sh SRCROOT CONFIGURATION_BUILD_DIR CONTENTS_FOLDER_PATH"
    exit 1
fi

ZF_ROOT_PATH=${iOS_SRCROOT}/../../../../../ZFFramework
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

COMPILE_DST_PATH=${iOS_SRCROOT}/ProjectFiles
sh $ZF_TOOLS_PATH/common/unity_build.sh "${COMPILE_DST_PATH}/zfgensrc_ZFFramework_test.mm" "${iOS_SRCROOT}/../../../zfsrc"

RES_DST_PATH=${iOS_CONFIGURATION_BUILD_DIR}/${iOS_CONTENTS_FOLDER_PATH}/zfres

sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFCore/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFAlgorithm/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFUtility/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFUIKit/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFUIWidget/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFLua/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFUIWebKit/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZF_impl/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFCore_impl/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFAlgorithm_impl/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFUIKit_impl/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFLua_impl/zfres" "$RES_DST_PATH"
sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/ZFUIWebKit_impl/zfres" "$RES_DST_PATH"

sh "$ZF_TOOLS_PATH/util/copy_res.sh" "${iOS_SRCROOT}/../../../zfres" "$RES_DST_PATH"

