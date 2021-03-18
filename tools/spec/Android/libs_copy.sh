WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  libs_copy.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..

mkdir -p "$ZF_ROOT_PATH/_release/Android/module/$PROJ_NAME/src/main/jniLibs" >/dev/null 2>&1
cp -rf "$PROJ_PATH/zflib/build/intermediates/library_and_local_jars_jni/release/." "$ZF_ROOT_PATH/_release/Android/module/$PROJ_NAME/src/main/jniLibs/" >/dev/null 2>&1
sh "$ZF_ROOT_PATH/tools/util/copy_header.sh" "$PROJ_PATH/../../../zfsrc" "$ZF_ROOT_PATH/_release/Android/module/$PROJ_NAME/src/main/cpp/include" >/dev/null 2>&1
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$PROJ_PATH/../../../zfres" "$ZF_ROOT_PATH/_release/Android/module/$PROJ_NAME/src/main/assets/zfres" >/dev/null 2>&1

JAR_FILE_SIZE=`wc -c "$PROJ_PATH/zflib/build/intermediates/packaged-classes/release/classes.jar" 2>/dev/null | awk '{print $1}' 2>/dev/null`
if test ! "x-$JAR_FILE_SIZE" = "x-" && test $JAR_FILE_SIZE -ge 1024 ; then
    mkdir -p "$ZF_ROOT_PATH/_release/Android/module/$PROJ_NAME/src/main/libs" >/dev/null 2>&1
    cp "$PROJ_PATH/zflib/build/intermediates/packaged-classes/release/classes.jar" "$ZF_ROOT_PATH/_release/Android/module/$PROJ_NAME/src/main/libs/$PROJ_NAME.jar" >/dev/null 2>&1
fi

sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$PROJ_PATH/zflib/build/outputs/aar/zflib-release.aar" "$ZF_ROOT_PATH/_release/Android/module/$PROJ_NAME/aar/$PROJ_NAME.aar" >/dev/null 2>&1

sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$ZF_ROOT_PATH/_release/Android/module/$PROJ_NAME" "$ZF_ROOT_PATH/_release/Android/all" >/dev/null 2>&1

exit 0

