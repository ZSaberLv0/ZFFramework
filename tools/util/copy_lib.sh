WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
SRC_PATH=$2
DST_PATH=$3
if test "x-$PROJ_NAME" = "x-" || test "x-$SRC_PATH" = "x-" || test "x-$DST_PATH" = "x-" ; then
    echo "usage:"
    echo "  copy_lib.sh PROJ_NAME SRC_PATH DST_PATH"
    exit 1
fi

mkdir -p "$DST_PATH" >/dev/null 2>&1

find "$SRC_PATH" -regex ".*/.*\.a.*" 2>/dev/null | xargs -I{} cp -L -R {} "$DST_PATH/lib$PROJ_NAME.a" >/dev/null 2>&1
find "$SRC_PATH" -regex ".*/.*\.so.*" 2>/dev/null | xargs -I{} cp -L -R {} "$DST_PATH/lib$PROJ_NAME.so" >/dev/null 2>&1
find "$SRC_PATH" -regex ".*/.*\.dylib.*" 2>/dev/null | xargs -I{} cp -L -R {} "$DST_PATH/lib$PROJ_NAME.dylib" >/dev/null 2>&1

exit 0

