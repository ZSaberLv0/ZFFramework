#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextEdit.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit : UITextField<UITextFieldDelegate>
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUITextEdit) *impl;
@property (nonatomic, assign) ZFUITextEdit *ownerZFUITextEdit;
@property (nonatomic, assign) zfstring lastText;
@property (nonatomic, assign) zfbool textOverrideFlag;

@property (nonatomic, strong) NSString *fontName;
@property (nonatomic, assign) zfint textSize;

- (void)_textFieldTextChanged:(UITextField *)textField;
@end

@implementation _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit

// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];

    self.autoresizesSubviews = NO;
    self.backgroundColor = [UIColor clearColor];

    self.fontName = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontName;

    self.delegate = self;
    [self addTarget:self action:@selector(_textFieldTextChanged:) forControlEvents:UIControlEventEditingChanged];

    return self;
}
- (void)dealloc
{
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    [self removeTarget:self action:@selector(_textFieldTextChanged:) forControlEvents:UIControlEventEditingChanged];
    self.fontName = nil;
}

- (void)setTextSize:(zfint)newTextSize ZFImpl_sys_iOS_overrideProperty
{
    if(self->_textSize != newTextSize)
    {
        self->_textSize = newTextSize;
        UIFont *newFont = [UIFont fontWithName:self.fontName size:newTextSize];
        self.font = newFont;
    }
}

- (void)setSelectedTextRange:(UITextRange *)selectedTextRange ZFImpl_sys_iOS_overrideProperty
{
    [super setSelectedTextRange:selectedTextRange];
    [self _ZFP_textSelectRangeNotifyChange];
}
- (void)_ZFP_textSelectRangeNotifyChange
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextSelectRangeChange(self.ownerZFUITextEdit);
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditBegin(self.ownerZFUITextEdit);
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextSelectRangeChange(self.ownerZFUITextEdit);
}
- (void)textFieldDidEndEditing:(UITextField *)textField
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditEnd(self.ownerZFUITextEdit);
}
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextReturnClicked(self.ownerZFUITextEdit);
    return YES;
}

- (void)_textFieldTextChanged:(UITextField *)textField
{
    if(self.textOverrideFlag)
    {
        return ;
    }

    zfstring text = textField.text.UTF8String;
    if(textField.markedTextRange != nil)
    {
        NSInteger start = [textField offsetFromPosition:textField.beginningOfDocument toPosition:textField.markedTextRange.start];
        NSInteger count = [textField offsetFromPosition:textField.markedTextRange.start toPosition:textField.markedTextRange.end];
        NSMutableString *s = [NSMutableString stringWithString:textField.text];
        [s replaceCharactersInRange:NSMakeRange(start, count) withString:@""];
        text = s.UTF8String;
    }

    if(self.lastText.compare(text) == 0)
    {
        return ;
    }

    if(!ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyCheckTextShouldChange(self.ownerZFUITextEdit, text))
    {
        self.textOverrideFlag = zftrue;
        textField.text = [NSString stringWithUTF8String:text.cString()];
        self.textOverrideFlag = zffalse;
        return ;
    }
    self.lastText = text;
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextChange(self.ownerZFUITextEdit, self.lastText);
    [self _ZFP_textSelectRangeNotifyChange];
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextEditImpl_sys_iOS, ZFUITextEdit, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UITextField")
public:
    virtual void *nativeTextEditCreate(ZF_IN ZFUITextEdit *textEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = [_ZFP_ZFUITextEditImpl_sys_iOS_TextEdit new];
        nativeImplView.ownerZFUITextEdit = textEdit;
        return (__bridge_retained void *)nativeImplView;
    }
    virtual void nativeTextEditDestroy(ZF_IN ZFUITextEdit *textEdit,
                                       ZF_IN void *nativeTextEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *tmp = (__bridge_transfer _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)nativeTextEdit;
        tmp = nil;
    }

// ============================================================
// properties
public:
    virtual void textEditEnable(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN zfbool textEditEnable)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.enabled = textEditEnable;
    }
    virtual void textEditSecure(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN zfbool textEditSecured)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.secureTextEntry = textEditSecured;
    }
    virtual void textEditKeyboardType(ZF_IN ZFUITextEdit *textEdit,
                                      ZF_IN ZFUITextEditKeyboardTypeEnum textEditKeyboardType)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        switch(textEditKeyboardType)
        {
            case ZFUITextEditKeyboardType::e_Normal:
                nativeImplView.keyboardType = UIKeyboardTypeDefault;
                break;
            case ZFUITextEditKeyboardType::e_CharBased:
                nativeImplView.keyboardType = UIKeyboardTypeASCIICapable;
                break;
            case ZFUITextEditKeyboardType::e_PhonePad:
                nativeImplView.keyboardType = UIKeyboardTypePhonePad;
                break;
            case ZFUITextEditKeyboardType::e_NumberPad:
                nativeImplView.keyboardType = UIKeyboardTypeNumberPad;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    virtual void textEditKeyboardReturnType(ZF_IN ZFUITextEdit *textEdit,
                                            ZF_IN ZFUITextEditKeyboardReturnTypeEnum textEditKeyboardReturnType)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        switch(textEditKeyboardReturnType)
        {
            case ZFUITextEditKeyboardReturnType::e_Normal:
                nativeImplView.returnKeyType = UIReturnKeyDefault;
                break;
            case ZFUITextEditKeyboardReturnType::e_Next:
                nativeImplView.returnKeyType = UIReturnKeyNext;
                break;
            case ZFUITextEditKeyboardReturnType::e_Search:
                nativeImplView.returnKeyType = UIReturnKeySearch;
                break;
            case ZFUITextEditKeyboardReturnType::e_Done:
                nativeImplView.returnKeyType = UIReturnKeyDone;
                break;
            case ZFUITextEditKeyboardReturnType::e_Go:
                nativeImplView.returnKeyType = UIReturnKeyGo;
                break;
            case ZFUITextEditKeyboardReturnType::e_Send:
                nativeImplView.returnKeyType = UIReturnKeySend;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }

    virtual void textSelectRange(ZF_IN ZFUITextEdit *textEdit, ZF_OUT ZFIndexRange &textSelectRange)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        UITextRange *rangeImpl = nativeImplView.selectedTextRange;
        if(rangeImpl == nil)
        {
            textSelectRange = ZFIndexRangeZero();
            return ;
        }
        NSInteger start = [nativeImplView offsetFromPosition:nativeImplView.beginningOfDocument toPosition:rangeImpl.start];
        NSInteger end = [nativeImplView offsetFromPosition:nativeImplView.beginningOfDocument toPosition:rangeImpl.end];
        textSelectRange.start = start;
        textSelectRange.count = end - start;
    }
    virtual void textSelectRange(ZF_IN ZFUITextEdit *textEdit, ZF_IN const ZFIndexRange &textSelectRange)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        UITextPosition *start = [nativeImplView positionFromPosition:nativeImplView.beginningOfDocument offset:textSelectRange.start];
        UITextPosition *end = [nativeImplView positionFromPosition:start offset:textSelectRange.count];
        [nativeImplView setSelectedTextRange:[nativeImplView textRangeFromPosition:start toPosition:end]];
    }

