#ifndef WINDOW_CLOSE_REQUESTED_HANDLER_H_
#define WINDOW_CLOSE_REQUESTED_HANDLER_H_
#include "../wv2.h"
#include "WebView2.h"

class WindowCloseRequested : public ICoreWebView2WindowCloseRequestedEventHandler {
public:
	WindowCloseRequested() {};
	STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv) override;
	ULONG STDMETHODCALLTYPE AddRef() override { return 1;  }
	ULONG STDMETHODCALLTYPE Release() override { return 1;  }
	STDMETHODIMP Invoke(ICoreWebView2 *sender, IUnknown *args) override;

	void add(ICoreWebView2* webview2);
	void remove(ICoreWebView2* webview2);
	void bind(windowCloseRequested handler, void* userData);
private:
	EventRegistrationToken token_ = { 0, };
	windowCloseRequested handler_ = nullptr;
	void* userData_ = nullptr;
};


#endif // WINDOW_CLOSE_REQUESTED_HANDLER_H_