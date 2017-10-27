#ifndef DMX_CUSTOM_FUNCTIONS_H
#define DMX_CUSTOM_FUNCTIONS_H
/*******************************************************************************

Copyright (c) 2015 Syncsort Inc. Woodcliff Lake  New Jersey   07677

Purpose
-------
To define the C++ interface for custom functions.

*******************************************************************************/
#include <string>
#include <vector>
#include <stdexcept>
#include <time.h>
#include <cstring>
/******************************************************************************/
/* Custom function return statuses */

#define DMX_CUSTOM_FUNCTION_SUCCESS     0
#define DMX_CUSTOM_FUNCTION_FAILURE     (-1)
#define DMX_CUSTOM_FUNCTION_EXCEPTION   (-2)

/******************************************************************************/
/* Custom function API version */

#define DMX_CUSTOM_FUNCTION_API_VERSION             "1.0"

/* Custom function metadata getter names */

#define DMX_GET_CUSTOM_FUNCTION_API_VERSION         dmxGetCustomFunctionApiVersion
#define DMX_GET_CUSTOM_FUNCTION_NAMES               dmxGetCustomFunctionNames
#define DMX_GET_CUSTOM_FUNCTION_ARG_TYPES_PREFIX    dmxGetArgTypes

/******************************************************************************/
/* Custom function argument type ids */

enum DmxTypeId
{
    DMXTYPEID_INT,
    DMXTYPEID_UNSIGNED_INT,
    DMXTYPEID_DOUBLE,
    DMXTYPEID_STRING,
    DMXTYPEID_DATE_TIME,
    DMXTYPEID_UNUSED
};

/* Custom function argument byte buffer */

struct DmxByteBuffer
{
    size_t m_bufferSize;
    char* m_dataPtr;
    size_t m_size;
};

/* Custom function argument datetime buffer */

struct DmxDateTimeBuffer
{
    struct tm m_dateTime;
    double m_fractionalSecond;
};

/******************************************************************************/
/* Custom function argument base type */

template<typename T, DmxTypeId typeId>
class DmxTypeBase
{
public:
    static const DmxTypeId s_typeId = typeId;
public:
    virtual ~DmxTypeBase() {}
    void setNull() { m_bufferPtr = NULL; }
    bool isNull() const { return m_bufferPtr == NULL; }
protected:
    DmxTypeBase(void* bufferPtr, bool isOutput = false)
    : m_bufferPtr(static_cast<T*>(bufferPtr)),
      m_isOutput(isOutput)
    {
    }
protected:
    T* m_bufferPtr;
    bool m_isOutput;
private:
    DmxTypeBase& operator=(const DmxTypeBase&);
};

/* Custom function argument numeric type */

template<typename T, DmxTypeId typeId>
class DmxNumericType : public DmxTypeBase<T, typeId>
{
public:
    using DmxTypeBase<T, typeId>::m_bufferPtr;
    DmxNumericType(void* bufferPtr, bool isOutput = false)
    : DmxTypeBase<T, typeId>(bufferPtr, isOutput)
    {
    }
    operator T() const { return *m_bufferPtr; }
    DmxNumericType& operator=(const T& rhs) { *m_bufferPtr = rhs; return *this; }
    DmxNumericType& operator=(const DmxNumericType& rhs) { *m_bufferPtr = rhs; return *this; }
    DmxNumericType& operator+=(const T& rhs) { *m_bufferPtr += rhs; return *this; }
    DmxNumericType& operator+=(const DmxNumericType& rhs) { *m_bufferPtr += rhs; return *this; }
    DmxNumericType& operator-=(const T& rhs) { *m_bufferPtr -= rhs; return *this; }
    DmxNumericType& operator-=(const DmxNumericType& rhs) { *m_bufferPtr -= rhs; return *this; }
    DmxNumericType& operator*=(const T& rhs) { *m_bufferPtr *= rhs; return *this; }
    DmxNumericType& operator*=(const DmxNumericType& rhs) { *m_bufferPtr *= rhs; return *this; }
    DmxNumericType& operator/=(const T& rhs) { *m_bufferPtr /= rhs; return *this; }
    DmxNumericType& operator/=(const DmxNumericType& rhs) { *m_bufferPtr /= rhs; return *this; }
};

