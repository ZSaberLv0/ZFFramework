WORK_DIR=$(cd "$(dirname "$0")"; pwd)
if test "x-$ZFCI_COCOAPODS_TOKEN" = "x-" ; then
    echo "ZFCI_COCOAPODS_TOKEN not set"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

_OLD_DIR=$(pwd)
cd "$ZF_ROOT_PATH"
export COCOAPODS_TRUNK_TOKEN=$ZFCI_COCOAPODS_TOKEN
pod trunk push --allow-warnings --use-libraries --skip-import-validation --verbose
_RESULT="$?"
cd "$_OLD_DIR"

exit $_RESULT

