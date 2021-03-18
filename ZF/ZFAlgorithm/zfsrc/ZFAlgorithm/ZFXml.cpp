#include "ZFXml.h"
#include "protocol/ZFProtocolZFXml.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFXmlType)
ZFENUM_DEFINE(ZFXmlVisitType)

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFXmlVisitData, ZFXmlVisitData)
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFXmlVisitCallback, ZFXmlVisitCallback)
ZFEXPORT_VAR_READONLY_DEFINE(ZFXmlVisitCallback, ZFXmlVisitCallbackDefault, ZFXmlVisitCallbackForOutput())

// ============================================================
zfbool ZFXmlOutputToken::operator == (ZF_IN ZFXmlOutputToken const &ref) const
{
    return (zftrue
            && this->xmlNewLineToken == ref.xmlNewLineToken
            && this->xmlIndentToken == ref.xmlIndentToken
            && this->xmlDeclarationTagLeft == ref.xmlDeclarationTagLeft
            && this->xmlDeclarationTagRight == ref.xmlDeclarationTagRight
            && this->xmlDocTypeTagLeft == ref.xmlDocTypeTagLeft
            && this->xmlDocTypeTagRight == ref.xmlDocTypeTagRight
            && this->xmlPITagLeft == ref.xmlPITagLeft
            && this->xmlPITagRight == ref.xmlPITagRight
            && this->xmlElementBeginTagLeft == ref.xmlElementBeginTagLeft
            && this->xmlElementBeginTagRight == ref.xmlElementBeginTagRight
            && this->xmlElementEndTagLeft == ref.xmlElementEndTagLeft
            && this->xmlElementEndTagRight == ref.xmlElementEndTagRight
            && this->xmlElementSingleTagLeft == ref.xmlElementSingleTagLeft
            && this->xmlElementSingleTagRight == ref.xmlElementSingleTagRight
            && this->xmlAttributeEqualTag == ref.xmlAttributeEqualTag
            && this->xmlAttributeQuoteTagLeft == ref.xmlAttributeQuoteTagLeft
            && this->xmlAttributeQuoteTagRight == ref.xmlAttributeQuoteTagRight
            && this->xmlAttributeSingleQuoteTagLeft == ref.xmlAttributeSingleQuoteTagLeft
            && this->xmlAttributeSingleQuoteTagRight == ref.xmlAttributeSingleQuoteTagRight
            && this->xmlTextCDATATagLeft == ref.xmlTextCDATATagLeft
            && this->xmlTextCDATATagRight == ref.xmlTextCDATATagRight
            && this->xmlCommentTagLeft == ref.xmlCommentTagLeft
            && this->xmlCommentTagRight == ref.xmlCommentTagRight
        );
}
zfbool ZFXmlOutputFlags::operator == (ZF_IN ZFXmlOutputFlags const &ref) const
{
    return (zftrue
            && this->xmlToken == ref.xmlToken
            && this->xmlGlobalLineBeginToken == ref.xmlGlobalLineBeginToken
            && this->xmlElementAddNewLineAtHeadIfNotSingleLine == ref.xmlElementAddNewLineAtHeadIfNotSingleLine
            && this->xmlElementAttributeCountBeforeAddNewLine == ref.xmlElementAttributeCountBeforeAddNewLine
            && this->xmlElementTrimTagIfNoChildren == ref.xmlElementTrimTagIfNoChildren
            && this->xmlElementEndTagAtSameLineIfNoChildElement == ref.xmlElementEndTagAtSameLineIfNoChildElement
            && this->xmlAttributeUseSingleQuote == ref.xmlAttributeUseSingleQuote
        );
}
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFXmlOutputFlags, ZFXmlOutputFlags)

// ============================================================
ZFEXPORT_VAR_READONLY_DEFINE(ZFXmlOutputFlags, ZFXmlOutputFlagsDefault, ZFXmlOutputFlags())

static const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsTrimInit(void)
{
    static ZFXmlOutputFlags d;
    d.xmlElementAttributeCountBeforeAddNewLine = zfindexMax();
    d.xmlElementTrimTagIfNoChildren = zftrue;
    d.xmlToken.xmlNewLineToken.removeAll();
    d.xmlToken.xmlIndentToken.removeAll();
    return d;
}
ZFEXPORT_VAR_READONLY_DEFINE(ZFXmlOutputFlags, ZFXmlOutputFlagsTrim, _ZFP_ZFXmlOutputFlagsTrimInit())

static const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsDetailedInit(void)
{
    static ZFXmlOutputFlags d;
    d.xmlElementAddNewLineAtHeadIfNotSingleLine = zftrue;
    d.xmlElementAttributeCountBeforeAddNewLine = 1;
    d.xmlElementEndTagAtSameLineIfNoChildElement = zftrue;
    return d;
}
ZFEXPORT_VAR_READONLY_DEFINE(ZFXmlOutputFlags, ZFXmlOutputFlagsDetailed, _ZFP_ZFXmlOutputFlagsDetailedInit())

