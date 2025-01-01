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

    // 如果想以单例模式可以使用宏
    DECLARE_CLASSFACTORY_SINGLETON(ThisClass);
    // DECLARE_CLASSFACTORY()

    // 如果不需要注册表的资源描述信息可以使用以下宏
    // 因为 UpdateRegistry 方法 是宏 OBJECT_ENTRY_AUTO 中必须的
    // DECLARE_NO_REGISTRY()
    // 否则应该使用宏
    // DECLARE_REGISTRY()
    // DECLARE_REGISTRY_XXX() 
    DECLARE_REGISTRY_RESOURCEID(IDR_CLSCOMAGGDEMO)

    // 要求组件不可被聚合
    DECLARE_NOT_AGGREGATABLE(ThisClass)
    // 声明组件可以被聚合
    // DECLARE_AGGREGATABLE(ThisClass)

    // 自定义类的描述, 一般不需要，在 CComCoClass 中有默认实现
    // DECLARE_OBJECT_DESCRIPTION(XXX)

    // 声明获取当前IUnknow指针的方法，通常在聚合模式中需要
    DECLARE_GET_CONTROLLING_UNKNOWN()

    // 当你需要关注当前对象创建和释放时，并在合适的时候让创建失败
    // 你可以使用如下宏 并搭配 创建创建方法 HRESULT FinalConstruct()
    // 和释放方法 void FinalRelease() 使用
    // 一般用于保护内部聚合组件引用计数
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(ThisClass)
        COM_INTERFACE_ENTRY(IUnknown)
        // 注意这里聚合时 应该聚合 IUnknown 接口类型对象，而不要直接聚合 特定其他接口对象。
        // 这里需要配合  FinalConstruct 里的方法说明。
        COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IDispatch), m_pInnerComDemo)
        COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(InfComDemo), m_pInnerComDemo)
        COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(InfComDemoEx), m_pInnerComDemo)
    END_COM_MAP()

    // 这个方法是 用了创建完整的类的 对象，而不是面向接口的对象
    // DECLARE_COM_MY_INSTANCE_CREATER(ThisClass)

    HRESULT FinalConstruct()
    {
        // 在创建接口对象时，如果创建的是 IUnknown对象 ，那返回的指针是 CComAggObject<TargetClass> 的对象的指针
        // 而 TargetClass 对象的实例，是 CComAggObject 的一个容器成员。通过这个对象查询特定接口对象，就是在TargetClass的对象映射表里查询。
        // 而如果一开始创建的不是 IUnknown 指针类型对象，则返回的是 具体的 TargetClass 对象的指针，
        // 用这个指针得到IUnknown类型对象，其实是聚合目标对象类型，也就是本类的IUnknown指针。
        // 所以如果 不 是在CreateInstance 时就创建IUnknown类型指针进行聚合，后续在查找映射表时会歘先栈死循环递归异常
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
// AtlComModuleGetClassObject 前置步骤未找到，则查找此处
OBJECT_ENTRY_AUTO(CLSID_ClsComAggDemo, ClsComAggDemo);