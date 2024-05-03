#pragma once
#include <atlcomcli.h>
#include "WebView2.h"
#include "wv2.h"
#include "BrowserProcessExited.h"

class cwv2env:
	public wv2env {
public:
	STDMETHODIMP Invoke(ICoreWebView2Environment* sender,
		ICoreWebView2BrowserProcessExitedEventArgs* args);

	wv2bool setBrowserProcessExitedHandler(browserProcessExited handler) override;

	HRESULT createCoreWebView2EnvironmentCompleted(ICoreWebView2Environment* env);

	void Release();

	ICoreWebView2Environment2* getEnv2() { return env2_; }
	ICoreWebView2Environment5* getEnv5() { return env5_; }

private:
	CComPtr<ICoreWebView2Environment2> env2_;
	CComPtr<ICoreWebView2Environment5> env5_;
	BrowserProcessExited browserProcessExitedHandler_;
};