// ============================================================
static zfbool _ZFP_ZFXmlOutputElementUseSingleTag(ZF_IN const ZFXmlItem &element,
                                                  ZF_IN const ZFXmlOutputFlags &flags,
                                                  ZF_IN const ZFXmlVisitData &data)
{
    return (flags.xmlElementTrimTagIfNoChildren && element.xmlChildFirst().xmlIsNull());
}
static zfbool _ZFP_ZFXmlOutputElementAttributeNeedNewLine(ZF_IN const ZFXmlItem &element,
                                                          ZF_IN const ZFXmlOutputFlags &flags,
                                                          ZF_IN const ZFXmlVisitData &data)
{
    if(element.xmlIsNull())
    {
        return zffalse;
    }
    ZFXmlItem xmlAttribute = element.xmlAttributeFirst();
    if(flags.xmlElementAttributeCountBeforeAddNewLine == zfindexMax())
    {
        return zffalse;
    }
    if(flags.xmlElementAttributeCountBeforeAddNewLine == 0)
    {
        return !xmlAttribute.xmlIsNull();
    }
    zfindex xmlAttributeCount = 0;
    while(!xmlAttribute.xmlIsNull())
    {
        ++xmlAttributeCount;
        if(xmlAttributeCount > flags.xmlElementAttributeCountBeforeAddNewLine)
        {
            return zftrue;
        }
        xmlAttribute = xmlAttribute.xmlAttributeNext();
    }
    return zffalse;
}
static zfbool _ZFP_ZFXmlOutputElementChildNeedNewLine(ZF_IN const ZFXmlItem &xmlChild,
                                                      ZF_IN const ZFXmlOutputFlags &flags,
                                                      ZF_IN const ZFXmlVisitData &data)
{
    if(flags.xmlElementTrimTagIfNoChildren && xmlChild.xmlIsNull())
    {
        return zffalse;
    }
    if(!flags.xmlElementEndTagAtSameLineIfNoChildElement)
    {
        return zftrue;
    }

    if(xmlChild.xmlType() != ZFXmlType::e_XmlText)
    {
        return zftrue;
    }
    for(ZFXmlItem t = xmlChild.xmlSiblingNext(); !t.xmlIsNull(); t = t.xmlSiblingNext())
    {
        if(t.xmlType() != ZFXmlType::e_XmlText)
        {
            return zftrue;
        }
    }
    for(ZFXmlItem t = xmlChild.xmlSiblingPrev(); !t.xmlIsNull(); t = t.xmlSiblingPrev())
    {
        if(t.xmlType() != ZFXmlType::e_XmlText)
        {
            return zftrue;
        }
    }

    return zffalse;
}
static zfbool _ZFP_ZFXmlOutputAttributeNeedNewLine(ZF_IN const ZFXmlItem &xmlAttribute,
                                                   ZF_IN const ZFXmlOutputFlags &flags,
                                                   ZF_IN const ZFXmlVisitData &data)
{
    return (flags.xmlElementAttributeCountBeforeAddNewLine == 0
        || (flags.xmlElementAttributeCountBeforeAddNewLine != zfindexMax()
            && data.siblingIndex > 0
            && (data.siblingIndex % flags.xmlElementAttributeCountBeforeAddNewLine) == 0));
}
zfclass _ZFP_I_ZFXmlOutputOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFXmlOutputOwner, ZFObject)

public:
    ZFOutput outputCallback;
    ZFXmlOutputFlags flags;

public:
    ZFMETHOD_INLINE_1(zfbool, onVisit, ZFMP_IN(const ZFXmlVisitData &, data))
    {
        switch(data.xmlItem.xmlType())
        {
            case ZFXmlType::e_XmlElement:
                return this->onVisitXmlElement(data);
            case ZFXmlType::e_XmlAttribute:
                return this->onVisitXmlAttribute(data);
            case ZFXmlType::e_XmlText:
                return this->onVisitXmlText(data);
            case ZFXmlType::e_XmlComment:
                return this->onVisitXmlComment(data);
            case ZFXmlType::e_XmlDocument:
                return this->onVisitXmlDocument(data);
            case ZFXmlType::e_XmlDeclaration:
                return this->onVisitXmlDeclaration(data);
            case ZFXmlType::e_XmlDocType:
                return this->onVisitXmlDocType(data);
            case ZFXmlType::e_XmlPI:
                return this->onVisitXmlPI(data);
            default:
                return zffalse;
        }
    }
private:
    zfbool onVisitXmlElement(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                if((data.depth > 0 && data.siblingIndex > 0) && flags.xmlElementAddNewLineAtHeadIfNotSingleLine
                    && (_ZFP_ZFXmlOutputElementAttributeNeedNewLine(data.xmlItem, flags, data)
                        || _ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem.xmlChildFirst(), flags, data)))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                }
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                if(_ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlElementSingleTagLeft);
                }
                else
                {
                    this->add(flags.xmlToken.xmlElementBeginTagLeft);
                }
                this->add(data.xmlItem.xmlName());
                break;
            case ZFXmlVisitType::e_Exit:
                if(!data.xmlItem.xmlAttributeFirst().xmlIsNull()
                    && _ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    this->add(" ");
                }
                if(_ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlElementSingleTagRight);
                }
                else
                {
                    this->add(flags.xmlToken.xmlElementBeginTagRight);
                }
                break;
            case ZFXmlVisitType::e_ExitChildren:
                if(!_ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    if(_ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem.xmlChildFirst(), flags, data)
                        || _ZFP_ZFXmlOutputElementAttributeNeedNewLine(data.xmlItem, flags, data))
                    {
                        this->add(flags.xmlToken.xmlNewLineToken);
                        this->add(flags.xmlGlobalLineBeginToken);
                        this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                    }
                    this->add(flags.xmlToken.xmlElementEndTagLeft);
                    this->add(data.xmlItem.xmlName());
                    this->add(flags.xmlToken.xmlElementEndTagRight);
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlAttribute(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
            {
                if(_ZFP_ZFXmlOutputAttributeNeedNewLine(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                    this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                }
                else if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(" ");
                }
                this->add(data.xmlItem.xmlName());
                this->add(flags.xmlToken.xmlAttributeEqualTag);
                if(flags.xmlAttributeUseSingleQuote)
                {
                    this->add(flags.xmlToken.xmlAttributeSingleQuoteTagLeft);
                    ZFXmlEscapeCharEncode(this->outputCallback, data.xmlItem.xmlValue());
                    this->add(flags.xmlToken.xmlAttributeSingleQuoteTagRight);
                }
                else
                {
                    this->add(flags.xmlToken.xmlAttributeQuoteTagLeft);
                    ZFXmlEscapeCharEncode(this->outputCallback, data.xmlItem.xmlValue());
                    this->add(flags.xmlToken.xmlAttributeQuoteTagRight);
                }
            }
                break;
            case ZFXmlVisitType::e_Exit:
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlText(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(_ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                    this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                }
                if(data.xmlItem.xmlTextCDATA())
                {
                    this->add(flags.xmlToken.xmlTextCDATATagLeft);
                }
                this->addEncoded(data.xmlItem.xmlValue());
                break;
            case ZFXmlVisitType::e_Exit:
                if(data.xmlItem.xmlTextCDATA())
                {
                    this->add(flags.xmlToken.xmlTextCDATATagRight);
                }
                if(_ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                    this->addIndent(flags.xmlToken.xmlIndentToken, data.depth - 1);
                }
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlComment(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
            {
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlCommentTagLeft);
                this->add(data.xmlItem.xmlValue());
                break;
            }
            case ZFXmlVisitType::e_Exit:
                this->add(flags.xmlToken.xmlCommentTagRight);
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlDocument(ZF_IN const ZFXmlVisitData &data)
    {
        return zftrue;
    }
    zfbool onVisitXmlDeclaration(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlDeclarationTagLeft);
                break;
            case ZFXmlVisitType::e_Exit:
            {
                if(data.xmlItem.xmlAttributeFirst().xmlIsNull())
                {
                    this->add(" ");
                }
                this->add(flags.xmlToken.xmlDeclarationTagRight);
            }
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlDocType(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlDocTypeTagLeft);
                this->add(" ");
                this->add(data.xmlItem.xmlValue());
                break;
            case ZFXmlVisitType::e_Exit:
                this->add(flags.xmlToken.xmlDocTypeTagRight);
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlPI(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlPITagLeft);
                this->add(data.xmlItem.xmlName());
                this->add(" ");
                this->add(data.xmlItem.xmlValue());
                break;
            case ZFXmlVisitType::e_Exit:
                this->add(flags.xmlToken.xmlPITagRight);
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }

private:
    inline void add(const zfchar *s)
    {
        if(s && *s)
        {
            this->outputCallback.execute(s);
        }
    }
    inline void addEncoded(const zfchar *s)
    {
        if(s && *s)
        {
            ZFXmlEscapeCharEncode(this->outputCallback, s);
        }
    }
    void addIndent(const zfchar *xmlIndentToken, zfindex indentLevel = 1)
    {
        if(xmlIndentToken && *xmlIndentToken)
        {
            for(zfindex i = 0; i < indentLevel; ++i)
            {
                this->outputCallback.execute(xmlIndentToken);
            }
        }
    }
};

