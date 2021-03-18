WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../ZFFramework
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
CLEAN_WHAT=$1

if test "x-$CLEAN_WHAT" = "x-" || test "x-$CLEAN_WHAT" = "x-build" ; then
    sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$WORK_DIR" "zfautoscript_cleanup*.zfsh"
    sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$WORK_DIR/../ZFModule" "zfautoscript_cleanup*.zfsh"
fi

if test "x-$CLEAN_WHAT" = "x-" || test "x-$CLEAN_WHAT" = "x-zf3rd" ; then
    sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$WORK_DIR" "zfautoscript_zf3rd_cleanup*.zfsh"
    sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$WORK_DIR/../ZFModule" "zfautoscript_zf3rd_cleanup*.zfsh"
fi

rm -rf "$WORK_DIR/_release" >/dev/null 2>&1
rm -rf "$WORK_DIR/_tmp" >/dev/null 2>&1

