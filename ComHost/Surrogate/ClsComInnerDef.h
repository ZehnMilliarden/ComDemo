#pragma once

#define DECLARE_COM_MY_INSTANCE_CREATER(cls)\
static HRESULT CreateMyInstance(CComPtr<CComObject<cls>>& pObj)\
{\
    CComObject<cls>* _pTarget = nullptr;\
    HRESULT hr = CComObject<cls>::CreateInstance(&_pTarget);\
    if (S_OK == hr)\
    {\
        pObj = _pTarget;\
    }\
    return hr;\
}\
static HRESULT CreateMyInstance(_Inout_opt_ LPUNKNOWN pUnkOuter, CComPtr<CComAggObject<cls>>& pObj)\
{\
    CComAggObject<cls>* _pTarget = nullptr;\
    HRESULT hr = CComAggObject<cls>::CreateInstance(pUnkOuter,&_pTarget);\
    if (S_OK == hr)\
    {\
        pObj = _pTarget;\
    }\
    return hr;\
}