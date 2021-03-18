/**
 * @page DocTag_Setup_Qt Qt
 *
 * @section DocTag_Setup_Qt_Requirement Requirement
 *
 * you must have these settings being prepared:
 * -  have qmake and proper make in your system path
 *
 * once prepared, it's recommended to follow @ref DocTag_Setup_QuickSetup for quick setup your project files
 *
 * @section DocTag_Setup_Qt_ManualSetup Manual Setup
 *
 * if you really want to setup manually:
 *
 * -# build necessary libraries of ZFFramework, by one of these methods:
 *   -  tools from ~/tools/release
 *   -  manually build projects under ~/ZF
 * -# create your own Qt project (*.pro)
 * -# have these lines in your pro file:
 *   @code
 *     CONFIG(debug, debug|release) {
 *         DEFINES += DEBUG
 *     }
 *   @endcode
 * -# specify include path and lib search path in your pro file:
 *   @code
 *     INCLUDEPATH += path_to_ZFFramework/_release/your_Qt_type/all/include
 *     LIBS += -Lpath_to_ZFFramework/_release/your_Qt_type/all/lib your_linker_flags -lyour_required_ZF_lib_name
 *   @endcode
 * -# manually add loader routine to your project:
 *   @code
 *     #include "ZFImpl/sys_Qt/ZFMainEntry_sys_Qt.h"
 *     int main(int argc, char **argv)
 *     {
 *         ZFImpl_sys_Qt_requireLib(ZF_lib_name_you_require)
 *         return ZFMainEntry_sys_Qt(argc, argv);
 *     }
 *   @endcode
 * -# copy all res and libs to your product output or proper location according to your platform
 *   -  for Windows or Linux
 *     @code
 *       ~/
 *           YourApp.exe
 *           QtCore.dll
 *           ZFCore.dll
 *           zfres/
 *     @endcode
 *   -  for MacOS
 *     @code
 *       YourApp.app/
 *           Contents/
 *               Frameworks/
 *                   ZFCore.dylib
 *               Resources/
 *                   zfres/
 *     @endcode
 * -# create cpp files in your project
 *   and follow the @ref DocTag_Tutorial to code with ZFFramework, enjoy
 */

