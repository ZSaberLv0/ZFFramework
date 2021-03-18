WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_PATH=$1
if test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  unique_proj_recursive.sh PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

for line in $(find "$PROJ_PATH" -name "project.pbxproj" -type f 2>/dev/null); do
    echo "$line" | grep -q "Pods.xcodeproj" && continue
    echo "$line" | grep -q "private" && continue
    echo "$line" | grep -q "zfres" && continue

    echo "processing $line"
    sh "$WORK_DIR/unique_proj.sh" "$line"
    if test ! "$?" = "0" ; then
        exit 1
    fi
done