typedef DmxNumericType<long long,           DMXTYPEID_INT>              DmxInt;
typedef DmxNumericType<unsigned long long,  DMXTYPEID_UNSIGNED_INT>     DmxUnsignedInt;
typedef DmxNumericType<double,              DMXTYPEID_DOUBLE>           DmxDouble;

/* Custom function argument string type */

typedef DmxTypeBase<DmxByteBuffer,          DMXTYPEID_STRING>           DmxStringBase;

class DmxString : public DmxStringBase, public std::string
{
public:
    using std::string::operator=;
    DmxString(void* bufferPtr, bool isOutput = false)
    : DmxStringBase(bufferPtr, isOutput)
    {
        if (!m_isOutput && m_bufferPtr != NULL) {
            this->assign(m_bufferPtr->m_dataPtr, m_bufferPtr->m_size);
        }
    }
    ~DmxString()
    {
        if (m_isOutput && m_bufferPtr != NULL) {
            size_t length = (this->length() < m_bufferPtr->m_bufferSize) ? this->length() : m_bufferPtr->m_bufferSize;
            for (size_t i = 0; i < length; ++i) {
                (m_bufferPtr->m_dataPtr)[i] = this->at(i);
            }
            m_bufferPtr->m_size = length;
        }
    }
    DmxString& operator=(const DmxString& rhs) {
        std::string::operator=(rhs);
        return *this;
    }
};

/* Custom function argument datetime type */

typedef DmxTypeBase<DmxDateTimeBuffer,      DMXTYPEID_DATE_TIME>        DmxDateTimeBase;

class DmxDateTime : public DmxDateTimeBase
{
public:
    DmxDateTime(void* bufferPtr, bool isOutput = false)
    : DmxDateTimeBase(bufferPtr, isOutput)
    {
    }
    struct tm getTime() const           { return m_bufferPtr->m_dateTime; }
    time_t getEpochTime()               { return mktime(&(m_bufferPtr->m_dateTime)); }
    int getDay() const                  { return (m_bufferPtr->m_dateTime).tm_mday; }
    int getMonth() const                { return (m_bufferPtr->m_dateTime).tm_mon; }
    int getYear() const                 { return (m_bufferPtr->m_dateTime).tm_year; }
    int getSecond() const               { return (m_bufferPtr->m_dateTime).tm_sec; }
    int getMinute() const               { return (m_bufferPtr->m_dateTime).tm_min; }
    int getHour() const                 { return (m_bufferPtr->m_dateTime).tm_hour; }
    double getFractionalSecond() const  { return m_bufferPtr->m_fractionalSecond; }
    void setDay(int day)                                { (m_bufferPtr->m_dateTime).tm_mday = day; }
    void setMonth(int month)                            { (m_bufferPtr->m_dateTime).tm_mon = month; }
    void setYear(int year)                              { (m_bufferPtr->m_dateTime).tm_year = year; }
    void setSecond(int second)                          { (m_bufferPtr->m_dateTime).tm_sec = second; }
    void setMinute(int minute)                          { (m_bufferPtr->m_dateTime).tm_min = minute; }
    void setHour(int hour)                              { (m_bufferPtr->m_dateTime).tm_hour = hour; }
    void setFractionalSecond(double fractionalSecond)   { m_bufferPtr->m_fractionalSecond = fractionalSecond; }
    DmxDateTime& operator=(const time_t& rhs)
    {
        m_bufferPtr->m_dateTime = *(localtime(&rhs));
        m_bufferPtr->m_fractionalSecond = 0;
        return *this;
    }
    DmxDateTime& operator=(const struct tm& rhs)
    {
        m_bufferPtr->m_dateTime = rhs;
        m_bufferPtr->m_fractionalSecond = 0;
        return *this;
    }
    DmxDateTime& operator=(const DmxDateTime& rhs)
    {
        m_bufferPtr->m_dateTime = (rhs.m_bufferPtr)->m_dateTime;
        m_bufferPtr->m_fractionalSecond = (rhs.m_bufferPtr)->m_fractionalSecond;
        return *this;
    }
};

/******************************************************************************/
#if !defined(__SSUPBUILD__) || defined(DMX_CUSTOM_FUNCTIONS_TEST)

/* Helper macros */

#define DMX_ARRAY_LENGTH(a)         sizeof(a) / sizeof(a[0])

#define DMX_CONCAT_BASE(a, b)       a ## b                  /* a and b do not need replacement */
#define DMX_CONCAT(a, b)            DMX_CONCAT_BASE(a, b)   /* a or b needs replacement */

