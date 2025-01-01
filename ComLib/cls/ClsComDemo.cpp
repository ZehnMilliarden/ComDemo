#include "pch.h"
#include "ClsComDemo.h"

ClsComDemo::ClsComDemo()
{

}

ClsComDemo::~ClsComDemo()
{

}

HRESULT STDMETHODCALLTYPE ClsComDemo::Method1()
{
    ::OutputDebugString(L"Method1\r\n");
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ClsComDemo::Method2()
{
    ::OutputDebugString(L"Method2\r\n");
    return S_OK;
}
