#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFCompress.h"

#include "ZFAlgorithm.h"

#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include "../_repo/miniz/miniz.h"

#ifndef MINIZ_NO_TIME
#include <time.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * note:
 * -  compressed as zip, which should compatible to most compress tools
 * -  for file contents, any encoding should work
 * -  for file and path names, UTF-8 encoding would be used,
 *   would work on:
 *   -  ZFCompress for any platform
 *   -  any compress tools under Mac and Linux (which also use UTF-8)
 *
 *   would not work on:
 *   -  any compress tools under Windows (which use UTF-16)
 *
 *   so, it's recommended to use ASCII chars only for file and path names
 */
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFCompressImpl_default, ZFCompress, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("miniz")
public:
    virtual void *compressBegin(ZF_IN_OUT const ZFOutput &outputZip,
                                ZF_IN ZFCompressLevelEnum compressLevel)
    {
        if(!outputZip.ioSeek(0))
        {
            // output must support random access
            return zfnull;
        }

        mz_zip_archive *zip = (mz_zip_archive *)zfmalloc(sizeof(mz_zip_archive) + sizeof(ZFCompressLevelEnum));
        zfmemset(zip, 0, sizeof(mz_zip_archive));
        *(ZFCompressLevelEnum *)(zip + 1) = compressLevel;

        zip->m_pIO_opaque = zfnew(ZFOutput, outputZip);
        zip->m_pWrite = _writeFuncForOutput;
        if(!mz_zip_writer_init(zip, 0))
        {
            zfdelete((ZFOutput *)zip->m_pIO_opaque);
            zffree(zip);
            return zfnull;
        }
        else
        {
            return zip;
        }
    }
    virtual zfbool compressEnd(ZF_IN_OUT void *compressToken)
    {
        mz_zip_archive *zip = (mz_zip_archive *)compressToken;
        zfbool success = zftrue;
        success &= mz_zip_writer_finalize_archive(zip);
        success &= mz_zip_writer_end(zip);
        zfdelete((ZFOutput *)zip->m_pIO_opaque);
        zffree(zip);
        return success;
    }
    virtual zfbool compressContent(ZF_IN_OUT void *compressToken,
                                   ZF_IN_OUT const ZFInput &inputRaw,
                                   ZF_IN const zfchar *filePathInZip)
    {
        mz_zip_archive *zip = (mz_zip_archive *)compressToken;
        mz_uint flags = MZ_DEFAULT_COMPRESSION;
        switch(*(ZFCompressLevelEnum *)(zip + 1))
        {
            case ZFCompressLevel::e_NoCompress:
                flags = MZ_NO_COMPRESSION;
                break;
            case ZFCompressLevel::e_BestSpeed:
                flags = MZ_BEST_SPEED;
                break;
            case ZFCompressLevel::e_GoodSpeed:
                flags = 3;
                break;
            case ZFCompressLevel::e_DefaultCompress:
                flags = MZ_DEFAULT_LEVEL;
                break;
            case ZFCompressLevel::e_GoodCompress:
                flags = 7;
                break;
            case ZFCompressLevel::e_BestCompress:
                flags = MZ_BEST_COMPRESSION;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return this->compressAdd(*zip, inputRaw, filePathInZip, flags);
    }
    virtual zfbool compressContentDir(ZF_IN_OUT void *compressToken,
                                      ZF_IN const zfchar *filePathInZip)
    {
        mz_zip_archive *zip = (mz_zip_archive *)compressToken;
        zfindex len = zfslen(filePathInZip);
        if(filePathInZip[len - 1] == ZFFileSeparator())
        {
            return mz_zip_writer_add_mem(zip, filePathInZip, NULL, 0, 0);
        }
        else
        {
            zfstring tmp = filePathInZip;
            tmp += ZFFileSeparator();
            return mz_zip_writer_add_mem(zip, tmp.cString(), NULL, 0, 0);
        }
    }

    virtual void *decompressBegin(ZF_IN_OUT const ZFInput &inputZip)
    {
        mz_zip_archive *zip = (mz_zip_archive *)zfmalloc(sizeof(mz_zip_archive));
        zfmemset(zip, 0, sizeof(mz_zip_archive));
        _DecompressToken *pOpaque = zfnew(_DecompressToken);
        zfindex inputSize = inputZip.ioSize();
        if(inputSize == zfindexMax())
        {
            pOpaque->zipBuffer = ZFInputReadToBuffer(inputZip);
            if(pOpaque->zipBuffer.buffer() == zfnull)
            {
                zfdelete(pOpaque);
                zffree(zip);
                return zfnull;
            }
            inputSize = pOpaque->zipBuffer.bufferSize();
            zip->m_pIO_opaque = pOpaque;
            zip->m_pRead = _readFuncForBuffer;
        }
        else
        {
            pOpaque->zipInput = inputZip;
            zip->m_pIO_opaque = pOpaque;
            zip->m_pRead = _readFuncForInput;
        }
        if(!mz_zip_reader_init(zip, (mz_uint64)inputSize, MZ_ZIP_FLAG_CASE_SENSITIVE))
        {
            zfdelete(pOpaque);
            zffree(zip);
            return zfnull;
        }
        return zip;
    }
    virtual zfbool decompressEnd(ZF_IN_OUT void *decompressToken)
    {
        mz_zip_archive *zip = (mz_zip_archive *)decompressToken;
        zfbool success = mz_zip_reader_end(zip);
        zfdelete((_DecompressToken *)zip->m_pIO_opaque);
        zffree(zip);
        return success;
    }
    virtual zfbool decompressContent(ZF_IN_OUT void *decompressToken,
                                     ZF_IN_OUT const ZFOutput &outputRaw,
                                     ZF_IN zfindex fileIndexInZip)
    {
        mz_zip_archive *zip = (mz_zip_archive *)decompressToken;
        ZFOutput _outputRawTmp = outputRaw;
        return mz_zip_reader_extract_to_callback(
            zip,
            (mz_uint)fileIndexInZip,
            _writeFuncForOutput,
            &_outputRawTmp,
            MZ_ZIP_FLAG_CASE_SENSITIVE);
    }
    virtual zfindex decompressContentCount(ZF_IN void *decompressToken)
    {
        mz_zip_archive *zip = (mz_zip_archive *)decompressToken;
        return (zfindex)mz_zip_reader_get_num_files(zip);
    }
    virtual zfindex decompressContentIndex(ZF_IN void *decompressToken,
                                           ZF_IN const zfchar *filePathInZip)
    {
        mz_zip_archive *zip = (mz_zip_archive *)decompressToken;
        int ret = mz_zip_reader_locate_file(zip, filePathInZip, NULL, MZ_ZIP_FLAG_CASE_SENSITIVE);
        if(ret < 0)
        {
            return zfindexMax();
        }
        else
        {
            return (zfindex)ret;
        }
    }
    virtual zfbool decompressContentPath(ZF_IN void *decompressToken,
                                         ZF_IN_OUT zfstring &filePathInZip,
                                         ZF_IN zfindex fileIndexInZip)
    {
        mz_zip_archive *zip = (mz_zip_archive *)decompressToken;
        zfindex size = (zfindex)mz_zip_reader_get_filename(zip, (mz_uint)fileIndexInZip, NULL, 0);
        if(size == 0)
        {
            return zffalse;
        }
        ZFBuffer buffer;
        buffer.bufferMalloc((size + 1) * sizeof(char));
        mz_zip_reader_get_filename(zip, (mz_uint)fileIndexInZip, buffer.bufferT<char *>(), (mz_uint)size);
        filePathInZip += buffer.bufferT<char *>();
        return zftrue;
    }

private:
    zfbool compressAdd(ZF_IN_OUT mz_zip_archive &zip,
                       ZF_IN const ZFInput &input,
                       ZF_IN const zfchar *filePath,
                       ZF_IN mz_uint flags)
    {
        zfindex inputSize = input.ioSize();
        if(inputSize == zfindexMax())
        {
            ZFBuffer buffer = ZFInputReadToBuffer(input);
            return mz_zip_writer_add_mem(&zip, filePath, buffer.buffer(), buffer.bufferSize(), flags);
        }

        mz_zip_archive *pZip = &zip;
        const char *pArchive_name = filePath;
        const void *pComment = NULL;
        mz_uint16 comment_size = 0;
        mz_uint level_and_flags = flags;

        mz_uint uncomp_crc32 = MZ_CRC32_INIT, level, num_alignment_padding_bytes;
        mz_uint16 method = 0, dos_time = 0, dos_date = 0, ext_attributes = 0;
        mz_uint64 local_dir_header_ofs, cur_archive_file_ofs, uncomp_size = 0, comp_size = 0;
        size_t archive_name_size;
        mz_uint8 local_dir_header[MZ_ZIP_LOCAL_DIR_HEADER_SIZE];

        if ((int)level_and_flags < 0)
            level_and_flags = MZ_DEFAULT_LEVEL;
        level = level_and_flags & 0xF;

        if ((!pZip) || (!pZip->m_pState) || (pZip->m_zip_mode != MZ_ZIP_MODE_WRITING) || (!pArchive_name) || ((comment_size) && (!pComment)) || (level > MZ_UBER_COMPRESSION))
            return MZ_FALSE;

        local_dir_header_ofs = cur_archive_file_ofs = pZip->m_archive_size;

        if (level_and_flags & MZ_ZIP_FLAG_COMPRESSED_DATA)
            return MZ_FALSE;
        if (!mz_zip_writer_validate_archive_name(pArchive_name))
            return MZ_FALSE;

        archive_name_size = strlen(pArchive_name);
        if (archive_name_size > 0xFFFF)
            return MZ_FALSE;

        num_alignment_padding_bytes = mz_zip_writer_compute_padding_needed_for_file_alignment(pZip);

        // no zip64 support yet
        if ((pZip->m_total_files == 0xFFFF) || ((pZip->m_archive_size + num_alignment_padding_bytes + MZ_ZIP_LOCAL_DIR_HEADER_SIZE + MZ_ZIP_CENTRAL_DIR_HEADER_SIZE + comment_size + archive_name_size) > 0xFFFFFFFF))
            return MZ_FALSE;

        #ifndef MINIZ_NO_TIME
            time_t cur_time; time(&cur_time);
            mz_zip_time_t_to_dos_time(cur_time, &dos_time, &dos_date);
        #endif

        uncomp_size = (mz_uint64)inputSize;

        if (uncomp_size > 0xFFFFFFFF)
        {
            // No zip64 support yet
            return MZ_FALSE;
        }
        if (uncomp_size <= 3)
            level = 0;

        if (!mz_zip_writer_write_zeros(pZip, cur_archive_file_ofs, num_alignment_padding_bytes + sizeof(local_dir_header)))
        {
            return MZ_FALSE;
        }
        local_dir_header_ofs += num_alignment_padding_bytes;
        if (pZip->m_file_offset_alignment) { MZ_ASSERT((local_dir_header_ofs & (pZip->m_file_offset_alignment - 1)) == 0); }
        cur_archive_file_ofs += num_alignment_padding_bytes + sizeof(local_dir_header);

        MZ_CLEAR_OBJ(local_dir_header);
        if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pArchive_name, archive_name_size) != archive_name_size)
        {
            return MZ_FALSE;
        }
        cur_archive_file_ofs += archive_name_size;

        if (uncomp_size)
        {
            mz_uint64 uncomp_remaining = uncomp_size;
            void *pRead_buf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, MZ_ZIP_MAX_IO_BUF_SIZE);
            if (!pRead_buf)
            {
                return MZ_FALSE;
            }

            if (!level)
            {
                while (uncomp_remaining)
                {
                    mz_uint n = (mz_uint)MZ_MIN(MZ_ZIP_MAX_IO_BUF_SIZE, uncomp_remaining);
                    if ((input.execute(pRead_buf, n) != n) || (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pRead_buf, n) != n))
                    {
                        pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
                        return MZ_FALSE;
                    }
                    uncomp_crc32 = (mz_uint32)mz_crc32(uncomp_crc32, (const mz_uint8 *)pRead_buf, n);
                    uncomp_remaining -= n;
                    cur_archive_file_ofs += n;
                }
                comp_size = uncomp_size;
            }
            else
            {
                mz_bool result = MZ_FALSE;
                mz_zip_writer_add_state state;
                tdefl_compressor *pComp = (tdefl_compressor *)pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, sizeof(tdefl_compressor));
                if (!pComp)
                {
                    pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
                    return MZ_FALSE;
                }

                state.m_pZip = pZip;
                state.m_cur_archive_file_ofs = cur_archive_file_ofs;
                state.m_comp_size = 0;

                if (tdefl_init(pComp, mz_zip_writer_add_put_buf_callback, &state, tdefl_create_comp_flags_from_zip_params(level, -15, MZ_DEFAULT_STRATEGY)) != TDEFL_STATUS_OKAY)
                {
                    pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
                    pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
                    return MZ_FALSE;
                }

                for ( ; ; )
                {
                    size_t in_buf_size = (mz_uint32)MZ_MIN(uncomp_remaining, MZ_ZIP_MAX_IO_BUF_SIZE);
                    tdefl_status status;

                    if (input.execute(pRead_buf, in_buf_size) != in_buf_size)
                        break;

                    uncomp_crc32 = (mz_uint32)mz_crc32(uncomp_crc32, (const mz_uint8 *)pRead_buf, in_buf_size);
                    uncomp_remaining -= in_buf_size;

                    status = tdefl_compress_buffer(pComp, pRead_buf, in_buf_size, uncomp_remaining ? TDEFL_NO_FLUSH : TDEFL_FINISH);
                    if (status == TDEFL_STATUS_DONE)
                    {
                        result = MZ_TRUE;
                        break;
                    }
                    else if (status != TDEFL_STATUS_OKAY)
                        break;
                }

                pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);

                if (!result)
                {
                    pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
                    return MZ_FALSE;
                }

                comp_size = state.m_comp_size;
                cur_archive_file_ofs = state.m_cur_archive_file_ofs;

                method = MZ_DEFLATED;
            }

            pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
        }

        // no zip64 support yet
        if ((comp_size > 0xFFFFFFFF) || (cur_archive_file_ofs > 0xFFFFFFFF))
            return MZ_FALSE;

        if (!mz_zip_writer_create_local_dir_header(pZip, local_dir_header, (mz_uint16)archive_name_size, 0, uncomp_size, comp_size, uncomp_crc32, method, 0, dos_time, dos_date))
            return MZ_FALSE;

        if (pZip->m_pWrite(pZip->m_pIO_opaque, local_dir_header_ofs, local_dir_header, sizeof(local_dir_header)) != sizeof(local_dir_header))
            return MZ_FALSE;

        if (!mz_zip_writer_add_to_central_dir(pZip, pArchive_name, (mz_uint16)archive_name_size, NULL, 0, pComment, comment_size, uncomp_size, comp_size, uncomp_crc32, method, 0, dos_time, dos_date, local_dir_header_ofs, ext_attributes))
            return MZ_FALSE;

        pZip->m_total_files++;
        pZip->m_archive_size = cur_archive_file_ofs;

        return MZ_TRUE;
    }

