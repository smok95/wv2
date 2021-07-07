#ifndef WEBVIEW2_C_WRAPPER_H_
#define WEBVIEW2_C_WRAPPER_H_

#include <windows.h>
#include <stdbool.h>

#ifdef WV2_EXPORTS
#define WV2_API __declspec(dllexport)
#else
#define WV2_API __declspec(dllimport)
#endif

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

typedef void (CALLBACK* createCompleted)(wv2_t w, HRESULT errorCode);

typedef void (CALLBACK* executeScriptCompleted)(wv2_t sender, 
	LPCWSTR resultObjectAsJson);

typedef bool(CALLBACK* navigationStarting)(wv2_t sender, LPCWSTR uri);

typedef void(CALLBACK* navigationCompleted)(wv2_t sender);

typedef navigationCompleted domContentLoaded;

typedef bool(CALLBACK* windowCloseRequested)(wv2_t sender);

typedef void(CALLBACK* historyChanged)(wv2_t sender, bool canGoBack, 
	bool canGoForward);

///////////////////////////////////////////////////////////////////////////////

WV2_API void wv2Create(LPCWSTR browserExecutableFolder, HWND parentWindow, 
	createCompleted handler, void* userData);

WV2_API wv2_t wv2CreateSync(LPCWSTR browserExecutableFolder, HWND parentWindow);

WV2_API void wv2Destroy(wv2_t* w);

WV2_API bool wv2SetUserData(wv2_t w, void* userData);

WV2_API void* wv2GetUserData(wv2_t w);

WV2_API bool wv2SetVirtualHostNameToFolderMapping(wv2_t w, LPCWSTR hostName, 
	LPCWSTR folderPath, wv2HostResourceAccessKind accessKind);

WV2_API bool wv2ExecuteScript(wv2_t w, LPCWSTR script, 
	executeScriptCompleted handler);

WV2_API LPCWSTR wv2ExecuteScriptSync(wv2_t w, LPCWSTR script);

WV2_API void wv2FreeMemory(void* p);

/*		
@brief		The URI of the current top level document.
*/
WV2_API LPCWSTR wv2GetSource(wv2_t w);

/*
@brief		Navigates the WebView to the previous page in the navigation history.
*/
WV2_API bool wv2GoBack(wv2_t w);

/*
@brief		Navigates the WebView to the next page in the navigation history.
@param		
@return		
@date		2021/06/24
*/
WV2_API bool wv2GoForward(wv2_t w);

WV2_API bool wv2Navigate(wv2_t w, const wchar_t* url);

/*
@brief		Initiates a navigation to htmlContent as source HTML of a new document.
			The htmlContent parameter may not be larger than 2 MB in total size. 
			The origin of the new page is `about:blank`.
*/
WV2_API bool wv2NavigateToString(wv2_t w, const wchar_t* htmlContent);

/*		
@brief		Reload the current page.
			This is similar to navigating to the URI of current top level 
			document including all navigation events firing and respecting 
			any entries in the HTTP cache. But, the back or forward history 
			are not modified.
*/
WV2_API bool wv2Reload(wv2_t w);

// 화면사이즈 변경
WV2_API bool wv2Resize(wv2_t w, int width, int height);

WV2_API bool wv2SetHistoryChangedHandler(wv2_t w, historyChanged handler);

WV2_API bool wv2SetNavigationStartingHandler(wv2_t w, 
	navigationStarting handler);

WV2_API bool wv2SetNavigationCompletedHandler(wv2_t w, 
	navigationCompleted handler);

WV2_API bool wv2SetDomContentLoadedHandler(wv2_t w, domContentLoaded handler);

WV2_API bool wv2SetWindowCloseRequestedHandler(wv2_t w, 
	windowCloseRequested handler);

/*		
@brief		Stop all navigations and pending resource fetches. Does not stop scripts.
*/
WV2_API bool wv2Stop(wv2_t w);

/*		
@brief		the zoom factor for the WebView
@param		wv2_t	wv2 handle
@param		newZoomFactor	변경할 zoomFactor value
@return		current zoomFactor, get/set실패시 -1리턴
*/
WV2_API double wv2ZoomFactor(wv2_t w, const double* newZoomFactor);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
struct wv2 {
	virtual ~wv2(){};
	virtual void destroy() = 0;
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

	virtual bool stop() = 0;
	virtual double zoomFactor(const double* newZoomFactor) = 0;

	virtual void* getUserData() = 0;
	virtual bool setUserData(void* userData) = 0;

	virtual bool setVirtualHostNameToFolderMapping(LPCWSTR hostName, 
		LPCWSTR folderPath, wv2HostResourceAccessKind accessKind) = 0;
	virtual void freeMemory(void* p) = 0;
};
#endif // __cplusplus


#endif // WEBVIEW2_C_WRAPPER_H_


