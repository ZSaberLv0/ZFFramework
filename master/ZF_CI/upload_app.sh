WORK_DIR=$(cd "$(dirname "$0")"; pwd)
APP_PATH=$1
REMOTE_FILE=$2

if test "x-$ZFCI_TOKEN" = "x-" ; then
    echo "ZFCI_TOKEN not set"
    exit 1
fi

if test "x-$APP_PATH" = "x-" || test "x-$REMOTE_FILE" = "x-" ; then
    echo "usage:"
    echo "  upload_app.sh APP_PATH REMOTE_FILE"
    exit 1
fi

if ! test -e "$APP_PATH" ; then
    echo "file not exist: $APP_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
REPO_PATH="$ZF_ROOT_PATH/_tmp/upload_app"
REPO_PATH_TMP="$ZF_ROOT_PATH/_tmp/upload_app_tmp"

sh "$ZF_TOOLS_PATH/common/git_check.sh" "https://github.com/ZFFramework/AppArchive" master "$REPO_PATH"

_OLD_DIR=$(pwd)
cd "$REPO_PATH"
git config user.email "z@zsaber.com"
git config user.name "ZSaberLv0"
git config push.default "simple"
git checkout .
git reset --hard
git clean -xdf
git pull

sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$REPO_PATH" "$REPO_PATH_TMP"
cd "$_OLD_DIR"
cp -r "$APP_PATH" "$REPO_PATH_TMP/$REMOTE_FILE"
cd "$REPO_PATH"
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch *' --prune-empty --tag-name-filter cat -- --all
sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$REPO_PATH_TMP" "$REPO_PATH"

git add -A
git commit -a -m "update by CI"
git push --force "https://ZSaberLv0:$ZFCI_TOKEN@github.com/ZFFramework/AppArchive"
cd "$_OLD_DIR"

rm -rf "$REPO_PATH"
rm -rf "$REPO_PATH_TMP"

