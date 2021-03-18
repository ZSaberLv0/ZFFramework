WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_ROOT=$1
PROJ_NAME=$2

if test "x-$PROJ_ROOT" = "x-" || test "x-$PROJ_NAME" = "x-" ; then
    echo "usage:"
    echo "  cleanup_Qt.sh PROJ_ROOT PROJ_NAME"
    exit 1
fi

find $PROJ_ROOT -name "build-*" -type d 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1
find $PROJ_ROOT -name "*.pro.user" -type f 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1
find $PROJ_ROOT -name "*.pro.user.*" -type f 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1

exit 0

