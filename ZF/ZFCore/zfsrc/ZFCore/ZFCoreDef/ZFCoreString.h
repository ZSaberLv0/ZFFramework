/**
 * @file ZFCoreString.h
 * @brief string impl to reduce dependency of std::string
 */

#ifndef _ZFI_ZFCoreString_h_
#define _ZFI_ZFCoreString_h_

#include "ZFCoreTypeDef_CharType.h"
#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

enum {
    _ZFP_zfstr_bufSize = (sizeof(void *) + sizeof(zfindex) * 2),
};
#define _ZFP_zfstr_dynamicBuf ((zfuint)-1)

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_Char>
zffinal zfclassPOD _zfstrD
{
public:
    union {
        struct {
            T_Char *s;
            zfindex capacity;
            zfindex length;
        } s;
        T_Char buf[_ZFP_zfstr_bufSize];
    } d;
    /*
     * when !=_ZFP_zfstr_dynamicBuf, using builtin buf, and length is string size
     * when ==_ZFP_zfstr_dynamicBuf, using dynamic buf
     */
    zfuint length;

public:
    _zfstrD(void)
    : d()
    , length(0)
    {
    }

public:
    inline T_Char *buf(void)
    {
        if(this->length != _ZFP_zfstr_dynamicBuf)
        {
            return d.buf;
        }
        else
        {
            return d.s.s;
        }
    }
    inline const T_Char *buf(void) const
    {
        if(this->length != _ZFP_zfstr_dynamicBuf)
        {
            return d.buf;
        }
        else
        {
            return d.s.s;
        }
    }
};
/** @endcond */
/**
 * @brief low level string container
 */
