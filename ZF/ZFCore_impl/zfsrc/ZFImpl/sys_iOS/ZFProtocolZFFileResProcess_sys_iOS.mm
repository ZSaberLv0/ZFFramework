#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFileResProcess.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_iOS
/*
 * res files must be placed at "YourApp.app/zfres/"
 *
 * recommended to use "Run Script" under "Build Phases" of XCode:
 * @code
 *   cp -rf "relative_path_to_res/." "${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/zfres/" >/dev/null 2>&1
 * @endcode
 */
#import <Foundation/Foundation.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileResProcessImpl_sys_iOS, ZFFileResProcess, ZFProtocolLevel::e_SystemNormal)
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        NSString *resRootDirTmp = [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"zfres"];
        this->_resRootPath = resRootDirTmp.UTF8String;
        this->_resRootPath += ZFFileSeparator();
    }

public:
    zfoverride
    virtual const zfstring &resRootPath(void)
    {
        return this->_resRootPath;
    }
private:
    zfstring _resRootPath;
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileResProcessImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileResProcessImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif

