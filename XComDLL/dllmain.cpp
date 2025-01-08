// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "xcom/XCom.h"
#include "CXComDemo.h"

XComModule g_XComModule;

XCOM_BEGIN_OBJECT_MAP(ObjMap)
    XCOM_OBJECT_ENTRY(XComGUID_CXComDemo, CXComDemo)
XCOM_END_OBJECT_MAP()

int __stdcall XComDllCanUnloadNow()
{
    return g_XComModule.DllCanUnloadNow();
}

int __stdcall XComDllGetClassObject(const XComGUID& rclsid, const XComGUID& riid,  void** ppv)
{
    return g_XComModule.DllGetClassObject(rclsid, riid, ppv);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_XComModule.Init(ObjMap);
        break;
    case DLL_PROCESS_DETACH:
        g_XComModule.Term();
        break;
    }
    return TRUE;
}