ZFMETHOD_FUNC_DEFINE_2(ZFXmlVisitCallback, ZFXmlVisitCallbackForOutput,
                       ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()),
                       ZFMP_IN_OPT(const ZFXmlOutputFlags &, flags, ZFXmlOutputFlagsDefault()))
{
    if(!outputCallback.callbackIsValid())
    {
        return ZFCallbackNull();
    }
    _ZFP_I_ZFXmlOutputOwner *owner = zfAlloc(_ZFP_I_ZFXmlOutputOwner);
    owner->outputCallback = outputCallback;
    owner->flags = flags;
    ZFXmlVisitCallback callback = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFXmlOutputOwner, onVisit));
    callback.callbackOwnerObjectRetain();
    zfRelease(owner);
    return callback;
}

// ============================================================
zfclassLikePOD _ZFP_ZFXmlMemoryPoolString
{
private:
    void *_token;
    union {
        zfchar *_s;
        const zfchar *_sInPool;
    } _s;

private:
    _ZFP_ZFXmlMemoryPoolString(ZF_IN const _ZFP_ZFXmlMemoryPoolString &ref);
    _ZFP_ZFXmlMemoryPoolString &operator = (ZF_IN const _ZFP_ZFXmlMemoryPoolString &ref);
public:
    _ZFP_ZFXmlMemoryPoolString(void)
    : _token(zfnull)
    , _s()
    {
    }
    _ZFP_ZFXmlMemoryPoolString(ZF_IN const zfchar *s)
    : _token(zfnull)
    , _s()
    {
        _s._s = zfsCopy(s);
    }
    _ZFP_ZFXmlMemoryPoolString(ZF_IN const zfchar *s, ZF_IN void *token)
    : _token(token)
    , _s()
    {
        _s._sInPool = s;
    }
    ~_ZFP_ZFXmlMemoryPoolString(void)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFXml)->xmlMemoryPoolRelease(_token, _s._sInPool);
        }
        else
        {
            zffree(_s._s);
        }
    }

