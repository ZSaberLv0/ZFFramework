WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_ROOT=$1
PROJ_NAME=$2

if test "x-$PROJ_ROOT" = "x-" || test "x-$PROJ_NAME" = "x-" ; then
    echo "usage:"
    echo "  cleanup_Windows.sh PROJ_ROOT PROJ_NAME"
    exit 1
fi

find $PROJ_ROOT -name "*.ncb" -type f 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1
find $PROJ_ROOT -name "*.suo" -type f 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1
find $PROJ_ROOT -name "*.vcproj.*.user" -type f 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1

exit 0

