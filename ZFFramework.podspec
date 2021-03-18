# note, as C++ pod, when `pod lib lint` you need
# `--use-libraries --skip-import-validation`,
# also, mute warnings by `--allow-warnings` if necessary

Pod::Spec.new do |s|
  s.name             = 'ZFFramework'
  s.version          = '9999' # ZFTAG_ADD_MODULE
  s.summary          = 'cross-platform C++ app framework'
  s.description      = <<-DESC
see http://ZFFramework.com for more info

minimal steps to use in iOS with cocoapods to embed to existing projects:
  1. in your Podfile:
      pod 'ZFFramework'
  2. add cpp files, with this as main entry
      #include "ZFUIKit.h" // ZF module header
      ZFMAIN_ENTRY()
      {
          // you cpp code
      }

recommended to use https://github.com/ZFFramework/ZFFramework/blob/master/tools/zfproj_creator/zfproj_creator.sh
to create project files if you want to deploy to multiple platforms

pod list:
  pod 'ZFFramework'              // include all modules
  pod 'ZFFramework/ZF_MODULE_NAME'
for ZF_MODULE_NAME list, please refer to https://github.com/ZFFramework/ZFFramework/tree/master/ZF
                       DESC
  s.homepage         = 'https://github.com/ZFFramework/ZFFramework'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'ZSaberLv0' => 'master@zsaber.com' }
  s.source           = { :git => 'https://github.com/ZFFramework/ZFFramework.git' }
  s.ios.deployment_target = '8.0'
  s.requires_arc     = true
  s.frameworks       = 'Foundation'
  s.libraries        = 'c++'
  # ZFTAG_ADD_MODULE
  s.prepare_command  = <<-CMD
          sh zfsetup.sh
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFAlgorithm.mm ZF/ZFAlgorithm/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFAlgorithm_impl.mm ZF/ZFAlgorithm_impl/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFCore.mm ZF/ZFCore/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFCore_impl.mm ZF/ZFCore_impl/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFLua.mm ZF/ZFLua/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFLua_impl.mm ZF/ZFLua_impl/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFUIKit.mm ZF/ZFUIKit/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFUIKit_impl.mm ZF/ZFUIKit_impl/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFUIWebKit.mm ZF/ZFUIWebKit/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFUIWebKit_impl.mm ZF/ZFUIWebKit_impl/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFUIWidget.mm ZF/ZFUIWidget/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZFUtility.mm ZF/ZFUtility/zfsrc
          sh tools/common/unity_build.sh _tmp/cocoapods/zfgensrc_ZF_impl.mm ZF/ZF_impl/zfsrc
      CMD
  s.xcconfig         = {
      'USER_HEADER_SEARCH_PATHS' => '"${PODS_ROOT}/Headers/Public/ZFFramework"',
      'OTHER_LDFLAGS' => '-ObjC -force_load "${TARGET_BUILD_DIR}/ZFFramework/libZFFramework.a"',
  }
  s.preserve_paths   = '**/*'

  class << self
    def ZFAddModule(s, moduleName, dependencyList, ext)
      s.subspec moduleName do |ss|
        ss.public_header_files = 'ZF/' + moduleName + '/zfsrc/**/*.{h,hpp}'
        ss.header_mappings_dir = 'ZF/' + moduleName + '/zfsrc'
        ss.source_files = [
          'ZF/' + moduleName + '/zfsrc/**/*.{h,hpp}',
          'ZF/' + moduleName + '/zfsrc_ext/**/*.{h,hpp,c,cpp,m,mm}',
          '_tmp/cocoapods/zfgensrc_' + moduleName + '.mm',
        ]
        ss.resources = [ 'ZF/' + moduleName + '/zfres' ]
        ss.exclude_files = 'ZF/**/ZF_PUT_RES_FILES_HERE' # not work for now, but no other side effect
        ss.preserve_paths   = '**/*'
        dependencyList.each { |d| ss.dependency 'ZFFramework/' + d }
        ext.call(ss)
      end
    end
  end

  # ZFTAG_ADD_MODULE
  ZFAddModule(s, 'ZFCore',            [],                                                                            lambda {|ss| })
  ZFAddModule(s, 'ZFAlgorithm',       [ 'ZFCore', ],                                                                 lambda {|ss| })
  ZFAddModule(s, 'ZFUtility',         [ 'ZFCore', ],                                                                 lambda {|ss| })
  ZFAddModule(s, 'ZFLua',             [ 'ZFCore', ],                                                                 lambda {|ss| })
  ZFAddModule(s, 'ZFUIKit',           [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', ],                                     lambda {|ss| })
  ZFAddModule(s, 'ZFUIWidget',        [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', 'ZFUIKit', ],                          lambda {|ss| })
  ZFAddModule(s, 'ZFUIWebKit',        [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', 'ZFUIKit', ],                          lambda {|ss| })
  ZFAddModule(s, 'ZF_impl',           [ 'ZFCore', ],                                                                 lambda {|ss| })
  ZFAddModule(s, 'ZFCore_impl',       [ 'ZFCore', 'ZF_impl', ],                                                      lambda {|ss| })
  ZFAddModule(s, 'ZFAlgorithm_impl',  [ 'ZFCore', 'ZFAlgorithm', 'ZF_impl', ],                                       lambda {|ss| })
  ZFAddModule(s, 'ZFLua_impl',        [ 'ZFCore', 'ZFLua', 'ZF_impl', ],                                             lambda {|ss| })
  ZFAddModule(s, 'ZFUIKit_impl',      [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', 'ZFUIKit', 'ZF_impl', ],               lambda {|ss| ss.frameworks = 'Foundation', 'UIKit', 'QuartzCore', 'CoreGraphics' })
  ZFAddModule(s, 'ZFUIWebKit_impl',   [ 'ZFCore', 'ZFAlgorithm', 'ZFUtility', 'ZFUIKit', 'ZFUIWebKit', 'ZF_impl', ], lambda {|ss| ss.frameworks = 'Foundation', 'UIKit', 'QuartzCore', 'CoreGraphics', 'WebKit' })

end

