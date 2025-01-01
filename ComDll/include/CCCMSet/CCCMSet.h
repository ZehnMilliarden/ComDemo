#ifndef __CCCMSet_h
#define __CCCMSet_h

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <objbase.h>

#include <functional>
#include <iterator>
#include <list>
#include <mutex>
#include <vector>
#include <xutility>

#include "ComDef.h"
#include "ICCMSet.h"

template <class Base, const CLSID *pclsid = &CLSID_NULL>
class CCCMSet
    : public CComObjectRootEx<CComMultiThreadModel>,
      public ICCMSetInterface,
      public CComCoClass<CCCMSet<Base, pclsid>, pclsid>
{
    using ThisClass   = CCCMSet<Base, pclsid>;
    using ThisCoClass = CComObject<ThisClass>;

public:
    CCCMSet()
    {
    }

    virtual ~CCCMSet()
    {
    }

    DECLARE_PROTECT_FINAL_CONSTRUCT()
    DECLARE_NOT_AGGREGATABLE(ThisClass)
    BEGIN_COM_MAP(ThisClass)
    COM_INTERFACE_ENTRY(ICCMSetInterface)
    END_COM_MAP()
    DECLARE_COM_MY_INSTANCE_CREATER(ThisCoClass)

public: // Interface
    unsigned int STDMETHODCALLTYPE GetDataCount() override final
    {
        std::lock_guard<std::recursive_mutex> lck(m_lckData);
        return m_lstData.size();
    }

    HRESULT STDMETHODCALLTYPE GetData(unsigned int nIndex,
                                      IUnknown **  pData) override final
    {
        if (nIndex >= GetDataCount())
        {
            return E_INVALIDARG;
        }

        CComPtr<Base> _pData = nullptr;

        {
            std::lock_guard<std::recursive_mutex> lck(m_lckData);
            auto                                  it = m_lstData.begin();
            std::advance(it, nIndex);

            if (it == m_lstData.end())
            {
                return E_INVALIDARG;
            }

            _pData = (*it);
            if (_pData == nullptr)
            {
                return E_FAIL;
            }
        }

        return _pData->QueryInterface(__uuidof(IUnknown), (void **)pData);
    }

    HRESULT STDMETHODCALLTYPE
    GetDatas(
        unsigned int nOffset, 
        unsigned int nCount,
        ICCMSetInterface **pSet) override final
    {
        HRESULT hr = E_FAIL;

        do
        {
            CComPtr<ThisCoClass> pDataSet = NULL;
            hr = ThisCoClass::CreateMyInstance(pDataSet);
            if (hr != S_OK)
            {
                break;
            }

            {
                std::lock_guard<std::recursive_mutex> lck(m_lckData);

                hr = pDataSet->CopyFrom(nOffset, nCount, m_lstData);
                if (S_OK != hr)
                {
                    break;
                }
            }

            hr = pDataSet->QueryInterface(
                __uuidof(ICCMSetInterface),
                (LPVOID *)pSet);
        }
        while (false);

        return hr;
    }

public:
    HRESULT CopyFrom(unsigned int nOffset, unsigned int nCount,
                     const std::list<CAdapt<CComPtr<Base>>> &lstData)
    {

        if (nOffset >= lstData.size() || nCount <= 0
            || (nOffset + nCount) > lstData.size())
        {
            return E_INVALIDARG;
        }

        auto start = std::next(lstData.begin(), nOffset);
        auto end   = std::next(start, nCount);

        {
            std::lock_guard<std::recursive_mutex> lck(m_lckData);
            std::copy(start, end, std::back_inserter(m_lstData));
        }

        return S_OK;
    }

    HRESULT CopyFrom(unsigned int nOffset, unsigned int nCount,
                     const CComPtr<ThisCoClass> &data)
    {
        return data->CopyTo(nOffset, nCount, m_lstData);
    }

    HRESULT CopyFrom(unsigned int                            nOffset,
                     const std::list<CAdapt<CComPtr<Base>>> &lstData)
    {
        unsigned int nCount = lstData.size();
        if (nCount <= nOffset)
        {
            return E_INVALIDARG;
        }

        return CopyFrom(nOffset, nCount - nOffset, lstData);
    }

    HRESULT CopyFrom(unsigned int nOffset, const CComPtr<ThisCoClass> &data)
    {
        return data->CopyTo(nOffset, m_lstData);
    }

    HRESULT CopyTo(unsigned int nOffset, unsigned int nCount,
                   std::list<CAdapt<CComPtr<Base>>> &lstData)
    {

        if (nOffset >= GetDataCount() || 0 == nCount
            || (nOffset + nCount) > GetDataCount())
        {
            return E_INVALIDARG;
        }

        {
            std::lock_guard<std::recursive_mutex> lck(m_lckData);
            auto start = std::next(m_lstData.begin(), nOffset);
            auto end   = std::next(start, nCount);
            std::copy(start, end, std::back_inserter(lstData));
        }

        return S_OK;
    }

    HRESULT CopyTo(unsigned int                      nOffset,
                   std::list<CAdapt<CComPtr<Base>>> &lstData)
    {
        unsigned int nCount = GetDataCount();
        if (nCount <= nOffset)
        {
            return E_INVALIDARG;
        }

        return CopyTo(nOffset, nCount - nOffset, lstData);
    }

    HRESULT CopyTo(unsigned int nOffset, unsigned int nCount,
                   CComPtr<ThisCoClass> &data)
    {
        std::lock_guard<std::recursive_mutex> lck(m_lckData);
        return data->CopyFrom(nOffset, nCount, m_lstData);
    }

    HRESULT CopyTo(unsigned int nOffset, CComPtr<ThisCoClass> &data)
    {
        std::lock_guard<std::recursive_mutex> lck(m_lckData);
        return data->CopyFrom(nOffset, m_lstData);
    }

    void PushData(const CComPtr<Base> &pData)
    {
        if (!pData)
        {
            return;
        }

        std::lock_guard<std::recursive_mutex> lck(m_lckData);
        m_lstData.push_back(pData);
    }

    bool EnumData(const std::function<bool(const CComPtr<Base> &value)> &cb,
                  bool reverse = false)
    {
        if (GetDataCount() == 0)
        {
            return false;
        }

        bool bRet = false;

        std::list<CAdapt<CComPtr<Base>>> lstDataCopy;

        {
            std::lock_guard<std::recursive_mutex> lck(m_lckData);
            lstDataCopy = m_lstData;
        }

        if (reverse)
        {
            std::reverse(lstDataCopy.begin(), lstDataCopy.end());
        }

        typename std::list<CAdapt<CComPtr<Base>>>::iterator iterBegin
            = lstDataCopy.begin();
        typename std::list<CAdapt<CComPtr<Base>>>::iterator iterEnd
            = lstDataCopy.end();

        while (iterBegin != iterEnd)
        {
            bRet = cb(*iterBegin);
            if (bRet)
            {
                break;
            }
            ++iterBegin;
        }

        return bRet;
    }

    bool RemoveData(const CComPtr<Base> &value)
    {
        bool                                                bRet = false;
        std::lock_guard<std::recursive_mutex>               lck(m_lckData);
        typename std::list<CAdapt<CComPtr<Base>>>::iterator iter
            = m_lstData.begin();

        while (iter != m_lstData.end())
        {
            if (value == *iter)
            {
                bRet = true;
                break;
            }
            ++iter;
        }

        if (bRet)
        {
            m_lstData.erase(iter);
        }

        return bRet;
    }

    void Clear()
    {
        std::lock_guard<std::recursive_mutex> lck(m_lckData);
        m_lstData.clear();
    }

private:
    std::list<CAdapt<CComPtr<Base>>> m_lstData;
    std::recursive_mutex             m_lckData;
};

#endif // __CCCMSet_h