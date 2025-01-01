// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "cls/ClsComDemo.h"
#include "cls/ClsComAggDemo.h"

// 72df6eb5-0675-42b7-a193-935811f55074
extern "C" const __declspec(selectany) GUID LIBID_ClsComDemoLib =
{ 0x72df6eb5, 0x0675, 0x42b7, { 0xa1, 0x93, 0x93, 0x58, 0x11, 0xf5, 0x50, 0x74 } };

class CClsComDemoModule : public ATL::CAtlDllModuleT<CClsComDemoModule>
{
public:
    DECLARE_LIBID(LIBID_ClsComDemoLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CLSCOMDEMO, "{6fc80bb0-4b65-4b99-97e2-1471d5a3bde9}")
};

CClsComDemoModule _AtlModule;

BEGIN_OBJECT_MAP(ObjMap)
    // DllGetClassObject -> CComModule::GetClassObject 优先查找此处
    OBJECT_ENTRY(CLSID_ClsComDemo, ClsComDemo)
    OBJECT_ENTRY(CLSID_ClsComAggDemo, ClsComAggDemo)
END_OBJECT_MAP()

_Use_decl_annotations_
STDAPI DllCanUnloadNow()
{
    return _AtlModule.DllCanUnloadNow();
}

_Use_decl_annotations_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

_Use_decl_annotations_
STDAPI DllRegisterServer(void)
{
    HRESULT hr = _AtlModule.RegisterServer(TRUE);
    if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

_Use_decl_annotations_
STDAPI DllUnregisterServer(void)
{
    HRESULT hr = _AtlModule.UnregisterServer(TRUE);
    if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return _AtlModule.DllMain(ul_reason_for_call, lpReserved);
}