public:
    void value(ZF_IN const zfchar *value)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFXml)->xmlMemoryPoolRelease(_token, _s._sInPool);
            _token = zfnull;
            _s._s = zfnull;
        }
        zfsChange(_s._s, value);
    }
    void value(ZF_IN const zfchar *value, ZF_IN void *token)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFXml)->xmlMemoryPoolRelease(_token, _s._sInPool);
            _token = zfnull;
            _s._s = zfnull;
        }
        else
        {
            zffree(_s._s);
        }
        _token = token;
        _s._sInPool = value;
    }
    inline const zfchar *value(void) const
    {
        return _s._sInPool;
    }
};
zfclassNotPOD _ZFP_ZFXmlItemPrivate
{
public:
    zfuint refCount;
    ZFXmlTypeEnum xmlType;
    _ZFP_ZFXmlItemPrivate *xmlParent;
    _ZFP_ZFXmlMemoryPoolString xmlName;
    _ZFP_ZFXmlMemoryPoolString xmlValue;

    _ZFP_ZFXmlItemPrivate *xmlAttributeFirst;
    _ZFP_ZFXmlItemPrivate *xmlAttributeLast;
    _ZFP_ZFXmlItemPrivate *xmlChildFirst;
    _ZFP_ZFXmlItemPrivate *xmlChildLast;

    // prev/next attribute or sibling depending on xmlType
    _ZFP_ZFXmlItemPrivate *xmlPrev;
    _ZFP_ZFXmlItemPrivate *xmlNext;

    zfbool xmlAttributeNeedSort;
    zfbool xmlTextCDATA;

public:
    _ZFP_ZFXmlItemPrivate(void)
    : refCount(1)
    , xmlType(ZFXmlType::e_XmlNull)
    , xmlParent(zfnull)
    , xmlName()
    , xmlValue()
    , xmlAttributeFirst(zfnull)
    , xmlAttributeLast(zfnull)
    , xmlChildFirst(zfnull)
    , xmlChildLast(zfnull)
    , xmlPrev(zfnull)
    , xmlNext(zfnull)
    , xmlAttributeNeedSort(zftrue)
    , xmlTextCDATA(zffalse)
    {
    }
    ~_ZFP_ZFXmlItemPrivate(void)
    {
        this->xmlAttributeRemoveAll();
        this->xmlChildRemoveAll();
    }

public:
    void xmlAttributeRemoveAll(void)
    {
        if(this->xmlAttributeFirst)
        {
            _ZFP_ZFXmlItemPrivate *xmlAttribute = this->xmlAttributeFirst;
            this->xmlAttributeFirst = zfnull;
            this->xmlAttributeLast = zfnull;
            while(xmlAttribute != zfnull)
            {
                _ZFP_ZFXmlItemPrivate *xmlAttributeTmp = xmlAttribute;
                xmlAttribute = xmlAttribute->xmlNext;

                xmlAttributeTmp->xmlParent = zfnull;
                xmlAttributeTmp->xmlPrev = zfnull;
                xmlAttributeTmp->xmlNext = zfnull;

                --(xmlAttributeTmp->refCount);
                if(xmlAttributeTmp->refCount == 0)
                {
                    zfdelete(xmlAttributeTmp);
                }
            }
        }
    }
    void xmlChildRemoveAll(void)
    {
        if(this->xmlChildFirst)
        {
            _ZFP_ZFXmlItemPrivate *xmlChild = this->xmlChildFirst;
            this->xmlChildFirst = zfnull;
            this->xmlChildLast = zfnull;
            while(xmlChild != zfnull)
            {
                _ZFP_ZFXmlItemPrivate *xmlChildTmp = xmlChild;
                xmlChild = xmlChild->xmlNext;

                xmlChildTmp->xmlParent = zfnull;
                xmlChildTmp->xmlPrev = zfnull;
                xmlChildTmp->xmlNext = zfnull;

                --(xmlChildTmp->refCount);
                if(xmlChildTmp->refCount == 0)
                {
                    zfdelete(xmlChildTmp);
                }
            }
        }
    }

public:
    void xmlAttributeAttach(ZF_IN _ZFP_ZFXmlItemPrivate *addThis, ZF_IN _ZFP_ZFXmlItemPrivate *beforeThis)
    {
        this->xmlAttributeNeedSort = zftrue;
        ++(addThis->refCount);
        addThis->xmlParent = this;

        if(beforeThis == zfnull)
        {
            addThis->xmlPrev = this->xmlAttributeLast;
            if(this->xmlAttributeLast == zfnull)
            {
                this->xmlAttributeLast = addThis;
            }
            else
            {
                this->xmlAttributeLast->xmlNext = addThis;
                this->xmlAttributeLast = addThis;
            }
            if(this->xmlAttributeFirst == zfnull)
            {
                this->xmlAttributeFirst = addThis;
            }
        }
        else
        {
            if(beforeThis->xmlPrev == zfnull)
            {
                this->xmlAttributeFirst = addThis;
            }
            else
            {
                beforeThis->xmlPrev->xmlNext = addThis;
            }
            addThis->xmlPrev = beforeThis->xmlPrev;
            beforeThis->xmlPrev = addThis;
            addThis->xmlNext = beforeThis;
        }
    }
    void xmlAttributeDetach(ZF_IN _ZFP_ZFXmlItemPrivate *removeThis)
    {
        this->xmlAttributeNeedSort = zftrue;
        --(removeThis->refCount);
        removeThis->xmlParent = zfnull;
        if(this->xmlAttributeFirst == removeThis)
        {
            this->xmlAttributeFirst = removeThis->xmlNext;
        }
        if(this->xmlAttributeLast == removeThis)
        {
            this->xmlAttributeLast = removeThis->xmlPrev;
        }
        if(removeThis->xmlPrev != zfnull)
        {
            removeThis->xmlPrev->xmlNext = removeThis->xmlNext;
        }
        if(removeThis->xmlNext != zfnull)
        {
            removeThis->xmlNext->xmlPrev = removeThis->xmlPrev;
        }
        removeThis->xmlPrev = zfnull;
        removeThis->xmlNext = zfnull;

        if(removeThis->refCount == 0)
        {
            zfdelete(removeThis);
        }
    }
    void xmlChildAttach(ZF_IN _ZFP_ZFXmlItemPrivate *addThis, ZF_IN _ZFP_ZFXmlItemPrivate *beforeThis)
    {
        ++(addThis->refCount);
        addThis->xmlParent = this;

        if(beforeThis == zfnull)
        {
            addThis->xmlPrev = this->xmlChildLast;
            if(this->xmlChildLast == zfnull)
            {
                this->xmlChildLast = addThis;
            }
            else
            {
                this->xmlChildLast->xmlNext = addThis;
                this->xmlChildLast = addThis;
            }
            if(this->xmlChildFirst == zfnull)
            {
                this->xmlChildFirst = addThis;
            }
        }
        else
        {
            if(beforeThis->xmlPrev == zfnull)
            {
                this->xmlChildFirst = addThis;
            }
            else
            {
                beforeThis->xmlPrev->xmlNext = addThis;
            }
            addThis->xmlPrev = beforeThis->xmlPrev;
            beforeThis->xmlPrev = addThis;
            addThis->xmlNext = beforeThis;
        }
    }
    void xmlChildDetach(ZF_IN _ZFP_ZFXmlItemPrivate *removeThis)
    {
        --(removeThis->refCount);
        removeThis->xmlParent = zfnull;
        if(this->xmlChildFirst == removeThis)
        {
            this->xmlChildFirst = removeThis->xmlNext;
        }
        if(this->xmlChildLast == removeThis)
        {
            this->xmlChildLast = removeThis->xmlPrev;
        }
        if(removeThis->xmlPrev != zfnull)
        {
            removeThis->xmlPrev->xmlNext = removeThis->xmlNext;
        }
        if(removeThis->xmlNext != zfnull)
        {
            removeThis->xmlNext->xmlPrev = removeThis->xmlPrev;
        }
        removeThis->xmlPrev = zfnull;
        removeThis->xmlNext = zfnull;

        if(removeThis->refCount == 0)
        {
            zfdelete(removeThis);
        }
    }

public:
    static ZFCompareResult _ZFP_ZFXmlAttributeSortComparer(ZF_IN _ZFP_ZFXmlItemPrivate * const &v0, ZF_IN _ZFP_ZFXmlItemPrivate * const &v1)
    {
        zfint cmpResult = zfscmp(v0->xmlName.value(), v1->xmlName.value());
        if(cmpResult < 0)
        {
            return ZFCompareSmaller;
        }
        else if(cmpResult == 0)
        {
            return ZFCompareTheSame;
        }
        else
        {
            return ZFCompareGreater;
        }
    }
    void xmlAttributeSort(void)
    {
        if(this->xmlAttributeNeedSort)
        {
            this->xmlAttributeNeedSort = zffalse;

            if(this->xmlAttributeFirst != zfnull)
            {
                ZFCoreArrayPOD<_ZFP_ZFXmlItemPrivate *> tmp;
                _ZFP_ZFXmlItemPrivate *xmlAttribute = this->xmlAttributeFirst;
                while(xmlAttribute != zfnull)
                {
                    tmp.add(xmlAttribute);
                    xmlAttribute = xmlAttribute->xmlNext;
                }
                tmp.sort(_ZFP_ZFXmlAttributeSortComparer);

                this->xmlAttributeFirst = tmp[0];
                this->xmlAttributeLast = tmp[tmp.count() - 1];

                for(zfindex i = tmp.count() - 1; i != zfindexMax(); --i)
                {
                    xmlAttribute = tmp[i];
                    if(i + 1 < tmp.count())
                    {
                        xmlAttribute->xmlNext = tmp[i + 1];
                    }
                    else
                    {
                        xmlAttribute->xmlNext = zfnull;
                    }
                    if(i > 0)
                    {
                        xmlAttribute->xmlPrev = tmp[i - 1];
                    }
                    else
                    {
                        xmlAttribute->xmlPrev = zfnull;
                    }
                }
            }
        }
    }
    void xmlAttributeSortRecursively(void)
    {
        this->xmlAttributeSort();

        if(this->xmlType == ZFXmlType::e_XmlElement || this->xmlType == ZFXmlType::e_XmlDocument)
        {
            _ZFP_ZFXmlItemPrivate *xmlChild = this->xmlChildFirst;
            while(xmlChild != zfnull)
            {
                xmlChild->xmlAttributeSortRecursively();
                xmlChild = xmlChild->xmlNext;
            }
        }
    }
};

