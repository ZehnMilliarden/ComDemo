#include "XCom/XCom.h"
#include "CXComDemo.h"

XComModule* g_pXComModule = nullptr;

XCOM_BEGIN_OBJECT_MAP(ObjMap)
    XCOM_OBJECT_ENTRY(XComGUID_CXComDemo, CXComDemo)
XCOM_END_OBJECT_MAP()

extern "C" {
    XCOMRESULT __attribute__((visibility("default"))) XCOMCALL XComCanUnloadNow()
    {
        return g_pXComModule->ModuleCanUnloadNow();
    }

    XCOMRESULT __attribute__((visibility("default"))) XCOMCALL XComGetClassObject(const XComGUID& rclsid, const XComGUID& riid,  void** ppv)
    {
        return g_pXComModule->ModuleGetClassObject(rclsid, riid, ppv);
    }
}

void 
__attribute__((constructor))
library_load() {
    g_pXComModule = new XComModule();
    g_pXComModule->Init(ObjMap);
}

void
__attribute__((destructor))
library_unload() {
    if (g_pXComModule)
    {
        g_pXComModule->Term();
        delete g_pXComModule;
        g_pXComModule = nullptr;
    }
}