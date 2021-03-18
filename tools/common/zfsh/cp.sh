WORK_DIR=$(cd "$(dirname "$0")"; pwd)
SRC_PATH=$1
DST_PATH=$2
if test "x-$SRC_PATH" = "x-" || test "x-$DST_PATH" = "x-" ; then
    echo "usage:"
    echo "  cp.sh SRC_PATH DST_PATH"
    exit 1
fi

cp -rf "$SRC_PATH" "$DST_PATH" >/dev/null 2>&1

