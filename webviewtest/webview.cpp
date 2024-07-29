#include "webview.h"
#include <codecvt>
#include <shlobj_core.h>
#include <xlocbuf>
#include "resource.h"

#pragma comment(lib, "Shell32.lib")

using namespace Microsoft::WRL;

wil::com_ptr<ICoreWebView2Controller> M_WEB_VIEW_CONTROLLER;
wil::com_ptr<ICoreWebView2> M_WEB_VIEW;

LRESULT CALLBACK window_proc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        {
            if (M_WEB_VIEW_CONTROLLER != nullptr)
            {
                RECT bounds;
                GetClientRect(hwnd, &bounds);
                M_WEB_VIEW_CONTROLLER->put_Bounds(bounds);
            }
            return 0;
        }
    case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void initialize_web_view(HWND hwnd)
{
    wil::unique_cotaskmem_string tempProfilePath;
    const HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &tempProfilePath);
    if (FAILED(hr))
    {
        MessageBox(hwnd, L"Failed to get local app data path.", L"Error", MB_OK);
        return;
    }

    std::wstring profilePath = tempProfilePath.get();
    profilePath.append(L"\\TempWebViewProfile");

    CreateCoreWebView2EnvironmentWithOptions(nullptr, profilePath.c_str(), nullptr,
                                             Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                                                 [hwnd](const HRESULT result, ICoreWebView2Environment* env) -> HRESULT
                                                 {
                                                     return on_create_environment_completed(result, env, hwnd);
                                                 }).Get());
}

HRESULT on_create_environment_completed(const HRESULT result, ICoreWebView2Environment* env, HWND hwnd)
{
    if (FAILED(result))
    {
        return result;
    }
    env->CreateCoreWebView2Controller(hwnd,
                                      Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                          [hwnd](const HRESULT resultNew,
                                                 ICoreWebView2Controller* controller) -> HRESULT
                                          {
                                              return on_create_core_web_view2_controller_completed(
                                                  resultNew, controller, hwnd);
                                          }).Get());
    return S_OK;
}

HRESULT on_create_core_web_view2_controller_completed(const HRESULT result, ICoreWebView2Controller* controller,
                                                      const HWND hwnd)
{
    if (FAILED(result))
    {
        return result;
    }
    M_WEB_VIEW_CONTROLLER = controller;
    M_WEB_VIEW_CONTROLLER->get_CoreWebView2(&M_WEB_VIEW);

    RECT bounds;
    GetClientRect(hwnd, &bounds);
    M_WEB_VIEW_CONTROLLER->put_Bounds(bounds);

    wil::com_ptr<ICoreWebView2Settings> settings;
    M_WEB_VIEW->get_Settings(&settings);
    settings->put_AreDefaultContextMenusEnabled(FALSE);
    settings->put_AreDevToolsEnabled(FALSE);

    M_WEB_VIEW->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                           [](ICoreWebView2* sender,
                                              ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                           {
                                               wil::unique_cotaskmem_string message;
                                               args->TryGetWebMessageAsString(&message);
                                               if (message.get() == std::wstring(L"ButtonClicked"))
                                               {
                                                   handle_button_click();
                                               }
                                               return S_OK;
                                           }).Get(), nullptr);

    const std::wstring htmlContent = load_html_resource(IDR_HTML1);
    M_WEB_VIEW->NavigateToString(htmlContent.c_str());

    return S_OK;
}

std::wstring load_html_resource(const UINT resourceId)
{
    return load_resource(resourceId, RT_HTML);
}

std::wstring load_resource(const UINT resourceId, const wchar_t* resourceType)
{
    const HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCE(resourceId), resourceType);
    if (!hResource)
    {
        return L"";
    }

    const HGLOBAL hLoadedResource = LoadResource(nullptr, hResource);
    if (!hLoadedResource)
    {
        return L"";
    }

    const LPVOID pLockedResource = LockResource(hLoadedResource);
    const DWORD dwResourceSize = SizeofResource(nullptr, hResource);
    if (!pLockedResource || dwResourceSize == 0)
    {
        return L"";
    }

    const std::string resourceContent(static_cast<char*>(pLockedResource), dwResourceSize);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wResourceContent = converter.from_bytes(resourceContent);

    return wResourceContent;
}

void handle_button_click()
{
    MessageBox(nullptr, L"Button was clicked!", L"Notification", MB_OK);
}