template<typename T_Char>
zfclassLikePOD ZF_ENV_EXPORT _zfstr
{
public:
    /** @brief construct an empty string */
    _zfstr(void)
    : d()
    {
    }
    /** @brief construct a string with specified capacity */
    _zfstr(ZF_IN zfindex capacity)
    : d()
    {
        _capacityRequire(capacity)[0] = '\0';
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const _zfstr &s)
    : d()
    {
        T_Char *buf = _capacityRequire(s.length());
        zfmemcpy(buf, s.cString(), (s.length() + 1) * sizeof(T_Char));
        _updateLength(s.length());
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const _zfstr &s, zfindex pos)
    : d()
    {
        if(pos < s.length())
        {
            zfindex len = s.length() - pos;
            T_Char *buf = _capacityRequire(len);
            zfmemcpy(buf, s.cString() + pos, (len + 1) * sizeof(T_Char));
            _updateLength(len);
        }
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const _zfstr &s, zfindex pos, zfindex len)
    : d()
    {
        if(pos < s.length())
        {
            if(len > s.length() - pos)
            {
                len = s.length() - pos;
            }
            T_Char *buf = _capacityRequire(len);
            zfmemcpy(buf, s.cString() + pos, (len + 1) * sizeof(T_Char));
            _updateLength(len);
        }
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const T_Char *s)
    : d()
    {
        if(s)
        {
            zfindex len = _len(s);
            T_Char *buf = _capacityRequire(len);
            zfmemcpy(buf, s, (len + 1) * sizeof(T_Char));
            _updateLength(len);
        }
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const T_Char *s, zfindex len)
    : d()
    {
        if(s)
        {
            if(len == zfindexMax())
            {
                len = _len(s);
            }
            T_Char *buf = _capacityRequire(len);
            zfmemcpy(buf, s, len * sizeof(T_Char));
            buf[len] = '\0';
            _updateLength(len);
        }
    }
    ~_zfstr(void)
    {
        if(d.length == _ZFP_zfstr_dynamicBuf)
        {
            zffree(d.d.s.s);
        }
    }

public:
    /** @cond ZFPrivateDoc */
    inline operator const T_Char *(void) const {return this->cString();}
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline _zfstr &operator = (ZF_IN const _zfstr &s)
    {
        if(&s == this)
        {
            return *this;
        }
        else
        {
            return this->assign(s);
        }
    }
    inline _zfstr &operator = (ZF_IN const T_Char *s) {return this->assign(s);}
    _zfstr &operator = (ZF_IN T_Char c)
    {
        T_Char *buf = d.buf();
        buf[0] = c;
        buf[1] = '\0';
        _updateLength(1);
        return *this;
    }
    zfbool operator == (ZF_IN const _zfstr &ref) const
    {
        return (this->compare(ref) == 0);
    }
    zfbool operator != (ZF_IN const _zfstr &ref) const
    {
        return (this->compare(ref) != 0);
    }
    zfbool operator == (ZF_IN const T_Char *ref) const
    {
        return (this->compare(ref) == 0);
    }
    zfbool operator != (ZF_IN const T_Char *ref) const
    {
        return (this->compare(ref) != 0);
    }
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline _zfstr &operator += (ZF_IN const _zfstr &s) {return this->append(s);}
    inline _zfstr &operator += (ZF_IN const T_Char *s) {return this->append(s);}
    _zfstr &operator += (ZF_IN T_Char c)
    {
        zfindex len = this->length();
        T_Char *buf = _capacityRequire(len + 1);
        buf[len] = c;
        ++len;
        buf[len] = '\0';
        _updateLength(len);
        return *this;
    }
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    template<typename T_Index>
    inline T_Char &operator [] (ZF_IN T_Index const &pos)
    {
        return d.buf()[pos];
    }
    template<typename T_Index>
    inline const T_Char &operator [] (ZF_IN T_Index const &pos) const
    {
        return d.buf()[pos];
    }
    /** @endcond */

public:
    /**
     * @brief swrap internal data without deep copy,
     *   designed for performance
     */
    void swap(ZF_IN_OUT _zfstr &ref)
    {
        zfbyte buf[sizeof(d)];
        zfmemcpy(buf, &d, sizeof(d));
        zfmemcpy(&d, &ref.d, sizeof(d));
        zfmemcpy(&ref.d, buf, sizeof(d));
    }

public:
    /** @brief append string */
    inline _zfstr &append(ZF_IN const _zfstr &s) {this->append(s.cString(), s.length()); return *this;}
    /** @brief append string */
    inline _zfstr &append(ZF_IN const T_Char *s) {return this->append(s, zfindexMax());}
    /** @brief append string */
    _zfstr &append(ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(s)
        {
            if(len == zfindexMax())
            {
                len = _len(s);
            }
            zfindex lenTmp = this->length();
            T_Char *buf = _capacityRequire(lenTmp + len);
            zfmemcpy(buf + lenTmp, s, len * sizeof(T_Char));
            lenTmp += len;
            buf[lenTmp] = '\0';
            _updateLength(lenTmp);
        }
        return *this;
    }

public:
    /** @brief replace all content of the string */
    inline _zfstr &assign(ZF_IN const _zfstr &s) {return this->assign(s.cString(), s.length());}
    /** @brief replace all content of the string */
    inline _zfstr &assign(ZF_IN const T_Char *s) {return this->assign(s, zfindexMax());}
    /** @brief replace all content of the string */
    _zfstr &assign(ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(s)
        {
            if(len == zfindexMax())
            {
                len = _len(s);
            }
            T_Char *buf = _capacityRequire(len);;
            zfmemcpy(buf, s, len * sizeof(T_Char));
            buf[len] = '\0';
            _updateLength(len);
        }
        else
        {
            d.buf()[0] = '\0';
            _updateLength(0);
        }
        return *this;
    }

public:
    /** @brief insert string */
    inline _zfstr &insert(ZF_IN zfindex insertAt, ZF_IN const _zfstr &s) {return this->insert(insertAt, s.cString(), s.length());}
    /** @brief insert string */
    inline _zfstr &insert(ZF_IN zfindex insertAt, ZF_IN const T_Char *s) {return this->insert(insertAt, s, zfindexMax());}
    /** @brief insert string */
    _zfstr &insert(ZF_IN zfindex insertAt, ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(insertAt >= this->length())
        {
            this->append(s, len);
        }
        else if(s)
        {
            if(len == zfindexMax())
            {
                len = _len(s);
            }
            zfindex lenTmp = this->length();
            T_Char *buf = _capacityRequire(lenTmp + len);
            zfmemmove(buf + insertAt + len, buf + insertAt, (lenTmp - insertAt) * sizeof(T_Char));
            zfmemcpy(buf + insertAt, s, len * sizeof(T_Char));
            lenTmp += len;
            buf[lenTmp] = '\0';
            _updateLength(lenTmp);
        }
        return *this;
    }

public:
    /** @brief replace string in range */
    inline _zfstr &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const _zfstr &s) {return this->replace(replacePos, replaceLen, s.cString(), s.length());}
    /** @brief replace string in range */
    inline _zfstr &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const T_Char *s) {return this->replace(replacePos, replaceLen, s, zfindexMax());}
    /** @brief replace string in range */
    _zfstr &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(replacePos >= this->length())
        {
            this->append(s, len);
        }
        else if(s)
        {
            zfindex lenTmp = this->length();
            if(replaceLen > lenTmp - replacePos)
            {
                replaceLen = lenTmp - replacePos;
            }
            if(len == zfindexMax())
            {
                len = _len(s);
            }
            T_Char *buf = _capacityRequire(lenTmp + len - replaceLen);
            zfmemmove(buf + replacePos + len, buf + replacePos + replaceLen, (lenTmp - replacePos - replaceLen) * sizeof(T_Char));
            zfmemcpy(buf + replacePos, s, len * sizeof(T_Char));
            lenTmp = lenTmp + len - replaceLen;
            buf[lenTmp] = '\0';
            _updateLength(lenTmp);
        }
        return *this;
    }