private:
    zfclassNotPOD _DecompressToken
    {
    public:
        ZFBuffer zipBuffer;
        ZFInput zipInput;
    };

private:
    static size_t _writeFuncForOutput(void *pOpaque, mz_uint64 file_ofs, const void *pBuf, size_t n)
    {
        const ZFOutput &output = *(ZFOutput *)pOpaque;
        if(!output.ioSeek((zfindex)file_ofs))
        {
            return 0;
        }
        return (size_t)output.execute(pBuf, (zfindex)n);
    }
    static size_t _readFuncForInput(void *pOpaque, mz_uint64 file_ofs, void *pBuf, size_t n)
    {
        const ZFInput &input = ((_DecompressToken *)pOpaque)->zipInput;
        if(!input.ioSeek((zfindex)file_ofs))
        {
            return 0;
        }
        return (size_t)input.execute(pBuf, (zfindex)n);
    }
    static size_t _readFuncForBuffer(void *pOpaque, mz_uint64 file_ofs, void *pBuf, size_t n)
    {
        ZFBuffer &buffer = ((_DecompressToken *)pOpaque)->zipBuffer;
        if((zfindex)file_ofs >= buffer.bufferSize())
        {
            return 0;
        }
        else if((zfindex)(file_ofs + n) >= buffer.bufferSize())
        {
            zfindex len = buffer.bufferSize() - (zfindex)file_ofs;
            zfmemcpy(pBuf, buffer.bufferT<zfbyte *>() + file_ofs, len);
            return (size_t)len;
        }
        else
        {
            zfmemcpy(pBuf, buffer.bufferT<zfbyte *>() + file_ofs, (zfindex)n);
            return n;
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFCompressImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFCompressImpl_default)

ZF_NAMESPACE_GLOBAL_END

