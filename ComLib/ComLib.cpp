// ComLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include "ComLib.h"

#include "cls/ClsComDemo.h"

CComModule _AtlModule;
BEGIN_OBJECT_MAP(ObjMap)
    OBJECT_ENTRY(CLSID_ClsComDemoLib, ClsComDemo)
END_OBJECT_MAP()

#include <atomic>

std::atomic<int> g_cRef = 0;

HRESULT __stdcall LibInitialize(HMODULE hModule)
{
    HRESULT hr = E_FAIL;
    if (g_cRef.fetch_add(1) == 0)
    {
        hr = _AtlModule.Init(ObjMap, hModule);
        if (FAILED(hr))
        {
            g_cRef.fetch_sub(1);
        }
    }

    return S_OK;
}

void __stdcall LibUninitialize(void)
{
    if (SUCCEEDED(_AtlModule.DllCanUnloadNow()))
    {
        _AtlModule.Term();
        g_cRef.store(0);
    }
}

HRESULT __stdcall LibGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
{
    if (g_cRef.load() <= 0)
    {
        return E_FAIL;
    }
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}