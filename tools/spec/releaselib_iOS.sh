WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  releaselib_iOS.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
RELEASE_PATH=$ZF_ROOT_PATH/_release/iOS

sh "$ZF_TOOLS_PATH/common/file_exist.sh" "$RELEASE_PATH/module/$PROJ_NAME/lib" "*$PROJ_NAME*"
if test "$?" = "0" ; then
    exit 0
fi

_OLD_DIR=$(pwd)
cd "$PROJ_PATH/zfproj/iOS/$PROJ_NAME"
xcodebuild -configuration "Release" -target "${PROJ_NAME}_aggregate" SYMROOT="$ZF_ROOT_PATH/_tmp/iOS/$PROJ_NAME/build"
cd "$_OLD_DIR"