public:
    /** @brief access string value */
    inline const T_Char *cString(void) const
    {
        return d.buf();
    }
    /** @brief length of the string */
    inline zfindex length(void) const
    {
        if(d.length != _ZFP_zfstr_dynamicBuf)
        {
            return d.length;
        }
        else
        {
            return d.d.s.length;
        }
    }
    /** @brief true if empty */
    inline zfbool isEmpty(void) const
    {
        if(d.length != _ZFP_zfstr_dynamicBuf)
        {
            return d.length == 0;
        }
        else
        {
            return d.d.s.length == 0;
        }
    }

public:
    /** @brief ensure the string's capacity */
    inline void capacity(ZF_IN zfindex capacity) {_capacityRequire(capacity);}
    /** @brief capacity of the string */
    zfindex capacity(void)
    {
        if(d.length != _ZFP_zfstr_dynamicBuf)
        {
            return _ZFP_zfstr_bufSize - 1;
        }
        else
        {
            return (zfindex)d.d.s.capacity;
        }
    }
    /** @brief remove part of the string */
    void remove(ZF_IN_OPT zfindex pos = 0, ZF_IN_OPT zfindex len = zfindexMax())
    {
        zfindex lenTmp = this->length();
        if(pos < lenTmp)
        {
            if(len > lenTmp - pos)
            {
                len = lenTmp - pos;
            }
            if(len > 0)
            {
                T_Char *buf = d.buf();
                zfmemmove(buf + pos, buf + pos + len, (lenTmp - pos - len) * sizeof(T_Char));
                lenTmp -= len;
                buf[lenTmp] = '\0';
                _updateLength(lenTmp);
            }
        }
    }
    /** @brief remove all content of the string */
    inline void removeAll(void)
    {
        d.buf()[0] = '\0';
        _updateLength(0);
    }

public:
    /** @brief compare with another string */
    inline zfint compare(ZF_IN const _zfstr &s) const {return _cmp(this->cString(), s.cString());}
    /** @brief compare with another string */
    zfint compare(ZF_IN const T_Char *s, ZF_IN zfindex len = zfindexMax()) const
    {
        const T_Char *buf = d.buf();
        if(s)
        {
            if(len == zfindexMax())
            {
                len = _len(s);
            }
            if(len > this->length())
            {
                return _ncmp(buf, s, this->length() + 1);
            }
            else
            {
                zfint t = _ncmp(buf, s, len);
                if(t)
                {
                    return t;
                }
                else
                {
                    return (zfint)buf[len];
                }
            }
        }
        else
        {
            return (zfint)buf[0];
        }
    }

private:
    _zfstrD<T_Char> d;
private:
    T_Char *_capacityRequire(ZF_IN zfindex size)
    {
        if(d.length != _ZFP_zfstr_dynamicBuf)
        {
            if(size >= _ZFP_zfstr_bufSize)
            {
                zfindex capacity = this->capacity();
                while(capacity <= size)
                {
                    capacity *= 2;
                }
                T_Char *buf = (T_Char *)zfmalloc(capacity * sizeof(T_Char));
                zfmemcpy(buf, d.d.buf, (d.length + 1) * sizeof(T_Char));
                d.d.s.s = buf;
                d.d.s.capacity = capacity;
                d.d.s.length = d.length;
                d.length = _ZFP_zfstr_dynamicBuf;
                return d.d.s.s;
            }
            return d.d.buf;
        }
        else
        {
            if((zfuint)size >= d.d.s.capacity)
            {
                do
                {
                    d.d.s.capacity *= 2;
                } while(d.d.s.capacity <= size);
                d.d.s.s = (T_Char *)zfrealloc(d.d.s.s, d.d.s.capacity * sizeof(T_Char));
            }
            return d.d.s.s;
        }
    }
    inline void _updateLength(ZF_IN zfindex len)
    {
        if(d.length != _ZFP_zfstr_dynamicBuf)
        {
            d.length = (zfuint)len;
        }
        else
        {
            d.d.s.length = len;
        }
    }
    static zfindex _len(ZF_IN const T_Char *s)
    {
        const T_Char *p = s;
        while(*p) {++p;}
        return p - s;
    }
    static zfint _cmp(ZF_IN const T_Char *s1, ZF_IN const T_Char *s2)
    {
        while(*s1 && *s2 && *s1 == *s2) {++s1, ++s2;}
        return *s1 - *s2;
    }
    static zfint _ncmp(ZF_IN const T_Char *s1, ZF_IN const T_Char *s2, ZF_IN zfindex len)
    {
        if(len == 0)
        {
            return 0;
        }
        else
        {
            while(--len && *s1 && *s2 && *s1 == *s2) {++s1, ++s2;}
            return *s1 - *s2;
        }
    }
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreString_h_

