# Intro

this file is written as a note for developers that want to know the mechanism of ZFFramework

# Contents

## Abbreviations

there are many abbreviations in ZFFramework which keep code style and you should follow:

* `_ZFP_` : `ZF Private` contents, should not be used for public
* `_ZFP_I_` : `ZF Private Internal` contents, which won't be logged by global observers
* `_ZFI_` : `ZF Include`, dummy macro to wrap headers
* `_ZFT_` : `ZF Type`, used to mark macro defined types
* `zft_` : `ZF Type`, primitive type wrapper
* `zfs` : `ZF String`, core string utils
* `zfm` : `ZF Math`, core math utils
* `zft` : `ZF Template`, core template utils
* `ZFM` : `ZF Macro`, core macro utils
* `ZFTAG_` : `ZF tag`, used to mark some workaround that currently can't be perfectly solved
* `ZF` and `zf` : short for ZFFramework namespace, also used in script impls


## Marks

there are some marks in source code, to make things more clear or correlative,
most of them has `ZFTAG_` as prefix:

* `@cond ZFPrivateDoc` : Doxygen condition to prevent something to be documented
* `ZFTAG_DOXYGEN_BUG` : tricks to solve Doxygen issues
* `ZFTAG_ADD_MODULE` : if you want to add modules for ZFFramework, you should take care of this
* `ZFTAG_LIMITATION` : limitation of ZFFramework, you should take care during coding
* `ZFTAG_TRICKS` : unconventional tricks to achieve unconventional goals


## ZFCastZFObject

we use ZFCastZFObject for dynamic type cast, instead of traditional `dynamic_cast`

the main reason for that is the performance, ZFFramework use multi-inheritance to simulate interface logic,
which would cause deep inherit tree, which cause `dynamic_cast` to be very slow

secondly, it reduce the dependency of RTTI, which is not much necessary for ZFFramework


## class namespace

since written in C++, raw namespace are supported, however,
it's strongly recommended to use `ZF_NAMESPACE_BEGIN` series,
since it make it possible to achieve reflection of namespace
as well as class and methods with namespaces


## STL dependency

we require STL containers internally, but we ensure there's no STL contents would be exported,
and it's recommended not to abuse STL in ZFFramework

the main reason is, it's a pain to export STL types,
especially for different STL versions when linking


## string type

we use zfstring as low level string container, which is reproduce of std::string

* to reduce dependency for STL, usually it's a pain to export STL containers because of different STL versions
* behaviors changed:
    * able to construct from a null char string
    * NO thread-safe supported,
        which should be implemented manually


## ZFCoreArray/ZFCoreMap

we use ZFCoreArray as low level array/list container, and ZFCoreMap for map/unordered_map,
mainly to reduce dependency for STL

however, the performance is not so good comparing to STL containers,
for performance, we would still use STL containers as internal types,
but make sure it won't be visible to public interface


## Styles

ZFFramework supply powerful serialization and style logic,
however, it cost much CPU for property comparing and copying

so, for performance, most of styleable object won't copy it's default style during init,
and most of default style are hard coded, only images are loaded from resource


## Reflection

all ZFClass/ZFMethod/ZFProperty are reflectable,
which are all implemented by callback functions

however, we have no direct support to export and make global vars serializable,
mainly to reduce complex rate of impl

to export global vars, we use `ZFEXPORT_VAR_DECLARE` / `ZFEXPORT_ENUM_DEFINE`,
which is actually a wrapper to ZFMethod


## script binding

since ZFFramework support reflection,
we are trying hard to achive script binding by reflection dynamically and automatically

1. all non-ZFObject type should be registered by `ZFTYPEID_DECLARE`
1. all ZFMethod supply a generic invoker (`ZFMethod::methodGenericInvoker`)
    that takes base ZFObject as param and invoke the original method
1. use reflection, to bind the generic invoker to scrypt languages dynamically
1. the final script language should look like (lua for example)

    ```
    local obj = MyZFObjectType();
    local objParam = YourType();
    local result = obj:myFunc(zfint(1), zfstring("2"), YourType("encodedData"), objParam);
    local result = obj:myFunc(1, "2", "encodedData", objParam);
    ```

    1. object allocation should be binded automatically by reflecting all `ZFClass` types
    1. `YourType` would be binded by `ZFTYPEID_DECLARE` series
    1. `YourType` can be serialized from encoded string data if it's serializable
      (declared by ZFTYPEID_DECLARE)
    1. `myFunc` should be binded automatically by reflecting all `ZFMethod` from `ZFClass` types
    1. when `ZFMethod` called, unknown param type would be serialized to desired type implicitly
    1. finally, `myFunc` would be invoked with all ZFObject type as params and result