static void _ZFP_ZFXmlCopyNode(ZF_IN ZFXmlItem &to, ZF_IN const ZFXmlItem &from)
{
    to.xmlName(from.xmlName());
    to.xmlValue(from.xmlValue());
    switch(from.xmlType())
    {
        case ZFXmlType::e_XmlText:
        {
            to.xmlTextCDATA(from.xmlTextCDATA());
            break;
        }
        case ZFXmlType::e_XmlElement:
        case ZFXmlType::e_XmlDeclaration:
        {
            ZFXmlItem fromAttribute = from.xmlAttributeFirst();
            while(!fromAttribute.xmlIsNull())
            {
                to.xmlAttributeAdd(fromAttribute.xmlClone());
                fromAttribute = fromAttribute.xmlAttributeNext();
            }
            break;
        }
        default:
            break;
    }
}
static void _ZFP_ZFXmlCopyTree(ZF_IN ZFXmlItem &to, ZF_IN const ZFXmlItem &from)
{
    _ZFP_ZFXmlCopyNode(to, from);

    switch(from.xmlType())
    {
        case ZFXmlType::e_XmlElement:
        {
            ZFXmlItem fromChild = from.xmlChildFirst();
            while(!fromChild.xmlIsNull())
            {
                to.xmlChildAdd(fromChild.xmlCloneTree());
                fromChild = fromChild.xmlSiblingNext();
            }
            break;
        }
        case ZFXmlType::e_XmlDocument:
        {
            ZFXmlItem fromChild = from.xmlChildFirst();
            while(!fromChild.xmlIsNull())
            {
                to.xmlChildAdd(fromChild.xmlCloneTree());
                fromChild = fromChild.xmlSiblingNext();
            }
            break;
        }
        default:
            break;
    }
}

#define _ZFP_ZFXmlAssertCanHaveAttribute(item) \
    zfCoreAssert((item).xmlType() == ZFXmlType::e_XmlElement || (item).xmlType() == ZFXmlType::e_XmlDeclaration)
#define _ZFP_ZFXmlAssertCanHaveChild(item) \
    zfCoreAssert((item).xmlType() == ZFXmlType::e_XmlElement || (item).xmlType() == ZFXmlType::e_XmlDocument)
#define _ZFP_ZFXmlAssertCanBeAttribute(item) \
    zfCoreAssert((item).xmlType() == ZFXmlType::e_XmlAttribute)
#define _ZFP_ZFXmlAssertCanBeChild(item) \
    zfCoreAssert((item).xmlType() != ZFXmlType::e_XmlAttribute && (item).xmlType() != ZFXmlType::e_XmlNull && (item).xmlType() != ZFXmlType::e_XmlDocument)

// ============================================================
// ZFXmlItem
ZFXmlItem::ZFXmlItem(ZF_IN _ZFP_ZFXmlItemPrivate *ref)
{
    if(ref)
    {
        d = ref;
        ++(d->refCount);
    }
    else
    {
        d = zfnew(_ZFP_ZFXmlItemPrivate);
    }
}

ZFXmlItem::ZFXmlItem(void)
: d(zfnew(_ZFP_ZFXmlItemPrivate))
{
}
ZFXmlItem::ZFXmlItem(ZF_IN ZFXmlTypeEnum xmlType)
: d(zfnew(_ZFP_ZFXmlItemPrivate))
{
    d->xmlType = xmlType;
}
ZFXmlItem::ZFXmlItem(ZF_IN const ZFXmlItem &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFXmlItem::~ZFXmlItem(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}

ZFXmlItem &ZFXmlItem::operator = (ZF_IN const ZFXmlItem &ref)
{
    _ZFP_ZFXmlItemPrivate *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0)
    {
        zfdelete(dTmp);
    }
    return *this;
}
zfbool ZFXmlItem::operator == (ZF_IN const ZFXmlItem &ref) const
{
    return (d == ref.d || (d->xmlType == ZFXmlType::e_XmlNull && ref.d->xmlType == ZFXmlType::e_XmlNull));
}

