WORK_DIR=$(cd "$(dirname "$0")"; pwd)
SRC_DIR=$1
DST_DIR=$2
if test "x-$SRC_DIR" = "x-" ; then
    echo "usage:"
    echo "  zfproj_recursive.sh SRC_DIR [DST_DIR]"
    echo "set ZF_EXCLUDE_FILE to exclude dirs, separated by space, match exact, e.g."
    echo "  export ZF_EXCLUDE_FILE=f0 f1"
    exit 1
fi

# ============================================================
# default exclude dirs
ZF_EXCLUDE_FILE_TMP=$ZF_EXCLUDE_FILE
ZF_EXCLUDE_FILE_TMP="$ZF_EXCLUDE_FILE_TMP private zfres _release _tmp"

# ============================================================
ZF_ROOT_PATH=$WORK_DIR/../../../ZFFramework

_FULL_CMD="find '$SRC_DIR' -name 'zfautoscript_zfproj.txt' | grep -v '\(_zf_dummy_\)"
for e in $ZF_EXCLUDE_FILE_TMP ; do
    _FULL_CMD="${_FULL_CMD}\|\(/$e/\)"
done
_FULL_CMD="${_FULL_CMD}'"

for f in `eval $_FULL_CMD` ; do
    sh "$WORK_DIR/zfproj_creator.sh" "$f" "$DST_DIR"
done

