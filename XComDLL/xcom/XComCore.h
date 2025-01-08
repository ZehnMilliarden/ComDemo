#ifndef __XCOM_CORE_H__
#define __XCOM_CORE_H__

#include <atomic>
#include <mutex>
#include "XComBase.h"

#define _XCOM_PACKING 8
#define xcomoffsetofclass(base, derived) ((unsigned long*)((unsigned long)(static_cast<base*>((derived*)_XCOM_PACKING))-_XCOM_PACKING))

typedef int (__stdcall _XCOM_CREATORFUNC)(
	void* pv,
	const XComGUID& riid,
	void** ppv);

typedef int(__stdcall _XCOM_CREATORARGFUNC)(
	void* pv,
	const XComGUID& riid,
	void** ppv,
    void* pCacheData);

#define _XCOM_NULLMAPENTRY ((_XCOM_CREATORARGFUNC*)0)
#define _XCOM_SIMPLEMAPENTRY ((_XCOM_CREATORARGFUNC*)1)

struct _XCOM_CACHEDATA {
    unsigned long nOffsetVar;
    _XCOM_CREATORFUNC* pCreatorFunc;
};

struct _XCOM_INTMAP_ENTRY {
	const XComGUID* piid;       // the interface id (IID)
	unsigned long* dw;
	_XCOM_CREATORARGFUNC* pFunc; //NULL:end, 1:offset, n:ptr
};

struct _XCOM_OBJMAP_ENTRY
{
	const XComGUID* pclsid;
	_XCOM_CREATORFUNC* pfnGetClassObject;
	_XCOM_CREATORFUNC* pfnCreateInstance;
	IXComUnknown* pCF;
};

inline int __stdcall XComInternalQueryInterface(void* pThis, _XCOM_INTMAP_ENTRY* pEntries, const XComGUID& riid, void** ppvObject) noexcept
{
    assert(nullptr != pEntries);
    assert(nullptr != pThis);

    if(pThis == NULL || pEntries == NULL)
	    return false;

    assert(_XCOM_SIMPLEMAPENTRY == pEntries->pFunc);
    if (nullptr == ppvObject)
    {
        return false;
    }

    if (IsEqualUnknownIID(riid))
    {
        IXComUnknown* pUnk = (IXComUnknown*)(unsigned long*)((unsigned long)pThis + (unsigned long)pEntries->dw);
        pUnk->AddRef();
        *ppvObject = pUnk;
        return true;
    }

    bool bRet = false;
    for(;;pEntries++)
    {
        if (nullptr == pEntries->pFunc)
        {
            break;
        }

        bool bBline = (nullptr == pEntries->piid);
        if (bBline || IsEqualGUID(riid, *(pEntries->piid)))
        {
            if (_XCOM_SIMPLEMAPENTRY == pEntries->pFunc)
            {
                assert(!bBline);
                IXComUnknown* pUnk = (IXComUnknown*)(unsigned long*)((unsigned long)pThis + (unsigned long)pEntries->dw);
                pUnk->AddRef();
                *ppvObject = pUnk;
                bRet = true;
                break;
            }

            bRet = pEntries->pFunc(pThis, riid, ppvObject, pEntries->dw);
            if (bRet)
            {
                break;
            }
            if (!bBline && !bRet)
            {
                break;
            }
        }
    }

    return bRet;
}

class XComModuleManger;

class XComModule {
public:
    XComModule();

    virtual ~XComModule() {
        
    }

    unsigned long Lock() noexcept {
        assert(-1L != m_nRef.load(), "-1L != m_nRef.load()");
        return m_nRef.fetch_add(1, std::memory_order_acq_rel) + 1;
    }

    unsigned long Unlock() noexcept {
        unsigned long l = m_nRef.fetch_sub(1, std::memory_order_acq_rel) - 1;
        assert(-1L != m_nRef.load(), "-1L != m_nRef.load()");
        return l;
    }

    unsigned long GetLockCount() const noexcept {
        return m_nRef.load();
    }

    int Init(_XCOM_OBJMAP_ENTRY* p) noexcept
    {
        _XCOM_OBJMAP_ENTRY* pEntry = nullptr;
        if ((_XCOM_OBJMAP_ENTRY*)-1 != p)
        {
            m_pObjMap = p;
            if (nullptr != m_pObjMap)
            {
                pEntry = m_pObjMap;
            }
        }

        return true;
    }

    void Term() noexcept
    {
        _XCOM_OBJMAP_ENTRY* pEntry = nullptr;
        if (nullptr != m_pObjMap)
        {
            pEntry = m_pObjMap;
            while(nullptr != pEntry->pclsid)
            {
                if (nullptr != pEntry->pCF)
                {
                    pEntry->pCF->Release();
                    pEntry->pCF = nullptr;
                }
                pEntry++;
            }
        }
    }

    int GetClassObject(const XComGUID& clsid,const XComGUID& riid, void** ppv) noexcept
    {
        *ppv = nullptr;
        int nRet = true;
        if (nullptr != m_pObjMap)
        {
            _XCOM_OBJMAP_ENTRY* pEntry = m_pObjMap;
            while(nullptr != pEntry->pclsid)
            {
                if ((nullptr != pEntry->pfnGetClassObject) && IsEqualGUID(clsid, *(pEntry->pclsid)))
                {
                    if (nullptr == pEntry->pCF)
                    {
                        std::lock_guard<std::mutex> lck(m_mtx);
                        if (nullptr == pEntry->pCF)
                        {
                            IXComUnknown* factory = NULL;
                            nRet = pEntry->pfnGetClassObject(pEntry->pfnCreateInstance, XComGuidOf<IXComUnknown>(), (void**)&factory);
                            if (nRet)
                            {
                                pEntry->pCF = factory;
                            }
                        }
                    }
                    
                    if (nullptr != pEntry->pCF)
                    {
                        nRet = pEntry->pCF->QueryInterface(riid, ppv);
                    }
                    break;
                }
                pEntry++;
            }
        }

        return nRet;
    }

    int DllCanUnloadNow() noexcept {
        return m_nRef.load() == 0;
    }

    int DllGetClassObject(const XComGUID& clsid, const XComGUID& riid, void** ppv) noexcept {
        return GetClassObject(clsid, riid, ppv);
    }

private:
    std::atomic_long m_nRef = 0;
    _XCOM_OBJMAP_ENTRY* m_pObjMap = nullptr;
    std::mutex m_mtx;
};

class XComModuleManger {
public:
    static XComModuleManger* GetInstance() {
        static XComModuleManger mgr;
        return &mgr;
    } 

    ~XComModuleManger() {
        
    }

    void AttachModule(XComModule* pModule) {
        m_pModule = pModule;
    }

    XComModule* GetModule() const {
        assert(nullptr != m_pModule, "nullptr != m_pModule");
        return m_pModule;
    }

    unsigned long Lock() noexcept {
        return GetModule()->Lock();
    }

    unsigned long Unlock() noexcept {
        return GetModule()->Unlock();
    }

private:
    XComModuleManger() {}
    
private:
    XComModule* m_pModule = nullptr;
};

XComModule::XComModule() {
    XComModuleManger::GetInstance()->AttachModule(this);
}

class XComModuleLockHelper {
public:
    XComModuleLockHelper() {
        XComModuleManger::GetInstance()->GetModule()->Lock();
    }
    ~XComModuleLockHelper() {
        XComModuleManger::GetInstance()->GetModule()->Unlock();
    }
};

#endif // __XCOM_CORE_H__