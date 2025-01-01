#ifndef __CCCMSetView_h__
#define __CCCMSetView_h__


#include <atlbase.h>
#include <atlcom.h>
#include <vector>
#include <functional>

#include "ICCMSet.h"

template <class Data>
class CCCMSetView
{
public:
    CCCMSetView()
    {
    }

    ~CCCMSetView()
    {
    }

    CCCMSetView(ICCMSet *other)
    {
        m_pDataSet = other;
    }

    CCCMSetView(const CCCMSetView<Data> &other)
    {
        m_pDataSet = other.m_pDataSet;
    }

    CCCMSetView<Data> &operator=(ICCMSet *pOther)
    {
        m_pDataSet = pOther;
        return *this;
    }

    ICCMSet **operator&() const
    {
        ATLASSERT(m_pDataSet == NULL);
        return (ICCMSet **)&m_pDataSet;
    }

public:
    unsigned int GetDataCount()
    {
        if (!m_pDataSet)
        {
            return 0;
        }

        return m_pDataSet->GetDataCount();
    }

    HRESULT GetData(unsigned int nIndex, CComPtr<Data> &pData)
    {

        HRESULT hr = E_FAIL;

        do
        {

            if (!m_pDataSet)
            {
                break;
            }

            CComPtr<IUnknown> pTempData = nullptr;
            hr = m_pDataSet->GetData(nIndex, &pTempData);
            if (S_OK != hr)
            {
                break;
            }

            hr = pTempData->QueryInterface(__uuidof(Data), (void **)&pData);
        }
        while (false);

        return hr;
    }

    HRESULT GetDatas(unsigned int nOffset, unsigned int nCount,
                     CCCMSetView<Data> &pSet)
    {
        HRESULT hr = E_FAIL;

        do
        {

            if (!m_pDataSet)
            {
                break;
            }

            CComPtr<IComCrossModuleSet> pTempData
                = nullptr;
            hr = m_pDataSet->GetDatas(nOffset, nCount, &pTempData);
            if (S_OK != hr)
            {
                break;
            }

            pSet = pTempData;
        }
        while (false);

        return hr;
    }

    HRESULT GetDatas(std::vector<CAdapt<CComPtr<Data>>> &vctData)
    {
        HRESULT hr = E_FAIL;
        do
        {

            if (!m_pDataSet)
            {
                break;
            }

            int nCount = m_pDataSet->GetDataCount();
            vctData.reserve(nCount);

            for (int nIndex = 0; nIndex < nCount; ++nIndex)
            {
                CComPtr<Data> pData = nullptr;
                if (GetData(nIndex, pData) == S_OK)
                {
                    vctData.push_back(pData);
                }
            }

            hr = S_OK;
        }
        while (false);

        return hr;
    }

    HRESULT GetDatas(unsigned int nOffset, unsigned int nCount,
                     std::vector<CAdapt<CComPtr<Data>>> &vctData)
    {
        HRESULT hr = E_FAIL;
        do
        {

            if (!m_pDataSet)
            {
                break;
            }

            CCCMSetView<Data> tmpSet;
            hr = GetDatas(nOffset, nCount, tmpSet);
            if (hr != S_OK)
            {
                break;
            }

            hr = tmpSet.GetDatas(vctData);
        }
        while (false);

        return hr;
    }

    bool EnumData(const std::function<bool(const CComPtr<Data> &value)> &cb)
    {
        if (!cb)
        {
            return false;
        }

        std::vector<CAdapt<CComPtr<Data>>> _vctData;
        if (S_OK != GetDatas(_vctData))
        {
            return false;
        }

        for (size_t nIndex = 0; nIndex < _vctData.size(); ++nIndex)
        {
            if (cb(_vctData[nIndex]))
            {
                return true;
            }
        }

        return false;
    }

private:
    CComPtr<ICCMSet> m_pDataSet = nullptr;
};

#endif // __CCCMSetView_h__