WORK_DIR=$(cd "$(dirname "$0")"; pwd)
DST_PATH=$1
FILE1_PATH=$2
if test "x-$DST_PATH" = "x-" || test "x-$FILE1_PATH" = "x-" ; then
    echo "usage:"
    echo "  file_merge.sh DST_PATH FILE1_PATH [FILE2_PATH ...]"
    exit 1
fi

DST_PARENT=${DST_PATH%[/\\]*}
if ! test -e "$DST_PARENT" ; then
    mkdir -p "$DST_PARENT"
fi
cat "$FILE1_PATH" > "$DST_PATH"

shift 2
FILEN_PATH=$1
while ! test "x-$FILEN_PATH" = "x-" ; do
    cat "$FILEN_PATH" >> "$DST_PATH"
    shift
    FILEN_PATH=$1
done

