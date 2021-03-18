#include "ZFImpl_sys_iOS_ZF_impl.h"

#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_iOS

#include <objc/runtime.h>
#include <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFImpl_sys_iOS_zfstringFromNSString(ZF_IN_OUT zfstring &result, ZF_IN NSString *nativeString)
{
    if(nativeString != nil)
    {
        const char *tmp = nativeString.UTF8String;
        if(tmp != zfnull)
        {
            result += tmp;
        }
    }
}
NSString *ZFImpl_sys_iOS_zfstringToNSString(ZF_IN const zfchar *s)
{
    if(s == zfnull)
    {
        return [NSString string];
    }
    else
    {
        return [NSString stringWithCString:s encoding:NSUTF8StringEncoding];
    }
}

static zfbool _ZFP_ZFImpl_sys_iOS_classIsSysClass(ZF_IN Class cls)
{
    return (zffalse
        || cls == [NSObject class]
        || cls == [UIResponder class]
        || cls == [UIView class]
        || cls == [UIViewController class]
        );
}
NSString *ZFImpl_sys_iOS_objectInfoNative(ZF_IN id nativeObject,
                                          ZF_IN_OPT NSArray *ignorePropertyNames /* = nil */)
{
    if(nativeObject == nil)
    {
        return @"<null>";
    }

    NSString *ret = [NSString stringWithFormat:@"<%@ %08X", [nativeObject class], (unsigned int)[nativeObject hash]];

    for(Class cls = [nativeObject class];
        cls != nil && !_ZFP_ZFImpl_sys_iOS_classIsSysClass(cls);
        cls = [cls superclass])
    {
        unsigned int propertyCount = 0;
        objc_property_t *allProperty = class_copyPropertyList(cls, &propertyCount);
        for(unsigned int i = 0; i < propertyCount; ++i)
        {
            char *propertyAttrName = property_copyAttributeValue(allProperty[i], "V");

            BOOL skipped = (propertyAttrName == nil);
            if(propertyAttrName != nil) {free(propertyAttrName);}
            if(skipped)
            {
                continue;
            }

            NSString *propertyName = [NSString stringWithCString:property_getName(allProperty[i]) encoding:NSUTF8StringEncoding];
            if([ignorePropertyNames count] > 0 && [ignorePropertyNames indexOfObject:propertyName] != NSNotFound)
            {
                continue;
            }
            ret = [ret stringByAppendingFormat:@" \"%@\"=\"%@\"", propertyName, [nativeObject valueForKey:propertyName]];
        }
        free(allProperty);
    }

    ret = [ret stringByAppendingString:@">"];

    return ret;
}
zfstring ZFImpl_sys_iOS_objectInfo(ZF_IN id nativeObject,
                                   ZF_IN_OPT ZFCoreArray<zfstring> *ignorePropertyNames /* = zfnull */)
{
    NSArray *ignorePropertyNamesNative = nil;
    if(ignorePropertyNames != zfnull)
    {
        NSMutableArray *ignorePropertyNamesNativeTmp = [NSMutableArray new];
        for(zfindex i = 0; i < ignorePropertyNames->count(); ++i)
        {
            [ignorePropertyNamesNativeTmp addObject:ZFImpl_sys_iOS_zfstringToNSString(ignorePropertyNames->get(i).cString())];
        }
        ignorePropertyNamesNative = ignorePropertyNamesNativeTmp;
    }
    NSString *tmp = ZFImpl_sys_iOS_objectInfoNative(nativeObject, ignorePropertyNamesNative);
    zfstring ret;
    ZFImpl_sys_iOS_zfstringFromNSString(ret, tmp);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS

