/**
 * @page DocTag_Tutorial_HelloWorldWithUI Hello world with UI
 *
 * time to start a quick hello world with UI elements:
 * @code
 *   // include ZFUIKit.h for UI module of ZFFramework
 *   // you may need to setup your project's include path first
 *   #include "ZFUIKit.h"
 *
 *   ZFMAIN_ENTRY()
 *   {
 *       // typically we will start UI by creating a ZFUIWindow
 *       zfblockedAlloc(ZFUIWindow, window);
 *       // show the window
 *       window->windowShow();
 *
 *       // create a text view and set text content
 *       zfblockedAlloc(ZFUITextView, textView);
 *       textView->text("hello world");
 *
 *       // add to window
 *       window->childAdd(textView);
 *
 *       // make it center in window
 *       textView->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
 *   }
 * @endcode
 */

