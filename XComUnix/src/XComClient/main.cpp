#include <memory>

#include "include/IXComDemo.h"
#include "XCom/XComLoader.h"

int main(int argc, char** argz)
{
    std::shared_ptr<XComLoader<IXComDemo, XComGUID_CXComDemo>> loader =
        std::make_shared<XComLoader<IXComDemo, XComGUID_CXComDemo>>();
    loader->Load(L"XComServer.so");

    XComPtr<IXComDemo> pInfComDemo;
    loader->CreateInstance(pInfComDemo);

    if (pInfComDemo)
    {
        pInfComDemo->Test();
    }

    XComPtr<IXComDemo2> pInfComDemo2;
    pInfComDemo2 = pInfComDemo;
    if (pInfComDemo2)
    {
        pInfComDemo2->Test2();
    }

    return 0;
}