#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFileReadWrite.h"
#include "ZFCore/ZFString.h"
#include <stdio.h>

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileReadWriteImpl_default, ZFFileReadWrite, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("C:FILE")
public:
    virtual void *fileOpen(ZF_IN const zfchar *filePath,
                           ZF_IN_OPT ZFFileOpenOptionFlags flag = ZFFileOpenOption::e_Read)
    {
        const zfchar *sFlag = zfnull;
        if(ZFBitTest(flag, ZFFileOpenOption::e_Append))
        {
            if(ZFBitTest(flag, ZFFileOpenOption::e_Read))
            {
                sFlag = "a+b";
            }
            else
            {
                sFlag = "ab";
            }
        }
        else if(ZFBitTest(flag, ZFFileOpenOption::e_Create))
        {
            if(ZFBitTest(flag, ZFFileOpenOption::e_Read))
            {
                sFlag = "w+b";
            }
            else
            {
                sFlag = "wb";
            }
        }
        else if(ZFBitTest(flag, ZFFileOpenOption::e_Write))
        {
            if(ZFBitTest(flag, ZFFileOpenOption::e_Read))
            {
                sFlag = "r+b";
            }
            else
            {
                sFlag = "wb";
            }
        }
        else if(ZFBitTest(flag, ZFFileOpenOption::e_Read))
        {
            sFlag = "rb";
        }
        else
        {
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
        }

        #if ZF_ENV_sys_Windows
            zfstringW _filePathSaved;
            zfstringToUTF16(_filePathSaved, filePath, ZFStringEncoding::e_UTF8);
            DWORD _fileAttrSaved = GetFileAttributesW(_filePathSaved.cString());
            SetFileAttributesW(_filePathSaved.cString(), FILE_ATTRIBUTE_NORMAL);
        #endif

        void *ret = fopen(filePath, sFlag);

        #if ZF_ENV_sys_Windows
            SetFileAttributesW(_filePathSaved.cString(), _fileAttrSaved);
        #endif

        return ret;
    }
    virtual zfbool fileClose(ZF_IN void *token)
    {
        if(token == zfnull)
        {
            return zftrue;
        }
        return (fclose((FILE *)token) == 0);
    }

    virtual zfindex fileTell(ZF_IN void *token)
    {
        if(token == zfnull)
        {
            return zfindexMax();
        }
        long result = ftell((FILE *)token);
        if(result == -1)
        {
            return zfindexMax();
        }
        return (zfindex)result;
    }
    virtual zfbool fileSeek(ZF_IN void *token,
                            ZF_IN zfindex byteSize,
                            ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin)
    {
        if(token == zfnull)
        {
            return zffalse;
        }
        int tmpPos = 0;
        long seekSize = (long)byteSize;
        switch(position)
        {
            case ZFSeekPosBegin:
                tmpPos = SEEK_SET;
                break;
            case ZFSeekPosCur:
                tmpPos = SEEK_CUR;
                break;
            case ZFSeekPosCurReversely:
                tmpPos = SEEK_CUR;
                seekSize = -seekSize;
                break;
            case ZFSeekPosEnd:
                tmpPos = SEEK_END;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return (fseek((FILE *)token, seekSize, tmpPos) == 0);
    }

    virtual zfindex fileRead(ZF_IN void *token,
                             ZF_IN void *buf,
                             ZF_IN zfindex maxByteSize)
    {
        if(token == zfnull)
        {
            return 0;
        }
        else
        {
            return (zfindex)fread(buf, 1, (size_t)maxByteSize, (FILE *)token);
        }
    }

    virtual zfindex fileWrite(ZF_IN void *token,
                              ZF_IN const void *src,
                              ZF_IN zfindex maxByteSize)
    {
        if(token == zfnull)
        {
            return 0;
        }
        else
        {
            return (zfindex)fwrite(src, 1, (size_t)maxByteSize, (FILE *)token);
        }
    }
    virtual void fileFlush(ZF_IN void *token)
    {
        if(token != zfnull)
        {
            fflush((FILE *)token);
        }
    }
    virtual zfbool fileIsEof(ZF_IN void *token)
    {
        return (feof((FILE *)token) != 0);
    }
    virtual zfbool fileIsError(ZF_IN void *token)
    {
        return (ferror((FILE *)token) != 0);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileReadWriteImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileReadWriteImpl_default)

ZF_NAMESPACE_GLOBAL_END

