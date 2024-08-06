#pragma once

#include "wv2.h"
#include "WebView2.h"
#include "cwv2deferral.h"
#include "cwv2types.h"
#include "cwv2webResourceResponse.h"

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
class cwv2contentLoadingEventArgs: public wv2contentLoadingEventArgs {
public:
	cwv2contentLoadingEventArgs(ICoreWebView2ContentLoadingEventArgs& args):
		args_(args) {}

	bool isErrorPage() override {
		BOOL isErrPage = FALSE;
		args_.get_IsErrorPage(&isErrPage);
		return isErrPage == TRUE;
	}

	uint64_t navigationId() override {
		UINT64 navId = 0;
		args_.get_NavigationId(&navId);
		return navId;
	}
private:
	ICoreWebView2ContentLoadingEventArgs& args_;
};

class ContentLoading:
	public EventHandlerBase<contentLoading, 
	ICoreWebView2ContentLoadingEventHandler> {	
public:
	STDMETHODIMP Invoke(ICoreWebView2* sender, 
		ICoreWebView2ContentLoadingEventArgs* args) override {
		if(handler_ && args) {
			cwv2contentLoadingEventArgs argsWrap(*args);
			handler_(userData_, &argsWrap);
		}
		return S_OK;
	}

	void add(ICoreWebView2* webview2) {
		if(!webview2) return;
		webview2->add_ContentLoading(this, &token_);
	}

	void remove(ICoreWebView2* webview2) {
		if(!webview2) return;
		webview2->remove_ContentLoading(token_);
	}
};

///////////////////////////////////////////////////////////////////////////////
class cwv2scriptDialogOpeningEventArgs: public wv2scriptDialogOpeningEventArgs {
public:
	cwv2scriptDialogOpeningEventArgs(ICoreWebView2ScriptDialogOpeningEventArgs& args):
		args_(args) {}

	LPWSTR uri() override {		
		return getStrVal([&](LPWSTR* value) -> HRESULT {
			return args_.get_Uri(value);
			});
	}

	wv2scriptDialogKind kind() override {
		wv2scriptDialogKind kind = wv2scriptDialogKind_undefined;
		args_.get_Kind((COREWEBVIEW2_SCRIPT_DIALOG_KIND*)&kind);
		return kind;
	}

	LPWSTR message() override {
		return getStrVal([&](LPWSTR* value) -> HRESULT {
			return args_.get_Message(value);
			});
	}

	HRESULT accept() override {
		return args_.Accept();
	}

	LPWSTR defaultText() override {
		return getStrVal([&](LPWSTR* value) -> HRESULT {
			return args_.get_DefaultText(value);
			});
	}

	LPWSTR resultText() override {
		return getStrVal([&](LPWSTR* value)->HRESULT {
			return args_.get_ResultText(value);
			});
	}

	HRESULT setResultText(LPCWSTR resultText) override {
		return args_.put_ResultText(resultText);
	}

	wv2deferral* getDeferral() override {
		CComPtr<ICoreWebView2Deferral> deferral;
		if(FAILED(args_.GetDeferral(&deferral))) {
			return nullptr;
		}

		return new cwv2deferral(deferral);
	}
private:
	ICoreWebView2ScriptDialogOpeningEventArgs& args_;
};

class ScriptDialogOpening:
	public EventHandlerBase<scriptDialogOpening,
	ICoreWebView2ScriptDialogOpeningEventHandler> {
public:
	STDMETHODIMP Invoke(ICoreWebView2* sender, 
		ICoreWebView2ScriptDialogOpeningEventArgs* args) override {
		if(handler_ && args) {
			cwv2scriptDialogOpeningEventArgs argsWrap(*args);
			handler_(userData_, &argsWrap);
		}
		return S_OK;
	}

	void add(ICoreWebView2* view) {
		if(!view) return;
		view->add_ScriptDialogOpening(this, &token_);
	}

	void remove(ICoreWebView2* view) {
		if(!view) return;
		view->remove_ScriptDialogOpening(token_);
	}
};

