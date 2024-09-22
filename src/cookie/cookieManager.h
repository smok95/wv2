#pragma once
#include <atlcomcli.h>
#include "WebView2.h"
#include "wv2.h"
#include "eventHandler/eventHandler.h"

namespace wv2_ {
	class cookieManager :
		public wv2cookieManager,
		public EventHandlerBase<getCookiesCompleted, ICoreWebView2GetCookiesCompletedHandler> {
	public:
		ICoreWebView2CookieManager* getCoreWebView2CookieManager();
		void setCoreWebView2CookieManager(CComPtr<ICoreWebView2CookieManager> cm);

		wv2cookie* createCookie(LPCWSTR name, LPCWSTR value,
			LPCWSTR domain, LPCWSTR path) override;
		wv2cookie* copyCookie(wv2cookie* cookieParam) override;
		HRESULT getCookies(LPCWSTR uri, getCookiesCompleted handler) override;
		HRESULT addOrUpdateCookie(wv2cookie* cookie) override;
		HRESULT deleteCookie(wv2cookie* cookie) override;
		HRESULT deleteCookies(LPCWSTR name, LPCWSTR uri) override;
		HRESULT deleteCookiesWithDomainAndPath(LPCWSTR name, LPCWSTR domain,
			LPCWSTR path) override;
		HRESULT deleteAllCookies(void) override;

	protected:		
		STDMETHODIMP Invoke(HRESULT result, ICoreWebView2CookieList* cookieList) override;
	private:
		CComPtr<ICoreWebView2CookieManager> cm_;		
		ULONG refCount_{ 1 };
	};

}; // namespace wv2


#pragma once
