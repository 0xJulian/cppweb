# CppWeb

**CppWeb** is a sample project demonstrating how to create modern Windows desktop applications using C++ for the backend and HTML/CSS for the frontend. This project leverages the WebView2 control to embed web technologies within a native C++ application, providing a flexible and powerful way to build beautiful and responsive user interfaces.

## Features

- **C++ Backend**: Utilize the performance and capabilities of C++ for your application logic.
- **HTML & CSS Frontend**: Design your user interface with contemporary web technologies.
- **WebView2 Integration**: Seamlessly embed and display web content within your C++ application.
- **Dark Mode Support**: Built-in support for an immersive dark mode.
- **Layered Windows**: Apply transparency and opacity effects to your application window.

## Getting Started

Follow these steps to set up and run the CppWeb project:

1. **Clone the Repository:**

    ```sh
    git clone https://github.com/0xJulian/cppweb.git
    cd cppweb
    ```

2. **Open in Visual Studio:**

    - Open the `webviewtest.sln` solution file in Visual Studio 2022.

3. **Build the Project:**

    - Select `Build` from the menu and click `Build Solution`.

4. **Run the Application:**

    - Press `F5` to run the application.

## Example Code

Here is a snippet showing how to set up the main window and initialize the WebView2 control:

```cpp
#include <windows.h>
#include "webview.h"
#include <dwmapi.h>

#pragma comment(lib, "Dwmapi.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    constexpr wchar_t className[] = L"CppWeb Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = window_proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
    {
        return 0;
    }

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED,
        className,
        L"CppWeb",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr)
    {
        return 0;
    }

    // Set window opacity to 80%
    SetLayeredWindowAttributes(hwnd, 0, (255 * 80) / 100, LWA_ALPHA);

    // Enable dark mode
    BOOL useDarkMode = true;
    DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));

    ShowWindow(hwnd, nCmdShow);
    initialize_web_view(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
```
This code snippet sets up the main window with transparency effects and dark mode support, and initializes the WebView2 control to render HTML/CSS content.

## Contributing

Contributions are welcome! To contribute to the project, you can:

- **Submit a Pull Request**: For code changes or additions.
- **Open an Issue**: To discuss bugs, enhancements, or suggestions.

## License

CppWeb is licensed under the [MIT License]([LICENSE](https://github.com/0xJulian/cppweb/blob/main/LICENSE)). See the LICENSE file for more details.

## Additional Notes

- **WebView2 Requirements**: Ensure that WebView2 is installed on the target system or included with your application package. [WebView2](https://learn.microsoft.com/en-us/microsoft-edge/webview2/) is a runtime that must be installed separately.
- **Documentation**: Expand the documentation as needed to cover advanced usage, WebView2 API interactions, and more detailed build configurations.
- **Testing**: Include information on how to run tests or validate the application if applicable.

Feel free to adjust the details to better fit your needs or add any additional features and documentation as the project evolves!
