WORK_DIR=$(cd "$(dirname "$0")"; pwd)
DST_PATH=$1
SRC_PATH=$2
if test "x-$DST_PATH" = "x-" || test "x-$SRC_PATH" = "x-" ; then
    echo "usage:"
    echo "  file_append.sh DST_PATH SRC_PATH"
    exit 1
fi

DST_PARENT=${DST_PATH%[/\\]*}
if ! test -e "$DST_PARENT" ; then
    mkdir -p "$DST_PARENT"
fi
cat "$SRC_PATH" >> "$DST_PATH"

