#include "cwv2webResourceResponse.h"
#include "cwv2types.h"

cwv2webResourceResponse::cwv2webResourceResponse() {
}
cwv2webResourceResponse::cwv2webResourceResponse(CComPtr<ICoreWebView2WebResourceResponse> response) {
	setResponse(response);
}

void cwv2webResourceResponse::setResponse(CComPtr<ICoreWebView2WebResourceResponse> response) {
	response_ = response;
}

int32_t cwv2webResourceResponse::statusCode() {
	if (!response_) return 0;
	int32_t statusCode = 0;
	response_->get_StatusCode(&statusCode);
	return statusCode;
}

HRESULT cwv2webResourceResponse::setStatusCode(int32_t statusCode) {
	if (!response_) return E_NOINTERFACE;
	return response_->put_StatusCode(statusCode);
}
LPWSTR cwv2webResourceResponse::reasonPhrase() {
	if (!response_) return nullptr;
	return getStrVal([&](LPWSTR* value)->HRESULT {
		return response_->get_ReasonPhrase(value);
		});
}

HRESULT cwv2webResourceResponse::setReasonPhrase(LPCWSTR reasonPhrase) {
	if (!response_) return E_NOINTERFACE;
	return response_->put_ReasonPhrase(reasonPhrase);
}

CComPtr<ICoreWebView2WebResourceResponse> cwv2webResourceResponse::getResponse() {
	return response_;
}