WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_PATH=$1
if test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  unique_proj.sh PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

sh "$ZF_TOOLS_PATH/common/zfsh.sh" "$WORK_DIR/zf3rd/zfautoscript_zf3rd_setup_xUnique.zfsh"

if test -d "$PROJ_PATH" ; then
    _SRC_PATH="$PROJ_PATH/project.pbxproj"
    _TMP_PATH="$PROJ_PATH/zfxunique_tmp"
    _DST_PATH="$_TMP_PATH/project.pbxproj"
else
    _SRC_PATH="$PROJ_PATH"
    _TMP_PATH="${PROJ_PATH%[/\\]*}/zfxunique_tmp"
    _DST_PATH="$_TMP_PATH/project.pbxproj"
fi

rm -rf "$_TMP_PATH" >/dev/null 2>&1
mkdir -p "$_TMP_PATH" >/dev/null 2>&1
cp -r "$_SRC_PATH" "$_DST_PATH" >/dev/null 2>&1
python "$WORK_DIR/zf3rd/_repo/xUnique/xUnique.py" "$_DST_PATH"

if test -e "$_TMP_PATH/project.pbxproj.ubak" ; then
    mv "$_TMP_PATH/project.pbxproj.ubak" "$_TMP_PATH/project.pbxproj"
fi

if test -e "$_TMP_PATH/project.pbxproj.sbak" ; then
    mv "$_TMP_PATH/project.pbxproj.sbak" "$_TMP_PATH/project.pbxproj"
fi

rsync -ruc "$_DST_PATH" "$_SRC_PATH" >/dev/null 2>&1

rm -rf "$_TMP_PATH" >/dev/null 2>&1

exit 0

