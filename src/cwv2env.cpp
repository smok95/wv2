#include "cwv2env.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static inline wv2bool wv2boolNotSupported() {
	wv2bool r = {0,};
	r.supported = false;
	r.hr = CO_E_NOT_SUPPORTED;
	return r;
}


void cwv2env::Release() {

	if(env2_) {
		env2_.Release();
	}

	if(env5_) {
		browserProcessExitedHandler_.remove(env5_);
		env5_.Release();
	}
}

HRESULT cwv2env::createCoreWebView2EnvironmentCompleted(ICoreWebView2Environment* env) {

	// CoreWebView2Environment2 È¹µæ, 2022.12.13 kim,jk
	HRESULT hr = env->QueryInterface(IID_ICoreWebView2Environment2,
		(void**)&env2_);

	if (FAILED(hr)) {
		return hr;
	}

	// CoreWebView2Environment5 È¹µæ, 2024.04.30
	hr = env->QueryInterface(IID_ICoreWebView2Environment5,
		(void**)&env5_);

	if(SUCCEEDED(hr)) {
		browserProcessExitedHandler_.add(env5_);
	}

	return hr;
}

wv2bool cwv2env::setBrowserProcessExitedHandler(browserProcessExited handler) {
	wv2bool r = wv2boolNotSupported();
	if(!env5_) {
		return r;
	}
	r.supported = true;

	browserProcessExitedHandler_.bind(handler, this);
	r.result = true;
	return r;
}
