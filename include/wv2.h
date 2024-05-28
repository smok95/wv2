/*
## 0.0.14(14)	2022-12-13
- minimum WebView2 SDK version 1.0.774.44
	For full API compatibility, this version of the WebView2 SDK requires WebView2
	Runtime version 89.0.774.44 or higher.

## 0.1.0(15)	2023-08-08
- minimum WebView2 SDK version 1.0.774.44
	For full API compatibility, this version of the WebView2 SDK requires WebView2
	Runtime version 89.0.774.44 or higher.

## 0.1.1(16)	2023-08-09
- Fix Casting Error

## 0.2.0(17)	2024-04-18
- Added support for isMutedChangedEvent, isDocumentPlayingAudioEvent.
- Added support for OpenTaskManagerWindow.
- minimum WebView2 SDK version 	1.0.1072.54
	For full API compatibility, this Release version of the WebView2 SDK requires 
	WebView2 Runtime version 97.0.1072.54 or higher.
	https://learn.microsoft.com/en-us/microsoft-edge/webview2/release-notes/archive?tabs=dotnetcsharp

## 0.3.0(18)	2024-05-02
- Added support for browserProcessExited event.

## 0.4.0(19)	2024-05-03
- Change the structure of the 'wv2bool' structure

## 0.5.0(20)	2024-05-17
- Added partial support for the NewWindowRequested event.

## 0.6.0(21)	2024-05-20
- Added documentTitleChanged event handler support
- Removed the old wv2settings structure and setSettings function

## 0.7.0(22)	2024-05-21
- Changed domContentLoaded event type
- Added support for contentLoading, scriptDialogOpening events

## 0.8.0(23)	2024-05-22
- Added `getDeferral` and `windowFeatures` to `newWindowRequestedEventArgs`.
- Added `userAgent` and `setUserAgent` to `settings`

## 0.9.0(24)	2024-05-27
- Added partial support for the `downloadStarting` event.

## 0.10.0(25)	2024-05-27
- Added partial support for the `webResourceRequested` event.

## 0.11.0(26)	2024-05-28
- Added support for the `webResourceRequest.headers` property.
*/
#ifndef WEBVIEW2_C_WRAPPER_H_
#define WEBVIEW2_C_WRAPPER_H_

#define WV2_VERSION			"0.11.0"
#define WV2_VERSION_NUM		26

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include "wv2envOpts.h"

#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 
#define DEPRECATED(message) __declspec(deprecated(message))
#else
#ifdef __cplusplus
#define DEPRECATED(message) [[deprecated(message)]]
#else 
#define DEPRECATED(message)
#endif // __cplusplus
#endif


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

typedef void* wv2_t;
typedef void* wv2env_t;	// CoreWebView2Environment

// Structure representing the result and support status of a function
typedef struct wv2bool {
	// Result of the function execution
	bool value;
	// HRESULT of the function execution
	HRESULT hr;
} wv2bool;

///////////////////////////////////////////////////////////////////////////////
typedef void* wv2deferral_t; // ICoreWebView2Deferral
WV2_API HRESULT wv2deferral_complete(wv2deferral_t d);

WV2_API void wv2deleteDeferral(wv2deferral_t* d);
///////////////////////////////////////////////////////////////////////////////
typedef void* wv2settings_t; // ICoreWebView2Settings
WV2_API wv2bool 
wv2settings_isScriptEnabled(wv2settings_t s);

WV2_API wv2bool 
wv2settings_setIsScriptEnabled(wv2settings_t s, bool isScriptEnabled);

WV2_API wv2bool 
wv2settings_isWebMessageEnabled(wv2settings_t s);

WV2_API wv2bool 
wv2settings_setIsWebMessageEnabled(wv2settings_t s, bool isWebMessageEnabled);

WV2_API wv2bool 
wv2settings_areDefaultScriptDialogsEnabled(wv2settings_t s);

WV2_API wv2bool 
wv2settings_setAreDefaultScriptDialogsEnabled(wv2settings_t s, bool areDefaultScriptDialogsEnabled);

