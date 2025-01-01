#pragma once

#include <Windows.h>

// 2312b16a-7166-4838-8628-a914d96bef9a
extern "C" const __declspec(selectany) CLSID CLSID_ClsComDemo =
{ 0x2312b16a, 0x7166, 0x4838, { 0x86, 0x28, 0xa9, 0x14, 0xd9, 0x6b, 0xef, 0x9a } };

// 0bc65693-f9d3-450d-8794-732e964cadf8
extern "C" const __declspec(selectany) CLSID CLSID_ClsComAggDemo =
{ 0x0bc65693, 0xf9d3, 0x450d, { 0x87, 0x94, 0x73, 0x2e, 0x96, 0x4c, 0xad, 0xf8 } };

// 11a2102a-6a38-4868-955a-bce204179c9e
extern "C" const __declspec(selectany) CLSID CLSID_ClsComDemoLib =
{ 0x11a2102a, 0x6a38, 0x4868, { 0x95, 0x5a, 0xbc, 0xe2, 0x04, 0x17, 0x9c, 0x9e } };

MIDL_INTERFACE("eabd8132-1ac2-4cf2-99aa-e94d1189ea2b")
InfComDemo : IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE Method1() = 0;
    virtual HRESULT STDMETHODCALLTYPE Method2() = 0;
};

MIDL_INTERFACE("13fb16b2-25ff-4c83-a7fa-375107e00267")
InfComDemoEx : InfComDemo
{
    virtual HRESULT STDMETHODCALLTYPE Method3() = 0;
    virtual HRESULT STDMETHODCALLTYPE Method4(VARIANT szText) = 0;
};

MIDL_INTERFACE("f845e630-fde7-4442-9589-84ca9c3c457c")
InfComDemoLib : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Method1() = 0;
    virtual HRESULT STDMETHODCALLTYPE Method2() = 0;
};