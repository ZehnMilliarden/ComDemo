#pragma once

#include <Windows.h>

HRESULT __stdcall LibInitialize(HMODULE hModule);
void __stdcall LibUninitialize(void);
HRESULT __stdcall LibGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv);