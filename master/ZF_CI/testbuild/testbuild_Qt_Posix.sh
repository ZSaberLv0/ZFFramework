WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  testbuild_Qt_Posix.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
_TMP_PATH=$ZF_ROOT_PATH/_tmp/Qt_Posix/$PROJ_NAME/release

_OLD_DIR=$(pwd)
cd "$PROJ_PATH"
_PROJ_PATH=$(pwd)
mkdir -p "$_TMP_PATH" >/dev/null 2>&1
cd "$_TMP_PATH"
qmake "$_PROJ_PATH/$PROJ_NAME.pro" CONFIG+=release
make -j2
_RESULT="$?"
cd "$_OLD_DIR"

exit $_RESULT

