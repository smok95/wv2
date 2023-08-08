/*
## 0.1.0(15)	2023-08-08
- minimum WebView2 SDK version 1.0.774.44
	For full API compatibility, this version of the WebView2 SDK requires WebView2
	Runtime version 89.0.774.44 or higher.

## 0.0.14(14)	2022-12-13
- minimum WebView2 SDK version 1.0.774.44
	For full API compatibility, this version of the WebView2 SDK requires WebView2
	Runtime version 89.0.774.44 or higher.
*/
#ifndef WEBVIEW2_C_WRAPPER_H_
#define WEBVIEW2_C_WRAPPER_H_

#include <windows.h>
#include <stdbool.h>
#include "wv2envOpts.h"

#ifdef _MSC_VER
#define DEPRECATED(message) [[deprecated(message)]]
#else
#define DEPRECATED(message)
#endif

#ifdef WV2_EXPORTS
#define WV2_API __declspec(dllexport)
#else
#define WV2_API __declspec(dllimport)
#endif

#define WV2_VERSION			"0.1.0"
#define WV2_VERSION_NUM		15

#ifdef __cplusplus
extern "C" {
#endif

// @see COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND ( WebView2.h )
#ifndef __wv2HostResourceAccessKind__DEFINED__
#define __wv2HostResourceAccessKind__DEFINED__
typedef enum wv2HostResourceAccessKind {
	wv2HostResourceAccessKindDeny = 0,
	wv2HostResourceAccessKindAllow = (wv2HostResourceAccessKindDeny + 1),
	wv2HostResourceAccessKindDenyCors = (wv2HostResourceAccessKindAllow + 1)
}wv2HostResourceAccessKind;
#endif // __wv2HostResourceAccessKind__DEFINED__

typedef void *wv2_t;

typedef struct wv2settings {
	bool isScriptEnabled;
	bool isWebMessageEnabled;
	bool areDefaultScriptDialogsEnabled;
	bool isStatusBarEnabled;
	bool areDevToolsEnabled;
	bool areDefaultContextMenusEnabled;
	bool areHostObjectsAllowed;
	bool isZoomControlEnabled;
	bool isBuiltInErrorPageEnabled;
}wv2settings;

typedef void (* createCompleted)(wv2_t w, HRESULT errorCode, void* userData);

typedef void (* executeScriptCompleted)(wv2_t sender, 
	LPCWSTR resultObjectAsJson);

typedef bool(* navigationStarting)(wv2_t sender, LPCWSTR uri);

typedef void(* navigationCompleted)(wv2_t sender);

typedef navigationCompleted domContentLoaded;

typedef bool(* windowCloseRequested)(wv2_t sender);

typedef void(* historyChanged)(wv2_t sender, bool canGoBack, 
	bool canGoForward);

typedef void (*webMessageReceived)(wv2_t sender, LPCWSTR message);

///////////////////////////////////////////////////////////////////////////////
WV2_API LPWSTR wv2getAvailableBrowserVersionString(LPCWSTR browserExecutableFolder);

DEPRECATED("wv2create deprecated. Use wv2create2 instead.")
WV2_API bool wv2create(LPCWSTR browserExecutableFolder, LPCWSTR userDataFolder, 
	HWND parentWindow, createCompleted handler, void* userData);

WV2_API bool wv2create2(LPCWSTR browserExecutableFolder, LPCWSTR userDataFolder,
	wv2envOpts_t environmentOptions, HWND parentWindow, 
	createCompleted handler, void* userData);

DEPRECATED("wv2createSync deprecated. Use wv2createSync2 instead.")
WV2_API wv2_t wv2createSync(LPCWSTR browserExecutableFolder, LPCWSTR userDataFolder, 
	HWND parentWindow);

WV2_API wv2_t wv2createSync2(LPCWSTR browserExecutableFolder, LPCWSTR userDataFolder,
	wv2envOpts_t environmentOptions, HWND parentWindow);

WV2_API void wv2destroy(wv2_t* w);

WV2_API bool wv2setUserData(wv2_t w, void* userData);

WV2_API void* wv2getUserData(wv2_t w);

WV2_API wv2settings* wv2getSettings(wv2_t w);
WV2_API bool wv2setSettings(wv2_t w, const wv2settings* settings);

WV2_API bool wv2setVirtualHostNameToFolderMapping(wv2_t w, LPCWSTR hostName, 
	LPCWSTR folderPath, wv2HostResourceAccessKind accessKind);

WV2_API bool wv2executeScript(wv2_t w, LPCWSTR script, 
	executeScriptCompleted handler);

WV2_API LPCWSTR wv2executeScriptSync(wv2_t w, LPCWSTR script);

WV2_API void wv2freeMemory(void* p);

/*		
@brief		The URI of the current top level document.
*/
WV2_API LPCWSTR wv2getSource(wv2_t w);

/*
@brief		Navigates the WebView to the previous page in the navigation history.
*/
WV2_API bool wv2goBack(wv2_t w);

/*
@brief		Navigates the WebView to the next page in the navigation history.
@param		
@return		
@date		2021/06/24
*/
WV2_API bool wv2goForward(wv2_t w);

/*
@brief		Causes a navigation of the top level document to the specified URI.
@param		w
@param		uri		The URI to navigate to.
@return		
@date		2022/12/13
*/
WV2_API bool wv2navigate(wv2_t w, const wchar_t* url);

/*
@brief		Initiates a navigation to htmlContent as source HTML of a new document.
			The htmlContent parameter may not be larger than 2 MB in total size. 
			The origin of the new page is `about:blank`.
*/
WV2_API bool wv2navigateToString(wv2_t w, const wchar_t* htmlContent);

/*
@brief		
@param		w, A handle to the wv2
@param		uri, URI parameter must be absolute URI. 
@param		method	
@param		postData
@param		byteSize
@param		headers, The headers string is the raw request header string 
			delimited by CRLF (optional in last header). 
@return		
@see		https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2environment2?view=webview2-1.0.1462.37#createwebresourcerequest
@date		2022/12/13
*/
WV2_API bool wv2navigateWithWebResource(wv2_t w, LPCWSTR uri,
	LPCWSTR method, BYTE* postData, size_t byteSize, LPCWSTR headers);

/*		
@brief		Reload the current page.
			This is similar to navigating to the URI of current top level 
			document including all navigation events firing and respecting 
			any entries in the HTTP cache. But, the back or forward history 
			are not modified.
*/
WV2_API bool wv2reload(wv2_t w);

// 화면사이즈 변경
WV2_API bool wv2resize(wv2_t w, int width, int height);

/*
@brief		Post the specified webMessage to the top level document in this WebView.
*/
WV2_API bool wv2postWebMessageAsJson(wv2_t w, LPCWSTR messageAsJson);

/*
@brief		Posts a message that is a simple string rather than a JSON string
			representation of a JavaScript object.
*/
WV2_API bool wv2postWebMessageAsString(wv2_t w, LPCWSTR messageAsString);


/*
@brief		Set an event handler for the historyChanged event.
*/
WV2_API bool wv2setHistoryChangedHandler(wv2_t w, historyChanged handler);     

/*
@brief		Set an event handler for the navigationStarting event.
*/
WV2_API bool wv2setNavigationStartingHandler(wv2_t w, navigationStarting handler);

/*
@brief		Set an event handler for the navigationCompleted event.
*/
WV2_API bool wv2setNavigationCompletedHandler(wv2_t w, navigationCompleted handler);

/*
@brief		Set an event handler for the domContentLoaded event.
*/
WV2_API bool wv2setDomContentLoadedHandler(wv2_t w, domContentLoaded handler);

/*
@brief		Set an event handler for the windowCloseRequested event.
*/
WV2_API bool wv2setWindowCloseRequestedHandler(wv2_t w, windowCloseRequested handler);

/*
@brief		Set an event handler for the webMessageReceived event.
 */
WV2_API bool wv2setWebMessageReceivedHandler(wv2_t w, webMessageReceived handler);

/*		
@brief		Stop all navigations and pending resource fetches. Does not stop scripts.
*/
WV2_API bool wv2stop(wv2_t w);

/*		
@brief		the zoom factor for the WebView
@param		wv2_t	wv2 handle
@param		newZoomFactor	변경할 zoomFactor value
@return		current zoomFactor, get/set실패시 -1리턴
*/
WV2_API double wv2zoomFactor(wv2_t w, const double* newZoomFactor);

WV2_API HRESULT wv2lastError(wv2_t w);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
struct wv2 {
	virtual ~wv2(){};
	virtual void destroy() = 0;
	virtual void detach() = 0;

	virtual wv2settings* getSettings() = 0;
	virtual bool setSettings(const wv2settings* settings) = 0;

	virtual bool executeScript(LPCWSTR script, executeScriptCompleted handler) = 0;
	virtual LPCWSTR executeScriptSync(LPCWSTR script) = 0;
	virtual LPCWSTR getSource() = 0;
	virtual bool goBack() = 0;
	virtual bool goForward() = 0;
	virtual bool navigate(const wchar_t* url) = 0;
	virtual bool navigateToString(const wchar_t* html) = 0;
	virtual bool reload() = 0;
	virtual bool resize(int width, int height) = 0;

	virtual bool setHistoryChangedHandler(historyChanged handler) = 0;
	virtual bool setNavigationStartingHandler(navigationStarting handler) = 0;
	virtual bool setNavigationCompletedHandler(navigationCompleted handler) = 0;
	virtual bool setDomContentLoadedHandler(domContentLoaded handler) = 0;
	virtual bool setWindowCloseRequestedHandler(windowCloseRequested handler) = 0;
	virtual bool setWebMessageReceivedHandler(webMessageReceived handler) = 0;

	virtual bool postWebMessageAsJson(LPCWSTR messageAsJson) = 0;
	virtual bool postWebMessageAsString(LPCWSTR messageAsString) = 0;

	virtual bool stop() = 0;
	virtual double zoomFactor(const double* newZoomFactor) = 0;

	virtual void* getUserData() = 0;
	virtual bool setUserData(void* userData) = 0;

	virtual bool setVirtualHostNameToFolderMapping(LPCWSTR hostName, 
		LPCWSTR folderPath, wv2HostResourceAccessKind accessKind) = 0;
	virtual void freeMemory(void* p) = 0;

	virtual bool navigateWithWebResource(LPCWSTR uri, LPCWSTR method,
		BYTE* postData, size_t byteSize, LPCWSTR headers) = 0;
};
#endif // __cplusplus


#endif // WEBVIEW2_C_WRAPPER_H_


