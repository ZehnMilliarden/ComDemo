#include "pch.h"

#include <iostream>
#include "ClsComDemoImpl.h"

ClsComDemoImpl::ClsComDemoImpl()
{
}

ClsComDemoImpl::~ClsComDemoImpl()
{
}

HRESULT STDMETHODCALLTYPE ClsComDemoImpl::Method1()
{
    std::wstringstream ss;
    ss << this << L" Method1\r\n";
    DumpInfo(ss);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ClsComDemoImpl::Method2()
{
    std::wstringstream ss;
    ss << this << L" Method2\r\n";
    DumpInfo(ss);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ClsComDemoImpl::Method3()
{
    std::wstringstream ss;
    ss << this << L" Method3\r\n";
    DumpInfo(ss);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ClsComDemoImpl::Method4(VARIANT szText)
{
    std::wstringstream ss;
    ss << this << szText.bstrVal << L"\r\n";
    DumpInfo(ss);
    return S_OK;
}

void ClsComDemoImpl::DumpInfo(const std::wstringstream& ss)
{
    std::wcout << ss.str() << std::endl;
    ::OutputDebugString(ss.str().c_str());
}
