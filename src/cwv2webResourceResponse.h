#pragma once
#include "WebView2.h"
#include "wv2.h"
#include <atlcomcli.h>


class cwv2webResourceResponse : public wv2webResourceResponse {
public:
	cwv2webResourceResponse();
	cwv2webResourceResponse(CComPtr<ICoreWebView2WebResourceResponse> response);

	void setResponse(CComPtr<ICoreWebView2WebResourceResponse> response);
	CComPtr<ICoreWebView2WebResourceResponse> getResponse();
	int32_t statusCode() override;
	HRESULT setStatusCode(int32_t statusCode) override;
	LPWSTR reasonPhrase() override;
	HRESULT setReasonPhrase(LPCWSTR reasonPhrase) override;

private:
	CComPtr<ICoreWebView2WebResourceResponse> response_;
};