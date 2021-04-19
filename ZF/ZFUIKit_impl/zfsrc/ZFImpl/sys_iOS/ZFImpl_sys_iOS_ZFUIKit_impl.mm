#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"

#if ZF_ENV_sys_iOS
ZF_NAMESPACE_GLOBAL_BEGIN

// convert utility
void ZFImpl_sys_iOS_ZFUIPointToCGPointT(ZF_OUT CGPoint &ret, ZF_IN const ZFUIPoint &point)
{
    ret.x = point.x;
    ret.y = point.y;
}
void ZFImpl_sys_iOS_ZFUIPointFromCGPointT(ZF_OUT ZFUIPoint &ret, ZF_IN const CGPoint &cgPoint)
{
    ret.x = zfmRound(cgPoint.x);
    ret.y = zfmRound(cgPoint.y);
}
void ZFImpl_sys_iOS_ZFUISizeToCGSizeT(ZF_OUT CGSize &ret, ZF_IN const ZFUISize &size)
{
    ret.width = size.width;
    ret.height = size.height;
}
void ZFImpl_sys_iOS_ZFUISizeFromCGSizeT(ZF_OUT ZFUISize &ret, ZF_IN const CGSize &cgSize)
{
    ret.width = zfmRound(cgSize.width);
    ret.height = zfmRound(cgSize.height);
}
void ZFImpl_sys_iOS_ZFUIRectToCGRectT(ZF_OUT CGRect &ret, ZF_IN const ZFUIRect &rect)
{
    ret.origin.x = rect.x;
    ret.origin.y = rect.y;
    ret.size.width = rect.width;
    ret.size.height = rect.height;
}
void ZFImpl_sys_iOS_ZFUIRectFromCGRectT(ZF_OUT ZFUIRect &ret, ZF_IN const CGRect &cgRect)
{
    ret.x = zfmRound(cgRect.origin.x);
    ret.y = zfmRound(cgRect.origin.y);
    ret.width = zfmRound(cgRect.size.width);
    ret.height = zfmRound(cgRect.size.height);
}

void ZFImpl_sys_iOS_ZFUIMarginToUIEdgeInsetsT(ZF_OUT UIEdgeInsets &ret, ZF_IN const ZFUIMargin &margin)
{
    ret.left = margin.left;
    ret.top = margin.top;
    ret.right = margin.right;
    ret.bottom = margin.bottom;
}
void ZFImpl_sys_iOS_ZFUIMarginFromUIEdgeInsetsT(ZF_OUT ZFUIMargin &ret, ZF_IN const UIEdgeInsets &edgeInsets)
{
    ret.left = zfmRound(edgeInsets.left);
    ret.top = zfmRound(edgeInsets.top);
    ret.right = zfmRound(edgeInsets.right);
    ret.bottom = zfmRound(edgeInsets.bottom);
}

UIColor *ZFImpl_sys_iOS_ZFUIColorToUIColor(ZF_IN const ZFUIColor &color)
{
    return [UIColor colorWithRed:ZFUIColorGetR(color) green:ZFUIColorGetG(color) blue:ZFUIColorGetB(color) alpha:ZFUIColorGetA(color)];
}
void ZFImpl_sys_iOS_ZFUIColorFromUIColorT(ZF_OUT ZFUIColor &ret, ZF_IN UIColor *uiColor)
{
    zffloat argb[4] = {0};
    ZFImpl_sys_iOS_UIColorToARGB(argb, uiColor);
    ret = ZFUIColorMake(argb[1], argb[2], argb[3], argb[0]);
}

// ============================================================
// util method
zfbool ZFImpl_sys_iOS_UIColorToARGB(ZF_IN_OUT zffloat *pARGB, ZF_IN UIColor *color)
{
    for(zfindex i = 0; i < 4; ++i)
    {
        pARGB[i] = 0;
    }
    if(color == nil)
    {
        return zftrue;
    }
    zfindex numComponents = CGColorGetNumberOfComponents(color.CGColor);
    if(numComponents == 4)
    {
        const CGFloat *buf = CGColorGetComponents([color CGColor]);
        pARGB[0] = (zffloat)buf[3];
        for(zfindex i = 1; i < 4; ++i)
        {
            pARGB[i] = (zffloat)buf[i - 1];
        }
    }
    else
    {
        CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();
        unsigned char buf[5] = {0};
        CGContextRef context = CGBitmapContextCreate(
            &buf,
            1,
            1,
            8,
            4,
            rgbColorSpace,
            kCGImageAlphaNoneSkipLast);
        CGContextSetFillColorWithColor(context, [color CGColor]);
        CGContextFillRect(context, CGRectMake(0, 0, 1, 1));
        CGContextRelease(context);
        CGColorSpaceRelease(rgbColorSpace);
        for (zfindex i = 0; i < 4; ++i)
        {
            pARGB[i] = (zfuint)buf[i] / 255.0f;
        }
    }

    return zftrue;
}
void ZFImpl_sys_iOS_UIColorInfoT(ZF_OUT zfstring &ret, ZF_IN UIColor *color)
{
    if(color != nil)
    {
        ZFUIColor t = ZFImpl_sys_iOS_ZFUIColorFromUIColor(color);
        ZFUIColorToString(ret, t);
    }
}

