WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZFSH_PATH=$1
shift
VA_PARAMS=$@

if test "x-$ZFSH_PATH" = "x-" ; then
    echo "usage:"
    echo "  zfsh.sh ZFSH_PATH [...]"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZFSH_WORK_DIR=${ZFSH_PATH%[/\\]*}
TMP_PATH=zfsh_tmp_${ZFSH_PATH##*[/\\]}.sh
TMP_PATH=$ZF_ROOT_PATH/_tmp/$TMP_PATH

rm -rf "$TMP_PATH" >/dev/null 2>&1
mkdir -p "${TMP_PATH%[/\\]*}" >/dev/null 2>&1

echo _ZF_ROOT_PATH=$ZF_ROOT_PATH>>"$TMP_PATH"
echo _ZFSH_WORK_DIR=$ZFSH_WORK_DIR>>"$TMP_PATH"
echo _ZF_P0=\$0>>"$TMP_PATH"
echo _ZF_P1=\$1>>"$TMP_PATH"
echo _ZF_P2=\$2>>"$TMP_PATH"
echo _ZF_P3=\$3>>"$TMP_PATH"
echo _ZF_P4=\$4>>"$TMP_PATH"
echo _ZF_P5=\$5>>"$TMP_PATH"
echo _ZF_P6=\$6>>"$TMP_PATH"
echo _ZF_P7=\$7>>"$TMP_PATH"
echo _ZF_P8=\$8>>"$TMP_PATH"

cat "$ZFSH_PATH" >> "$TMP_PATH"

# the `sed -i` has different behavior in different shell,
# use another tmp file as workaround
cp "$TMP_PATH" "$TMP_PATH.tmp" >/dev/null 2>&1
sed -e 's/\\/\//g' \
    \
    -e 's/<ZF_COMMENT>/#/g' \
    -e 's/<ZF_SH>/sh/g' \
    -e 's/<ZF_SH_EXT>/sh/g' \
    \
    -e 's/<ZF_ROOT_PATH>/\$_ZF_ROOT_PATH/g' \
    -e 's/<ZF_WORK_DIR>/\$_ZFSH_WORK_DIR/g' \
    \
    -e 's/<ZF_P0>/\$\{_ZF_P0\}/g' \
    -e 's/<ZF_P1>/\$\{_ZF_P1\}/g' \
    -e 's/<ZF_P2>/\$\{_ZF_P2\}/g' \
    -e 's/<ZF_P3>/\$\{_ZF_P3\}/g' \
    -e 's/<ZF_P4>/\$\{_ZF_P4\}/g' \
    -e 's/<ZF_P5>/\$\{_ZF_P5\}/g' \
    -e 's/<ZF_P6>/\$\{_ZF_P6\}/g' \
    -e 's/<ZF_P7>/\$\{_ZF_P7\}/g' \
    -e 's/<ZF_P8>/\$\{_ZF_P8\}/g' \
    \
    -e 's/<ZF_SET>//g' \
    -e 's/<ZF_PL>/\$\{/g' \
    -e 's/<ZF_PR>/\}/g' \
    -e 's/<ZF_EXIT_SUCCESS>/exit 0/g' \
    -e 's/<ZF_EXIT_FAILED>/exit 1/g' \
    -e 's/<ZF_IF_SUCCESS_BEGIN>/if test \"\$?\" = \"0\" ; then/g' \
    -e 's/<ZF_IF_SUCCESS_END>/fi/g' \
    -e 's/<ZF_IF_FAILED_BEGIN>/if test ! \"\$?\" = \"0\" ; then/g' \
    -e 's/<ZF_IF_FAILED_END>/fi/g' \
    "$TMP_PATH.tmp" > "$TMP_PATH"
rm -rf "$TMP_PATH.tmp" >/dev/null 2>&1

sh "$TMP_PATH" $VA_PARAMS
RESULT=$?
rm -rf "$TMP_PATH" >/dev/null 2>&1
exit $RESULT

