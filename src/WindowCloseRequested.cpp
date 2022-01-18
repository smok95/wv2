#include "WindowCloseRequested.h"

WindowCloseRequested::WindowCloseRequested(): handler_(nullptr), 
userData_(nullptr) {
	token_.value = 0;
}

STDMETHODIMP WindowCloseRequested::QueryInterface(REFIID riid, LPVOID* ppv) {
	return S_OK;
}

STDMETHODIMP WindowCloseRequested::Invoke(ICoreWebView2 *sender, IUnknown *args) {
	if (handler_) {
		if (handler_(sender)){
			return S_OK;
		}
	}

	// S_OK를 리턴하면 WebView를 종료시켜버리니 주의.
	return E_NOTIMPL;
}

void WindowCloseRequested::add(ICoreWebView2* webview2) {
	if (!webview2) return;
	webview2->add_WindowCloseRequested(this, &token_);
}

void WindowCloseRequested::remove(ICoreWebView2* webview2) {
	if (!webview2) return;
	webview2->remove_WindowCloseRequested(token_);
}

void WindowCloseRequested::bind(windowCloseRequested handler, void* userData) {
	handler_ = handler;
	userData_ = userData;
}