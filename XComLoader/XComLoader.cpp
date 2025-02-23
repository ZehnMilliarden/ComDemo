// XComLoader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <atomic>
#include <memory>

#include "IXComDemo.h"
#include "xcom/XComLoader.h"

int main()
{

    std::shared_ptr<XComLoader<IXComDemo, XComGUID_CXComDemo>> loader =
        std::make_shared<XComLoader<IXComDemo, XComGUID_CXComDemo>>();
    loader->Load(L"XComDll.dll");

    XComPtr<IXComDemo> pInfComDemo;
    loader->CreateInstance(pInfComDemo);

    if (pInfComDemo)
    {
        pInfComDemo->Test();
    }

    XComPtr<IXComDemo2> pInfComDemo2 = nullptr;
    pInfComDemo2 = pInfComDemo;

    if (pInfComDemo2)
    {
        pInfComDemo2->Test2();
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
