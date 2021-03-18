WORK_DIR=$(cd "$(dirname "$0")"; pwd)
DIR_TO_CHECK=$1
CHECK_INTERVAL=$2
TIMESTAMP_FILE_NAME=$3
if test "x-$DIR_TO_CHECK" = "x-" || test "x-$CHECK_INTERVAL" = "x-" ; then
    echo "usage:"
    echo "  timestamp_save.sh DIR_TO_CHECK CHECK_INTERVAL [TIMESTAMP_FILE_NAME]"
    exit 1
fi

if test "x-$TIMESTAMP_FILE_NAME" = "x-" ; then
    TIMESTAMP_FILE_NAME=_zf_timestamp_
fi

_CUR_TIME=$(date +%s)
mkdir -p "$DIR_TO_CHECK" >/dev/null 2>&1
echo $_CUR_TIME > "$DIR_TO_CHECK/$TIMESTAMP_FILE_NAME"

