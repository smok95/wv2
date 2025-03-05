#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.

#include <CommCtrl.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "resource.h"

#include "wv2.h"
#pragma comment(lib,"wv2")



#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
wv2_t webview = NULL;
HWND hStatusWnd = NULL; // statusBar
LPCWSTR testFilterUri = L"*youtube*";

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void GetErrorMessage(DWORD errorCode, LPWSTR buffer, DWORD bufferSize);

void OnIsMutedChanged(wv2_t sender);
void OnIsDocumentPlayingAudioChanged(wv2_t sender);
void OnBrowserProcessExited(wv2env_t sender, wv2browserProcessExitedEventArgs* e);
void OnNewWindowRequested(wv2_t sender, wv2newWindowRequestedEventArgs_t args);
void OnDocumentTitleChanged(wv2_t sender);
void OnDomContentLoaded(wv2_t sender, wv2domContentLoadedEventArgs_t args);
void OnScriptDialogOpening(wv2_t sender, wv2scriptDialogOpeningEventArgs_t args);
void OnDownloadStarting(wv2_t sender, wv2downloadStartingEventArgs_t args);
void OnWebResourceRequested(wv2_t sender, wv2webResourceRequestedEventArgs_t args);
void OnWebMessageReceived(wv2_t sender, LPCWSTR message);
void OnNavigationCompleted(wv2_t sender, wv2navigationCompletedEventArgs_t args);
void OnGetCookiesCompleted(wv2cookieManager_t sender, HRESULT result, wv2cookieList_t cookieList);
void OnAcceleratorKeyPressed(wv2_t sender, wv2acceleratorKeyPressedEventArgs_t args);

