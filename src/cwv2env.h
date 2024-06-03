#pragma once
#include <atlcomcli.h>
#include "WebView2.h"
#include "wv2.h"
#include "eventHandler.h"

class cwv2env:
	public wv2environment {
public:
	wv2bool setBrowserProcessExitedHandler(browserProcessExited handler) override;

	HRESULT createCoreWebView2EnvironmentCompleted(ICoreWebView2Environment* env);

	wv2webResourceResponse* createWebResourceResponse(
		IStream* content, int32_t statusCode, LPCWSTR reasonPhrase, LPCWSTR headers) override;

	void Release();

	ICoreWebView2Environment2* getEnv2() { return env2_; }
	ICoreWebView2Environment5* getEnv5() { return env5_; }

private:
	CComPtr<ICoreWebView2Environment2> env2_;
	CComPtr<ICoreWebView2Environment5> env5_;
	BrowserProcessExited browserProcessExitedHandler_;
};