WV2_API wv2bool 
wv2settings_isStatusBarEnabled(wv2settings_t s);

WV2_API wv2bool 
wv2settings_setIsStatusBarEnabled(wv2settings_t s, bool isStatusBarEnabled);

WV2_API wv2bool 
wv2settings_areDevToolsEnabled(wv2settings_t s);

WV2_API wv2bool 
wv2settings_setAreDevToolsEnabled(wv2settings_t s, bool areDevToolsEnabled);

WV2_API wv2bool 
wv2settings_areDefaultContextMenusEnabled(wv2settings_t s);

WV2_API wv2bool 
wv2settings_setAreDefaultContextMenusEnabled(wv2settings_t s, bool enabled);

WV2_API wv2bool 
wv2settings_areHostObjectsAllowed(wv2settings_t s);

WV2_API wv2bool 
wv2settings_setAreHostObjectsAllowed(wv2settings_t s, bool allowed);

WV2_API wv2bool
wv2settings_isZoomControlEnabled(wv2settings_t s);

WV2_API wv2bool
wv2settings_setIsZoomControlEnabled(wv2settings_t s, bool enabled);

WV2_API wv2bool
wv2settings_isBuiltInErrorPageEnabled(wv2settings_t s);

WV2_API wv2bool 
wv2settings_setIsBuiltInErrorPageEnabled(wv2settings_t s, bool enabled);

WV2_API LPWSTR
wv2settings_userAgent(wv2settings_t s);

WV2_API HRESULT
wv2settings_setUserAgent(wv2settings_t s, LPCWSTR userAgent);

// @see COREWEBVIEW2_BROWSER_PROCESS_EXIT_KIND ( WebView2.h )
typedef enum wv2browserProcessExitKind {
	wv2browserProcessExitKindNormal = 0,
	wv2browserProcessExitKindFailed = (wv2browserProcessExitKindNormal + 1)
}wv2browserProcessExitKind;

// @see COREWEBVIEW2_SCRIPT_DIALOG_KIND
typedef enum wv2scriptDialogKind {
	wv2scriptDialogKind_undefined = -1,
	wv2scriptDialogKind_alert = 0,
	wv2scriptDialogKind_confirm = (wv2scriptDialogKind_alert + 1),
	wv2scriptDialogKind_prompt = (wv2scriptDialogKind_confirm + 1),
	wv2scriptDialogKind_beforeunload = (wv2scriptDialogKind_prompt + 1)
}wv2scriptDialogKind;


// @see COREWEBVIEW2_WEB_RESOURCE_CONTEXT
typedef enum wv2webResourceContext
{
	wv2webResourceContext_all = 0,
	wv2webResourceContext_document = (wv2webResourceContext_all + 1),
	wv2webResourceContext_stylesheet = (wv2webResourceContext_document + 1),
	wv2webResourceContext_image = (wv2webResourceContext_stylesheet + 1),
	wv2webResourceContext_media = (wv2webResourceContext_image + 1),
	wv2webResourceContext_font = (wv2webResourceContext_media + 1),
	wv2webResourceContext_script = (wv2webResourceContext_font + 1),
	wv2webResourceContext_xml_http_request = (wv2webResourceContext_script + 1),
	wv2webResourceContext_fetch = (wv2webResourceContext_xml_http_request + 1),
	wv2webResourceContext_text_track = (wv2webResourceContext_fetch + 1),
	wv2webResourceContext_event_source = (wv2webResourceContext_text_track + 1),
	wv2webResourceContext_websocket = (wv2webResourceContext_event_source + 1),
	wv2webResourceContext_manifest = (wv2webResourceContext_websocket + 1),
	wv2webResourceContext_signed_exchange = (wv2webResourceContext_manifest + 1),
	wv2webResourceContext_ping = (wv2webResourceContext_signed_exchange + 1),
	wv2webResourceContext_csp_violation_report = (wv2webResourceContext_ping + 1),
	wv2webResourceContext_other = (wv2webResourceContext_csp_violation_report + 1)
}wv2webResourceContext;

