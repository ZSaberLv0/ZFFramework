WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  releaselib_Qt_MacOS.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
QT_TYPE=Qt_MacOS
RELEASE_PATH=$ZF_ROOT_PATH/_release/$QT_TYPE

sh "$ZF_TOOLS_PATH/common/file_exist.sh" "$RELEASE_PATH/module/$PROJ_NAME/lib" "*$PROJ_NAME*"
if test "$?" = "0" ; then
    exit 0
fi

_OLD_DIR=$(pwd)
mkdir -p "$ZF_ROOT_PATH/_tmp/$QT_TYPE/$PROJ_NAME/build" >/dev/null 2>&1
cd "$ZF_ROOT_PATH/_tmp/$QT_TYPE/$PROJ_NAME/build"
find . -name "*.a.*" 2>/dev/null | xargs -I{} rm {}
find . -name "*.so.*" 2>/dev/null | xargs -I{} rm {}
find . -name "*.dylib.*" 2>/dev/null | xargs -I{} rm {}
qmake "$PROJ_PATH/zfproj/Qt/$PROJ_NAME/$PROJ_NAME.pro" CONFIG+=release
make -j2
cd "$_OLD_DIR"

