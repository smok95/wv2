# wv2
**Note:** This project is not yet stable, and API changes may occur frequently.

## Introduction
C/C++ wrapper for Microsoft Edge WebView2 Control

## Example

This example demonstrates a basic Win32 application that integrates Microsoft's WebView2 control to navigate to a web page.

```cpp
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   ...
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd) return FALSE;

   LPCWSTR url = L"https://www.youtube.com";
   
   if (webview = wv2createSync2(NULL, NULL, NULL, hWnd)) {
       wv2navigate(webview, url);
   } 

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
```