typedef struct {
	wv2browserProcessExitKind browserProcessExitKind;
	uint32_t browserProcessId;
}wv2browserProcessExitedEventArgs;

///////////////////////////////////////////////////////////////////////////////
typedef void* wv2windowFeatures_t; // ICoreWebView2WindowFeatures

WV2_API wv2bool 
wv2windowFeatures_hasPosition(wv2windowFeatures_t f);

WV2_API wv2bool 
wv2windowFeatures_hasSize(wv2windowFeatures_t f);

WV2_API uint32_t 
wv2windowFeatures_left(wv2windowFeatures_t f);

WV2_API uint32_t 
wv2windowFeatures_top(wv2windowFeatures_t f);

WV2_API uint32_t 
wv2windowFeatures_height(wv2windowFeatures_t f);

WV2_API uint32_t 
wv2windowFeatures_width(wv2windowFeatures_t f);

WV2_API wv2bool 
wv2windowFeatures_shouldDisplayMenuBar(wv2windowFeatures_t f);

WV2_API wv2bool 
wv2windowFeatures_shouldDisplayStatus(wv2windowFeatures_t f);

WV2_API wv2bool 
wv2windowFeatures_shouldDisplayToolbar(wv2windowFeatures_t f);

WV2_API wv2bool 
wv2windowFeatures_shouldDisplayScrollBars(wv2windowFeatures_t f);

///////////////////////////////////////////////////////////////////////////////
typedef void* wv2newWindowRequestedEventArgs_t; // ICoreWebView2NewWindowRequestedEventArgs

/*
@brief      Gets the target uri of the new window request.
@return     The target URI of the new window request.
@note       The returned LPWSTR must be freed using wv2freeMemory after use.
*/
WV2_API LPWSTR 
wv2newWindowRequestedEventArgs_uri(wv2newWindowRequestedEventArgs_t args);

/*
@brief		Indicates whether the NewWindowRequested event is handled by host.
*/
WV2_API bool 
wv2newWindowRequestedEventArgs_handled(wv2newWindowRequestedEventArgs_t args);

/*
@brief		Indicates whether the NewWindowRequested event is handled by host.
*/
WV2_API wv2bool 
wv2newWindowRequestedEventArgs_setHandled(wv2newWindowRequestedEventArgs_t args, bool handled);

/*
@brief		true when the new window request was initiated through a user 
			gesture such as selecting an anchor tag with target.
*/
WV2_API bool 
wv2newWindowRequestedEventArgs_isUserInitiated(wv2newWindowRequestedEventArgs_t args);

/*
@brief		Gets a wv2deferral_t object and put the event into a deferred state.
*/
WV2_API wv2deferral_t 
wv2newWindowRequestedEventArgs_getDeferral(wv2newWindowRequestedEventArgs_t args);

/*
@brief		Gets the window features specified by the window.open() call. 
			These features should be considered for positioning and sizing of new WebView windows.
*/
WV2_API wv2windowFeatures_t
wv2newWindowRequestedEventArgs_windowFeatures(wv2newWindowRequestedEventArgs_t args);
//wv2bool setNewWindow(ICoreWebView2* newWindow);
//ICoreWebView2* newWindow();

///////////////////////////////////////////////////////////////////////////////
typedef void* wv2domContentLoadedEventArgs_t; // ICoreWebView2DOMContentLoadedEventArgs

WV2_API uint64_t  wv2domContentLoadedEventArgs_navigationId(wv2domContentLoadedEventArgs_t args);
///////////////////////////////////////////////////////////////////////////////
typedef void* wv2contentLoadingEventArgs_t; // ICoreWebView2ContentLoadingEventArgs

WV2_API bool wv2contentLoadingEventArgs_isErrorPage(wv2contentLoadingEventArgs_t args);
WV2_API uint64_t wv2contentLoadingEventArgs_t_navigationId(wv2contentLoadingEventArgs_t args);

