WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PATH_TO_CHECK=$1
FILE_PATTERN=$2
if test "x-$PATH_TO_CHECK" = "x-" || test "x-$FILE_PATTERN" = "x-" ; then
    echo "usage:"
    echo "  file_exist.sh PATH_TO_CHECK FILE_PATTERN"
    echo "return by $ ?:"
    echo "  0: exist"
    echo "  1: not exist"
    exit 1
fi

RESULT=`find "$PATH_TO_CHECK" -type f -name "$FILE_PATTERN" 2>/dev/null`
if test "x-$RESULT" = "x-" ; then
    exit 1
else
    exit 0
fi

