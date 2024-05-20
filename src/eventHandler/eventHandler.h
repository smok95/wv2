#pragma once

#include "wv2.h"
#include "WebView2.h"

template <typename T, typename I>
class EventHandlerBase: public I {

public:
	STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv) override {
		if(riid == IID_IUnknown || riid == __uuidof(I)) {
			*ppv = static_cast<I*>(this);
			AddRef();
			return S_OK;
		}

		*ppv = nullptr;
		return E_NOINTERFACE;
	}

	ULONG STDMETHODCALLTYPE AddRef() override {
		return InterlockedIncrement(&refCount_);
	}

	ULONG STDMETHODCALLTYPE Release() override {
		const ULONG newCount = InterlockedDecrement(&refCount_);
		if(newCount == 0) {
			// 동적생성하지 않는 관계로 일단 주석처리함.
			// delete this;
		}

		return newCount;
	}

	void bind(T handler, void* userData) {
		handler_ = handler;
		userData_ = userData;
	}

protected:
	EventHandlerBase():refCount_(1), handler_(nullptr), userData_(nullptr) {
		token_.value = 0;
	}

	virtual ~EventHandlerBase() {}

protected:
	EventRegistrationToken token_;
	T handler_;
	void* userData_;

private:
	LONG refCount_;
};

///////////////////////////////////////////////////////////////////////////////

class BrowserProcessExited:
	public EventHandlerBase<browserProcessExited,
	ICoreWebView2BrowserProcessExitedEventHandler> {
public:
	STDMETHODIMP Invoke(ICoreWebView2Environment* sender,
		ICoreWebView2BrowserProcessExitedEventArgs* args) override {
		if(handler_) {

			wv2browserProcessExitedEventArgs wv2args;
			args->get_BrowserProcessExitKind(
				(COREWEBVIEW2_BROWSER_PROCESS_EXIT_KIND*)&wv2args.browserProcessExitKind);
			args->get_BrowserProcessId((UINT32*)&wv2args.browserProcessId);

			handler_(userData_, &wv2args);
		}
		return S_OK;
	}

	void add(ICoreWebView2Environment5* env) {
		if(!env) return;
		env->add_BrowserProcessExited(this, &token_);
	}

	void remove(ICoreWebView2Environment5* env) {
		if(!env) return;
		env->remove_BrowserProcessExited(token_);
	}
};

///////////////////////////////////////////////////////////////////////////////
class IsDocumentPlayingAudioChanged:
	public EventHandlerBase<isDocumentPlayingAudioChanged,
	ICoreWebView2IsDocumentPlayingAudioChangedEventHandler> {
public:
	STDMETHODIMP Invoke(ICoreWebView2* sender, IUnknown* args) override {
		if(handler_) {
			handler_(userData_);
		}
		return S_OK;
	}

	void add(ICoreWebView2_8* webview2) {
		if(!webview2) return;
		webview2->add_IsDocumentPlayingAudioChanged(this, &token_);
	}

	void remove(ICoreWebView2_8* webview2) {
		if(!webview2) return;
		webview2->remove_IsDocumentPlayingAudioChanged(token_);
	}
};

///////////////////////////////////////////////////////////////////////////////

class IsMutedChanged:
	public EventHandlerBase< isMutedChanged,
	ICoreWebView2IsMutedChangedEventHandler> {
public:
	STDMETHODIMP Invoke(ICoreWebView2* sender, IUnknown* args) override {
		if(handler_) {
			handler_(userData_);
		}
		return S_OK;
	}

	void add(ICoreWebView2_8* webview2) {
		if(!webview2) return;
		webview2->add_IsMutedChanged(this, &token_);
	}

	void remove(ICoreWebView2_8* webview2) {
		if(!webview2) return;
		webview2->remove_IsMutedChanged(token_);
	}
};


///////////////////////////////////////////////////////////////////////////////
class WindowCloseRequested:
	public EventHandlerBase<windowCloseRequested,
	ICoreWebView2WindowCloseRequestedEventHandler> {
public:
	STDMETHODIMP Invoke(ICoreWebView2* sender, IUnknown* args) override {
		if(handler_) {
			if(handler_(userData_)) {
				return S_OK;
			}
		}

		// S_OK를 리턴하면 WebView를 종료시켜버리니 주의.
		return E_NOTIMPL;
	}

	void add(ICoreWebView2* webview2) {
		if(!webview2) return;
		webview2->add_WindowCloseRequested(this, &token_);
	}

	void remove(ICoreWebView2* webview2) {
		if(!webview2) return;
		webview2->remove_WindowCloseRequested(token_);
	}
};

///////////////////////////////////////////////////////////////////////////////
class DocumentTitleChanged:
	public EventHandlerBase<documentTitleChanged,
	ICoreWebView2DocumentTitleChangedEventHandler> {
public:
	STDMETHODIMP Invoke(ICoreWebView2* sender, IUnknown* args) override {
		if(handler_) {
			handler_(userData_);
		}
		return S_OK;
	}

	void add(ICoreWebView2* webview2) {
		if(!webview2) return;
		webview2->add_DocumentTitleChanged(this, &token_);
	}

	void remove(ICoreWebView2* webview2) {
		if(!webview2) return;
		webview2->remove_DocumentTitleChanged(token_);
	}
};

///////////////////////////////////////////////////////////////////////////////
