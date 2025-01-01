#pragma once

#include <atlcom.h>
#include <atlbase.h>
#include <Windows.h>

#include "ComLib.h"

template<class T, const CLSID& clsid >
class LibEasyComLoader
{
public:
    LibEasyComLoader()
    {
        LibGetClassObject(clsid, __uuidof(IClassFactory), reinterpret_cast<LPVOID*>(&m_pClassFactory));
    }

    ~LibEasyComLoader()
    {
    }

    bool CreateInstance(CComPtr<T>& pInstance) {
        if (m_pClassFactory == nullptr) {
            return false;
        }

        HRESULT hr = m_pClassFactory->CreateInstance(
            nullptr,
            __uuidof(T),
            reinterpret_cast<LPVOID*>(&pInstance));

        return SUCCEEDED(hr);
    }

private:
    CComPtr<IClassFactory> m_pClassFactory;
};