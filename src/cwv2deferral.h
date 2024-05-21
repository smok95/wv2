#pragma once
#include <atlcomcli.h>
#include "wv2.h"
#include "WebView2.h"

class cwv2deferral: public wv2deferral {
public:
	cwv2deferral(CComPtr<ICoreWebView2Deferral> deferral):deferral_(deferral) {}

	HRESULT complete() override {
		if(deferral_) {
			return deferral_->Complete();
		}
		return E_POINTER;
	}

private:
	CComPtr<ICoreWebView2Deferral> deferral_;
};