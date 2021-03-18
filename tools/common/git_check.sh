WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_GIT=$1
GIT_BRANCH=$2
DST_PATH=$3
shift 3
CLONE_OPTION=$@
if test "x-$PROJ_GIT" = "x-" || test "x-$DST_PATH" = "x-" ; then
    echo "usage:"
    echo "  git_check.sh PROJ_GIT GIT_BRANCH DST_PATH [options]"
    exit 1
fi
if test "x-$CLONE_OPTION" = "x-" ; then
    CLONE_OPTION=--depth=1
fi

_OLD_DIR=$(pwd)
# 60*60*24*7, one week
_TIMEOUT=604800
_GIT_VALID=0

if test -e "$DST_PATH/.git"; then
    _GIT_PATH_DESIRED="$DST_PATH/.git"
    cd "$_GIT_PATH_DESIRED" >/dev/null 2>&1
    _GIT_PATH_DESIRED=$(pwd -P)
    cd "$_OLD_DIR"

    cd "$DST_PATH" >/dev/null 2>&1
    _GIT_PATH_EXIST=`git rev-parse --show-toplevel 2>/dev/null`/.git
    cd "$_OLD_DIR"
    cd "$_GIT_PATH_EXIST" >/dev/null 2>&1
    _GIT_PATH_EXIST=$(pwd)
    cd "$_OLD_DIR"

    if test "$_GIT_PATH_DESIRED" = "$_GIT_PATH_EXIST" ; then
        _GIT_VALID=1
    fi
fi

if test "$_GIT_VALID" = "1"; then
    sh "$WORK_DIR/timestamp_check.sh" "$DST_PATH/.git" $_TIMEOUT

    if test ! "$?" = "0" ; then
        cd "$DST_PATH"
        git stash
        git checkout .
        git reset --hard
        git fetch --all && git pull
        _SUCCESS="$?"
        git checkout $GIT_BRANCH
        git stash pop
        cd "$_OLD_DIR"

        if test "$_SUCCESS" = "0"; then
            sh "$WORK_DIR/timestamp_save.sh" "$DST_PATH/.git" $_TIMEOUT
        fi
    fi
else
    rm -rf "$DST_PATH" >/dev/null 2>&1
    mkdir -p "$DST_PATH" >/dev/null 2>&1
    git clone -b $GIT_BRANCH $CLONE_OPTION "$PROJ_GIT" "$DST_PATH"

    if test "$?" = "0"; then
        sh "$WORK_DIR/timestamp_save.sh" "$DST_PATH/.git" $_TIMEOUT
    fi
fi

