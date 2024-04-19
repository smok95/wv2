#ifndef IS_MUTED_CHANGED_HANDLER_
#define IS_MUTED_CHANGED_HANDLER_
#include "wv2.h"
#include "WebView2.h"

class IsMutedChanged : public ICoreWebView2IsMutedChangedEventHandler {
public:
	IsMutedChanged();
	STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv) override;
	ULONG STDMETHODCALLTYPE AddRef() override { return 1; }
	ULONG STDMETHODCALLTYPE Release() override { return 1; }
	STDMETHODIMP Invoke(ICoreWebView2* sender, IUnknown* args) override;

	void add(ICoreWebView2_8* webview2);
	void remove(ICoreWebView2_8* webview2);

	void bind(isMutedChanged handler, void* userData);
private:
	EventRegistrationToken token_;
	isMutedChanged handler_;
	void* userData_;
};


#endif // IS_MUTED_CHANGED_HANDLER_