///////////////////////////////////////////////////////////////////////////////
typedef void* wv2scriptDialogOpeningEventArgs_t; // ICoreWebView2ScriptDialogOpeningEventArgs
WV2_API LPWSTR 
wv2scriptDialogOpeningEventArgs_uri(wv2scriptDialogOpeningEventArgs_t args);

WV2_API wv2scriptDialogKind 
wv2scriptDialogOpeningEventArgs_kind(wv2scriptDialogOpeningEventArgs_t args);

WV2_API LPWSTR 
wv2scriptDialogOpeningEventArgs_message(wv2scriptDialogOpeningEventArgs_t args);

WV2_API HRESULT 
wv2scriptDialogOpeningEventArgs_accept(wv2scriptDialogOpeningEventArgs_t args);

WV2_API LPWSTR 
wv2scriptDialogOpeningEventArgs_defaultText(wv2scriptDialogOpeningEventArgs_t args);

WV2_API LPWSTR 
wv2scriptDialogOpeningEventArgs_resultText(wv2scriptDialogOpeningEventArgs_t args);

WV2_API HRESULT 
wv2scriptDialogOpeningEventArgs_setResultText(wv2scriptDialogOpeningEventArgs_t args, LPCWSTR resultText);

WV2_API wv2deferral_t 
wv2scriptDialogOpeningEventArgs_getDeferral(wv2scriptDialogOpeningEventArgs_t args);

///////////////////////////////////////////////////////////////////////////////
typedef void* wv2downloadStartingEventArgs_t; // ICoreWebView2DownloadStartingEventArgs 

// HRESULT get_DownloadOperation(ICoreWebView2DownloadOperation** downloadOperation);
WV2_API bool 
wv2downloadStartingEventArgs_cancel(wv2downloadStartingEventArgs_t args);

WV2_API HRESULT
wv2downloadStartingEventArgs_setCancel(wv2downloadStartingEventArgs_t args, bool cancel);

WV2_API LPWSTR
wv2downloadStartingEventArgs_resultFilePath(wv2downloadStartingEventArgs_t args);

WV2_API HRESULT
wv2downloadStartingEventArgs_setResultFilePath(wv2downloadStartingEventArgs_t args, LPCWSTR resultFilePath);

WV2_API bool
wv2downloadStartingEventArgs_handled(wv2downloadStartingEventArgs_t args);

WV2_API HRESULT
wv2downloadStartingEventArgs_setHandled(wv2downloadStartingEventArgs_t args, bool handled);

WV2_API wv2deferral_t
wv2downloadStartingEventArgs_getDeferral(wv2downloadStartingEventArgs_t args);

///////////////////////////////////////////////////////////////////////////////
typedef void* wv2httpRequestHeaders_t; // ICoreWebView2HttpRequestHeaders

WV2_API LPWSTR 
wv2httpRequestHeaders_getHeader(wv2httpRequestHeaders_t handle, LPCWSTR name);

// HRESULT GetHeaders(LPCWSTR name, ICoreWebView2HttpHeadersCollectionIterator** iterator)

WV2_API bool
wv2httpRequestHeaders_contains(wv2httpRequestHeaders_t handle, LPCWSTR name);

WV2_API HRESULT
wv2httpRequestHeaders_setHeader(wv2httpRequestHeaders_t handle, LPCWSTR name, LPCWSTR value);

WV2_API HRESULT
wv2httpRequestHeaders_removeHeader(wv2httpRequestHeaders_t handle, LPCWSTR name);

// HRESULT GetIterator(ICoreWebView2HttpHeadersCollectionIterator** iterator)

///////////////////////////////////////////////////////////////////////////////
typedef void* wv2webResourceRequest_t; // ICoreWebView2WebResourceRequest

WV2_API LPWSTR
wv2webResourceRequest_uri(wv2webResourceRequest_t handle);

WV2_API HRESULT
wv2webResourceRequest_setUri(wv2webResourceRequest_t handle, LPCWSTR uri);

WV2_API LPWSTR
wv2webResourceRequest_method(wv2webResourceRequest_t handle);

