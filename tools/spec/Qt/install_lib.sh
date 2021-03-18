WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
SRC_PATH=$2
DST_PATH=$3
if test "x-$PROJ_NAME" = "x-" || test "x-$SRC_PATH" = "x-" || test "x-$DST_PATH" = "x-" ; then
    echo "usage:"
    echo "  install_lib.sh PROJ_NAME SRC_PATH DST_PATH"
    exit 1
fi

mkdir -p "$DST_PATH" >/dev/null 2>&1
find "$SRC_PATH" -regex ".*/.*\.dll" 2>/dev/null | xargs -I{} cp -R {} "$DST_PATH" >/dev/null 2>&1
find "$SRC_PATH" -regex ".*/.*\.a" 2>/dev/null | xargs -I{} cp -R {} "$DST_PATH" >/dev/null 2>&1
find "$SRC_PATH" -regex ".*/.*\.so" 2>/dev/null | xargs -I{} cp -R {} "$DST_PATH" >/dev/null 2>&1
find "$SRC_PATH" -regex ".*/.*\.dylib" 2>/dev/null | xargs -I{} cp -R {} "$DST_PATH" >/dev/null 2>&1

for line in $(find "$DST_PATH" -type f -name "*.a" 2>/dev/null); do
    OLD_DIR=$(pwd)
    cd "$(dirname "$line")"
    rm "lib$PROJ_NAME.a.1" >/dev/null 2>&1
    ln -s "lib$PROJ_NAME.a" "lib$PROJ_NAME.a.1"
    rm "lib$PROJ_NAME.a.1.0" >/dev/null 2>&1
    ln -s "lib$PROJ_NAME.a" "lib$PROJ_NAME.a.1.0"
    rm "lib$PROJ_NAME.a.1.0.0" >/dev/null 2>&1
    ln -s "lib$PROJ_NAME.a" "lib$PROJ_NAME.a.1.0.0"
    cd $OLD_DIR
done

for line in $(find "$DST_PATH" -type f -name "*.so" 2>/dev/null); do
    OLD_DIR=$(pwd)
    cd "$(dirname "$line")"
    rm "lib$PROJ_NAME.so.1" >/dev/null 2>&1
    ln -s "lib$PROJ_NAME.so" "lib$PROJ_NAME.so.1"
    rm "lib$PROJ_NAME.so.1.0" >/dev/null 2>&1
    ln -s "lib$PROJ_NAME.so" "lib$PROJ_NAME.so.1.0"
    rm "lib$PROJ_NAME.so.1.0.0" >/dev/null 2>&1
    ln -s "lib$PROJ_NAME.so" "lib$PROJ_NAME.so.1.0.0"
    cd $OLD_DIR
done

for line in $(find "$DST_PATH" -type f -name "*.dylib" 2>/dev/null); do
    OLD_DIR=$(pwd)
    cd "$(dirname "$line")"
    rm "lib$PROJ_NAME.1.dylib" >/dev/null 2>&1
    ln -s "lib$PROJ_NAME.dylib" "lib$PROJ_NAME.1.dylib"
    rm "lib$PROJ_NAME.1.0.dylib" >/dev/null 2>&1
    ln -s "lib$PROJ_NAME.dylib" "lib$PROJ_NAME.1.0.dylib"
    rm "lib$PROJ_NAME.1.0.0.dylib" >/dev/null 2>&1
    ln -s "lib$PROJ_NAME.dylib" "lib$PROJ_NAME.1.0.0.dylib"
    cd $OLD_DIR
done