#define DMX_STRINGIFY(a)            #a                      /* a -> "a" */

#define DMX_EXPAND_VA_ARGS(...)     __VA_ARGS__             /* workaround for MSVC __VA_ARGS__ bug */

/* Count number of __VA_ARGS__ (max 20) */
#define DMX_NUM_VA_ARGS_IMPL(arg1, arg2, arg3, arg4, arg5, \
                             arg6, arg7, arg8, arg9, arg10, \
                             arg11, arg12, arg13, arg14, arg15, \
                             arg16, arg17, arg18, arg19, arg20, \
                             N, ...) \
                             N
#define DMX_NUM_VA_ARGS(...) \
    DMX_EXPAND_VA_ARGS( \
        DMX_NUM_VA_ARGS_IMPL(__VA_ARGS__ , \
                             20, 19, 18, 17, 16, \
                             15, 14, 13, 12, 11, \
                             10, 9, 8, 7, 6, \
                             5, 4, 3, 2, 1) \
    )

/******************************************************************************/
/* Custom function export */

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
    #define DMX_DLL_EXPORT __declspec(dllexport)
#else
    #define DMX_DLL_EXPORT
#endif

#define DMX_EXPORT_FUNCTION \
    extern "C" DMX_DLL_EXPORT

/******************************************************************************/
/* Custom function library metadata */

DMX_EXPORT_FUNCTION const char* DMX_GET_CUSTOM_FUNCTION_API_VERSION() {
    return DMX_CUSTOM_FUNCTION_API_VERSION;
}

class DmxCustomFunctionNames
{
public:
    DmxCustomFunctionNames(const char* functionNamePtr) { s_functionNames.push_back(functionNamePtr); }
    static const std::vector<const char*>& get() { return s_functionNames; }
private:
    static std::vector<const char*> s_functionNames;
};
std::vector<const char*> DmxCustomFunctionNames::s_functionNames;

DMX_EXPORT_FUNCTION const char* const* DMX_GET_CUSTOM_FUNCTION_NAMES(size_t* numFunctionsPtr) {
    const std::vector<const char*>& functionNames = DmxCustomFunctionNames::get();
    *numFunctionsPtr = functionNames.size();
    return &functionNames[0];
}

#define DECLARE_DMX_CUSTOM_FUNCTION_NAME(functionName) \
    static const DmxCustomFunctionNames DMX_CONCAT(dmxCustomFunctionName, functionName)(DMX_STRINGIFY(functionName));

/******************************************************************************/
/* Custom function metadata */

#define DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, ...) \
    DMX_EXPORT_FUNCTION const DmxTypeId* DMX_CONCAT(DMX_GET_CUSTOM_FUNCTION_ARG_TYPES_PREFIX, functionName)(size_t* numArgsPtr) { \
        static const DmxTypeId dmxCustomFunctionArgTypeIds[] = { __VA_ARGS__ }; \
        *numArgsPtr = DMX_ARRAY_LENGTH(dmxCustomFunctionArgTypeIds); \
        return dmxCustomFunctionArgTypeIds; \
    }

/******************************************************************************/
/* Custom function arguments */

#define DMX_INT(variableName) \
    DmxInt, variableName

#define DMX_UNSIGNED_INT(variableName) \
    DmxUnsignedInt, variableName

#define DMX_DOUBLE(variableName) \
    DmxDouble, variableName

#define DMX_STRING(variableName) \
    DmxString, variableName

#define DMX_DATE_TIME(variableName) \
    DmxDateTime, variableName

/******************************************************************************/
/* Custom function exception handling */

extern "C" int dmxReportCustomFunctionException(DmxByteBuffer* dmxExceptionBufferPtr, const char* exceptionMessagePtr) {
    size_t length = strlen(exceptionMessagePtr);
    if (length > dmxExceptionBufferPtr->m_bufferSize) {
        length = dmxExceptionBufferPtr->m_bufferSize;
    }
    for (size_t i = 0; i < length; ++i) {
        (dmxExceptionBufferPtr->m_dataPtr)[i] = exceptionMessagePtr[i];
    }
    dmxExceptionBufferPtr->m_size = length;
    return DMX_CUSTOM_FUNCTION_EXCEPTION;
}

