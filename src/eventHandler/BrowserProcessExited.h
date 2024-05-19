#ifndef BROWSER_PROCESS_EXITED_HANDLER_
#define BROWSER_PROCESS_EXITED_HANDLER_
#include "wv2.h"
#include "WebView2.h"

class BrowserProcessExited: public ICoreWebView2BrowserProcessExitedEventHandler {
public:
	BrowserProcessExited();
	STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv) override;
	ULONG STDMETHODCALLTYPE AddRef() override { return 1; }
	ULONG STDMETHODCALLTYPE Release() override { return 1; }
	STDMETHODIMP Invoke(ICoreWebView2Environment* sender,
		ICoreWebView2BrowserProcessExitedEventArgs* args) override;

	void add(ICoreWebView2Environment5* env);
	void remove(ICoreWebView2Environment5* env);

	void bind(browserProcessExited handler, void* userData);
private:
	EventRegistrationToken token_;
	browserProcessExited handler_;
	void* userData_;
};

#endif // BROWSER_PROCESS_EXITED_HANDLER_