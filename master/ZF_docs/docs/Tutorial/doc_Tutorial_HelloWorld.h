/**
 * @page DocTag_Tutorial_HelloWorld Hello world
 *
 * to start a quick hello world:
 * @code
 *   // include ZFCore.h for core function of ZFFramework
 *   // you may need to setup your project's include path first
 *   #include "ZFCore.h"
 *
 *   // declare app's main entry
 *   ZFMAIN_ENTRY()
 *   {
 *       // well, hello world to ZFFramework
 *       zfLogT() << "hello world";
 *   }
 * @endcode
 *
 * ZFFramework would be started from #ZFMAIN_ENTRY,
 * you may write any C++ compatible code here\n
 * in this example,
 * we just output "hello world" to zfLogT,
 * which would output to a proper console by default
 */