WV2_API HRESULT
wv2webResourceRequest_setMethod(wv2webResourceRequest_t handle, LPCWSTR method);

// HRESULT get_Content(IStream** content)
// HRESULT put_Content(IStream* content)
// 
WV2_API wv2httpRequestHeaders_t
wv2webResourceRequest_headers(wv2webResourceRequest_t handle);
///////////////////////////////////////////////////////////////////////////////
typedef void* wv2webResourceResponse_t; // ICoreWebView2WebResourceResponse

// HRESULT get_Content(IStream** content)
// HRESULT put_Content(IStream* content)
// HRESULT get_Headers(ICoreWebView2HttpResponseHeaders** headers)

WV2_API int32_t 
wv2webResourceResponse_statusCode(wv2webResourceResponse_t handle);

WV2_API HRESULT
wv2webResourceResponse_setStatusCode(wv2webResourceResponse_t handle, int statusCode);

WV2_API LPWSTR
wv2webResourceResponse_reasonPhrase(wv2webResourceResponse_t handle);

WV2_API HRESULT
wv2webResourceResponse_setReasonPhrase(wv2webResourceResponse_t handle, LPCWSTR reasonPhrase);
///////////////////////////////////////////////////////////////////////////////
typedef void* wv2webResourceRequestedEventArgs_t; // ICoreWebView2WebResourceRequestedEventArgs

WV2_API wv2webResourceRequest_t
wv2webResourceRequestedEventArgs_request(wv2webResourceRequestedEventArgs_t args);

//WV2_API wv2webResourceResponse_t
//wv2webResourceRequestedEventArgs_response(wv2webResourceRequestedEventArgs_t args);

// HRESULT put_Response(ICoreWebView2WebResourceResponse* response)
// HRESULT GetDeferral(ICoreWebView2Deferral** deferral)
// HRESULT get_ResourceContext(COREWEBVIEW2_WEB_RESOURCE_CONTEXT* context)
///////////////////////////////////////////////////////////////////////////////

typedef void 
(*createCompleted)(wv2_t w, HRESULT errorCode, void* userData);

typedef void 
(*executeScriptCompleted)(wv2_t sender, LPCWSTR resultObjectAsJson);

typedef bool
(*navigationStarting)(wv2_t sender, LPCWSTR uri);

typedef void
(*navigationCompleted)(wv2_t sender);

typedef bool
(*windowCloseRequested)(wv2_t sender);

typedef void
(*historyChanged)(wv2_t sender, bool canGoBack, bool canGoForward);

typedef void 
(*webMessageReceived)(wv2_t sender, LPCWSTR message);

typedef void
(*isMutedChanged)(wv2_t sender);

typedef isMutedChanged isDocumentPlayingAudioChanged;

typedef void
(*browserProcessExited)(wv2env_t sender, wv2browserProcessExitedEventArgs* e);

typedef void
(*newWindowRequested)(wv2_t sender, wv2newWindowRequestedEventArgs_t args);

typedef navigationCompleted documentTitleChanged;

typedef void
(*domContentLoaded)(wv2_t sender, wv2domContentLoadedEventArgs_t args);

typedef void
(*contentLoading)(wv2_t sender, wv2contentLoadingEventArgs_t args);

typedef void
(*scriptDialogOpening)(wv2_t sender, wv2scriptDialogOpeningEventArgs_t args);

typedef void
(*downloadStarting)(wv2_t sender, wv2downloadStartingEventArgs_t args);

typedef void
(*webResourceRequested)(wv2_t sender, wv2webResourceRequestedEventArgs_t args);

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

WV2_API wv2env_t wv2getEnv(wv2_t w);

/*
@brief		Set an event handler for the browserProcessExited event.
@remark		Minimum WebView2 SDK version required: 	1.0.992.28
*/
WV2_API wv2bool wv2envSetBrowserProcessExitedHandler(wv2env_t e, 
	browserProcessExited handler);

WV2_API void wv2destroy(wv2_t* w);

WV2_API bool wv2setUserData(wv2_t w, void* userData);

