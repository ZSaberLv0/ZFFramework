WORK_DIR=$(cd "$(dirname "$0")"; pwd)
SRC_DIR=$1
FILE_NAME=$2
shift 2
VA_PARAMS=$@

if test "x-$SRC_DIR" = "x-" || test "x-$FILE_NAME" = "x-" ; then
    echo "usage:"
    echo "  run_recursive.sh SRC_DIR FILE_NAME [params...]"
    echo "set ZF_EXCLUDE to exclude dirs, separated by space, match exact, e.g."
    echo "  export ZF_EXCLUDE=f0 f1"
    exit 1
fi

# ============================================================
# default exclude dirs
ZF_EXCLUDE_TMP=$ZF_EXCLUDE
ZF_EXCLUDE_TMP="$ZF_EXCLUDE_TMP private zfres _release _tmp"

# ============================================================
_FULL_CMD="find '$SRC_DIR' -name '$FILE_NAME' | grep -v '\(_zf_dummy_\)"
for e in $ZF_EXCLUDE_TMP ; do
    _FULL_CMD="${_FULL_CMD}\|\(/$e/\)"
done
_FULL_CMD="${_FULL_CMD}'"

for f in `eval $_FULL_CMD` ; do
    echo "running $f"
    sh "$f" "$WORK_DIR/../.." $VA_PARAMS
    if test ! "$?" = "0" ; then
        exit 1
    fi
done

