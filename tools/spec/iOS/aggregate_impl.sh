WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
RELEASE_PATH=$2
BUILD_PATH=$3
COCOAPODS_PROJECT_TRUE_OR_FALSE=$4
HEADER_PATH_OR_DUMMY=$5
RES_PATH_OR_DUMMY=$6
if test "x-$PROJ_NAME" = "x-"\
    || test "x-$RELEASE_PATH" = "x-"\
    || test "x-$BUILD_PATH" = "x-"\
    || test "x-$COCOAPODS_PROJECT_TRUE_OR_FALSE" = "x-"\
    || test "x-$HEADER_PATH_OR_DUMMY" = "x-"\
    || test "x-$RES_PATH_OR_DUMMY" = "x-"\
    ; then
    echo "usage:"
    echo "  aggregate_impl.sh PROJ_NAME RELEASE_PATH BUILD_PATH COCOAPODS_PROJECT_TRUE_OR_FALSE HEADER_PATH_OR_DUMMY RES_PATH_OR_DUMMY"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

# ============================================================
# no need to change these
if test "$HEADER_PATH_OR_DUMMY" = "DUMMY" ; then
    HEADER_SRC_PATH=
    HEADER_DST_PATH=
else
    HEADER_SRC_PATH=$HEADER_PATH_OR_DUMMY
    HEADER_DST_PATH=$RELEASE_PATH/include
fi
if test "$RES_PATH_OR_DUMMY" = "DUMMY" ; then
    RES_SRC_PATH=
    RES_DST_PATH=
else
    RES_SRC_PATH=$RES_PATH_OR_DUMMY
    RES_DST_PATH=$RELEASE_PATH/zfres
fi

# build paths
DEVICE_DIR=$BUILD_PATH/Release-iphoneos
SIMULATOR_DIR=$BUILD_PATH/Release-iphonesimulator

# -configuration $CONFIGURATION
# Clean and Building both architectures.
if test "x-$COCOAPODS_PROJECT_TRUE_OR_FALSE" = "x-TRUE" ; then
    xcodebuild -workspace "$PROJ_NAME.xcworkspace" -scheme "$PROJ_NAME" -configuration "Release" -sdk iphoneos SYMROOT=$BUILD_PATH
    xcodebuild -workspace "$PROJ_NAME.xcworkspace" -scheme "$PROJ_NAME" -configuration "Release" -sdk iphonesimulator SYMROOT=$BUILD_PATH
else
    xcodebuild -configuration "Release" -target "$PROJ_NAME" -sdk iphoneos SYMROOT=$BUILD_PATH
    xcodebuild -configuration "Release" -target "$PROJ_NAME" -sdk iphonesimulator SYMROOT=$BUILD_PATH
fi

# Cleaning the oldest.
if test -e "$RELEASE_PATH" ; then
    rm -rf "$RELEASE_PATH" >/dev/null 2>&1
fi

mkdir -p "$RELEASE_PATH/lib" >/dev/null 2>&1

# Uses the Lipo Tool to merge both binary files (i386 + armv6/armv7) into one Universal final product.
lipo -create "$DEVICE_DIR/lib$PROJ_NAME.a" "$SIMULATOR_DIR/lib$PROJ_NAME.a" -output "$RELEASE_PATH/lib/lib$PROJ_NAME.a"

# copy headers
if ! test "x-$HEADER_SRC_PATH" = "x-" && ! test "x-$HEADER_DST_PATH" = "x-" ; then
    sh $ZF_TOOLS_PATH/util/copy_header.sh $HEADER_SRC_PATH $HEADER_DST_PATH
fi

# copy res
if ! test "x-$RES_SRC_PATH" = "x-" && ! test "x-$RES_DST_PATH" = "x-" ; then
    sh $ZF_TOOLS_PATH/util/copy_res.sh $RES_SRC_PATH $RES_DST_PATH
fi

