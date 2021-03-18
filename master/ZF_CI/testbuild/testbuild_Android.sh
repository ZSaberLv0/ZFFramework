WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  testbuild_Android.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

_OLD_DIR=$(pwd)
cd "$PROJ_PATH"
chmod +x gradlew
./gradlew assembleRelease
_RESULT="$?"
cd "$_OLD_DIR"

exit $_RESULT