// ============================================================
void ZFXmlItem::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += ZFXmlType::EnumNameForValue(this->xmlType());
    if(this->xmlName() != zfnull)
    {
        ret += ", name: ";
        ret += this->xmlName();
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

zfindex ZFXmlItem::objectRetainCount(void) const
{
    return d->refCount;
}

// ============================================================
ZFXmlTypeEnum ZFXmlItem::xmlType(void) const
{
    return d->xmlType;
}

ZFXmlItem ZFXmlItem::xmlParent(void) const
{
    return ZFXmlItem(d->xmlParent);
}

void ZFXmlItem::xmlName(ZF_IN const zfchar *name)
{
    zfCoreAssert(!this->xmlIsNull());
    d->xmlName.value(name);
}
const zfchar *ZFXmlItem::xmlName(void) const
{
    return d->xmlName.value();
}

void ZFXmlItem::xmlValue(ZF_IN const zfchar *value)
{
    zfCoreAssert(!this->xmlIsNull());
    d->xmlValue.value(value);
}
const zfchar *ZFXmlItem::xmlValue(void) const
{
    return d->xmlValue.value();
}

void ZFXmlItem::_ZFP_ZFXml_xmlMemoryPool_xmlName(ZF_IN const zfchar *xmlName, ZF_IN void *token)
{
    d->xmlName.value(xmlName, token);
}
void ZFXmlItem::_ZFP_ZFXml_xmlMemoryPool_xmlValue(ZF_IN const zfchar *xmlValue, ZF_IN void *token)
{
    d->xmlValue.value(xmlValue, token);
}

// ============================================================
void ZFXmlItem::xmlVisit(ZF_IN const ZFXmlVisitCallback &callback /* = ZFXmlVisitCallbackForOutput() */) const
{
    if(!callback.callbackIsValid() || this->xmlType() == ZFXmlType::e_XmlNull)
    {
        return ;
    }

    ZFCoreArray<ZFXmlVisitData> datas;
    datas.add(ZFXmlVisitData(*this, ZFXmlVisitType::e_Enter, 0, 0));

    while(datas.count() > 0)
    {
        ZFXmlVisitData data = datas.removeLastAndGet();
        if(data.xmlVisitType == ZFXmlVisitType::e_Enter)
        {
            if(callback.execute(data))
            {
                ZFXmlTypeEnum xmlType = data.xmlItem.xmlType();
                if(xmlType == ZFXmlType::e_XmlElement || xmlType == ZFXmlType::e_XmlDocument)
                {
                    if(data.xmlItem.xmlType() == ZFXmlType::e_XmlElement)
                    {
                        datas.add(ZFXmlVisitData(data.xmlItem, ZFXmlVisitType::e_ExitChildren, data.depth, data.siblingIndex));
                    }

                    ZFXmlItem xmlChild = data.xmlItem.xmlChildLast();
                    if(!xmlChild.xmlIsNull())
                    {
                        zfindex xmlChildDepth = ((data.xmlItem.xmlType() == ZFXmlType::e_XmlDocument)
                            ? data.depth
                            : data.depth + 1);
                        zfindex startIndex = ((zfindex)datas.count()) - 1;
                        do
                        {
                            datas.add(ZFXmlVisitData(xmlChild, ZFXmlVisitType::e_Enter, xmlChildDepth, 0));
                            xmlChild = xmlChild.xmlSiblingPrev();
                        } while(!xmlChild.xmlIsNull());
                        for(zfindex i = ((zfindex)datas.count()) - 1, xmlChildIndex = 0;
                            i != startIndex;
                            --i, ++xmlChildIndex)
                        {
                            datas[i].siblingIndex = xmlChildIndex;
                        }
                    }
                }

                datas.add(ZFXmlVisitData(data.xmlItem, ZFXmlVisitType::e_Exit, data.depth, data.siblingIndex));

                if(xmlType == ZFXmlType::e_XmlElement || xmlType == ZFXmlType::e_XmlDeclaration)
                {
                    ZFXmlItem xmlAttribute = data.xmlItem.xmlAttributeLast();
                    if(!xmlAttribute.xmlIsNull())
                    {
                        zfindex xmlAttributeDepth = data.depth + 1;
                        zfindex startIndex = ((zfindex)datas.count()) - 1;
                        do
                        {
                            datas.add(ZFXmlVisitData(xmlAttribute, ZFXmlVisitType::e_Enter, xmlAttributeDepth, 0));
                            xmlAttribute = xmlAttribute.xmlAttributePrev();
                        } while(!xmlAttribute.xmlIsNull());
                        for(zfindex i = ((zfindex)datas.count()) - 1, xmlChildIndex = 0;
                            i != startIndex;
                            --i, ++xmlChildIndex)
                        {
                            datas[i].siblingIndex = xmlChildIndex;
                        }
                    }
                }
            }
        }
        else
        {
            if(!callback.execute(data))
            {
                for(zfindex i = ((zfindex)datas.count()) - 1; i != zfindexMax(); --i)
                {
                    if(datas[i].depth == data.depth)
                    {
                        datas.remove(i);
                    }
                }
            }
        }
    }
}

// ============================================================
ZFXmlItem ZFXmlItem::xmlClone(void) const
{
    ZFXmlItem ret(this->xmlType());
    if(this->xmlType() != ZFXmlType::e_XmlNull)
    {
        _ZFP_ZFXmlCopyNode(ret, *this);
    }
    return ret;
}
ZFXmlItem ZFXmlItem::xmlCloneTree(void) const
{
    ZFXmlItem ret(this->xmlType());
    if(this->xmlType() != ZFXmlType::e_XmlNull)
    {
        _ZFP_ZFXmlCopyTree(ret, *this);
    }
    return ret;
}

// ============================================================
void ZFXmlItem::xmlAttributeAdd(ZF_IN const ZFXmlItem &addThis,
                                ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */)
{
    _ZFP_ZFXmlAssertCanHaveAttribute(*this);
    _ZFP_ZFXmlAssertCanBeAttribute(addThis);
    zfCoreAssertWithMessage(addThis.d->xmlParent == zfnull, "adding a attribute that already has parent, remove it first");

    if(beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() == ZFXmlType::e_XmlAttribute)
    {
        d->xmlAttributeAttach(addThis.d, beforeThis->d);
    }
    else
    {
        d->xmlAttributeAttach(addThis.d, zfnull);
    }
}
void ZFXmlItem::xmlAttributeAdd(ZF_IN const zfchar *key,
                                ZF_IN const zfchar *value,
                                ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */)
{
    if(!zfsIsEmpty(key))
    {
        ZFXmlItem xmlAttribute(ZFXmlType::e_XmlAttribute);
        xmlAttribute.xmlName(key);
        xmlAttribute.xmlValue(value);
        this->xmlAttributeAdd(xmlAttribute, beforeThis);
    }
}
void ZFXmlItem::xmlAttributeRemove(ZF_IN const ZFXmlItem &removeThis)
{
    if(removeThis.d->xmlParent != d || removeThis.xmlType() != ZFXmlType::e_XmlAttribute)
    {
        return ;
    }
    d->xmlAttributeDetach(removeThis.d);
}
void ZFXmlItem::xmlAttributeRemove(ZF_IN const zfchar *name /* = zfnull */)
{
    if(d->xmlAttributeFirst == zfnull || zfsIsEmpty(name))
    {
        return ;
    }

    for(_ZFP_ZFXmlItemPrivate *xmlAttribute = d->xmlAttributeFirst; xmlAttribute != zfnull; xmlAttribute = xmlAttribute->xmlNext)
    {
        if(zfscmpTheSame(xmlAttribute->xmlName.value(), name))
        {
            d->xmlAttributeDetach(xmlAttribute);
            break;
        }
    }
}
void ZFXmlItem::xmlAttributeRemoveAll(void)
{
    d->xmlAttributeRemoveAll();
}

ZFXmlItem ZFXmlItem::xmlAttribute(ZF_IN const zfchar *name) const
{
    if(d->xmlAttributeFirst == zfnull || zfsIsEmpty(name))
    {
        return ZFXmlItem();
    }

    for(_ZFP_ZFXmlItemPrivate *xmlAttribute = d->xmlAttributeFirst; xmlAttribute != zfnull; xmlAttribute = xmlAttribute->xmlNext)
    {
        if(zfscmpTheSame(xmlAttribute->xmlName.value(), name))
        {
            return ZFXmlItem(xmlAttribute);
        }
    }
    return ZFXmlItem();
}
const zfchar *ZFXmlItem::xmlAttributeValue(ZF_IN const zfchar *name) const
{
    if(d->xmlAttributeFirst == zfnull || zfsIsEmpty(name))
    {
        return zfnull;
    }

    for(_ZFP_ZFXmlItemPrivate *xmlAttribute = d->xmlAttributeFirst; xmlAttribute != zfnull; xmlAttribute = xmlAttribute->xmlNext)
    {
        if(zfscmpTheSame(xmlAttribute->xmlName.value(), name))
        {
            return xmlAttribute->xmlValue.value();
        }
    }
    return zfnull;
}
ZFXmlItem ZFXmlItem::xmlAttributeFirst(void) const
{
    return ZFXmlItem(d->xmlAttributeFirst);
}
ZFXmlItem ZFXmlItem::xmlAttributeLast(void) const
{
    return ZFXmlItem(d->xmlAttributeLast);
}

ZFXmlItem ZFXmlItem::xmlAttributeNext(void) const
{
    return ZFXmlItem((d->xmlType == ZFXmlType::e_XmlAttribute) ? d->xmlNext : zfnull);
}
ZFXmlItem ZFXmlItem::xmlAttributePrev(void) const
{
    return ZFXmlItem((d->xmlType == ZFXmlType::e_XmlAttribute) ? d->xmlPrev : zfnull);
}

// ============================================================
void ZFXmlItem::xmlAttributeSort(void)
{
    d->xmlAttributeSort();
}
void ZFXmlItem::xmlAttributeSortRecursively(void)
{
    d->xmlAttributeSortRecursively();
}

// ============================================================
void ZFXmlItem::xmlChildAdd(ZF_IN const ZFXmlItem &addThis,
                            ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */)
{
    _ZFP_ZFXmlAssertCanHaveChild(*this);
    _ZFP_ZFXmlAssertCanBeChild(addThis);
    zfCoreAssertWithMessage(addThis.d->xmlParent == zfnull, "adding a child that already has parent, remove it first");

    if(beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() != ZFXmlType::e_XmlAttribute)
    {
        d->xmlChildAttach(addThis.d, beforeThis->d);
    }
    else
    {
        d->xmlChildAttach(addThis.d, zfnull);
    }
}
void ZFXmlItem::xmlChildRemove(ZF_IN const ZFXmlItem &removeThis)
{
    if(removeThis.d->xmlParent != d || removeThis.xmlType() == ZFXmlType::e_XmlAttribute)
    {
        return ;
    }
    d->xmlChildDetach(removeThis.d);
}
void ZFXmlItem::xmlChildRemoveAll(void)
{
    d->xmlChildRemoveAll();
}

ZFXmlItem ZFXmlItem::xmlChildFirst(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                   ZF_IN_OPT const ZFXmlItem *afterThis /* = zfnull */) const
{
    _ZFP_ZFXmlItemPrivate *xmlChild = (afterThis != zfnull && afterThis->d->xmlParent == d && afterThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? afterThis->d->xmlNext
        : d->xmlChildFirst;

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(xmlChild);
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name))
        {
            xmlChild = xmlChild->xmlNext;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::xmlChildLast(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                  ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */) const
{
    _ZFP_ZFXmlItemPrivate *xmlChild = (beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? beforeThis->d->xmlPrev
        : d->xmlChildLast;

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(xmlChild);
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name))
        {
            xmlChild = xmlChild->xmlPrev;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::xmlChildElementFirst(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                          ZF_IN_OPT const ZFXmlItem *afterThis /* = zfnull */) const
{
    _ZFP_ZFXmlItemPrivate *xmlChild = (afterThis != zfnull && afterThis->d->xmlParent == d && afterThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? afterThis->d->xmlNext
        : d->xmlChildFirst;

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(xmlChild);
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name) && xmlChild->xmlType != ZFXmlType::e_XmlElement)
        {
            xmlChild = xmlChild->xmlNext;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::xmlChildElementLast(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                         ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */) const
{
    _ZFP_ZFXmlItemPrivate *xmlChild = (beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? beforeThis->d->xmlPrev
        : d->xmlChildLast;

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(xmlChild);
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name) && xmlChild->xmlType != ZFXmlType::e_XmlElement)
        {
            xmlChild = xmlChild->xmlPrev;
        }
        return ZFXmlItem(xmlChild);
    }
}

ZFXmlItem ZFXmlItem::xmlSiblingNext(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(d->xmlType == ZFXmlType::e_XmlAttribute)
    {
        return ZFXmlItem();
    }

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(d->xmlNext);
    }
    else
    {
        _ZFP_ZFXmlItemPrivate *xmlChild = d->xmlNext;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name))
        {
            xmlChild = xmlChild->xmlNext;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::xmlSiblingPrev(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(d->xmlType == ZFXmlType::e_XmlAttribute)
    {
        return ZFXmlItem();
    }

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(d->xmlPrev);
    }
    else
    {
        _ZFP_ZFXmlItemPrivate *xmlChild = d->xmlPrev;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name))
        {
            xmlChild = xmlChild->xmlPrev;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::xmlSiblingElementNext(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(d->xmlType == ZFXmlType::e_XmlAttribute)
    {
        return ZFXmlItem();
    }

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(d->xmlNext);
    }
    else
    {
        _ZFP_ZFXmlItemPrivate *xmlChild = d->xmlNext;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name) && xmlChild->xmlType != ZFXmlType::e_XmlElement)
        {
            xmlChild = xmlChild->xmlNext;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::xmlSiblingElementPrev(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(d->xmlType == ZFXmlType::e_XmlAttribute)
    {
        return ZFXmlItem();
    }

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(d->xmlPrev);
    }
    else
    {
        _ZFP_ZFXmlItemPrivate *xmlChild = d->xmlPrev;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name) && xmlChild->xmlType != ZFXmlType::e_XmlElement)
        {
            xmlChild = xmlChild->xmlPrev;
        }
        return ZFXmlItem(xmlChild);
    }
}

