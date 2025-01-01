#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <sstream>

#include "interface/InfComDemo.h"
#include "ClsComInnerDef.h"

class ClsComDemoImpl
    // : public IDispatchImpl<InfComDemoEx, &__uuidof(InfComDemoEx), &LIBID_ClsComDemo>
    // 这个是部分完整的写法，当你的接口 选择继承自 IDispatch 来支持ole时就需要这么写
    // 如果不需要支持 直接写 : public InfComDemoEx，即可，不过此时 就要记得 idl 文件中的接口定义，要去除 ole 的支持标记和 dual标记
    : public IDispatchImpl<InfComDemoEx> // 这是偷懒的写法
    // 如果  ClsComDemoImpl 需要继承多个 接口 来实现，那这里需要继承 自动化的 每个接口 都要 继承 IDispatch
    // 都要继承  IDispatchImpl<IXXX> ，IDispatchImpl<IYYY> ，实现等等
    , public CComObjectRootEx<CComMultiThreadModel>
{
public:
    ClsComDemoImpl();
    ~ClsComDemoImpl();

public: //InfComDemo
    virtual HRESULT STDMETHODCALLTYPE Method1() override;
    virtual HRESULT STDMETHODCALLTYPE Method2() override;
public: //InfComDemoEx
    virtual HRESULT STDMETHODCALLTYPE Method3() override;
    virtual HRESULT STDMETHODCALLTYPE Method4(VARIANT szText) override;
private:
    void DumpInfo(const std::wstringstream& ss);
};