WORK_DIR=$(cd "$(dirname "$0")"; pwd)
SCRIPT_FILE=$1
shift
VA_PARAMS=$@

if test "x-$SCRIPT_FILE" = "x-" ; then
    echo "usage:"
    echo "  run_if_exist.sh SCRIPT_FILE [...]"
    exit 1
fi

if test ! -e "$SCRIPT_FILE"; then
    exit 0
fi

sh "$SCRIPT_FILE" $VA_PARAMS

