WORK_DIR=$(cd "$(dirname "$0")"; pwd)
POD_SUBSPEC=$1

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools


_OLD_DIR=$(pwd)
cd "$ZF_ROOT_PATH"
if test "x-$POD_SUBSPEC" = "x-" ; then
    pod lib lint --allow-warnings --use-libraries --skip-import-validation --verbose
    _RESULT="$?"
else
    pod lib lint --allow-warnings --use-libraries --skip-import-validation --verbose --subspec=$POD_SUBSPEC
    _RESULT="$?"
fi
cd "$_OLD_DIR"

exit $_RESULT

