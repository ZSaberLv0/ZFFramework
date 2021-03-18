/**
 * @page DocTag_Setup_iOS iOS
 *
 * @section DocTag_Setup_iOS_Requirement Requirement
 *
 * you must have these settings being prepared:
 * -  have XCode installed
 *
 * once prepared, it's recommended to follow @ref DocTag_Setup_QuickSetup for quick setup your project files
 *
 * @section DocTag_Setup_iOS_Cocoapods Using as Cocoapods
 *
 * you may also use ZFFramework as [Cocoapods](https://cocoapods.org) for convenient
 * @code
 *   pod 'ZFFramework'
 *   // or, if you only want specified sub module
 *   // pod 'ZFFramework/ZFCore'
 * @endcode
 * if you want to keep update with the development version,
 * you may clone the [development repo](https://github.com/ZFFramework/ZFFramework)
 * to the same dir of your project, and use:
 * @code
 *   pod 'ZFFramework',:path => '../ZFFramework'
 * @endcode
 * \n
 * \n
 * specially, when adding resource files,
 * you must ensure they are placed under `YourApp.app/zfres/`,
 * see the Manual Setup below for how to do it
 *
 * @section DocTag_Setup_iOS_ManualSetup Manual Setup
 *
 * if you really want to setup manually:
 *
 * -# build necessary libraries of ZFFramework, by one of these methods:
 *   -  tools from ~/tools/release
 *   -  manually build projects under ~/ZF
 * -# create your own iOS project
 * -# drag and drop ZFFramework's libraries (*.a or *.dylib files) to your project
 * -# modify your project's target settings:
 *   -  Header Search Paths
 *   -  Library Search Paths
 *   -  Other Linker Flags:\n
 *     "-ObjC -all_load"
 * -# ensure these libraries seleted in "Link Binary With Libraries" of XCode's target settings:
 *   -  Foundation.framework
 *   -  UIKit.framework
 *   -  QuartzCore.framework
 *   -  CoreGraphics.framework
 *   -  WebKit.framework, if you want the ZFUIWebKit module
 *   -  all ZFFramework's *.a files
 * -# add "New Run Script Phase" in your project's target's "Build Phases",
 *   to copy all resource files:
 *   @code
 *     ZF_ROOT_PATH=${SRCROOT}/path_to_ZFFramework
 *     ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
 *     RES_DST_PATH=${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/zfres
 *
 *     sh "$ZF_TOOLS_PATH/util/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/required_ZF_lib_name/zfres" "$RES_DST_PATH"
 *     # ...
 *
 *     sh "$ZF_TOOLS_PATH/util/copy_res.sh" "${SRCROOT}/../../../zfres" "$RES_DST_PATH"
 *   @endcode
 *   or, if you prefer to make it manually,
 *   please ensure all resource files of all modules are placed like this:
 *   (folder structure must be kept)
 *   @code
 *     YourApp.app/
 *         zfres/
 *             ZFUIKit/
 *                 xxx.xml
 *             ZFUIWidget/
 *                 xxx.xml
 *   @endcode
 * -# create cpp files in your project
 *   and follow the @ref DocTag_Tutorial to code with ZFFramework, enjoy
 */

