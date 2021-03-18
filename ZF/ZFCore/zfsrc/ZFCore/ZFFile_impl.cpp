#ifndef _ZFI_ZFFile_impl_cpp_
#define _ZFI_ZFFile_impl_cpp_

#include "ZFFile.h"

#include "protocol/ZFProtocolZFFile.h"
#include "protocol/ZFProtocolZFFileCwd.h"
#include "protocol/ZFProtocolZFFilePath.h"
#include "protocol/ZFProtocolZFFileReadWrite.h"
#include "protocol/ZFProtocolZFFileResProcess.h"

#define _ZFP_ZFFileImpl ZFPROTOCOL_ACCESS(ZFFile)
#define _ZFP_ZFFilePathImpl ZFPROTOCOL_ACCESS(ZFFilePath)
#define _ZFP_ZFFileReadWriteImpl ZFPROTOCOL_ACCESS(ZFFileReadWrite)
#define _ZFP_ZFFileResProcessImpl ZFPROTOCOL_ACCESS(ZFFileResProcess)

#endif // #ifndef _ZFI_ZFFile_impl_cpp_

