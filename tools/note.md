# project dir structure

```
// for ZF libs
~/ZF/LibName/
    zfproj/
        Android/
            LibName/
                gradle/
                zflib/
                    src/
                    build.gradle
                    CMakeLists.txt
                build.gradle
        iOS/
            LibName/
                LibName.xcodeproj/
        Qt/
            LibName/
                LibName.pro
    zfres/
    zfsrc/

// for ZF impl libs
~/ZF/ImplLibName_impl/
    zfproj/
        Android/
            ImplLibName_impl/
                gradle/
                zflib/
                    src/
                    build.gradle
                    CMakeLists.txt
                build.gradle
        iOS/
            ImplLibName_impl/
                ImplLibName_impl.xcodeproj/
        Qt/
            ImplLibName_impl/
                ImplLibName_impl.pro
    zfres/
    zfsrc/

// for app
~/
    zfproj/
        Android/
            AppName/
                gradle/
                zfapp/
                    src/
                    build.gradle
                    CMakeLists.txt
                build.gradle
        iOS/
            AppName/
                AppName.xcodeproj/
        Qt/
            AppName/
                AppName.pro
    zfres/
    zfsrc/
```

# release dir structure

```
~/_release/
    Android/
        all/
        module/
            LibName/
                src/
                    main/
                        assets/
                            zfres/
                        cpp/
                            include/
                        jniLibs/
                            armeabi/
                                libLibName.so
                        libs/
                            libLibName.jar
                aar/
                    LibName.aar
    iOS/
        all/
        module/
            LibName/
                include/
                lib/
                    libLibName.a
                zfres/
    Qt_MacOS/
    Qt_Posix
    Qt_Windows/
        all/
        module/
            LibName/
                include/
                lib/
                    libLibName.[a|so|dll|dylib]
                zfres/
```

# tmp dir structure

```
~/
    _tmp/
        Android/
        iOS/
        Qt_MacOS/
        Qt_Posix/
        Qt_Windows/
            ProjName/
                build/
                release/
                    ProjName.xxx
                debug/
                    ProjName.xxx
```

