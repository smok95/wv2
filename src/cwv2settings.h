#pragma once
#include <atlcomcli.h>
#include "WebView2.h"
#include "wv2.h"

class cwv2settings: public wv2settings {
public:	
	ICoreWebView2Settings* getCoreWebView2Settings();
	void setCoreWebView2Settings(CComPtr< ICoreWebView2Settings> settings);
	ICoreWebView2Settings2* getCoreWebView2Settings2();
	void setCoreWebView2Settings2(CComPtr<ICoreWebView2Settings2> settings2);

	wv2bool isScriptEnabled() override;
	wv2bool setIsScriptEnabled(bool isScriptEnabled) override;
	wv2bool isWebMessageEnabled() override;
	wv2bool setIsWebMessageEnabled(bool isWebMessageEnabled) override;
	wv2bool areDefaultScriptDialogsEnabled() override;
	wv2bool setAreDefaultScriptDialogsEnabled(bool areDefaultScriptDialogsEnabled) override;
	wv2bool isStatusBarEnabled() override;
	wv2bool setIsStatusBarEnabled(bool isStatusBarEnabled) override;
	wv2bool areDevToolsEnabled() override;
	wv2bool setAreDevToolsEnabled(bool areDevToolsEnabled) override;
	wv2bool areDefaultContextMenusEnabled() override;
	wv2bool setAreDefaultContextMenusEnabled(bool enabled) override;
	wv2bool areHostObjectsAllowed() override;
	wv2bool setAreHostObjectsAllowed(bool allowed) override;
	wv2bool isZoomControlEnabled() override;
	wv2bool setIsZoomControlEnabled(bool enabled) override;
	wv2bool isBuiltInErrorPageEnabled() override;
	wv2bool setIsBuiltInErrorPageEnabled(bool enabled) override;
	LPWSTR userAgent() override;
	HRESULT setUserAgent(LPCWSTR userAgent) override;
private:
	CComPtr<ICoreWebView2Settings> settings_;
	CComPtr<ICoreWebView2Settings2> settings2_;
};

