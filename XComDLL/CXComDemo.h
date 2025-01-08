#pragma once

#include "IXComDemo.h"

#include "xcom/XCom.h"

class CXComDemo 
    : public IXComDemo
    , public IXComDemo2
    , public XCComObjectRootEx<XCComMultiThreadModel>
    , public XCComCoClass<CXComDemo, &XComGUID_CXComDemo>
{
public:
    CXComDemo() = default;
    ~CXComDemo() = default;

    XCOM_DECLARE_CLASSFACTORY()
    XCOM_DECLARE_AGGREGATABLE(CXComDemo)

    XCOM_BEGIN_MAP(CXComDemo)
        XCOM_INTERFACE_ENTRY(IXComDemo)
        XCOM_INTERFACE_ENTRY(IXComDemo2)
    XCOM_END_MAP()

public:
    virtual int __stdcall Test() { 
        return 0; 
    }

    virtual int __stdcall Test2() {
        return 0;
    }
};
