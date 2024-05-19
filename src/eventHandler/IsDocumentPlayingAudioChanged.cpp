#include "IsDocumentPlayingAudioChanged.h"

IsDocumentPlayingAudioChanged::IsDocumentPlayingAudioChanged() : handler_(nullptr),
userData_(nullptr) {
	token_.value = 0;
}

STDMETHODIMP IsDocumentPlayingAudioChanged::QueryInterface(REFIID riid, LPVOID* ppv) {
	return S_OK;
}

STDMETHODIMP IsDocumentPlayingAudioChanged::Invoke(ICoreWebView2* sender, IUnknown* args) {
	if (handler_) {
		handler_(userData_);
	}
	return S_OK;
}

void IsDocumentPlayingAudioChanged::add(ICoreWebView2_8* webview2) {
	if (!webview2) return;
	webview2->add_IsDocumentPlayingAudioChanged(this, &token_);
}

void IsDocumentPlayingAudioChanged::remove(ICoreWebView2_8* webview2) {
	if (!webview2) return;
	webview2->remove_IsDocumentPlayingAudioChanged(token_);
}

void IsDocumentPlayingAudioChanged::bind(isMutedChanged handler, void* userData) {
	handler_ = handler;
	userData_ = userData;
}