#define DMX_CUSTOM_FUNCTION_TRY \
    try {

#define DMX_CUSTOM_FUNCTION_CATCH \
    } catch (const std::exception& e) { \
        return dmxReportCustomFunctionException(dmxExceptionBufferPtr, e.what()); \
    } catch (...) { \
        return dmxReportCustomFunctionException(dmxExceptionBufferPtr, "Unknown exception"); \
    }

/******************************************************************************/
/* Custom function expanded declaration */

/* 1 argument */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_2(functionName, \
                                              DmxType1, variableName1) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1)

/* 2 arguments */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_4(functionName, \
                                              DmxType1, variableName1, \
                                              DmxType2, variableName2) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId, \
                                          DmxType2::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1, \
                                         void* variableName2) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&, \
                                               const DmxType2&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput, \
                                                                         variableName2); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1, \
                                                  const DmxType2& variableName2)

/* 3 arguments */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_6(functionName, \
                                              DmxType1, variableName1, \
                                              DmxType2, variableName2, \
                                              DmxType3, variableName3) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId, \
                                          DmxType2::s_typeId, \
                                          DmxType3::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1, \
                                         void* variableName2, \
                                         void* variableName3) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&, \
                                               const DmxType2&, \
                                               const DmxType3&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput, \
                                                                         variableName2, \
                                                                         variableName3); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1, \
                                                  const DmxType2& variableName2, \
                                                  const DmxType3& variableName3)

/* 4 arguments */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_8(functionName, \
                                              DmxType1, variableName1, \
                                              DmxType2, variableName2, \
                                              DmxType3, variableName3, \
                                              DmxType4, variableName4) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId, \
                                          DmxType2::s_typeId, \
                                          DmxType3::s_typeId, \
                                          DmxType4::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1, \
                                         void* variableName2, \
                                         void* variableName3, \
                                         void* variableName4) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&, \
                                               const DmxType2&, \
                                               const DmxType3&, \
                                               const DmxType4&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput, \
                                                                         variableName2, \
                                                                         variableName3, \
                                                                         variableName4); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1, \
                                                  const DmxType2& variableName2, \
                                                  const DmxType3& variableName3, \
                                                  const DmxType4& variableName4)

/* 5 arguments */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_10(functionName, \
                                               DmxType1, variableName1, \
                                               DmxType2, variableName2, \
                                               DmxType3, variableName3, \
                                               DmxType4, variableName4, \
                                               DmxType5, variableName5) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId, \
                                          DmxType2::s_typeId, \
                                          DmxType3::s_typeId, \
                                          DmxType4::s_typeId, \
                                          DmxType5::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1, \
                                         void* variableName2, \
                                         void* variableName3, \
                                         void* variableName4, \
                                         void* variableName5) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&, \
                                               const DmxType2&, \
                                               const DmxType3&, \
                                               const DmxType4&, \
                                               const DmxType5&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput, \
                                                                         variableName2, \
                                                                         variableName3, \
                                                                         variableName4, \
                                                                         variableName5); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1, \
                                                  const DmxType2& variableName2, \
                                                  const DmxType3& variableName3, \
                                                  const DmxType4& variableName4, \
                                                  const DmxType5& variableName5)

/* 6 arguments */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_12(functionName, \
                                               DmxType1, variableName1, \
                                               DmxType2, variableName2, \
                                               DmxType3, variableName3, \
                                               DmxType4, variableName4, \
                                               DmxType5, variableName5, \
                                               DmxType6, variableName6) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId, \
                                          DmxType2::s_typeId, \
                                          DmxType3::s_typeId, \
                                          DmxType4::s_typeId, \
                                          DmxType5::s_typeId, \
                                          DmxType6::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1, \
                                         void* variableName2, \
                                         void* variableName3, \
                                         void* variableName4, \
                                         void* variableName5, \
                                         void* variableName6) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&, \
                                               const DmxType2&, \
                                               const DmxType3&, \
                                               const DmxType4&, \
                                               const DmxType5&, \
                                               const DmxType6&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput, \
                                                                         variableName2, \
                                                                         variableName3, \
                                                                         variableName4, \
                                                                         variableName5, \
                                                                         variableName6); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1, \
                                                  const DmxType2& variableName2, \
                                                  const DmxType3& variableName3, \
                                                  const DmxType4& variableName4, \
                                                  const DmxType5& variableName5, \
                                                  const DmxType6& variableName6)