WV2_API void* wv2getUserData(wv2_t w);

/*
@brief		Defines properties that enable, disable, or modify WebView features.
*/
WV2_API wv2settings_t wv2getSettings(wv2_t w);

WV2_API bool wv2setVirtualHostNameToFolderMapping(wv2_t w, LPCWSTR hostName, 
	LPCWSTR folderPath, wv2HostResourceAccessKind accessKind);

WV2_API bool wv2executeScript(wv2_t w, LPCWSTR script, executeScriptCompleted handler);

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

/*
@brief      Changes the size of the WebView2 window.
@param      w, A handle to the wv2.
@param      width, The new width of the window.
@param      height, The new height of the window.
@return     true if the resize operation was successful; otherwise, false.
*/
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
@brief		Set an event handler for the isMutedChanged event.
*/
WV2_API bool wv2setIsMutedChangedHandler(wv2_t w, isMutedChanged handler);

/*
@brief		Set an event handler for the isDocumentPlayingAudioChanged event.
*/
WV2_API bool wv2setIsDocumentPlayingAudioChangedHandler(wv2_t w, 
	isDocumentPlayingAudioChanged handler);

/*
@brief		Set an event handler for the newWindowRequested event.
*/
WV2_API wv2bool wv2setNewWindowRequestedHandler(wv2_t w, newWindowRequested handler);

/*
@brief		Set an event handler for the documentTitleChanged event.
*/
WV2_API wv2bool wv2setDocumentTitleChangedHandler(wv2_t w, documentTitleChanged handler);

/*
@brief		Set an event handler for the contentLoading event.
*/
WV2_API wv2bool wv2setContentLoadingHandler(wv2_t w, contentLoading handler);

/*
@brief		Set an event handler for the scriptDialogOpening event.
 */
WV2_API wv2bool wv2setScriptDialogOpningHandler(wv2_t w, scriptDialogOpening handler);

/*
@brief		Set an event handler for the downloadingStarting event.
@remark		Minimum WebView2 SDK version required: 1.0.902.49
*/
WV2_API wv2bool wv2setDownloadStartingHandler(wv2_t w, downloadStarting handler);

/*
@brief		Set an event handler for the webResourceRequested event.
*/
WV2_API wv2bool wv2setWebResourceRequestedHandler(wv2_t w, webResourceRequested handler);

/*		
@brief		Stop all navigations and pending resource fetches. Does not stop scripts.
*/
WV2_API bool wv2stop(wv2_t w);

/*
@brief     Get or set the zoom factor for the WebView.
@param     w,	The handle to the wv2 instance.
@param     newZoomFactor,	The new value for the zoom factor.
@return    The current zoom factor. Returns -1 if get/set operation fails.
*/
WV2_API double wv2zoomFactor(wv2_t w, const double* newZoomFactor);

/*
@brief     Get the mute status of the WebView.
@param     wv2_t w      The handle to the wv2.
@return    A wv2bool indicating whether the WebView is muted.
@remark    Minimum WebView2 SDK version required: 1.0.1072.54
*/
WV2_API wv2bool wv2isMuted(wv2_t w);

/*
@brief     Set the mute status of the WebView.
@param     wv2_t w      The handle to the wv2.
@param     muted        The new mute status to be set.
@return    A wv2bool indicating the success or failure of the operation.
@remark    Minimum WebView2 SDK version required: 1.0.1072.54
*/
WV2_API wv2bool wv2setIsMuted(wv2_t w, const bool muted);

/*
@brief     Check if the WebView is currently playing audio.
@param     wv2_t w      The handle to the wv2.
@return    A wv2bool indicating whether the WebView is playing audio.
@remark    Minimum WebView2 SDK version required: 1.0.1072.54
*/
WV2_API wv2bool wv2isDocumentPlayingAudio(wv2_t w);

/*
@brief		Opens the Browser Task Manager view as a new window in the foreground.
@param		wv2_t w      The handle to the wv2.
@return		
@remark		Minimum WebView2 SDK version required: 1.0.992.28
*/
WV2_API wv2bool wv2openTaskManagerWindow(wv2_t w);

