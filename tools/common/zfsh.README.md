a util script runner to reduce dependency

# how it work

plain text replace, then run it using as local shell script, example:

```
for Windows:
call zfsh.bat test.zfsh

for *nix:
sh zfsh.sh test.zfsh
```

the test.zfsh file:

```
<ZF_SH> "<ZF_ROOT_PATH>\tools\common\copy_check.<ZF_SH_EXT>" "path\from" "path\to"
```

replace result:

```
result for Windows:
call "path_of_ZF\tools\common\copy_check.bat" "path\from" "path\to"

result for *nix:
sh "path_of_ZF/tools/common/copy_check.sh" "path/from" "path/to"
```

# replace list

common:

* `\` : `\` for Windows and `/` for other
* `ZF_COMMENT` : `rem` for Windows and `#` for other
* `ZF_SH` : `call` for Windows and `sh` for other
* `ZF_SH_EXT` : `bat` for Windows and `sh` for other

path:

* `ZF_ROOT_PATH` : the absolute path of ZFFramework
* `ZF_WORK_DIR` : the absolute path of the zfsh script file

param:

* `ZF_P0` ~ `ZF_P8` : 0 ~ 8th param

logic:

* `ZF_SET` : `set` for Windows and empty for other
* `ZF_PL` / `ZF_PR` : ref to other param, `%xxx%` for Windows and `${xxx}` for other
* `ZF_EXIT_SUCCESS` : exit with success state
* `ZF_EXIT_FAILED` : exit with failed state
* `ZF_IF_SUCCESS_BEGIN` : if statement
* `ZF_IF_SUCCESS_END` : if statement
* `ZF_IF_FAILED_BEGIN` : if statement
* `ZF_IF_FAILED_END` : if statement

