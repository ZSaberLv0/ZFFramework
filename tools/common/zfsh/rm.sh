WORK_DIR=$(cd "$(dirname "$0")"; pwd)
TARGET_PATH=$1
if test "x-$TARGET_PATH" = "x-" ; then
    echo "usage:"
    echo "  rm.sh TARGET_PATH"
    exit 1
fi

rm -rf "$TARGET_PATH" >/dev/null 2>&1

