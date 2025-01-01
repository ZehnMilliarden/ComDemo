#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>

#include "resource.h"

#include "ClsComInnerDef.h"
#include "ClsComDemo.h"

class ATL_NO_VTABLE ClsComAggDemo
    : public IUnknown
    , public CComObjectRootEx<CComMultiThreadModel>
    , public CComCoClass<ClsComAggDemo, &CLSID_ClsComAggDemo>
{

public:
    using ThisClass  = ClsComAggDemo;

public:
    ClsComAggDemo() = default;
    ~ClsComAggDemo() = default;

    // ������Ե���ģʽ����ʹ�ú�
    DECLARE_CLASSFACTORY_SINGLETON(ThisClass);
    // DECLARE_CLASSFACTORY()

    // �������Ҫע������Դ������Ϣ����ʹ�����º�
    // ��Ϊ UpdateRegistry ���� �Ǻ� OBJECT_ENTRY_AUTO �б����
    // DECLARE_NO_REGISTRY()
    // ����Ӧ��ʹ�ú�
    // DECLARE_REGISTRY()
    // DECLARE_REGISTRY_XXX() 
    DECLARE_REGISTRY_RESOURCEID(IDR_CLSCOMAGGDEMO)

    // Ҫ��������ɱ��ۺ�
    DECLARE_NOT_AGGREGATABLE(ThisClass)
    // ����������Ա��ۺ�
    // DECLARE_AGGREGATABLE(ThisClass)

    // �Զ����������, һ�㲻��Ҫ���� CComCoClass ����Ĭ��ʵ��
    // DECLARE_OBJECT_DESCRIPTION(XXX)

    // ������ȡ��ǰIUnknowָ��ķ�����ͨ���ھۺ�ģʽ����Ҫ
    DECLARE_GET_CONTROLLING_UNKNOWN()

    // ������Ҫ��ע��ǰ���󴴽����ͷ�ʱ�����ں��ʵ�ʱ���ô���ʧ��
    // �����ʹ�����º� ������ ������������ HRESULT FinalConstruct()
    // ���ͷŷ��� void FinalRelease() ʹ��
    // һ�����ڱ����ڲ��ۺ�������ü���
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(ThisClass)
        COM_INTERFACE_ENTRY(IUnknown)
        // ע������ۺ�ʱ Ӧ�þۺ� IUnknown �ӿ����Ͷ��󣬶���Ҫֱ�Ӿۺ� �ض������ӿڶ���
        // ������Ҫ���  FinalConstruct ��ķ���˵����
        COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IDispatch), m_pInnerComDemo)
        COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(InfComDemo), m_pInnerComDemo)
        COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(InfComDemoEx), m_pInnerComDemo)
    END_COM_MAP()

    // ��������� ���˴������������ ���󣬶���������ӿڵĶ���
    // DECLARE_COM_MY_INSTANCE_CREATER(ThisClass)

    HRESULT FinalConstruct()
    {
        // �ڴ����ӿڶ���ʱ������������� IUnknown���� ���Ƿ��ص�ָ���� CComAggObject<TargetClass> �Ķ����ָ��
        // �� TargetClass �����ʵ������ CComAggObject ��һ��������Ա��ͨ����������ѯ�ض��ӿڶ��󣬾�����TargetClass�Ķ���ӳ������ѯ��
        // �����һ��ʼ�����Ĳ��� IUnknown ָ�����Ͷ����򷵻ص��� ����� TargetClass �����ָ�룬
        // �����ָ��õ�IUnknown���Ͷ�����ʵ�Ǿۺ�Ŀ��������ͣ�Ҳ���Ǳ����IUnknownָ�롣
        // ������� �� ����CreateInstance ʱ�ʹ���IUnknown����ָ����оۺϣ������ڲ���ӳ���ʱ��_��ջ��ѭ���ݹ��쳣
        return ClsComDemo::CreateInstance(GetControllingUnknown(), &m_pInnerComDemo);
    }

    void FinalRelease()
    {
        m_pInnerComDemo = nullptr;
    }

private:
    CComPtr<IUnknown> m_pInnerComDemo = nullptr;
};

// DllGetClassObject -> CComModule::GetClassObject ->
// AtlComModuleGetClassObject ǰ�ò���δ�ҵ�������Ҵ˴�
OBJECT_ENTRY_AUTO(CLSID_ClsComAggDemo, ClsComAggDemo);