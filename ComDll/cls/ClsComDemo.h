#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>

#include "resource.h"

#include "ClsComInnerDef.h"
#include "ClsComDemoImpl.h"

class ATL_NO_VTABLE ClsComDemo
    : public ClsComDemoImpl
    , public CComCoClass<ClsComDemo, &CLSID_ClsComDemo>
{
public:
    using ThisClass  = ClsComDemo;
    using ThisCoClass = CComObject<ThisClass>;
    using ThisCoAggClass = CComAggObject<ThisClass>;

public:
    ClsComDemo() = default;
    ~ClsComDemo() = default;

    // ������Ե���ģʽ����ʹ�ú�
    // DECLARE_CLASSFACTORY_SINGLETON(ThisClass);
    DECLARE_CLASSFACTORY()

    // �������Ҫע������Դ������Ϣ����ʹ�����º�
    // ��Ϊ UpdateRegistry ���� �Ǻ� OBJECT_ENTRY_AUTO �б����
    // DECLARE_NO_REGISTRY()
    // ����Ӧ��ʹ�ú�DECLARE_REGISTRY()ϵͳע����������ɾ�����������Ŀ(��������Ѳ���֧����)
    // DECLARE_REGISTRY(ClsComDemo, _T("Demo.ClsComDemo.1"), _T("Demo.ClsComDemo"), (UINT)0, THREADFLAGS_APARTMENT)
    // ����ʹ��������������ʹ�� .rgs �ű��ļ����ע��
    // DECLARE_REGISTRY_RESOURCE(_T("XXX.rgs")) ���� DECLARE_REGISTRY_RESOURCEID(IDR_XXX)
    DECLARE_REGISTRY_RESOURCEID(IDR_CLSCOMDEMO)

    // �ú궨���˱����׼�Ĵ���ʵ������
    // ͨ�� _CreatorClass ����ʵ�ֶԽӿڶ���Ĵ���
    // Ҫ��������ɱ��ۺ�
    // DECLARE_NOT_AGGREGATABLE(ThisClass)
    // ����������Ա��ۺ�
    DECLARE_AGGREGATABLE(ThisClass)

    // �Զ����������, һ�㲻��Ҫ���� CComCoClass ����Ĭ��ʵ��
    // DECLARE_OBJECT_DESCRIPTION(XXX)

    // ������ȡ��ǰIUnknowָ��ķ�����ͨ���ھۺ�ģʽ����Ҫ
    // DECLARE_GET_CONTROLLING_UNKNOWN()

    // ������Ҫ��ע��ǰ���󴴽����ͷ�ʱ�����ں��ʵ�ʱ���ô���ʧ��
    // �����ʹ�����º� ������ ������������ HRESULT FinalConstruct()
    // ���ͷŷ��� void FinalRelease() ʹ��
    // һ�����ڱ����ڲ��ۺ�������ü���
    // DECLARE_PROTECT_FINAL_CONSTRUCT()

    // ���䴴������ʵ������, �÷�����ʧȥ DECLARE_AGGREGATABLE �� �궨��� ����
    // ����뱣�������Կ���ʹ�� ThisClass::CreateInstance ��������������ӿڵ�ʵ��
    // ��� ��� DECLARE_NOT_AGGREGATABLE �ĺ궨��, ThisClass::CreateInstance(GetControllingUnknown(),Ptr); �ͻᱨ��˵�಻֧�־ۺ�
    DECLARE_COM_MY_INSTANCE_CREATER(ThisClass)

    BEGIN_COM_MAP(ClsComDemo)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(InfComDemo)
        COM_INTERFACE_ENTRY(InfComDemoEx)
    END_COM_MAP()
};

// DllGetClassObject -> CComModule::GetClassObject ->
// AtlComModuleGetClassObject ǰ�ò���δ�ҵ�������Ҵ˴�
OBJECT_ENTRY_AUTO(CLSID_ClsComDemo, ClsComDemo);