/*
@brief		Gets the title for the current top-level document.
@param		wv2_t w      The handle to the wv2.
@return		
@note       The returned LPCWSTR must be freed using wv2freeMemory after use.
*/
WV2_API LPCWSTR wv2documentTitle(wv2_t w);

/*
@brief		Adds a URI and resource context filter for the WebResourceRequested event.
*/
WV2_API HRESULT wv2addWebResourceRequestedFilter(wv2_t w, 
	LPCWSTR uri, const wv2webResourceContext resourceContext);

/*
@brief		Removes a matching WebResource filter that was previously added for the WebResourceRequested event.
*/
WV2_API HRESULT wv2removeWebResourceRequestedFilter(wv2_t w, 
	LPCWSTR uri, const wv2webResourceContext resourceContext);


WV2_API HRESULT wv2lastError(wv2_t w);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
struct wv2env {
	virtual ~wv2env() {};

	virtual wv2bool setBrowserProcessExitedHandler(browserProcessExited handler) = 0;
};

struct wv2deferral {
	virtual ~wv2deferral() {};

	virtual HRESULT complete() = 0;
};

struct wv2settings {
	virtual wv2bool isScriptEnabled() = 0;
	virtual wv2bool setIsScriptEnabled(bool isScriptEnabled) = 0;
	virtual wv2bool isWebMessageEnabled() = 0;
	virtual wv2bool setIsWebMessageEnabled(bool isWebMessageEnabled) = 0;
	virtual wv2bool areDefaultScriptDialogsEnabled() = 0;
	virtual wv2bool setAreDefaultScriptDialogsEnabled(bool areDefaultScriptDialogsEnabled) = 0;
	virtual wv2bool isStatusBarEnabled() = 0;
	virtual wv2bool setIsStatusBarEnabled(bool isStatusBarEnabled) = 0;
	virtual wv2bool areDevToolsEnabled() = 0;
	virtual wv2bool setAreDevToolsEnabled(bool areDevToolsEnabled) = 0;
	virtual wv2bool areDefaultContextMenusEnabled() = 0;
	virtual wv2bool setAreDefaultContextMenusEnabled(bool enabled) = 0;
	virtual wv2bool areHostObjectsAllowed() = 0;
	virtual wv2bool setAreHostObjectsAllowed(bool allowed) = 0;
	virtual wv2bool isZoomControlEnabled() = 0;
	virtual wv2bool setIsZoomControlEnabled(bool enabled) = 0;
	virtual wv2bool isBuiltInErrorPageEnabled() = 0;
	virtual wv2bool setIsBuiltInErrorPageEnabled(bool enabled) = 0;

	// ICoreWebView2Settings2
	virtual LPWSTR userAgent() = 0;
	virtual HRESULT setUserAgent(LPCWSTR userAgent) = 0;
};

struct wv2windowFeatures {
	virtual ~wv2windowFeatures() {};
	virtual wv2bool hasPosition() = 0;
	virtual wv2bool hasSize() = 0;
	virtual uint32_t left() = 0;
	virtual uint32_t top() = 0;
	virtual uint32_t height() = 0;
	virtual uint32_t width() = 0;
	virtual wv2bool shouldDisplayMenuBar() = 0;
	virtual wv2bool shouldDisplayStatus() = 0;
	virtual wv2bool shouldDisplayToolbar() = 0;
	virtual wv2bool shouldDisplayScrollBars() = 0;
};

struct wv2newWindowRequestedEventArgs {
	virtual LPWSTR uri() = 0;
	virtual bool handled() = 0;
	virtual wv2bool setHandled(bool handled) = 0;
	virtual bool isUserInitiated() = 0;
	virtual wv2deferral* getDeferral() = 0;
	virtual wv2windowFeatures* windowFeatures() = 0;
};

struct wv2domContentLoadedEventArgs {
	virtual uint64_t navigationId() = 0;
};

struct wv2contentLoadingEventArgs {
	virtual bool isErrorPage() = 0;
	virtual uint64_t navigationId() = 0;
};

