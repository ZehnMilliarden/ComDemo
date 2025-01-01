#include <iostream>
#include <string>
#include <regex>

#include "Surrogate/CSurrogate.h"

class CSurrogateModule : public CAtlExeModuleT<CSurrogateModule> {};
CSurrogateModule _AtlModule;

void DllSurrogate()
{
    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    CComPtr<CComObject<CSurrogate>> pSurrogate;
    CComObject<CSurrogate>::CreateMyInstance(pSurrogate);
    CComPtr<CComObject<CSurrogate>> pSurrogateCopy = pSurrogate;

    HRESULT hr = ::CoRegisterSurrogate(pSurrogate);

    std::wstring commandLine = ::GetCommandLineW();
    std::wregex guidRegex(LR"(\{[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}\})");
    std::wsmatch match;

    if (std::regex_search(commandLine, match, guidRegex)) {
        std::wcout << L"Found GUID: " << match.str(0) << std::endl;

        GUID guid;
        hr = CLSIDFromString(match.str(0).c_str(), &guid);
        hr = pSurrogate->LoadDllServer(guid);

        MSG msg;
        while (::GetMessage(&msg, NULL, 0, 0))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }
    else {
        std::wcout << L"No GUID found in the command line." << std::endl;
    }

    ::CoUninitialize();
}

int main(int argc, char* argv[])
{
    DllSurrogate();
    return 0;
}