/**
 * @file ZFCoreQueue.h
 * @brief core queue type for private use only
 */

#ifndef _ZFI_ZFCoreQueue_h_
#define _ZFI_ZFCoreQueue_h_

#include "ZFCoreArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFCoreQueuePODBuiltinBufSize 8

/**
 * @brief core queue type for performance and for private use only
 *
 * @warning can hold POD types only
 */
template<typename T_POD>
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFCoreQueuePOD
{
public:
    /**
     * @brief main constructor
     */
    ZFCoreQueuePOD(void)
    : _bufBuiltin()
    , _bufHead(_bufBuiltin)
    , _bufTail(_bufBuiltin + _ZFP_ZFCoreQueuePODBuiltinBufSize)
    , _pHead(_bufHead)
    , _pTail(_pHead)
    {
    }
    virtual ~ZFCoreQueuePOD(void)
    {
        if(_bufHead != _bufBuiltin)
        {
            zffree(_bufHead);
        }
    }
private:
    // we don't allow copy
    ZFCoreQueuePOD(ZF_IN const ZFCoreQueuePOD<T_POD> &ref);
    ZFCoreQueuePOD<T_POD> &operator = (ZF_IN const ZFCoreQueuePOD<T_POD> &ref);

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const
    {
        this->objectInfoOfContentT(ret, 10);
    }
    /** @brief return object info */
    zffinal inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief see #objectInfoOfContent */
    zffinal void objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                      ZF_IN_OPT zfindex maxCount = zfindexMax(),
                                      ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault(),
                                      ZF_IN_OPT typename ZFCoreInfoGetter<T_POD>::InfoGetter elementInfoGetter = zfnull) const
    {
        if(elementInfoGetter == zfnull)
        {
            elementInfoGetter = ZFCoreElementInfoGetter<T_POD>::elementInfoGetter;
        }

        zfindex count = 0;
        ret += token.tokenLeft;
        for(T_POD *p = _pHead; p != _pTail && count < maxCount; ++count, _loopNext(p))
        {
            if(count > 0)
            {
                ret += token.tokenSeparator;
            }
            ret += token.tokenValueLeft;
            elementInfoGetter(ret, *p);
            ret += token.tokenValueRight;
        }
        if(count < this->count())
        {
            if(count > 0)
            {
                ret += token.tokenSeparator;
            }
            ret += token.tokenEtc;
        }
        ret += token.tokenRight;
    }
    /**
     * @brief return contents info
     */
    zffinal zfstring objectInfoOfContent(ZF_IN_OPT zfindex maxCount = zfindexMax(),
                                         ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault(),
                                         ZF_IN_OPT typename ZFCoreInfoGetter<T_POD>::InfoGetter elementInfoGetter = zfnull) const
    {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token, elementInfoGetter);
        return ret;
    }

public:
    /**
     * @brief get current capacity
     *
     * capacity would be increased automatically during adding elements
     */
    inline zfindex capacity(void) const
    {
        return (_bufTail - _bufHead);
    }
    /**
     * @brief trim the buffer, call only if necessary
     */
    zffinal void queueTrim(void)
    {
        zfindex capacityNew = this->count();
        if(capacityNew < _ZFP_ZFCoreQueuePODBuiltinBufSize)
        {
            _bufChange(_bufBuiltin, _bufBuiltin + _ZFP_ZFCoreQueuePODBuiltinBufSize, zffalse);
        }
        else
        {
            capacityNew = ((capacityNew / _ZFP_ZFCoreQueuePODBuiltinBufSize) + 1) * _ZFP_ZFCoreQueuePODBuiltinBufSize;
            T_POD *bufHeadNew = (T_POD *)zfmalloc(sizeof(T_POD) * capacityNew);
            _bufChange(bufHeadNew, bufHeadNew + capacityNew, zftrue);
        }
    }

public:
    /**
     * @brief push element at tail of the queue,
     *   auto increase capacity if necessary
     */
    inline void queuePut(ZF_IN T_POD const &e)
    {
        *_pTail = e;
        _loopNext(_pTail);
        if(_pTail == _pHead)
        {
            zfindex capacityNew = this->capacity() * 2;
            T_POD *bufHeadNew = (T_POD *)zfmalloc(sizeof(T_POD) * capacityNew);
            _bufChange(bufHeadNew, bufHeadNew + capacityNew, zftrue);
        }
    }
    /**
     * @brief take element at head of the queue, assert fail if empty
     */
    inline T_POD queueTake(void)
    {
        zfCoreAssertWithMessage(_pHead != _pTail, "take from an empty queue");
        T_POD *ret = _pHead;
        _loopNext(_pHead);
        return *ret;
    }

public:
    /**
     * @brief element count of this array
     */
    inline zfindex count(void) const
    {
        return ((_pTail >= _pHead)
            ? (_pTail - _pHead)
            : ((_bufTail - _pHead) + (_pTail - _bufHead)));
    }
    /**
     * @brief true if empty
     */
    inline zfbool isEmpty(void) const
    {
        return (_pHead == _pTail);
    }

public:
    /**
     * @brief remove all contents
     */
    void removeAll(void)
    {
        _pHead = _bufHead;
        _pTail = _bufHead;
    }

public:
    /**
     * @brief copy contents to array
     */
    void toArrayT(ZF_IN_OUT ZFCoreArray<T_POD> &array) const
    {
        array.capacity(array.capacity() + this->count());
        T_POD *p = _pHead;
        while(p != _pTail)
        {
            array.add(*p);
            _loopNext(p);
        }
    }
    /**
     * @brief copy contents to array
     */
    ZFCoreArrayPOD<T_POD> toArray(void) const
    {
        ZFCoreArrayPOD<T_POD> ret;
        this->toArrayT(ret);
        return ret;
    }

private:
    T_POD _bufBuiltin[_ZFP_ZFCoreQueuePODBuiltinBufSize];
    T_POD *_bufHead;
    T_POD *_bufTail;
    T_POD *_pHead;
    T_POD *_pTail;
private:
    inline void _loopNext(ZF_IN_OUT T_POD *&p) const
    {
        ++p;
        if(p >= _bufTail)
        {
            p = _bufHead;
        }
    }
    void _bufChange(ZF_IN T_POD *bufHeadNew, ZF_IN T_POD *bufTailNew, ZF_IN zfbool bufFull)
    {
        T_POD *pNew = bufHeadNew;
        T_POD *pOld = _pHead;
        if(bufFull)
        {
            *pNew = *pOld;
            _loopNext(pOld);
            ++pNew;
        }
        while(pOld != _pTail)
        {
            *pNew = *pOld;
            _loopNext(pOld);
            ++pNew;
        }
        if(_bufHead != _bufBuiltin)
        {
            zffree(_bufHead);
        }
        _bufHead = bufHeadNew;
        _bufTail = bufTailNew;
        _pHead = _bufHead;
        _pTail = pNew;
    }
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreQueue_h_

