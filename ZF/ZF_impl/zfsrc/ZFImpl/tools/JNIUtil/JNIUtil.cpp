#include "JNIUtil.h"
#include <assert.h>
#include <string>
#include <vector>

#if NEED_JNIUTIL
namespace JNIUtil {

// ============================================================
// wrapper
JNIString::JNIString(void)
: _d(new std::string())
{
}
JNIString::JNIString(JNIString const &ref)
: _d(new std::string(*(const std::string *)ref._d))
{
}
JNIString::JNIString(const char *s)
: _d(new std::string(s))
{
}
JNIString::~JNIString(void)
{
    delete (std::string *)_d;
}
JNIString &JNIString::operator = (JNIString const &ref)
{
    *(std::string *)_d = *(const std::string *)ref._d;
    return *this;
}
JNIString &JNIString::operator = (const char *s)
{
    *(std::string *)_d = s;
    return *this;
}
const char *JNIString::c_str(void) const
{
    return ((std::string *)_d)->c_str();
}
JNIString::operator const char * (void) const
{
    return this->c_str();
}
JNIString &JNIString::operator += (char c)
{
    *(std::string *)_d += c;
    return *this;
}
JNIString &JNIString::operator += (const char *s)
{
    *(std::string *)_d += s;
    return *this;
}
void JNIString::clear(void)
{
    ((std::string *)_d)->clear();
}

JNIParamTypeContainer::JNIParamTypeContainer(void)
: _d(new std::vector<JNIType>())
{
}
JNIParamTypeContainer::~JNIParamTypeContainer(void)
{
    delete (std::vector<JNIType> *)_d;
}
JNIParamTypeContainer &JNIParamTypeContainer::add(const JNIType &paramType)
{
    ((std::vector<JNIType> *)_d)->push_back(paramType);
    return *this;
}

// ============================================================
static JavaVM *gs_javaVM = NULL;
static jint gs_jniVersionDesired = -1;

// ============================================================
bool JNIInit(JavaVM *javaVM, jint version)
{
    JNIEnv* jniEnv = NULL;
    if(javaVM->GetEnv((void **)&jniEnv, version) != JNI_OK
        || jniEnv == NULL)
    {
        return false;
    }
    gs_javaVM = javaVM;
    gs_jniVersionDesired = version;
    return true;
}

JavaVM *JNIGetJavaVM(void)
{
    return gs_javaVM;
}

jint JNIGetDesiredVersion(void)
{
    return gs_jniVersionDesired;
}

JNIEnv *JNIGetJNIEnv(void)
{
    if(gs_javaVM == NULL)
    {
        return NULL;
    }
    JNIEnv* jniEnv;
    gs_javaVM->GetEnv((void**)&jniEnv, gs_jniVersionDesired);
    return jniEnv;
}

void JNIConvertClassNameToClassSig(JNIString &ret, const char *className)
{
    if(className == NULL || *className == '\0')
    {
        assert(false);
    }

    ret += 'L';
    while(*className != '\0')
    {
        if(*className == '.')
        {
            ret += '/';
        }
        else
        {
            ret += *className;
        }
        ++className;
    }
    ret += ';';
}

void JNIConvertClassNameFromClassSig(JNIString &ret, const char *classSig)
{
    if(classSig == NULL || *classSig == '\0')
    {
        assert(false);
    }

    ++classSig;
    while(*classSig != '\0' && *classSig != ';')
    {
        if(*classSig == '/')
        {
            ret += '.';
        }
        else
        {
            ret += *classSig;
        }
        ++classSig;
    }
}

void JNIConvertClassNameForFindClass(JNIString &ret, const char *className)
{
    if(className == NULL || *className == '\0')
    {
        assert(false);
    }

    while(*className != '\0')
    {
        if(*className == '.')
        {
            ret += '/';
        }
        else
        {
            ret += *className;
        }
        ++className;
    }
}

// ============================================================
#define _JNIUtil_DEFINE_STATIC_TYPE(Type) \
    const JNIType &JNIType::S_##Type(JNIType::T_##Type);
_JNIUtil_DEFINE_STATIC_TYPE(boolean)
_JNIUtil_DEFINE_STATIC_TYPE(byte)
_JNIUtil_DEFINE_STATIC_TYPE(char)
_JNIUtil_DEFINE_STATIC_TYPE(short)
_JNIUtil_DEFINE_STATIC_TYPE(int)
_JNIUtil_DEFINE_STATIC_TYPE(long)
_JNIUtil_DEFINE_STATIC_TYPE(float)
_JNIUtil_DEFINE_STATIC_TYPE(double)
_JNIUtil_DEFINE_STATIC_TYPE(void)

class _JNITypePrivate
{
public:
    JNIType::Type type;
    JNIString classNameOrArrayElementTypeId;
    JNIString typeId;
    bool needUpdateTypeId;

public:
    void updateId(void)
    {
        this->typeId.clear();
        switch(this->type)
        {
            case JNIType::T_boolean:
                this->typeId += "Z";
                break;
            case JNIType::T_byte:
                this->typeId += "B";
                break;
            case JNIType::T_char:
                this->typeId += "C";
                break;
            case JNIType::T_short:
                this->typeId += "S";
                break;
            case JNIType::T_int:
                this->typeId += "I";
                break;
            case JNIType::T_long:
                this->typeId += "J";
                break;
            case JNIType::T_float:
                this->typeId += "F";
                break;
            case JNIType::T_double:
                this->typeId += "D";
                break;
            case JNIType::T_void:
                this->typeId += "V";
                break;
            case JNIType::T_object:
                JNIConvertClassNameToClassSig(this->typeId, this->classNameOrArrayElementTypeId.c_str());
                break;
            case JNIType::T_array:
                this->typeId += '[';
                this->typeId += this->classNameOrArrayElementTypeId;
                break;
            default:
                break;
        }
    }
};

JNIType::JNIType(void)
{
    d = new _JNITypePrivate();
    d->type = JNIType::T_boolean;
    d->needUpdateTypeId = true;
}
JNIType::JNIType(JNIType::Type type,
                 const char *classNameOrArrayElementTypeId /* = NULL */)
{
    d = new _JNITypePrivate();
    d->needUpdateTypeId = true;
    this->setType(type);
    this->setClassNameOrArrayElementTypeId(classNameOrArrayElementTypeId);
}
JNIType::JNIType(const JNIType &ref)
{
    d = new _JNITypePrivate();
    d->needUpdateTypeId = true;
    this->setType(ref.getType());
    this->setClassNameOrArrayElementTypeId(ref.getClassNameOrArrayElementTypeId());
}
JNIType &JNIType::operator = (const JNIType &ref)
{
    if(d != ref.d)
    {
        this->setType(ref.getType());
        this->setClassNameOrArrayElementTypeId(ref.getClassNameOrArrayElementTypeId());
    }
    return *this;
}

void JNIType::setType(JNIType::Type type)
{
    d->needUpdateTypeId = true;
    d->type = type;
}
JNIType::Type JNIType::getType(void) const
{
    return d->type;
}
void JNIType::setClassNameOrArrayElementTypeId(const char *s)
{
    d->needUpdateTypeId = true;
    if(s == NULL)
    {
        d->classNameOrArrayElementTypeId.clear();
    }
    else
    {
        d->classNameOrArrayElementTypeId = s;
    }
}
const char *JNIType::getClassNameOrArrayElementTypeId(void) const
{
    return d->classNameOrArrayElementTypeId.c_str();
}
const char *JNIType::getId(void) const
{
    if(d->needUpdateTypeId)
    {
        d->needUpdateTypeId = false;
        d->updateId();
    }
    return d->typeId.c_str();
}

// ============================================================
JNIString JNIGetMethodSig(const JNIType &returnType,
                          const JNIParamTypeContainer &paramTypeList)
{
    std::vector<JNIType> &p = *(std::vector<JNIType> *)paramTypeList._d;
    JNIString s;
    s += "(";
    for(std::size_t i = 0; i < p.size(); ++i)
    {
        const JNIType &t = p[i];
        s += t.getId();
    }
    s += ")";
    s += returnType.getId();
    return s;
}

jbyteArray _JNIConvertPointerToJNITypeAction(JNIEnv *jniEnv, void *p)
{
    static unsigned int size = sizeof(void *);
    jbyte jByteBuf[32] = {0};
    memcpy(jByteBuf, &p, size);
    jbyteArray ret = jniEnv->NewByteArray(size);
    jniEnv->SetByteArrayRegion(ret, 0, size, jByteBuf);
    return ret;
}
void *_JNIConvertPointerFromJNITypeAction(JNIEnv *jniEnv, jbyteArray d)
{
    if(d == NULL)
    {
        return NULL;
    }
    static unsigned int size = sizeof(void *);
    jbyte jByteBuf[32] = {0};
    jniEnv->GetByteArrayRegion(d, 0, size, jByteBuf);
    void *p = NULL;
    memcpy(&p, jByteBuf, size);
    return p;
}

} // namespace JNIUtil
#endif // #if NEED_JNIUTIL

