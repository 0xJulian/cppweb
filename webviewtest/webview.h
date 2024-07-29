#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <string>
#include <windows.h>
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>

LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void initialize_web_view(HWND hwnd);
HRESULT on_create_environment_completed(HRESULT result, ICoreWebView2Environment* env, HWND hwnd);
HRESULT on_create_core_web_view2_controller_completed(HRESULT result, ICoreWebView2Controller* controller, HWND hwnd);
std::wstring load_html_resource(UINT resourceId);
std::wstring load_resource(UINT resourceId, const wchar_t* resourceType);
void handle_button_click();

extern wil::com_ptr<ICoreWebView2Controller> M_WEB_VIEW_CONTROLLER;
extern wil::com_ptr<ICoreWebView2> M_WEB_VIEW;

#endif // WEBVIEW_H
