WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../ZFFramework
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

if ! test -e "$ZF_TOOLS_PATH"; then
    rm -rf "$ZF_ROOT_PATH" >/dev/null 2>&1
    git clone "https://github.com/ZFFramework/ZFFramework.git" "$ZF_ROOT_PATH"
fi

sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$ZF_ROOT_PATH" "zfautoscript_zf3rd_setup*.zfsh"
sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$ZF_ROOT_PATH/../ZFModule" "zfautoscript_zf3rd_setup*.zfsh"

sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$ZF_ROOT_PATH" "zfautoscript_zfmodule.zfsh"
sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$ZF_ROOT_PATH/../ZFModule" "zfautoscript_zfmodule.zfsh"

ABS_ZF_ROOT_PATH=$(cd -- "$ZF_ROOT_PATH" && pwd)
ABS_WORK_DIR=$(cd -- "$WORK_DIR" && pwd)
if ! test "$ABS_ZF_ROOT_PATH" = "$ABS_WORK_DIR" ; then
    sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$WORK_DIR" "zfautoscript_zf3rd_setup*.zfsh"
    sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$WORK_DIR" "zfautoscript_zfmodule.zfsh"
fi

