#pragma once
#include <atlcomcli.h>
#include "WebView2.h"
#include "wv2.h"

namespace wv2_ {
	class cookieList : public wv2cookieList {
	public:
		cookieList(ICoreWebView2CookieList& cookieList) :
			cookieList_(cookieList) {};

		UINT count() override;
		wv2cookie* getValueAtIndex(UINT index) override;

	private:
		ICoreWebView2CookieList& cookieList_;
	};

} // namespace cwv2
