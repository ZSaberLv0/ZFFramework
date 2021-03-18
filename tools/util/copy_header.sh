WORK_DIR=$(cd "$(dirname "$0")"; pwd)
SRC_PATH=$1
DST_PATH=$2
if test "x-$SRC_PATH" = "x-" || test "x-$DST_PATH" = "x-" ; then
    echo "$SRC_PATH"
    echo "$DST_PATH"
    echo "usage:"
    echo "  copy_header.sh SRC_PATH DST_PATH"
    exit 1
fi

rm -rf "$DST_PATH" >/dev/null 2>&1
mkdir -p "$DST_PATH" >/dev/null 2>&1
cp -rf "$SRC_PATH/." "$DST_PATH/" >/dev/null 2>&1

find "$DST_PATH" -type f -name "*.c" 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1
find "$DST_PATH" -type f -name "*.cpp" 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1
find "$DST_PATH" -type f -name "*.m" 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1
find "$DST_PATH" -type f -name "*.mm" 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1
find "$DST_PATH" -type f -name "*.java" 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1
find "$DST_PATH" -type f -name ".*" 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1

find "$DST_PATH" -type d -name "_repo" 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1
find "$DST_PATH" -type d -name "_tmp" 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1
find "$DST_PATH" -depth -type d -empty -exec rm -rf {} ';' >/dev/null 2>&1

exit 0

