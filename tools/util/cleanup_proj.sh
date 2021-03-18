WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_ROOT=$1
PROJ_NAME=$2

if test "x-$PROJ_ROOT" = "x-" || test "x-$PROJ_NAME" = "x-" ; then
    echo "usage:"
    echo "  cleanup_proj.sh PROJ_ROOT PROJ_NAME"
    exit 1
fi

# ============================================================
# common
find "$PROJ_ROOT" -name "zfgensrc_*" -type f 2>/dev/null | xargs -I{} rm {} >/dev/null 2>&1

# ============================================================
# Windows
sh "$WORK_DIR/cleanup_proj/cleanup_Windows.sh" "$PROJ_ROOT/Windows" "$PROJ_NAME"

# ============================================================
# Android
sh "$WORK_DIR/cleanup_proj/cleanup_Android.sh" "$PROJ_ROOT/Android" "$PROJ_NAME"

# ============================================================
# iOS
sh "$WORK_DIR/cleanup_proj/cleanup_iOS.sh" "$PROJ_ROOT/iOS" "$PROJ_NAME"

# ============================================================
# Qt
sh "$WORK_DIR/cleanup_proj/cleanup_Qt.sh" "$PROJ_ROOT/Qt" "$PROJ_NAME"

exit 0

