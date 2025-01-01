#ifndef __COMMON_DEFINE_IMPL_h
#define __COMMON_DEFINE_IMPL_h

#define SUCCEEDED_STRICT(hr) (S_OK == ((HRESULT)(hr)))
#define FAILED_STRICT(hr) (S_OK != ((HRESULT)(hr)))

#define DECLARE_COM_MY_INSTANCE_CREATER(cls)\
\
static HRESULT CreateMyInstance(\
CComPtr<cls>& pObj)\
{\
    cls* _pTarget = nullptr;\
    HRESULT hr = cls::CreateInstance(&_pTarget);\
    if (S_OK == hr)\
    {\
        pObj = _pTarget;\
    }\
    return hr;\
}

#endif // __COMMON_DEFINE_IMPL_h