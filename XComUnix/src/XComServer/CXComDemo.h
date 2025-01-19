#pragma once

#include "include/IXComDemo.h"

#include "XCom/XCom.h"

#include <stdio.h>

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
    virtual int XCOMCALL Test() { 
        printf("S_OK\n");
        return 0; 
    }

    virtual int XCOMCALL Test2() { 
        printf("S_OK 2\n");
        return 0; 
    }
};