void NavigatePostExample();
void SetStatusText(LPCWSTR text);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EXAMPLEWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXAMPLEWIN32));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXAMPLEWIN32));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EXAMPLEWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   wv2settings_t settings = NULL;
   wv2envOpts_t options = wv2envOptsCreate();
   wv2envOptsSetString(options, "AdditionalBrowserArguments", L"--auto-open-devtools-for-tabs");

   LPCWSTR url = L"";   
   //url = L"https://www.w3schools.com/jsref/tryit.asp?filename=tryjsref_document_getelementbyid2";   
   //url = L"https://www.w3schools.com/jsref/tryit.asp?filename=tryjsref_alert";
   url = L"https://www.youtube.com";
   
   if (webview = wv2createSync2(NULL, NULL, options, hWnd)) {

       // set browserProcessExited event handler
       wv2env_t env = wv2getEnv(webview);
       if(env) {
           wv2environment_setBrowserProcessExitedHandler(env, OnBrowserProcessExited);
       }

       wv2setWebMessageReceivedHandler(webview, OnWebMessageReceived);

       // set isMutedChanged event handler
       wv2setIsMutedChangedHandler(webview, OnIsMutedChanged);

       // set isDocumentPlayingAudioChanged event handler
       wv2setIsDocumentPlayingAudioChangedHandler(webview, OnIsDocumentPlayingAudioChanged);

       // set newWindowRequested event handler
       wv2setNewWindowRequestedHandler(webview, OnNewWindowRequested);

       // set documentTitleChanged event handler
       wv2setDocumentTitleChangedHandler(webview, OnDocumentTitleChanged);

       // set domContentLoaded event handler
       wv2setDomContentLoadedHandler(webview, OnDomContentLoaded);

       // set scriptDialogOpening event handler
       wv2setScriptDialogOpningHandler(webview, OnScriptDialogOpening);

       // set downloadStarting event handler
       wv2setDownloadStartingHandler(webview, OnDownloadStarting);

       // set webResourceRequested event handler
       wv2setWebResourceRequestedHandler(webview, OnWebResourceRequested);

       wv2addWebResourceRequestedFilter(webview, testFilterUri, wv2webResourceContext_all);

       // set navigationCompleted event handler
       wv2setNavigationCompletedHandler(webview, OnNavigationCompleted);

       // set acceleratorKeyPressed event handler
       wv2setAcceleratorKeyPressedHandler(webview, OnAcceleratorKeyPressed);
       
       settings = wv2getSettings(webview);
       if(settings) {           
		   // Check if the zoom control feature is currently enabled.
		   if(wv2settings_isZoomControlEnabled(settings).value) {
			   // Disable the zoom control feature.
			   wv2settings_setIsZoomControlEnabled(settings, false);
		   }

           if(wv2settings_areDefaultScriptDialogsEnabled(settings).value) {
               wv2settings_setAreDefaultScriptDialogsEnabled(settings, false);
           }
                      
           //HRESULT hr = wv2settings_setUserAgent(settings, L"user-agent test");

       }

       // set cookie
       wv2cookieManager_t cookieMgr = wv2getCookieManager(webview);
       if (cookieMgr) {
           wv2cookie_t cookie = wv2cookieManager_createCookie(cookieMgr, L"MyCookieName", L"MyCookieValue", NULL, NULL);
           if (cookie) {
               wv2cookieManager_addOrUpdateCookie(cookieMgr, cookie);
                wv2cookie_destroy(&cookie);
           }

           cookie = wv2cookieManager_createCookie(cookieMgr, L"MyYouTubeCookieName", L"MyYouTubeCookieValue", L".youtube.com", NULL);
           if (cookie) {
               wv2cookieManager_addOrUpdateCookie(cookieMgr, cookie);
               wv2cookie_destroy(&cookie);
           }
       }
       
       wv2navigate(webview, url);
   }
   else {
       WCHAR errMsg[1024];
       GetErrorMessage(wv2lastError(NULL), errMsg, sizeof(errMsg));
       MessageBoxW(hWnd, errMsg, L"wv2createSync", MB_ICONERROR);
   }

   wv2envOptsDestroy(&options);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        hStatusWnd = CreateWindowEx(0, STATUSCLASSNAME, NULL, SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
            0, 0, 0, 0, hWnd, (HMENU)IDC_STATUSBAR, hInst, NULL);

		SendMessage(hStatusWnd, SB_SETTEXT, 0, (LPARAM)L"Ready");
		int parts[1] = {-1};
		SendMessage(hStatusWnd, SB_SETPARTS, 1, (LPARAM)parts);
    }break;
    case WM_SIZE: {
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		SendMessage(hStatusWnd, WM_SIZE, 0, MAKELONG(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top));

        RECT rcStatusbar;
        GetClientRect(hStatusWnd, &rcStatusbar);

        if (webview) {
            const int w = LOWORD(lParam);
            const int h = HIWORD(lParam) - (rcStatusbar.bottom - rcStatusbar.top);
            wv2resize(webview, w, h);
        }        

    }break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_NAVIGATE_WITH_POST:
                NavigatePostExample();
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        if (webview) {
            wv2destroy(&webview);
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void NavigatePostExample() {
    if (!webview) return;


	LPCWSTR script = L"document.getElementById(\"tryitLeaderboard\").style.backgroundColor = \"green\";";
	//wv2executeScriptSync(webview, script);

    wv2executeScript(webview, script, NULL);
    return;

    LPCSTR postData = "hello=world&this=is&wv2=post example";
    size_t byteSize = strlen(postData) * sizeof(char);
    LPCWSTR headers = L"Content-Type: application/x-www-form-urlencoded";
    wv2navigateWithWebResource(webview, L"https://www.w3schools.com/action_page.php",
        L"POST", (BYTE*)postData, byteSize, headers);
}

void OnIsMutedChanged(wv2_t sender) {
	wv2bool result = wv2isMuted(sender);
	if (FAILED(result.hr)) {
        if(result.hr == E_NOINTERFACE) {
            // The function is not supported.
        }
	}
	else {
		const bool isMuted = result.value;
		if (isMuted) {
			// The WebView is muted.
            SetStatusText(L"The WebView is muted.");
		}
		else {
			// The WebView is not muted.
            SetStatusText(L"The WebView is not muted.");
		}
	}
}

void OnIsDocumentPlayingAudioChanged(wv2_t sender) {
	// Check if the document in the WebView is currently playing audio
	wv2bool result = wv2isDocumentPlayingAudio(sender);

	// Check if the function is supported
	if (FAILED(result.hr)) {
        if(result.hr == E_NOINTERFACE) {
            // The function is not supported in the current version of WebView2.
        }
	}
	else {
		// Extract the result indicating whether the document is playing audio
		const bool isDocumentPlayingAudio = result.value;

		// Show a message box based on the result
		if (isDocumentPlayingAudio) {
			// The document in the WebView is playing audio.
            SetStatusText(L"The document in the WebView is playing audio.");
		}
		else {
			// The document in the WebView is not playing audio.
            SetStatusText(L"The document in the WebView is not playing audio.");
		}
	}
}

void OnBrowserProcessExited(wv2env_t sender, wv2browserProcessExitedEventArgs* e) {
    MessageBox(NULL, L"browserProcessExited", L"Process Status", MB_OK | MB_ICONWARNING);
}

void OnNewWindowRequested(wv2_t sender, wv2newWindowRequestedEventArgs_t args) {
    const int response = MessageBox(NULL,
        L"Do you want to allow the new window?",
        L"New Window Requested",
        MB_YESNO | MB_ICONQUESTION);

    const bool handled = response != IDYES;
    wv2newWindowRequestedEventArgs_setHandled(args, handled);
}

void OnDocumentTitleChanged(wv2_t sender) {

    WCHAR buf[2048];
    LPCWSTR title = wv2documentTitle(sender);
    wsprintf(buf, L"documentTitleChanged - %s", title);
    SetStatusText(buf);
    wv2freeMemory((void*)title);
}

void OnDomContentLoaded(wv2_t sender, wv2domContentLoadedEventArgs_t args) {
    WCHAR buf[2048];
    uint64_t navigationId = wv2domContentLoadedEventArgs_navigationId(args);
    wsprintf(buf, L"domContentLoaded: navigationId=%I64u", navigationId);
    SetStatusText(buf);
}

void OnScriptDialogOpening(wv2_t sender, wv2scriptDialogOpeningEventArgs_t args) {

    if(wv2scriptDialogOpeningEventArgs_kind(args) == wv2scriptDialogKind_alert) {        
        LPWSTR message = wv2scriptDialogOpeningEventArgs_message(args);
        MessageBox(NULL, message, L"Custom Alert", MB_OK|MB_ICONWARNING);
        wv2freeMemory((void*)message);
    }

    wv2deferral_t deferral = wv2scriptDialogOpeningEventArgs_getDeferral(args);
    if(deferral) {
        wv2deferral_complete(deferral);

        wv2deleteDeferral(&deferral);
    }
}

void OnDownloadStarting(wv2_t sender, wv2downloadStartingEventArgs_t args) {

    LPWSTR resultFilePath = wv2downloadStartingEventArgs_resultFilePath(args);
    if (resultFilePath) {

        // Hide the default download dialog.
        wv2downloadStartingEventArgs_setHandled(args, true);

        MessageBox(NULL, resultFilePath, L"downloadStarting", MB_OK | MB_ICONINFORMATION);

        wv2freeMemory((void*)resultFilePath);
    }
}

void OnWebResourceRequested(wv2_t sender, wv2webResourceRequestedEventArgs_t args) {
    wv2webResourceRequest_t request = wv2webResourceRequestedEventArgs_request(args);
    if (request) {
        wv2httpRequestHeaders_t headers = wv2webResourceRequest_headers(request);
        if (headers) {
            LPWSTR value = NULL;
            wv2httpRequestHeaders_setHeader(headers, L"testName", L"testValue");

            value = wv2httpRequestHeaders_getHeader(headers, L"testName");
			if (wcscmp(L"testValue", value) != 0) {
				MessageBox(NULL, L"The retrieved value does not match the expected value!", L"Error", MB_OK | MB_ICONERROR);
			}

            wv2freeMemory((void*)value);
        }

        LPCWSTR uri = wv2webResourceRequest_uri(request);
        if (uri) {
            MessageBox(NULL, uri, L"webResourceRequested", MB_OK | MB_ICONINFORMATION);

            wv2freeMemory((void*)uri);
        }
    }

	wv2webResourceResponse_t response = wv2webResourceRequestedEventArgs_response(args);
	if (response) {
		int32_t statusCode = wv2webResourceResponse_statusCode(response);
		LPWSTR reasonPhrase = wv2webResourceResponse_reasonPhrase(response);
		WCHAR buf[2048];
		wsprintf(buf, L"Status Code: %d, Reason Phrase: %s", statusCode, reasonPhrase);

		wv2freeMemory((void*)reasonPhrase);

        MessageBox(NULL, buf, L"webResourceRequested", MB_OK | MB_ICONINFORMATION);
	}


    wv2removeWebResourceRequestedFilter(webview, testFilterUri, wv2webResourceContext_all);
}

void OnWebMessageReceived(wv2_t sender, LPCWSTR message) {
	MessageBox(NULL, message, L"WebMessageReceived", MB_OK | MB_ICONINFORMATION);
}

void OnNavigationCompleted(wv2_t sender, wv2navigationCompletedEventArgs_t args) {
    const bool isSuccess = wv2navigationCompletedEventArgs_isSuccess(args);
    const uint64_t navId = wv2navigationCompletedEventArgs_navigationId(args);
    const wv2webErrorStatus status = wv2navigationCompletedEventArgs_webErrorStatus(args);
    WCHAR buf[2048];
    wsprintf(buf, L"NavicationCompleted, isSuccess: %d, navagationID: %d, webErrorStatus: %d",
        isSuccess, navId, status);

    SetStatusText(buf);

    wv2cookieManager_t cookieMgr = wv2getCookieManager(sender);
    wv2cookieManager_getCookies(cookieMgr, wv2getSource(sender), OnGetCookiesCompleted);
}

void OnGetCookiesCompleted(wv2cookieManager_t sender, HRESULT result, wv2cookieList_t cookieList) {
    UINT count = wv2cookieList_count(cookieList);
    for (UINT i = 0; i < count; i++) {
        wv2cookie_t cookie = wv2cookieList_getValueAtIndex(cookieList, i);
        if (cookie) {
            LPWSTR name = wv2cookie_name(cookie);
            wv2freeMemory(name);
            wv2cookie_destroy(&cookie);
        }
    }
}

void OnAcceleratorKeyPressed(wv2_t sender, wv2acceleratorKeyPressedEventArgs_t args) {
    const wv2KeyEventKind eventKind = wv2acceleratorKeyPressedEventArgs_keyEventKind(args);
    const uint32_t vKey = wv2acceleratorKeyPressedEventArgs_virtualKey(args);

    WCHAR buf[2048];
    wsprintf(buf, L"AcceleratorKeyPressed, event=%d, virtuak key: %d", eventKind, vKey);
    SetStatusText(buf);

    if (eventKind == wv2KeyEventKind_key_down && vKey == VK_ESCAPE) {
        SetStatusText(L"Esc pressed");
        wv2acceleratorKeyPressedEventArgs_setHandled(args, true);
    }
}

void GetErrorMessage(DWORD errorCode, LPWSTR buffer, DWORD bufferSize) {
	FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buffer,
		bufferSize,
		NULL
	);
}

void SetStatusText(LPCWSTR text) {
    if(!IsWindow(hStatusWnd)) return;
    SendMessage(hStatusWnd, SB_SETTEXT, 0, (LPARAM)text);
}
