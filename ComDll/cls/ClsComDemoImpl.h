#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <sstream>

#include "interface/InfComDemo.h"
#include "ClsComInnerDef.h"

class ClsComDemoImpl
    // : public IDispatchImpl<InfComDemoEx, &__uuidof(InfComDemoEx), &LIBID_ClsComDemo>
    // ����ǲ���������д��������Ľӿ� ѡ��̳��� IDispatch ��֧��oleʱ����Ҫ��ôд
    // �������Ҫ֧�� ֱ��д : public InfComDemoEx�����ɣ�������ʱ ��Ҫ�ǵ� idl �ļ��еĽӿڶ��壬Ҫȥ�� ole ��֧�ֱ�Ǻ� dual���
    : public IDispatchImpl<InfComDemoEx> // ����͵����д��
    // ���  ClsComDemoImpl ��Ҫ�̳ж�� �ӿ� ��ʵ�֣���������Ҫ�̳� �Զ����� ÿ���ӿ� ��Ҫ �̳� IDispatch
    // ��Ҫ�̳�  IDispatchImpl<IXXX> ��IDispatchImpl<IYYY> ��ʵ�ֵȵ�
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