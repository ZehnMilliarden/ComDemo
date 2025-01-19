#ifndef __IXCOMDEMO_H__
#define __IXCOMDEMO_H__

#include "XCom/XComBase.h"

extern "C" static const XComGUID XComGUID_CXComDemo("674ed757-fdbb-481f-9f91-c01dfbec689f");

struct IXComDemo : public IXComUnknown
{
    virtual int XCOMCALL Test() = 0;
};
XCOM_DEFINE_GUID_FOR_TYPE(IXComDemo, "9db38791-dcc5-400d-bdfd-2ddb32394b82");

struct IXComDemo2 : public IXComUnknown
{
    virtual int XCOMCALL Test2() = 0;
};
XCOM_DEFINE_GUID_FOR_TYPE(IXComDemo2, "b100cbc3-79a8-4bb6-aa9f-7f92fbaf0d09");

#endif // __IXCOMDEMO_H__ 