struct wv2scriptDialogOpeningEventArgs {
	virtual ~wv2scriptDialogOpeningEventArgs() {};
	virtual LPWSTR uri() = 0;
	virtual wv2scriptDialogKind kind() = 0;
	virtual LPWSTR message() = 0;
	virtual HRESULT accept() = 0;
	virtual LPWSTR defaultText() = 0;
	virtual LPWSTR resultText() = 0;
	virtual HRESULT setResultText(LPCWSTR resultText) = 0;
	virtual wv2deferral* getDeferral() = 0;
};

struct wv2downloadStartingEventArgs {
	// HRESULT get_DownloadOperation(ICoreWebView2DownloadOperation** downloadOperation);
	virtual bool cancel() = 0;
	virtual HRESULT setCancel(bool cancel) = 0;
	virtual LPWSTR resultFilePath() = 0;
	virtual HRESULT setResultFilePath(LPCWSTR resultFilePath) = 0;
	virtual bool handled() = 0;
	virtual HRESULT setHandled(bool handled) = 0;
	virtual wv2deferral* getDeferral() = 0;
};

struct wv2httpRequestHeaders {
	virtual LPWSTR getHeader(LPCWSTR name) = 0;
	virtual bool contains(LPCWSTR name) = 0;
	virtual HRESULT setHeader(LPCWSTR name, LPCWSTR value) = 0;
	virtual HRESULT removeHeader(LPCWSTR name) = 0;
};

struct wv2webResourceRequest {
	virtual LPWSTR uri() = 0;
	virtual HRESULT setUri(LPCWSTR uri) = 0;
	virtual LPWSTR method() = 0;
	virtual HRESULT setMethod(LPCWSTR method) = 0;
	virtual wv2httpRequestHeaders* headers() = 0;
};

struct wv2webResourceResponse {
	virtual int32_t statusCode() = 0;
	virtual HRESULT setStatusCode(int statusCode) = 0;
	virtual LPWSTR reasonPhrase() = 0;
	virtual HRESULT setReasonPhrase(LPCWSTR reasonPhrase) = 0;
};

struct wv2webResourceRequestedEventArgs {
	virtual wv2webResourceRequest* request() = 0;
	//virtual wv2webResourceResponse* response() = 0;
};


struct wv2 {
	virtual ~wv2(){};
	virtual void destroy() = 0;
	virtual void detach() = 0;

	virtual wv2settings* getSettings() = 0;
	
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

	virtual bool setIsMutedChangedHandler(isMutedChanged handler) = 0;

	virtual wv2bool isMuted() = 0;
	virtual wv2bool setIsMuted(const bool muted) = 0;

	virtual bool setIsDocumentPlayingAudioChangedHandler(
		isDocumentPlayingAudioChanged handler) = 0;

	virtual wv2bool isDocumentPlayingAudio() = 0;
	virtual wv2bool openTaskManagerWindow() = 0;

	virtual wv2env* getEnvironment() = 0;

	virtual wv2bool setNewWindowRequestedHandler(newWindowRequested handler) = 0;
	virtual wv2bool setDocumentTitleChangedHandler(documentTitleChanged handler) = 0;
	virtual wv2bool setContentLoadingHandler(contentLoading handler) = 0;

	virtual LPCWSTR documentTitle() = 0;
	virtual wv2bool setScriptDialogOpeningHandler(scriptDialogOpening handler) = 0;
	virtual wv2bool setDownloadingStartingHandler(downloadStarting handler) = 0;
	virtual wv2bool setWebResourceRequestedHandler(webResourceRequested handler) = 0;
	virtual HRESULT addWebResourceRequestedFilter(LPCWSTR uri, 
		const wv2webResourceContext resourceContext) = 0;
	virtual HRESULT removeWebResourceRequestedFilter(LPCWSTR uri,
		const wv2webResourceContext resourceContext) = 0;
};
#endif // __cplusplus


#endif // WEBVIEW2_C_WRAPPER_H_
