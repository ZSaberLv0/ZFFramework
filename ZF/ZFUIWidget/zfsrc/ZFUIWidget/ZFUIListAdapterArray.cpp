#include "ZFUIListAdapterArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListAdapterArray)

zfbool ZFUIListAdapterArray::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                             ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                             ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->cellRemoveAll();

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(!zfscmpTheSame(category, ZFSerializableKeyword_ZFUIListAdapterArray_cell)) {continue;}

        zfautoObject element;
        if(!ZFObjectFromData(element, categoryData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        if(element == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                "null cell");
            return zffalse;
        }
        if(!element.toObject()->classData()->classIsTypeOf(ZFUIListCell::ClassData()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                "%s not type of %s",
                element.toObject()->objectInfoOfInstance().cString(), ZFUIListCell::ClassData()->classNameFull());
            return zffalse;
        }
        this->cellAdd(element);

        categoryData.resolveMark();
    }
    return zftrue;
}
zfbool ZFUIListAdapterArray::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                           ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                           ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull || ref->cellCount() == 0)
    {
        for(zfindex i = 0, count = this->cellCount(); i < count; ++i)
        {
            ZFSerializableData cellData;
            if(!ZFObjectToData(cellData, this->cellAtIndex(i), outErrorHint))
            {
                return zffalse;
            }
            cellData.category(ZFSerializableKeyword_ZFUIListAdapterArray_cell);
            serializableData.elementAdd(cellData);
        }
    }
    else
    {
        zfbool cellAllSame = zftrue;
        if(this->cellCount() != ref->cellCount())
        {
            cellAllSame = zffalse;
        }
        else
        {
            for(zfindex i = 0; i < this->cellCount(); ++i)
            {
                if(!ZFObjectCompare(this->cellAtIndex(i).toObject(), ref->cellAtIndex(i).toObject()))
                {
                    cellAllSame = zffalse;
                }
            }
        }
        if(!cellAllSame)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                "cell list mismatch, this: %s, ref: %s",
                this->objectInfoOfInstance().cString(), ref->objectInfoOfInstance().cString());
            return zffalse;
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

