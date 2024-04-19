#include "IsMutedChanged.h"

IsMutedChanged::IsMutedChanged() : handler_(nullptr),
userData_(nullptr) {
	token_.value = 0;
}

STDMETHODIMP IsMutedChanged::QueryInterface(REFIID riid, LPVOID* ppv) {
	return S_OK;
}

STDMETHODIMP IsMutedChanged::Invoke(ICoreWebView2* sender, IUnknown* args) {
	if (handler_) {
		handler_(userData_);
	}
	return S_OK;
}

void IsMutedChanged::add(ICoreWebView2_8* webview2) {
	if (!webview2) return;
	webview2->add_IsMutedChanged(this, &token_);
}

void IsMutedChanged::remove(ICoreWebView2_8* webview2) {
	if (!webview2) return;
	webview2->remove_IsMutedChanged(token_);
}

void IsMutedChanged::bind(isMutedChanged handler, void* userData) {
	handler_ = handler;
	userData_ = userData;
}