// ============================================================
void ZFXmlItem::xmlTextCDATA(ZF_IN zfbool xmlTextCDATA)
{
    zfCoreAssert(this->xmlType() == ZFXmlType::e_XmlText);
    d->xmlTextCDATA = xmlTextCDATA;
}
zfbool ZFXmlItem::xmlTextCDATA(void) const
{
    return d->xmlTextCDATA;
}

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFXmlItem, ZFXmlItem, {
        v = ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(src, srcLen);
        return !v.xmlIsNull();
    }, {
        return ZFXmlItemToString(s, v, ZFXmlOutputFlagsTrim());
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, objectInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, zfindex, objectRetainCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlTypeEnum, xmlType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, zfbool, xmlIsNull)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlParent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, xmlName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, const zfchar *, xmlName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, xmlValue, ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, const zfchar *, xmlValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, xmlVisit, ZFMP_IN(const ZFXmlVisitCallback &, callback))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlClone)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlCloneTree)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, void, xmlAttributeAdd, ZFMP_IN(const ZFXmlItem &, addThis), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFXmlItem, void, xmlAttributeAdd, ZFMP_IN(const zfchar *, key), ZFMP_IN(const zfchar *, value), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, xmlAttributeRemove, ZFMP_IN(const ZFXmlItem &, removeThis))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, xmlAttributeRemove, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, void, xmlAttributeRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, xmlAttribute, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, const zfchar *, xmlAttributeValue, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlAttributeFirst)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlAttributeLast)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlAttributeNext)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlAttributePrev)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, void, xmlAttributeSort)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, void, xmlAttributeSortRecursively)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, void, xmlChildAdd, ZFMP_IN(const ZFXmlItem &, addThis), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, xmlChildRemove, ZFMP_IN(const ZFXmlItem &, removeThis))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, void, xmlChildRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem, xmlChildFirst, ZFMP_IN_OPT(const zfchar *, name, zfnull), ZFMP_IN_OPT(const ZFXmlItem *, afterThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem, xmlChildLast, ZFMP_IN_OPT(const zfchar *, name, zfnull), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem, xmlChildElementFirst, ZFMP_IN_OPT(const zfchar *, name, zfnull), ZFMP_IN_OPT(const ZFXmlItem *, afterThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem, xmlChildElementLast, ZFMP_IN_OPT(const zfchar *, name, zfnull), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, xmlSiblingNext, ZFMP_IN_OPT(const zfchar *, name, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, xmlSiblingPrev, ZFMP_IN_OPT(const zfchar *, name, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, xmlSiblingElementNext, ZFMP_IN_OPT(const zfchar *, name, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, xmlSiblingElementPrev, ZFMP_IN_OPT(const zfchar *, name, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, xmlTextCDATA, ZFMP_IN(zfbool, xmlTextCDATA))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, zfbool, xmlTextCDATA)

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlElement) {return ZFXmlItem(ZFXmlType::e_XmlElement);}
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlAttribute) {return ZFXmlItem(ZFXmlType::e_XmlAttribute);}
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlText) {return ZFXmlItem(ZFXmlType::e_XmlText);}
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlComment) {return ZFXmlItem(ZFXmlType::e_XmlComment);}
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlDocument) {return ZFXmlItem(ZFXmlType::e_XmlDocument);}
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlDeclaration) {return ZFXmlItem(ZFXmlType::e_XmlDeclaration);}
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlDocType) {return ZFXmlItem(ZFXmlType::e_XmlDocType);}
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlPI) {return ZFXmlItem(ZFXmlType::e_XmlPI);}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFXmlItem, ZFXmlItemFromInput,
                       ZFMP_IN(const ZFInput &, callback))
{
    return ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(callback);
}
ZFMETHOD_FUNC_DEFINE_2(ZFXmlItem, ZFXmlItemFromString,
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, size, zfindexMax()))
{
    return ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(src, size);
}

