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

    // 如果想以单例模式可以使用宏
    // DECLARE_CLASSFACTORY_SINGLETON(ThisClass);
    DECLARE_CLASSFACTORY()

    // 如果不需要注册表的资源描述信息可以使用以下宏
    // 因为 UpdateRegistry 方法 是宏 OBJECT_ENTRY_AUTO 中必须的
    // DECLARE_NO_REGISTRY()
    // 否则应该使用宏DECLARE_REGISTRY()系统注册表中输入或删除主对象的条目(这个方法已不被支持了)
    // DECLARE_REGISTRY(ClsComDemo, _T("Demo.ClsComDemo.1"), _T("Demo.ClsComDemo"), (UINT)0, THREADFLAGS_APARTMENT)
    // 或者使用下面这两个宏使用 .rgs 脚本文件完成注册
    // DECLARE_REGISTRY_RESOURCE(_T("XXX.rgs")) 或者 DECLARE_REGISTRY_RESOURCEID(IDR_XXX)
    DECLARE_REGISTRY_RESOURCEID(IDR_CLSCOMDEMO)

    // 该宏定义了本类标准的创建实例方法
    // 通过 _CreatorClass 创建实现对接口对象的创建
    // 要求组件不可被聚合
    // DECLARE_NOT_AGGREGATABLE(ThisClass)
    // 声明组件可以被聚合
    DECLARE_AGGREGATABLE(ThisClass)

    // 自定义类的描述, 一般不需要，在 CComCoClass 中有默认实现
    // DECLARE_OBJECT_DESCRIPTION(XXX)

    // 声明获取当前IUnknow指针的方法，通常在聚合模式中需要
    // DECLARE_GET_CONTROLLING_UNKNOWN()

    // 当你需要关注当前对象创建和释放时，并在合适的时候让创建失败
    // 你可以使用如下宏 并搭配 创建创建方法 HRESULT FinalConstruct()
    // 和释放方法 void FinalRelease() 使用
    // 一般用于保护内部聚合组件引用计数
    // DECLARE_PROTECT_FINAL_CONSTRUCT()

    // 补充创建该类实例方法, 该方法会失去 DECLARE_AGGREGATABLE 等 宏定义的 特性
    // 如果想保留该特性可以使用 ThisClass::CreateInstance 方法来创建面向接口的实例
    // 如果 添加 DECLARE_NOT_AGGREGATABLE 的宏定义, ThisClass::CreateInstance(GetControllingUnknown(),Ptr); 就会报警说类不支持聚合
    DECLARE_COM_MY_INSTANCE_CREATER(ThisClass)

    BEGIN_COM_MAP(ClsComDemo)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(InfComDemo)
        COM_INTERFACE_ENTRY(InfComDemoEx)
    END_COM_MAP()
};

// DllGetClassObject -> CComModule::GetClassObject ->
// AtlComModuleGetClassObject 前置步骤未找到，则查找此处
OBJECT_ENTRY_AUTO(CLSID_ClsComDemo, ClsComDemo);