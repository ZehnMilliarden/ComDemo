
#include "CTestLoader.h"

#include <iostream>
#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>

#include "interface/InfComDemo.h"
#include "EasyComLoader.h"
#include "EasyComLibLoader.h"

void TestNormal()
{
    std::shared_ptr<EasyComLoader<InfComDemo, CLSID_ClsComDemo>> loader =
        std::make_shared<EasyComLoader<InfComDemo, CLSID_ClsComDemo>>();
    loader->Load(L"ComDll.dll");

    CComPtr<InfComDemo> pInfComDemo1;
    CAdapt<CComPtr<InfComDemo>> pInfComDemo2;
    loader->CreateInstance(pInfComDemo1);
    loader->CreateInstance(pInfComDemo2);

    CComPtr<InfComDemoEx> pInfComDemo3 = nullptr;
    pInfComDemo3 = pInfComDemo1;

    pInfComDemo1->Method1();
    pInfComDemo2->Method2();
    pInfComDemo3->Method3();
}

void TestAggNormal()
{
    std::shared_ptr<EasyComLoader<InfComDemo, CLSID_ClsComAggDemo>> loaderV2 =
        std::make_shared<EasyComLoader<InfComDemo, CLSID_ClsComAggDemo>>();
    loaderV2->Load(L"ComDll.dll");
    CComPtr<InfComDemo> pInfComDemo1;
    loaderV2->CreateInstance(pInfComDemo1);

    CComPtr<InfComDemoEx> pInfComDemo3 = nullptr;
    pInfComDemo3 = pInfComDemo1;

    pInfComDemo3->Method1();
    pInfComDemo3->Method2();
    pInfComDemo3->Method3();
}

void TestInprocServer()
{
    // ���� 1: ��ʼ�� COM ��
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize COM library. Error code = 0x"
            << std::hex << hr << std::endl;
        return ;
    }

    // ���� 2: ���� COM ����ʵ��
    CComPtr<InfComDemoEx> pObj;
    // hr = pObj.CoCreateInstance(CLSID_ClsComAggDemo, NULL, CLSCTX_INPROC_SERVER);
    hr = pObj.CoCreateInstance(CLSID_ClsComDemo, NULL, CLSCTX_INPROC_SERVER);
    if (FAILED(hr))
    {
        std::cout << "Failed to create COM object. Error code = 0x"
            << std::hex << hr << std::endl;
        CoUninitialize();
        return ;
    }

    // ���� 3: ʹ�� COM ����
    // ���磬������Ľӿ���һ����Ϊ DoSomething �ķ�����

    pObj->Method2();

    VARIANT var;
    VariantInit(&var);
    BSTR bStr = SysAllocString(L"From Remote");
    var.vt = VT_BSTR;
    var.bstrVal = bStr;
    hr = pObj->Method4(var);
    SysFreeString(bStr);
    VariantClear(&var);
    if (SUCCEEDED(hr))
    {
        std::cout << "Successfully called DoSomething()" << std::endl;
    }
    else
    {
        std::cout << "Failed to call DoSomething(). Error code = 0x"
            << std::hex << hr << std::endl;
    }

    OLECHAR* szMember = (wchar_t*)L"Method4";
    DISPID dispid;
    pObj->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispid);
    // ע������ĵ�һ������ʱ �����и��ӿڼ̳� IDispatch ʱ���з�������ʱ����ָ���ӿ�ID

    VARIANTARG varg[1];
    VariantInit(&varg[0]);
    BSTR bStrParam = SysAllocString(L"From Remote");
    if (!bStrParam) {
        std::cout << "SysAllocString failed." << std::endl;
        CoUninitialize();
        return ;
    }
    varg[0].vt = VT_BSTR;
    varg[0].bstrVal = bStrParam;

    // ���� DISPPARAMS �ṹ��
    DISPPARAMS dispparams;
    dispparams.cArgs = 1;
    dispparams.rgvarg = varg;
    dispparams.cNamedArgs = 0;
    dispparams.rgdispidNamedArgs = nullptr;

    // ���� Method4
    VARIANT result;
    VariantInit(&result);
    hr = pObj->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &result, nullptr, nullptr);
    if (FAILED(hr)) {
        std::cout << "Invoke failed. Error code = 0x" << std::hex << hr << std::endl;
        SysFreeString(bStrParam);
        CoUninitialize();
        return ;
    }

    // ����
    SysFreeString(bStrParam);
    VariantClear(&result);

    // ���� 4: �ͷ� COM ����
    // ʹ�� CComPtr��������Զ��ͷ�
    pObj = nullptr;

    // ���� 5: ȡ����ʼ�� COM ��
    CoUninitialize();
}

