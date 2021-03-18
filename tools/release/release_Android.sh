WORK_DIR=$(cd "$(dirname "$0")"; pwd)
CHECK_ONLY=$1
ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

if test ! "x-$CHECK_ONLY" = "x-" && test -e "$ZF_ROOT_PATH/_release/Android/all/aar"; then
    exit 0
fi

sh "$ZF_TOOLS_PATH/common/zfsh.sh" "$WORK_DIR/release.zfsh" Android

