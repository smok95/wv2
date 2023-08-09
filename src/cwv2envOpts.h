#ifndef ICoreWebView2EnvironmentOptions_IMPLEMENT_H_
#define ICoreWebView2EnvironmentOptions_IMPLEMENT_H_

#ifndef OVERRIDE
#define OVERRIDE override
#endif // OVERRIDE

#include <string>
#include <atlcomcli.h>

#include "WebView2.h"
#include "wv2envOpts.h"


class AutoCoMemStr {
public:
	AutoCoMemStr() {}
	~AutoCoMemStr() { Release(); }
	void Release() {
		if (m_string) {
			::CoTaskMemFree(m_string);
			m_string = nullptr;
		}
	}

	LPCWSTR Set(LPCWSTR str) {
		Release();
		if (str) {
			m_string = MakeCoMemString(str);
		}
		return m_string;
	}
	LPCWSTR Get() { return m_string; }
	LPWSTR Copy() {
		if (m_string)
			return MakeCoMemString(m_string);
		return nullptr;
	}

protected:
	LPWSTR MakeCoMemString(LPCWSTR source) {
		const size_t length = wcslen(source);
		const size_t bytes = (length + 1) * sizeof(*source);
		// Ensure we didn't overflow during our size calculation.
		if (bytes <= length) {
			return nullptr;
		}

		wchar_t* result = reinterpret_cast<wchar_t*>(::CoTaskMemAlloc(bytes));
		if (result)
			memcpy(result, source, bytes);

		return result;
	}

	LPWSTR m_string = nullptr;
};

class cwv2envOpts : 	
	public ICoreWebView2EnvironmentOptions,
	public wv2envOpts {
public:

	cwv2envOpts();
	virtual ~cwv2envOpts();

	// ICoreWebView2EnvironmentOptions interfaces	///////////////////////////
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) OVERRIDE;
	ULONG STDMETHODCALLTYPE AddRef() OVERRIDE;
	ULONG STDMETHODCALLTYPE Release() OVERRIDE;
	
	STDMETHODIMP get_AdditionalBrowserArguments(LPWSTR* value) OVERRIDE;
	STDMETHODIMP put_AdditionalBrowserArguments(LPCWSTR value) OVERRIDE;

	STDMETHODIMP get_Language(LPWSTR* value) OVERRIDE;
	STDMETHODIMP put_Language(LPCWSTR value) OVERRIDE;

	STDMETHODIMP get_TargetCompatibleBrowserVersion(LPWSTR* value) OVERRIDE;
	STDMETHODIMP put_TargetCompatibleBrowserVersion(LPCWSTR value) OVERRIDE;

	STDMETHODIMP get_AllowSingleSignOnUsingOSPrimaryAccount(BOOL* allow) OVERRIDE;
	STDMETHODIMP put_AllowSingleSignOnUsingOSPrimaryAccount(BOOL allow) OVERRIDE;
	
	// ICoreWebView2EnvironmentOptions interfaces	///////////////////////////

	// wv2envOpts interface	///////////////////////////////////////////////////
	void destroy() OVERRIDE;

	LPWSTR getString(LPCSTR name) OVERRIDE;
	HRESULT setString(LPCSTR name, LPCWSTR value) OVERRIDE;
	BOOL getBool(LPCSTR name) OVERRIDE;
	HRESULT setBool(LPCSTR name, BOOL value) OVERRIDE;
	// wv2envOpts interface	///////////////////////////////////////////////////

	ICoreWebView2EnvironmentOptions* GetCoreWebView2EnvironmentOptions();
	wv2envOpts* Getwv2envOpts();
private:
	ULONG refCount_;
	AutoCoMemStr additionalBrowserArguments_;
	AutoCoMemStr language_;
	AutoCoMemStr targetCompatibleBrowserVersion_;
	BOOL allowSingleSignOnUsingOSPrimaryAccount_;
};

#endif // ICoreWebView2EnvironmentOptions_IMPLEMENT_H_