/* 7 arguments */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_14(functionName, \
                                               DmxType1, variableName1, \
                                               DmxType2, variableName2, \
                                               DmxType3, variableName3, \
                                               DmxType4, variableName4, \
                                               DmxType5, variableName5, \
                                               DmxType6, variableName6, \
                                               DmxType7, variableName7) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId, \
                                          DmxType2::s_typeId, \
                                          DmxType3::s_typeId, \
                                          DmxType4::s_typeId, \
                                          DmxType5::s_typeId, \
                                          DmxType6::s_typeId, \
                                          DmxType7::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1, \
                                         void* variableName2, \
                                         void* variableName3, \
                                         void* variableName4, \
                                         void* variableName5, \
                                         void* variableName6, \
                                         void* variableName7) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&, \
                                               const DmxType2&, \
                                               const DmxType3&, \
                                               const DmxType4&, \
                                               const DmxType5&, \
                                               const DmxType6&, \
                                               const DmxType7&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput, \
                                                                         variableName2, \
                                                                         variableName3, \
                                                                         variableName4, \
                                                                         variableName5, \
                                                                         variableName6, \
                                                                         variableName7); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1, \
                                                  const DmxType2& variableName2, \
                                                  const DmxType3& variableName3, \
                                                  const DmxType4& variableName4, \
                                                  const DmxType5& variableName5, \
                                                  const DmxType6& variableName6, \
                                                  const DmxType7& variableName7)

/* 8 arguments */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_16(functionName, \
                                               DmxType1, variableName1, \
                                               DmxType2, variableName2, \
                                               DmxType3, variableName3, \
                                               DmxType4, variableName4, \
                                               DmxType5, variableName5, \
                                               DmxType6, variableName6, \
                                               DmxType7, variableName7, \
                                               DmxType8, variableName8) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId, \
                                          DmxType2::s_typeId, \
                                          DmxType3::s_typeId, \
                                          DmxType4::s_typeId, \
                                          DmxType5::s_typeId, \
                                          DmxType6::s_typeId, \
                                          DmxType7::s_typeId, \
                                          DmxType8::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1, \
                                         void* variableName2, \
                                         void* variableName3, \
                                         void* variableName4, \
                                         void* variableName5, \
                                         void* variableName6, \
                                         void* variableName7, \
                                         void* variableName8) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&, \
                                               const DmxType2&, \
                                               const DmxType3&, \
                                               const DmxType4&, \
                                               const DmxType5&, \
                                               const DmxType6&, \
                                               const DmxType7&, \
                                               const DmxType8&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput, \
                                                                         variableName2, \
                                                                         variableName3, \
                                                                         variableName4, \
                                                                         variableName5, \
                                                                         variableName6, \
                                                                         variableName7, \
                                                                         variableName8); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1, \
                                                  const DmxType2& variableName2, \
                                                  const DmxType3& variableName3, \
                                                  const DmxType4& variableName4, \
                                                  const DmxType5& variableName5, \
                                                  const DmxType6& variableName6, \
                                                  const DmxType7& variableName7, \
                                                  const DmxType8& variableName8)

/* 9 arguments */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_18(functionName, \
                                               DmxType1, variableName1, \
                                               DmxType2, variableName2, \
                                               DmxType3, variableName3, \
                                               DmxType4, variableName4, \
                                               DmxType5, variableName5, \
                                               DmxType6, variableName6, \
                                               DmxType7, variableName7, \
                                               DmxType8, variableName8, \
                                               DmxType9, variableName9) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId, \
                                          DmxType2::s_typeId, \
                                          DmxType3::s_typeId, \
                                          DmxType4::s_typeId, \
                                          DmxType5::s_typeId, \
                                          DmxType6::s_typeId, \
                                          DmxType7::s_typeId, \
                                          DmxType8::s_typeId, \
                                          DmxType9::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1, \
                                         void* variableName2, \
                                         void* variableName3, \
                                         void* variableName4, \
                                         void* variableName5, \
                                         void* variableName6, \
                                         void* variableName7, \
                                         void* variableName8, \
                                         void* variableName9) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&, \
                                               const DmxType2&, \
                                               const DmxType3&, \
                                               const DmxType4&, \
                                               const DmxType5&, \
                                               const DmxType6&, \
                                               const DmxType7&, \
                                               const DmxType8&, \
                                               const DmxType9&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput, \
                                                                         variableName2, \
                                                                         variableName3, \
                                                                         variableName4, \
                                                                         variableName5, \
                                                                         variableName6, \
                                                                         variableName7, \
                                                                         variableName8, \
                                                                         variableName9); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1, \
                                                  const DmxType2& variableName2, \
                                                  const DmxType3& variableName3, \
                                                  const DmxType4& variableName4, \
                                                  const DmxType5& variableName5, \
                                                  const DmxType6& variableName6, \
                                                  const DmxType7& variableName7, \
                                                  const DmxType8& variableName8, \
                                                  const DmxType9& variableName9)

