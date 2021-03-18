/**
 * @page DocTag_Setup_Android Android
 *
 * @section DocTag_Setup_Android_Requirement Requirement
 *
 * you must have these settings being prepared:
 * -  have <a href="https://developer.android.com/ndk/index.html">Android NDK</a> installed and setup properly
 *
 * once prepared, it's recommended to follow @ref DocTag_Setup_QuickSetup for quick setup your project files
 *
 * @section DocTag_Setup_Android_ManualSetup Manual Setup
 *
 * if you really want to setup manually:
 *
 * -# build necessary libraries of ZFFramework, by one of these methods:
 *   -  tools from ~/tools/release
 *   -  manually build projects under ~/ZF
 * -# create your own Android project
 * -# merge all src files under _release dir to your project's path,
 *   or use the Android aar file
 * -# set up JNI env like other Android JNI project,
 *   especially to specify header include path and so lib path of ZFFramework,
 *   which located in ~/_release
 * -# in your Android project's AndroidManifest.xml,
 *   add these activity declaration to your \<application\> node:
 *   @code
 *     <activity
 *         android:name="com.ZFFramework.Android.ZF_impl.ZFMainEntry"
 *         android:label="ZFFramework"
 *         android:launchMode="singleTask" >
 *     </activity>
 *     <activity
 *         android:name="com.ZFFramework.Android.ZFUIKit_impl.ZFUISysWindow"
 *         android:configChanges="orientation|keyboardHidden|screenSize"
 *         android:label="ZFFramework"
 *         android:windowSoftInputMode="adjustResize" >
 *     </activity>
 *   @endcode
 *   and these permission:
 *   @code
 *     <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
 *     <uses-permission android:name="android.permission.INTERNET" />
 *   @endcode
 * -# in your Android project,
 *   create a loader activity that,
 *   load necessary libraries,
 *   and start ZFMainEntry then finish itself:
 *   @code
 *     static {
 *         System.loadLibrary("stlport_shared");
 *         System.loadLibrary("ZFCore");
 *         // all other ZF libs here
 *         System.loadLibrary("your_app_lib_name");
 *     }
 *
 *     @Override
 *     protected void onCreate(Bundle savedInstanceState) {
 *         Intent intent = new Intent(Loader.this, ZFMainEntry.class);
 *         startActivity(intent);
 *         this.finish();
 *     }
 *   @endcode
 * -# ensure all of your resource files put in "src/main/assets/zfres" directory of your Android project,
 *   and ensured all of them have an extra ".mp2" as file name postfix if they are large than 1M bytes
 * -# create cpp files in your project
 *   and follow the @ref DocTag_Tutorial to code with ZFFramework, enjoy
 */

