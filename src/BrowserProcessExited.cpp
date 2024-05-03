#include "BrowserProcessExited.h"

BrowserProcessExited::BrowserProcessExited():handler_(nullptr),
userData_(nullptr) {
	token_.value = 0;
}

STDMETHODIMP BrowserProcessExited::QueryInterface(REFIID riid, LPVOID* ppv) {
	return S_OK;
}

STDMETHODIMP BrowserProcessExited::Invoke(ICoreWebView2Environment* sender,
	ICoreWebView2BrowserProcessExitedEventArgs* args) {
	if(handler_) {

		wv2browserProcessExitedEventArgs wv2args;
		args->get_BrowserProcessExitKind(
			(COREWEBVIEW2_BROWSER_PROCESS_EXIT_KIND*)&wv2args.browserProcessExitKind);	
		args->get_BrowserProcessId((UINT32*)&wv2args.browserProcessId);

		handler_(userData_, &wv2args);
	}
	return S_OK;
}

void BrowserProcessExited::add(ICoreWebView2Environment5* env) {
	if(!env) return;
	env->add_BrowserProcessExited(this, &token_);
}

void BrowserProcessExited::remove(ICoreWebView2Environment5* env) {
	if(!env) return;
	env->remove_BrowserProcessExited(token_);
}

void BrowserProcessExited::bind(browserProcessExited handler, void* userData) {
	handler_ = handler;
	userData_ = userData;
}