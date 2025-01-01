#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <memory>

template<class T, const CLSID& clsid >
class EasyComLoader 
{

    typedef HRESULT(__stdcall* CREATE_COM_OBJECT_FUNC)(REFCLSID, REFIID, LPVOID*);
    typedef HRESULT(__stdcall* DLL_CAN_UNLOAD_FUNC)();

public:
    EasyComLoader() {

    }

    ~EasyComLoader()
    {
        Unload();
    }

    bool Load(const wchar_t* szModule) 
    {
        if (m_hModule)
        {
            return true;
        }

        m_hModule = ::LoadLibraryW(szModule);

        if (m_hModule == nullptr)
        {
            return false;
        }

        
        CREATE_COM_OBJECT_FUNC pCreateFunc = reinterpret_cast<CREATE_COM_OBJECT_FUNC>(
            GetProcAddress(m_hModule, "DllGetClassObject"));

        m_pfDllCanUnload = reinterpret_cast<DLL_CAN_UNLOAD_FUNC>(
            GetProcAddress(m_hModule, "DllCanUnloadNow"));

        if (pCreateFunc == nullptr)
        {
            Unload();
            return false;
        }

        HRESULT hr = pCreateFunc(
            clsid, 
            IID_IClassFactory, 
            reinterpret_cast<LPVOID*>(&m_pClassFactory));

        if (FAILED(hr))
        {
            Unload();
            return false;
        }

        return true;
    }

    void Unload() 
    {
        if (m_pClassFactory)
        {
            m_pClassFactory.Release();
            m_pClassFactory = nullptr;
        }
        
        if (S_OK != m_pfDllCanUnload())
        {
            return;
        }

        if (m_hModule)
        {
            ::FreeLibrary(m_hModule);
            m_hModule = nullptr;
        }
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
    EasyComLoader(const EasyComLoader&) = delete;
    EasyComLoader& operator=(const EasyComLoader&) = delete;

private:
    HMODULE m_hModule = nullptr;
    CComPtr<IClassFactory> m_pClassFactory = nullptr;
    DLL_CAN_UNLOAD_FUNC m_pfDllCanUnload = nullptr;
};