///////////////////////////////////////////////////////////////////////////////
class cwv2downloadStartingEventArgs : public wv2downloadStartingEventArgs {
public:
	cwv2downloadStartingEventArgs(ICoreWebView2DownloadStartingEventArgs& args):args_(args){}

	bool cancel() override {
		bool result = false;
		args_.get_Cancel((BOOL*)&result);
		return result;
	}

	HRESULT setCancel(bool cancel) override {
		return args_.put_Cancel((BOOL)cancel);
	}

	LPWSTR resultFilePath() override {
		return getStrVal([&](LPWSTR* value)->HRESULT {
			return args_.get_ResultFilePath(value);
			});
	}

	HRESULT setResultFilePath(LPCWSTR resultFilePath) override {
		return args_.put_ResultFilePath(resultFilePath);
	}

	bool handled() override {
		bool result = false;
		args_.get_Handled((BOOL*)result);
		return result;
	}

	HRESULT setHandled(bool handled) override {
		return args_.put_Handled((BOOL)handled);
	}

	wv2deferral* getDeferral() override {
		CComPtr<ICoreWebView2Deferral> deferral;
		if (FAILED(args_.GetDeferral(&deferral))) {
			return nullptr;
		}

		return new cwv2deferral(deferral);
	}

private:
	ICoreWebView2DownloadStartingEventArgs& args_;
};

class DownloadStarting :
	public EventHandlerBase<downloadStarting,
	ICoreWebView2DownloadStartingEventHandler> {
public:
	STDMETHODIMP Invoke(ICoreWebView2* sender, 
		ICoreWebView2DownloadStartingEventArgs* args) override {
		if (handler_ && args) {
			cwv2downloadStartingEventArgs argsWrap(*args);
			handler_(userData_, &argsWrap);
		}
		return S_OK;
	}

	void add(CComPtr<ICoreWebView2_4> view) {
		if(!view) return;
		webview_ = view;
		webview_->add_DownloadStarting(this, &token_);
	}

	void remove() {
		if (!webview_) return;
		webview_->remove_DownloadStarting(token_);
	}

	bool IsSupported() const {
		return webview_ != nullptr;
	}

private:
	CComPtr<ICoreWebView2_4> webview_;
};

///////////////////////////////////////////////////////////////////////////////
class cwv2httpRequestHeaders :public wv2httpRequestHeaders {
public:
	void setHttpRequestHeaders(CComPtr<ICoreWebView2HttpRequestHeaders> headers) {
		headers_ = headers;
	}

	LPWSTR getHeader(LPCWSTR name) override {
		if (!headers_) return nullptr;

		return getStrVal([&](LPWSTR* value) -> HRESULT {
			return headers_->GetHeader(name, value);
			});
	}

	bool contains(LPCWSTR name) override {
		if (!headers_) return false;

		BOOL contains = FALSE;
		headers_->Contains(name, &contains);
		return contains == TRUE;
	}

	HRESULT setHeader(LPCWSTR name, LPCWSTR value) override {
		if (!headers_) return E_NOINTERFACE;

		return headers_->SetHeader(name, value);
	}

	HRESULT removeHeader(LPCWSTR name) override {
		if (!headers_) return E_NOINTERFACE;
		return headers_->RemoveHeader(name);
	}

private:
	CComPtr<ICoreWebView2HttpRequestHeaders> headers_;
};

class cwv2webResourceRequest : public wv2webResourceRequest {
public:
	void setRequest(CComPtr<ICoreWebView2WebResourceRequest> request) {
		request_ = request;
	}

	LPWSTR uri() override {
		if (!request_) return nullptr;
		return getStrVal([&](LPWSTR* value)->HRESULT {
			return request_->get_Uri(value);
			});
	}

