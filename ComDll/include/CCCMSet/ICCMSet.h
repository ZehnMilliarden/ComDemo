#ifndef __ICCMSet_h
#define __ICCMSet_h

#include <Windows.h>

MIDL_INTERFACE("23c51f20-c8c0-4c41-8bf9-47801f2e098d")
ICCMSet : IUnknown
{
    virtual unsigned int STDMETHODCALLTYPE GetDataCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE      GetData(
        unsigned int nIndex,
        IUnknown **  pData)
        = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDatas(
        unsigned int nOffset, 
        unsigned int nCount, 
        ICCMSet **pSet)
        = 0;
};

#endif // __ICCMSet_h