ZFMETHOD_FUNC_DEFINE_1(ZFXmlItem, ZFXmlParseFirstElement,
                       ZFMP_IN(const ZFInput &, callback))
{
    return ZFXmlItem(ZFXmlItemFromInput(callback).xmlChildElementFirst());
}
ZFMETHOD_FUNC_DEFINE_2(ZFXmlItem, ZFXmlParseFirstElement,
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, size, zfindexMax()))
{
    return ZFXmlItem(ZFXmlItemFromString(src, size).xmlChildElementFirst());
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFXmlItemToOutput,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFXmlItem &, xmlItem),
                       ZFMP_IN_OPT(const ZFXmlOutputFlags &, outputFlags, ZFXmlOutputFlagsDefault()))
{
    if(!output.callbackIsValid() || xmlItem.xmlIsNull())
    {
        return zffalse;
    }
    xmlItem.xmlVisit(ZFXmlVisitCallbackForOutput(output, outputFlags));
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFXmlItemToString,
                       ZFMP_IN_OUT(zfstring &, ret),
                       ZFMP_IN(const ZFXmlItem &, xmlItem),
                       ZFMP_IN(const ZFXmlOutputFlags &, outputFlags))
{
    return ZFXmlItemToOutput(ZFOutputForString(ret), xmlItem, outputFlags);
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFXmlItemToString,
                       ZFMP_IN(const ZFXmlItem &, xmlItem),
                       ZFMP_IN(const ZFXmlOutputFlags &, outputFlags))
{
    zfstring ret;
    ZFXmlItemToString(ret, xmlItem, outputFlags);
    return ret;
}

// ============================================================
// escape chars
ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharEncode,
                       ZFMP_OUT(zfstring &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFXmlEscapeCharEncode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharEncode,
                       ZFMP_OUT(const ZFOutput &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFPROTOCOL_ACCESS(ZFXml)->xmlEscapeCharEncode(dst, src, count);
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharDecode,
                       ZFMP_OUT(zfstring &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFXmlEscapeCharDecode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharDecode,
                       ZFMP_OUT(const ZFOutput &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFPROTOCOL_ACCESS(ZFXml)->xmlEscapeCharDecode(dst, src, count);
}

ZF_NAMESPACE_GLOBAL_END

