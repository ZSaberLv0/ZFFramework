
// added by ZFFramework
// system() not available since iOS11
#include "ZFImpl.h"
#if ZF_ENV_sys_iOS
#define system(...) (-1)
#endif

