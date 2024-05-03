#ifndef ICoreWebView2EnvironmentOptions_C_WRAPPER_H_
#define ICoreWebView2EnvironmentOptions_C_WRAPPER_H_

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

typedef void* wv2envOpts_t;
struct wv2envOpts;

WV2_API wv2envOpts_t 
wv2envOptsCreate();
WV2_API void 
wv2envOptsDestroy(wv2envOpts_t* opt);

/*
@brief		
@param		name, Specify one of the following:
			"AdditionalBrowserArguments", 
			"Language", 
			"TargetCompatibleBrowserVersion"
@return		
@see		https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2environmentoptions?view=webview2-1.0.774.44
@date		2023/08/08
*/
WV2_API LPWSTR 
wv2envOptsGetString(wv2envOpts_t h, LPCSTR name);

/*
@brief
@param		name, Specify one of the following:
			"AdditionalBrowserArguments",
			"Language",
			"TargetCompatibleBrowserVersion"
@return
@see		https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2environmentoptions?view=webview2-1.0.774.44
@date		2023/08/08
*/
WV2_API HRESULT 
wv2envOptsSetString(wv2envOpts_t h, LPCSTR name, LPCWSTR value);

/*
@brief
@param		name, Specify one of the following:
			"AllowSingleSignOnUsingOSPrimaryAccount"
@return
@see		https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2environmentoptions?view=webview2-1.0.774.44
@date		2023/08/08
*/
WV2_API BOOL 
wv2envOptsGetBool(wv2envOpts_t h, LPCSTR name);

/*
@brief
@param		name, Specify one of the following:
			"AllowSingleSignOnUsingOSPrimaryAccount"
@return
@see		https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2environmentoptions?view=webview2-1.0.774.44
@date		2023/08/08
*/
WV2_API HRESULT 
wv2envOptsSetBool(wv2envOpts_t h, LPCSTR name, BOOL value);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

struct wv2envOpts {
	virtual ~wv2envOpts() {};
	virtual void destroy() = 0;

	virtual LPWSTR getString(LPCSTR name) = 0;
	virtual HRESULT setString(LPCSTR name, LPCWSTR value) = 0;
	virtual BOOL getBool(LPCSTR name) = 0;
	virtual HRESULT setBool(LPCSTR name, BOOL value) = 0;
};

WV2_API wv2envOpts* wv2envOptsGetPtr(wv2envOpts_t h);
#endif // __cplusplus


#endif // WEBVIEW2_C_WRAPPER_H_


