WORK_DIR=$(cd "$(dirname "$0")"; pwd)
SRC_PATH=$1
DST_PATH=$2
if test "x-$SRC_PATH" = "x-" || test "x-$DST_PATH" = "x-" ; then
    echo "$SRC_PATH"
    echo "$DST_PATH"
    echo "usage:"
    echo "  copy_res.sh SRC_PATH DST_PATH"
    exit 1
fi

mkdir -p "$DST_PATH" >/dev/null 2>&1
if test -e "$SRC_PATH"; then
    cp -rf "$SRC_PATH/." "$DST_PATH/" >/dev/null 2>&1
fi

find "$DST_PATH" -type d -name "_repo" 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1
find "$DST_PATH" -type d -name "_tmp" 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1
find "$DST_PATH" -type f -name "ZF_PUT_RES_FILES_HERE" 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1
find "$DST_PATH" -depth -type d -empty -exec rm -rf {} ';' >/dev/null 2>&1

exit 0