void TestLocalServer()
{
    // ���� 1: ��ʼ�� COM ��
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize COM library. Error code = 0x"
            << std::hex << hr << std::endl;
        return;
    }

    // ���� 2: ���� COM ����ʵ��
    CComPtr<InfComDemoEx> pObj;
    // hr = pObj.CoCreateInstance(CLSID_ClsComAggDemo, NULL, CLSCTX_LOCAL_SERVER);
    hr = pObj.CoCreateInstance(CLSID_ClsComDemo, NULL, CLSCTX_LOCAL_SERVER);
    if (FAILED(hr))
    {
        std::cout << "Failed to create COM object. Error code = 0x"
            << std::hex << hr << std::endl;
        CoUninitialize();
        return;
    }

    // ���� 3: ʹ�� COM ����
    // ���磬������Ľӿ���һ����Ϊ DoSomething �ķ�����

    pObj->Method2();

    VARIANT var;
    VariantInit(&var);
    BSTR bStr = SysAllocString(L"From Remote");
    var.vt = VT_BSTR;
    var.bstrVal = bStr;
    hr = pObj->Method4(var);
    SysFreeString(bStr);
    VariantClear(&var);
    if (SUCCEEDED(hr))
    {
        std::cout << "Successfully called DoSomething()" << std::endl;
    }
    else
    {
        std::cout << "Failed to call DoSomething(). Error code = 0x"
            << std::hex << hr << std::endl;
    }

    OLECHAR* szMember = (wchar_t*)L"Method4";
    DISPID dispid;
    pObj->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispid);
    // ע������ĵ�һ������ʱ �����и��ӿڼ̳� IDispatch ʱ���з�������ʱ����ָ���ӿ�ID

    VARIANTARG varg[1];
    VariantInit(&varg[0]);
    BSTR bStrParam = SysAllocString(L"From Remote");
    if (!bStrParam) {
        std::cout << "SysAllocString failed." << std::endl;
        CoUninitialize();
        return;
    }
    varg[0].vt = VT_BSTR;
    varg[0].bstrVal = bStrParam;

    // ���� DISPPARAMS �ṹ��
    DISPPARAMS dispparams;
    dispparams.cArgs = 1;
    dispparams.rgvarg = varg;
    dispparams.cNamedArgs = 0;
    dispparams.rgdispidNamedArgs = nullptr;

    // ���� Method4
    VARIANT result;
    VariantInit(&result);
    hr = pObj->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &result, nullptr, nullptr);
    if (FAILED(hr)) {
        std::cout << "Invoke failed. Error code = 0x" << std::hex << hr << std::endl;
        SysFreeString(bStrParam);
        CoUninitialize();
        return;
    }

    // ����
    SysFreeString(bStrParam);
    VariantClear(&result);

    // ���� 4: �ͷ� COM ����
    // ʹ�� CComPtr��������Զ��ͷ�
    pObj = nullptr;

    // ���� 5: ȡ����ʼ�� COM ��
    CoUninitialize();
}

void TestLibNormal()
{
#ifdef _DEBUG
#ifdef WIN32
#pragma comment(lib, "Debug/ComLib.lib")
#else
#pragma comment(lib, "x64/Debug/ComLib.lib")
#endif
#else
#ifdef WIN32
#pragma comment(lib, "Release/ComLib.lib")
#else
#pragma comment(lib, "x64/Release/ComLib.lib")
#endif
#endif 

    LibInitialize(::GetModuleHandleA(NULL));

    std::shared_ptr<LibEasyComLoader<InfComDemoLib, CLSID_ClsComDemoLib>> loader =
        std::make_shared<LibEasyComLoader<InfComDemoLib, CLSID_ClsComDemoLib>>();

    CComPtr<InfComDemoLib> pInfComDemo;
    loader->CreateInstance(pInfComDemo);

    pInfComDemo->Method1();
    pInfComDemo->Method2();

    LibUninitialize();
}