static void _ZFP_ZFImpl_sys_iOS_viewTreePrint_recursive(ZF_IN_OUT zfstring &s, UIView *view, zfindex depth, zfindex siblingIndex)
{
    zfstringAppend(s, "|%2d ", siblingIndex);
    for(zfindex i = 0; i < depth; ++i)
    {
        s += "| ";
    }

    NSString *viewInfo = [NSString stringWithFormat:@"<%@ %08X>", [view class], (zfuint)[view hash]];
    viewInfo = [viewInfo stringByAppendingFormat:@" (%d, %d, %d, %d)",
        (int)view.frame.origin.x, (int)view.frame.origin.y, (int)view.frame.size.width, (int)view.frame.size.height];

#if 0 // test
#define _TEST_PROPERTY
    if([view respondsToSelector:@selector(_TEST_PROPERTY)])
    {
        id value = [view performSelector:@selector(_TEST_PROPERTY)];
        if(value != nil)
        {
            viewInfo = [viewInfo stringByAppendingFormat:@" %s:%@", ZFM_TOSTRING(_TEST_PROPERTY), value];
        }
    }
#endif // test

#if 1 // scroll content
    if([view isKindOfClass:[UIScrollView class]])
    {
        UIScrollView *scrollView = (UIScrollView *)view;
        zfstring info = zfstringWithFormat("(%d, %d, %d, %d)",
            (zfint)scrollView.contentOffset.x, (zfint)scrollView.contentOffset.y, (zfint)scrollView.contentSize.width, (zfint)scrollView.contentSize.height);
        viewInfo = [viewInfo stringByAppendingFormat:@" scroll:%@", [NSString stringWithUTF8String:info]];
    }
#endif // scroll content

#if 1 // bg
    if(view.backgroundColor != nil)
    {
        zfstring colorInfo = ZFImpl_sys_iOS_UIColorInfo(view.backgroundColor);
        viewInfo = [viewInfo stringByAppendingFormat:@" bg:%@", [NSString stringWithUTF8String:colorInfo]];
    }
#endif // bg

#if 1 // hidden
    if(view.hidden)
    {
        viewInfo = [viewInfo stringByAppendingString:@" hidden"];
    }
#endif // hidden

#if 1 // UIDisabled
    if(!view.userInteractionEnabled)
    {
        viewInfo = [viewInfo stringByAppendingString:@" UIDisabled"];
    }
#endif // UIDisabled

#if 1 // text
    if([view respondsToSelector:@selector(text)])
    {
        id value = [view performSelector:@selector(text)];
        if(value != nil)
        {
            viewInfo = [viewInfo stringByAppendingFormat:@" text:\"%@\"", value];
        }
    }
#endif // text

#if 1 // title
    if([view respondsToSelector:@selector(currentTitle)])
    {
        id value = [view performSelector:@selector(currentTitle)];
        if(value != nil)
        {
            viewInfo = [viewInfo stringByAppendingFormat:@" currentTitle:%@", value];
        }
    }
    else if([view respondsToSelector:@selector(titleLabel)])
    {
        id value = ((UILabel *)[view performSelector:@selector(titleLabel)]).text;
        if(value != nil)
        {
            viewInfo = [viewInfo stringByAppendingFormat:@" titleLabel:%@", value];
        }
    }
#endif // title

    s += [viewInfo UTF8String];
    s += "\n";

    NSArray *subviews = view.subviews;
    for(NSUInteger i = 0; i < [subviews count]; ++i)
    {
        _ZFP_ZFImpl_sys_iOS_viewTreePrint_recursive(s, [subviews objectAtIndex:i], depth + 1, i);
    }
}
void ZFImpl_sys_iOS_viewTreePrintT(ZF_OUT zfstring &ret, ZF_IN UIView *view)
{
    ret += "==================== UIView tree begin ====================\n";
    if(view != nil)
    {
        @autoreleasepool {
            _ZFP_ZFImpl_sys_iOS_viewTreePrint_recursive(ret, view, 0, 0);
        }
    }
    ret += "==================== UIView tree  end  ====================\n";
}

#if ZF_ENV_DEBUG && 0
    ZF_GLOBAL_INITIALIZER_INIT(ZFImpl_sys_iOS_autoPrintViewTree)
    {
        if(!ZFProtocolIsAvailable("ZFUIView"))
        {
            return ;
        }
        ZFLISTENER_LOCAL(windowOnPause, {
            ZFUISysWindow *sysWindow = listenerData.sender<ZFUISysWindow *>();
            zfstring s;
            ZFImpl_sys_iOS_viewTreePrintT(s, (__bridge UIView *)sysWindow->rootView()->nativeView());
            zfLogTrimT() << s;
        })
        this->windowOnPauseListener = windowOnPause;
        ZFGlobalObserver().observerAdd(
            ZFUISysWindow::EventSysWindowOnPause(), this->windowOnPauseListener);
    }
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_iOS_autoPrintViewTree)
    {
        ZFGlobalObserver().observerRemove(
            ZFUISysWindow::EventSysWindowOnPause(), this->windowOnPauseListener);
    }
    private:
        ZFListener windowOnPauseListener;
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_iOS_autoPrintViewTree)
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

