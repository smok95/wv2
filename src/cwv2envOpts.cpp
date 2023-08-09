#include "cwv2envOpts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

LPCSTR AdditionalBrowserArguments = "AdditionalBrowserArguments";
LPCSTR Language = "Language";
LPCSTR TargetCompatibleBrowserVersion = "TargetCompatibleBrowserVersion";
LPCSTR AllowSingleSignOnUsingOSPrimaryAccount = "AllowSingleSignOnUsingOSPrimaryAccount";

#define CORE_WEBVIEW_TARGET_PRODUCT_VERSION L"89.0.774.44"

HRESULT _getStr(AutoCoMemStr& src, LPWSTR* value);
HRESULT _putStr(AutoCoMemStr& dest, LPCWSTR value);
/// ///////////////////////////////////////////////////////////////////////////

cwv2envOpts::cwv2envOpts():allowSingleSignOnUsingOSPrimaryAccount_(FALSE), 
refCount_(0) {
	AddRef();

	put_TargetCompatibleBrowserVersion(CORE_WEBVIEW_TARGET_PRODUCT_VERSION);
}

cwv2envOpts::~cwv2envOpts() {
	Release();
}

void cwv2envOpts::destroy() {	
	Release();
}

STDMETHODIMP cwv2envOpts::QueryInterface(REFIID riid, void** ppv) {
	// Always set out parameter to NULL, validating it first.
	if (!ppv) {
		return E_INVALIDARG;
	}
	*ppv = NULL;
	if (riid == IID_IUnknown || 
		riid == IID_ICoreWebView2EnvironmentOptions) {
		// Increment the reference count and return the pointer.

		*ppv = reinterpret_cast<ICoreWebView2EnvironmentOptions*>(this);
		//*ppv = (LPVOID)this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE cwv2envOpts::AddRef() {
	InterlockedIncrement(&refCount_);
	return refCount_;
}

ULONG STDMETHODCALLTYPE cwv2envOpts::Release() {
	// Decrement the object's internal counter.
	ULONG ulRefCount = InterlockedDecrement(&refCount_);
	if (0 == refCount_) {
		delete this;
	}
	return ulRefCount;
}

HRESULT _getStr(AutoCoMemStr& src, LPWSTR* value) {
	if (!value)	return E_POINTER;
	*value = src.Copy();
	if ((*value == nullptr) && (src.Get() != nullptr)) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

HRESULT _putStr(AutoCoMemStr& dest, LPCWSTR value) {
	LPCWSTR result = dest.Set(value);
	if ((result == nullptr) && (value != nullptr)) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

STDMETHODIMP cwv2envOpts::get_AdditionalBrowserArguments(LPWSTR* value) {
	return _getStr(additionalBrowserArguments_, value);
}
STDMETHODIMP cwv2envOpts::put_AdditionalBrowserArguments(LPCWSTR value) {
	return _putStr(additionalBrowserArguments_, value);
}

STDMETHODIMP cwv2envOpts::get_Language(LPWSTR* value) {
	return _getStr(language_, value);
}
STDMETHODIMP cwv2envOpts::put_Language(LPCWSTR value) {
	return _putStr(language_, value);
}

STDMETHODIMP cwv2envOpts::get_TargetCompatibleBrowserVersion(LPWSTR* value) {
	return _getStr(targetCompatibleBrowserVersion_, value);
}

STDMETHODIMP cwv2envOpts::put_TargetCompatibleBrowserVersion(LPCWSTR value) {
	return _putStr(targetCompatibleBrowserVersion_, value);
}

STDMETHODIMP cwv2envOpts::get_AllowSingleSignOnUsingOSPrimaryAccount(BOOL* allow) {	
	if (!allow) return E_POINTER;
	*allow = allowSingleSignOnUsingOSPrimaryAccount_;
	return S_OK;
}

STDMETHODIMP cwv2envOpts::put_AllowSingleSignOnUsingOSPrimaryAccount(BOOL allow) {
	allowSingleSignOnUsingOSPrimaryAccount_ = allow;
	return S_OK;
}

LPWSTR cwv2envOpts::getString(LPCSTR name) {
	if (!name) return nullptr;

	LPWSTR value = nullptr;
	LPWSTR result = nullptr;
	HRESULT hr = S_FALSE;
	if (_stricmp(AdditionalBrowserArguments, name) == 0) {
		hr = get_AdditionalBrowserArguments(&value);
	}
	else if (_stricmp(Language, name) == 0) {
		hr = get_Language(&value);
	}
	else if (_stricmp(TargetCompatibleBrowserVersion, name) == 0) {
		hr = get_TargetCompatibleBrowserVersion(&value);
	}

	if (SUCCEEDED(hr) && value) {
		result = _wcsdup(value);
		::CoTaskMemFree(value);
	}

	return result;
}

HRESULT cwv2envOpts::setString(LPCSTR name, LPCWSTR value) {
	if (!name) return E_INVALIDARG;

	HRESULT hr = S_FALSE;
	if (_stricmp(AdditionalBrowserArguments, name) == 0) {
		hr = put_AdditionalBrowserArguments(value);
	}
	else if (_stricmp(Language, name) == 0) {
		hr = put_Language(value);
	}
	else if (_stricmp(TargetCompatibleBrowserVersion, name) == 0) {
		hr = put_TargetCompatibleBrowserVersion(value);
	}
	return hr;
}

BOOL cwv2envOpts::getBool(LPCSTR name) {
	if (!name) return FALSE;

	HRESULT hr = S_FALSE;
	BOOL ret = FALSE;
	if (_stricmp(AllowSingleSignOnUsingOSPrimaryAccount, name) == 0) {
		hr = get_AllowSingleSignOnUsingOSPrimaryAccount(&ret);
	}

	return ret;
}

HRESULT cwv2envOpts::setBool(LPCSTR name, BOOL value) {
	HRESULT hr = E_INVALIDARG;
	if (!name) return hr;

	if (_stricmp(AllowSingleSignOnUsingOSPrimaryAccount, name) == 0) {
		hr = put_AllowSingleSignOnUsingOSPrimaryAccount(value);
	}

	return hr;
}


ICoreWebView2EnvironmentOptions* cwv2envOpts::GetCoreWebView2EnvironmentOptions() {
	return (ICoreWebView2EnvironmentOptions*)this;
}

wv2envOpts* cwv2envOpts::Getwv2envOpts() {
	return dynamic_cast<wv2envOpts*>(this);
}