/* 10 arguments */

#define DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_20(functionName, \
                                               DmxType1, variableName1, \
                                               DmxType2, variableName2, \
                                               DmxType3, variableName3, \
                                               DmxType4, variableName4, \
                                               DmxType5, variableName5, \
                                               DmxType6, variableName6, \
                                               DmxType7, variableName7, \
                                               DmxType8, variableName8, \
                                               DmxType9, variableName9, \
                                               DmxType10, variableName10) \
    DECLARE_DMX_CUSTOM_FUNCTION_ARG_TYPES(functionName, \
                                          DmxType1::s_typeId, \
                                          DmxType2::s_typeId, \
                                          DmxType3::s_typeId, \
                                          DmxType4::s_typeId, \
                                          DmxType5::s_typeId, \
                                          DmxType6::s_typeId, \
                                          DmxType7::s_typeId, \
                                          DmxType8::s_typeId, \
                                          DmxType9::s_typeId, \
                                          DmxType10::s_typeId); \
    DMX_EXPORT_FUNCTION int functionName(DmxByteBuffer* dmxExceptionBufferPtr, \
                                         bool* dmxIsOutputNullPtr, \
                                         void* variableName1, \
                                         void* variableName2, \
                                         void* variableName3, \
                                         void* variableName4, \
                                         void* variableName5, \
                                         void* variableName6, \
                                         void* variableName7, \
                                         void* variableName8, \
                                         void* variableName9, \
                                         void* variableName10) { \
        DMX_CUSTOM_FUNCTION_TRY \
            DmxType1 dmxCustomFunctionOutput(variableName1, true); \
            int DMX_CONCAT(functionName, Impl)(DmxType1&, \
                                               const DmxType2&, \
                                               const DmxType3&, \
                                               const DmxType4&, \
                                               const DmxType5&, \
                                               const DmxType6&, \
                                               const DmxType7&, \
                                               const DmxType8&, \
                                               const DmxType9&, \
                                               const DmxType10&); \
            int dmxCustomFunctionStatus = DMX_CONCAT(functionName, Impl)(dmxCustomFunctionOutput, \
                                                                         variableName2, \
                                                                         variableName3, \
                                                                         variableName4, \
                                                                         variableName5, \
                                                                         variableName6, \
                                                                         variableName7, \
                                                                         variableName8, \
                                                                         variableName9, \
                                                                         variableName10); \
            *dmxIsOutputNullPtr = dmxCustomFunctionOutput.isNull(); \
            return dmxCustomFunctionStatus; \
        DMX_CUSTOM_FUNCTION_CATCH \
    } \
    extern "C" int DMX_CONCAT(functionName, Impl)(DmxType1& variableName1, \
                                                  const DmxType2& variableName2, \
                                                  const DmxType3& variableName3, \
                                                  const DmxType4& variableName4, \
                                                  const DmxType5& variableName5, \
                                                  const DmxType6& variableName6, \
                                                  const DmxType7& variableName7, \
                                                  const DmxType8& variableName8, \
                                                  const DmxType9& variableName9, \
                                                  const DmxType10& variableName10)

/******************************************************************************/
/* Custom function declaration */

#define DMX_CUSTOM_FUNCTION(functionName, ...) \
    DECLARE_DMX_CUSTOM_FUNCTION_NAME(functionName); \
    DMX_EXPAND_VA_ARGS( \
        DMX_CONCAT(DECLARE_DMX_CUSTOM_FUNCTION_VA_ARGS_, DMX_NUM_VA_ARGS(__VA_ARGS__))(functionName, __VA_ARGS__) \
    )

/******************************************************************************/
#endif /* #if !defined(__SSUPBUILD__) || defined(DMX_CUSTOM_FUNCTIONS_TEST) */

#endif /* #ifndef DMX_CUSTOM_FUNCTIONS_H */
