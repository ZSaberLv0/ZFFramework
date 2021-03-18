WORK_DIR=$(cd "$(dirname "$0")"; pwd)
MODULE_REPO=$1
MODULE_NAME=$2
MODULE_BRANCH=$3
if test "x-$MODULE_REPO" = "x-" ; then
    echo "usage:"
    echo "  zfmodule.sh MODULE_REPO [MODULE_NAME MODULE_BRANCH]"
    exit 1
fi

if test "x-$MODULE_NAME" = "x-" ; then
    MODULE_NAME=${MODULE_REPO##*[/\\]}
fi
if test "x-$MODULE_NAME" = "x-" ; then
    echo "unable to parse MODULE_NAME"
    exit 1
fi

if test "x-$MODULE_BRANCH" = "x-" ; then
    MODULE_BRANCH=master
fi

ZF_ROOT_PATH=$WORK_DIR/../..

if ! test "x-$MODULE_BRANCH" = "x-LOCAL" ; then
    sh "$ZF_ROOT_PATH/tools/common/git_check.sh" $MODULE_REPO $MODULE_BRANCH "$ZF_ROOT_PATH/../ZFModule/ZF/$MODULE_NAME"
fi

