#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>

#include "ClsComInnerDef.h"

class CSurrogate
    : public ISurrogate
    , public CComObjectRootEx<CComMultiThreadModel>
    , public CComCoClass<CSurrogate>
{
public:
    CSurrogate(void) = default;
    ~CSurrogate(void) = default;

    DECLARE_CLASSFACTORY()
    DECLARE_NO_REGISTRY()
    DECLARE_NOT_AGGREGATABLE(CSurrogate)
    DECLARE_GET_CONTROLLING_UNKNOWN()
    DECLARE_COM_MY_INSTANCE_CREATER(CSurrogate)

    BEGIN_COM_MAP(CSurrogate)
        COM_INTERFACE_ENTRY(IUnknown)
        COM_INTERFACE_ENTRY(ISurrogate)
    END_COM_MAP()

public: // ISurrogate
    virtual HRESULT STDMETHODCALLTYPE LoadDllServer(
        /* [in] */ __RPC__in REFCLSID Clsid) override;

    virtual HRESULT STDMETHODCALLTYPE FreeSurrogate(void) override;

private:
    DWORD m_cookie = 0;
    
};