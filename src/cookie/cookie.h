#pragma once
#include <atlcomcli.h>
#include "wv2.h"
#include "WebView2.h"

namespace wv2_ {
	class cookie :public wv2cookie {
		cookie(CComPtr<ICoreWebView2Cookie> cookie) :cookie_(cookie) {};
	public:
		static cookie* newCookie(CComPtr<ICoreWebView2Cookie> cookie);
		void destroy() override;

		LPWSTR name() override;
		LPWSTR value() override;
		HRESULT setValue(LPCWSTR value) override;
		LPWSTR domain() override;
		LPWSTR path() override;
		double expires() override;
		HRESULT setExpires(double expires) override;
		bool isHttpOnly() override;
		HRESULT setIsHttpOnly(bool isHttpOnly) override;
		wv2cookieSameSiteKind sameSite() override;
		HRESULT setSameSite(wv2cookieSameSiteKind sameSite) override;
		bool isSecure() override;
		HRESULT setIsSecure(bool isSecure) override;
		bool isSession() override;

		ICoreWebView2Cookie* getCoreWebView2Cookie() const;
	private:
		CComPtr<ICoreWebView2Cookie> cookie_;
	};
} // namespace cwv2