public:
    virtual void text(ZF_IN ZFUITextEdit *textEdit,
                      ZF_IN const zfchar *text)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.text = ZFImpl_sys_iOS_zfstringToNSString(text);
    }
    virtual void textAppearance(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN ZFUITextAppearanceEnum const &textAppearance)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        switch(textAppearance)
        {
            case ZFUITextAppearance::e_Normal:
                nativeImplView.fontName = [UIFont systemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case ZFUITextAppearance::e_Bold:
                nativeImplView.fontName = [UIFont boldSystemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case ZFUITextAppearance::e_Italic:
                nativeImplView.fontName = [UIFont italicSystemFontOfSize:[UIFont systemFontSize]].fontName;
                break;
            case ZFUITextAppearance::e_BoldItalic:
                nativeImplView.fontName = @"Helvetica-BoldOblique";
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    virtual void textAlign(ZF_IN ZFUITextEdit *textEdit,
                           ZF_IN ZFUIAlignFlags const &textAlign)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        if(ZFBitTest(textAlign, ZFUIAlign::e_LeftInner))
        {
            nativeImplView.textAlignment = NSTextAlignmentLeft;
        }
        else if(ZFBitTest(textAlign, ZFUIAlign::e_RightInner))
        {
            nativeImplView.textAlignment = NSTextAlignmentRight;
        }
        else if(textAlign == ZFUIAlign::e_Center)
        {
            nativeImplView.textAlignment = NSTextAlignmentCenter;
        }
        else
        {
            nativeImplView.textAlignment = NSTextAlignmentLeft;
        }
    }
    virtual void textColor(ZF_IN ZFUITextEdit *textEdit,
                           ZF_IN ZFUIColor const &textColor)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.textColor = ZFImpl_sys_iOS_ZFUIColorToUIColor(textColor);
    }
    virtual void textShadowColor(ZF_IN ZFUITextEdit *textEdit,
                                 ZF_IN ZFUIColor const &textShadowColor)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.layer.shadowColor = ZFImpl_sys_iOS_ZFUIColorToUIColor(textShadowColor).CGColor;
    }
    virtual void textShadowOffset(ZF_IN ZFUITextEdit *textEdit,
                                  ZF_IN ZFUISize const &textShadowOffset)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.layer.shadowOffset = ZFImpl_sys_iOS_ZFUISizeToCGSize(textShadowOffset);
    }
    virtual void textSize(ZF_IN ZFUITextEdit *textEdit,
                          ZF_IN zfint textSize)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        nativeImplView.textSize = textSize;
    }

// ============================================================
// layout
public:
    virtual ZFUISize measureNativeTextEdit(ZF_IN ZFUITextEdit *textEdit,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zfint textSize)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        zfint textSizeSaved = nativeImplView.textSize;
        nativeImplView.textSize = textSize;
        CGSize sizeTmp = ZFImpl_sys_iOS_ZFUISizeToCGSize(sizeHint);
        if(sizeTmp.width <= 0)
        {
            sizeTmp.width = 30000;
        }
        if(sizeTmp.height <= 0)
        {
            sizeTmp.height = 30000;
        }
        sizeTmp = [nativeImplView sizeThatFits:sizeTmp];
        nativeImplView.textSize = textSizeSaved;
        return ZFImpl_sys_iOS_ZFUISizeFromCGSize(sizeTmp);
    }

public:
    virtual void textEditBegin(ZF_IN ZFUITextEdit *textEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        [nativeImplView becomeFirstResponder];
    }
    virtual void textEditEnd(ZF_IN ZFUITextEdit *textEdit)
    {
        _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *nativeImplView = (__bridge _ZFP_ZFUITextEditImpl_sys_iOS_TextEdit *)textEdit->nativeImplView();
        [nativeImplView resignFirstResponder];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextEditImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUITextEditImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