	HRESULT setUri(LPCWSTR uri) override {
		if (!request_) return E_NOINTERFACE;

		return request_->put_Uri(uri);
	}

	LPWSTR method() override {
		if (!request_) return nullptr;
		return getStrVal([&](LPWSTR* value)->HRESULT {
			return request_->get_Method(value);
			});
	}

	HRESULT setMethod(LPCWSTR method) override {
		if (!request_) return E_NOINTERFACE;
		return request_->put_Method(method);
	}

	wv2httpRequestHeaders* headers() override {
		if (!request_) return nullptr;

		CComPtr<ICoreWebView2HttpRequestHeaders> headers;
		if (SUCCEEDED(request_->get_Headers(&headers))) {
			headers_.setHttpRequestHeaders(headers);
		}

		return &headers_;
	}

private:
	CComPtr<ICoreWebView2WebResourceRequest> request_;
	cwv2httpRequestHeaders headers_;
};


class cwv2webResourceRequestedEventArgs : public wv2webResourceRequestedEventArgs {
public:
	cwv2webResourceRequestedEventArgs(ICoreWebView2WebResourceRequestedEventArgs& args) :args_(args) {}

	wv2webResourceRequest* request() override {		
		CComPtr<ICoreWebView2WebResourceRequest> req;
		if (FAILED(args_.get_Request(&req))) {
			return nullptr;
		}

		request_.setRequest(req);

		if (!req) {
			return nullptr;
		}
		return &request_;
	}

	wv2webResourceResponse* response() override {
		CComPtr<ICoreWebView2WebResourceResponse> res;
		if (FAILED(args_.get_Response(&res))) {
			return nullptr;
		}

		response_.setResponse(res);

		if (!res) {
			return nullptr;
		}
		return &response_;
	}

	wv2webResourceContext resourceContext() override {
		wv2webResourceContext context = wv2webResourceContext_undefined;
		args_.get_ResourceContext((COREWEBVIEW2_WEB_RESOURCE_CONTEXT*)&context);
		return context;
	}

private:
	ICoreWebView2WebResourceRequestedEventArgs& args_;
	cwv2webResourceRequest request_;
	cwv2webResourceResponse response_;
};

class WebResourceRequested :
	public EventHandlerBase<webResourceRequested,
	ICoreWebView2WebResourceRequestedEventHandler> {
public:
	STDMETHODIMP Invoke(ICoreWebView2* sender, 
		ICoreWebView2WebResourceRequestedEventArgs* args) override {
		if (handler_ && args) {
			cwv2webResourceRequestedEventArgs argsWrap(*args);
			handler_(userData_, &argsWrap);
		}
		return S_OK;
	}

	void add(CComPtr<ICoreWebView2> view) {
		if (!view) return;
		webview_ = view;
		webview_->add_WebResourceRequested(this, &token_);
	}

	void remove() {
		if (!webview_) return;
		webview_->remove_WebResourceRequested(token_);
	}

	bool IsSupported() const {
		return webview_ != nullptr;
	}

private:
	CComPtr<ICoreWebView2> webview_;
};

//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
class cwv2navigationCompletedEventArgs: public wv2navigationCompletedEventArgs {
public:
	cwv2navigationCompletedEventArgs(ICoreWebView2NavigationCompletedEventArgs& args)
		: args_(args) {}

	bool isSuccess() override {
		BOOL b = FALSE;
		args_.get_IsSuccess(&b);
		return b == TRUE;
	}
	
	wv2webErrorStatus webErrorStatus() override {
		COREWEBVIEW2_WEB_ERROR_STATUS status = COREWEBVIEW2_WEB_ERROR_STATUS_UNKNOWN;
		args_.get_WebErrorStatus(&status);
		return (wv2webErrorStatus)status;
	}

	uint64_t navigationId() override {
		UINT64 navId = 0;
		args_.get_NavigationId(&navId);
		return navId;
	}

private:
	ICoreWebView2NavigationCompletedEventArgs& args_;
};