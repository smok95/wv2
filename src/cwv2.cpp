#include "cwv2.h"

#include <winnt.h>
#include <string>
#include <ciso646>
#include <codecvt>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include "cwv2newWindowRequestedEventArgs.h"
#include "cwv2types.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _M_IX86	// x86
#pragma message("webview2/x86/WebView2Loader.dll.lib")
#pragma comment(lib, "webview2/x86/WebView2Loader.dll.lib")	// DLL버전
//#pragma comment(lib, "webview2/x86/WebView2LoaderStatic.lib")	// Static버전
#elif _M_X64
#pragma message("webview2/x64/WebView2Loader.dll.lib")
#pragma comment(lib, "webview2/x64/WebView2Loader.dll.lib")	// DLL버전 
//#pragma comment(lib, "webview2/x64/WebView2LoaderStatic.lib")	// Static버전
#elif _M_ARM64
#pragma message("webview2/arm64/WebView2Loader.dll.lib")
#pragma comment(lib, "webview2/arm64/WebView2Loader.dll.lib")	// DLL버전
//#pragma comment(lib, "webview2/arm64/WebView2LoaderStatic.lib")	// Static버전
#endif

using namespace std;
static void wait();

EventRegistrationToken emptyEventRegistrationToken();

///////////////////////////////////////////////////////////////////////////////
void wait() {
	MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


class cwv2domContentLoadedEventArgs: public wv2domContentLoadedEventArgs {
public:
	cwv2domContentLoadedEventArgs(ICoreWebView2DOMContentLoadedEventArgs& args):args_(args) {}

	uint64_t navigationId() override {
		UINT64 id = 0;
		args_.get_NavigationId(&id);
		return id;
	}

private:
	ICoreWebView2DOMContentLoadedEventArgs& args_;
};

///////////////////////////////////////////////////////////////////////////////
cwv2::cwv2(HWND parentWindow,
	createCompleted createCompletedHandler /*=nullptr*/,
	void* userData /*=nullptr*/) :parentWindow_(parentWindow),
	createCompletedHandler_(createCompletedHandler), userData_(userData),
	refCount_(0), createStatus_(none), executeScriptSyncResult_(nullptr),
	executeScriptCompletedHandler_(nullptr),
	historyChangedHandler_(nullptr), 
	historyChangedToken_(emptyEventRegistrationToken()),
	navigationCompletedHandler_(nullptr),
	navigationCompletedToken_(emptyEventRegistrationToken()),
	navigationStartingHandler_(nullptr),
	navigationStartingToken_(emptyEventRegistrationToken()),
	domContentLoadedHandler_(nullptr),
	domContentLoadedToken_(emptyEventRegistrationToken()),
	permissionRequestedToken_(emptyEventRegistrationToken()),
	webMessageReceivedHandler_(nullptr),
	webMessageReceivedToken_(emptyEventRegistrationToken()),
	newWindowRequestedHandler_(nullptr),
	newWindowRequestedToken_(emptyEventRegistrationToken())
{
	lastError_ = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	coInitilized_ = SUCCEEDED(lastError_);
	createStatus_ = created;
};

cwv2::~cwv2() {
	clearAll();
	if (coInitilized_) CoUninitialize();
}

void cwv2::clearAll(bool detachController/*=false*/) {
	userData_ = nullptr;

	if (executeScriptSyncResult_) {
		free(executeScriptSyncResult_);
		executeScriptSyncResult_ = nullptr;
	}

	if (view2_3_) {
		windowCloseRequestedHandler_.remove(view2_3_);
		view2_3_->remove_WebMessageReceived(webMessageReceivedToken_);
		view2_3_->remove_PermissionRequested(permissionRequestedToken_);
		view2_3_->remove_HistoryChanged(historyChangedToken_);
		view2_3_->remove_DOMContentLoaded(domContentLoadedToken_);
		view2_3_->remove_NavigationStarting(navigationStartingToken_);
		view2_3_->remove_NavigationCompleted(navigationCompletedToken_);	
		if (virtualHostName_.length() > 0) {
			view2_3_->ClearVirtualHostNameToFolderMapping(virtualHostName_.c_str());
		}

		view2_3_->remove_NewWindowRequested(newWindowRequestedToken_);
		documentTitleChangedHandler_.remove(view2_3_);
		contentLoadingHandler_.remove(view2_3_);
		scriptDialogOpeningHandler_.remove(view2_3_);

		downloadStartingHandler_.remove();
		webResourceRequestedHandler_.remove();

		view2_3_.Release();
	}

	if (view2_8_) {
		isMutedChangedHandler_.remove(view2_8_);
		isDocumentPlayingAudioChangedHandler_.remove(view2_8_);
		view2_8_.Release();
	}

	if (controller_) {
		if (detachController) {
			controller_.Detach();
		}
		else {
			controller_->Close();
			controller_.Release();
		}		
	}

	env_.Release();
}

wv2settings* cwv2::getSettings() {
	if (!view2_3_) return nullptr;

	if(!settings_.getCoreWebView2Settings()) {
		CComPtr<ICoreWebView2Settings> s;
		lastError_ = view2_3_->get_Settings(&s);
		if(FAILED(lastError_)) {
			return nullptr;
		}

		settings_.setCoreWebView2Settings(s);

		CComPtr<ICoreWebView2Settings2> s2;
		if(SUCCEEDED(s->QueryInterface(__uuidof(ICoreWebView2Settings2), (void**)&s2))) {
			settings_.setCoreWebView2Settings2(s2);
		}
	}

	return &settings_;
}

void* cwv2::getUserData() {
	return userData_;
}

bool cwv2::setUserData(void* userData) {
	userData_ = userData;
	return true;
}

HRESULT cwv2::setStatusCreateFail(const HRESULT errorCode) {
	lastError_ = errorCode;
	// 상태 실패로 설정
	createStatus_ = failed;

	// createCompleted핸들러에 생설실패 결과 전달
	if (createCompletedHandler_) {
		createCompletedHandler_(nullptr, errorCode, userData_);
		// 객체 정리
		Release();
	}
	return errorCode;
}

STDMETHODIMP cwv2::Invoke(HRESULT errorCode, ICoreWebView2Environment *env) {
	if (FAILED(errorCode)) {
		return setStatusCreateFail(errorCode);
	}

	// CoreWebView2Environment 획득
	HRESULT hr = env_.createCoreWebView2EnvironmentCompleted(env);	
	if(FAILED(hr)) {
		lastError_ = hr;
		createStatus_ = failed;
	}

	hr = env->CreateCoreWebView2Controller(parentWindow_, this);
	if (FAILED(hr)) {
		lastError_ = hr;
		createStatus_ = failed;
	}

	return hr;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2 *sender, ICoreWebView2PermissionRequestedEventArgs *args) {
	if (sender != view2_3_) return E_UNEXPECTED;
	OutputDebugString(L"WebView2PermissionRequested");
	args->put_State(COREWEBVIEW2_PERMISSION_STATE_ALLOW);
	return S_OK;
}

STDMETHODIMP cwv2::Invoke(HRESULT errorCode, ICoreWebView2Controller* controller) {
	if (FAILED(errorCode)) {
		return setStatusCreateFail(errorCode);
	}

	// WebView2 획득
	CComPtr<ICoreWebView2> webview2;
	HRESULT hr = controller->get_CoreWebView2(&webview2);
	if (FAILED(hr)) {
		return setStatusCreateFail(hr);
	}

	// WebView2 3번째 버전 획득
	hr = webview2->QueryInterface(IID_ICoreWebView2_3, (void**)&view2_3_);
	if (FAILED(hr)) {
		return setStatusCreateFail(hr);
	}

	// WebView2 8번째 버전 획득
	hr = webview2->QueryInterface(IID_ICoreWebView2_8, (void**)&view2_8_);
		
	// 부모화면 사이즈에 맞게 변경
	RECT bounds = { 0, };
	GetClientRect(parentWindow_, &bounds);
	controller->put_Bounds(bounds);

	hr = controller->QueryInterface(IID_ICoreWebView2Controller3, (void**)&controller_);
	if (FAILED(hr)) {
		return setStatusCreateFail(hr);
	}
			
	if (view2_3_ != nullptr) {
		createStatus_ = completed;
		windowCloseRequestedHandler_.add(view2_3_);
		view2_3_->add_PermissionRequested(this, &permissionRequestedToken_);
		view2_3_->add_HistoryChanged(this, &historyChangedToken_);
		view2_3_->add_DOMContentLoaded(this, &domContentLoadedToken_);
		view2_3_->add_NavigationStarting(this, &navigationStartingToken_);
		view2_3_->add_NavigationCompleted(this, &navigationCompletedToken_);
		view2_3_->add_WebMessageReceived(this, &webMessageReceivedToken_);
		view2_3_->add_NewWindowRequested(this, &newWindowRequestedToken_);
		documentTitleChangedHandler_.add(view2_3_);
		contentLoadingHandler_.add(view2_3_);
		scriptDialogOpeningHandler_.add(view2_3_);

		CComPtr<ICoreWebView2_4> view4;
		if (SUCCEEDED(webview2->QueryInterface(__uuidof(ICoreWebView2_4), (void**)&view4))) {
			downloadStartingHandler_.add(view4);
		}

		webResourceRequestedHandler_.add(webview2);
	}
	else {
		createStatus_ = failed;
	}

	if (view2_8_ != nullptr) {
		isMutedChangedHandler_.add(view2_8_);
		isDocumentPlayingAudioChangedHandler_.add(view2_8_);
	}

	if (lastRequest_.uriOrHtmlContent.size() > 0) {
		LPCWSTR uriOrHtml = lastRequest_.uriOrHtmlContent.c_str();
		if (lastRequest_.isHtmlContent) {
			navigateToString(uriOrHtml);
		}
		else {
			navigate(uriOrHtml);
		}
		lastRequest_.clear();
	}

	controller_->put_IsVisible(TRUE);

	if (createCompletedHandler_) {
		createCompletedHandler_(this, errorCode, userData_);
	}

	return S_OK;
}

STDMETHODIMP cwv2::Invoke(HRESULT errorCode, LPCWSTR resultObjectAsJson) {
	if (executeScriptCompletedHandler_) {
		executeScriptCompletedHandler_((wv2_t)this, resultObjectAsJson);
	}
	else {
		// sync 호출인 경우
		executeScriptSyncResult_ = _wcsdup(resultObjectAsJson);
	}

	return errorCode;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2 *sender, IUnknown *args) {
	if (sender != view2_3_) return E_UNEXPECTED;
		
	if (historyChangedHandler_) {
		BOOL canGoBack = FALSE;
		BOOL canGoForward = FALSE;
		sender->get_CanGoBack(&canGoBack);
		sender->get_CanGoForward(&canGoForward);
		historyChangedHandler_(this, canGoBack == TRUE, canGoForward == TRUE);
	}

	return S_OK;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2 *sender, ICoreWebView2NavigationStartingEventArgs *args) {
	if (sender != view2_3_) return E_UNEXPECTED;

	if (navigationStartingHandler_) {
		LPWSTR uri = nullptr;
		args->get_Uri(&uri);
		
		if (!navigationStartingHandler_(this, uri)) {
			args->put_Cancel(TRUE);
		}

		CoTaskMemFree(uri);
	}

	return S_OK;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) {
	if (sender != view2_3_) return E_UNEXPECTED;

	if (webMessageReceivedHandler_) {
		LPWSTR msg = nullptr;
		if (SUCCEEDED(args->TryGetWebMessageAsString(&msg))) {
			webMessageReceivedHandler_(this, msg);
			CoTaskMemFree(msg);
		}
	}
	return S_OK;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2 *sender, 
	ICoreWebView2NavigationCompletedEventArgs *args) {
	if (sender != view2_3_) return E_UNEXPECTED;
	
	if (navigationCompletedHandler_) {
		cwv2navigationCompletedEventArgs argsWrap(*args);
		navigationCompletedHandler_(this, &argsWrap);
	}
	
	return S_OK;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2 *sender, ICoreWebView2DOMContentLoadedEventArgs *args) {
	if (sender != view2_3_ || args == nullptr) return E_UNEXPECTED;

	if (domContentLoadedHandler_) {
		cwv2domContentLoadedEventArgs argsWrap(*args);		
		domContentLoadedHandler_(this, &argsWrap);
	}
	return S_OK;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args) {
	if(sender != view2_3_ || args == nullptr) return E_UNEXPECTED;

	if(newWindowRequestedHandler_) {
		cwv2newWindowRequestedEventArgs argsWrap(*args);
		newWindowRequestedHandler_(this, &argsWrap);
	}

	return S_OK;
}

STDMETHODIMP cwv2::QueryInterface(REFIID riid, LPVOID* ppv) {
	// Always set out parameter to NULL, validating it first.
	if (!ppv) {
		return E_INVALIDARG;
	}
	*ppv = NULL;
	if (riid == IID_IUnknown || riid == IID_ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler ||
		riid == IID_ICoreWebView2CreateCoreWebView2ControllerCompletedHandler)
	{
		// Increment the reference count and return the pointer.
		*ppv = (LPVOID)this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE cwv2::AddRef() {
	InterlockedIncrement(&refCount_);
	return refCount_;
}

ULONG STDMETHODCALLTYPE cwv2::Release() {
	// Decrement the object's internal counter.
	ULONG ulRefCount = InterlockedDecrement(&refCount_);
	if (0 == refCount_) {
		delete this;
	}
	return ulRefCount;
}

void cwv2::destroy() {
	clearAll();
	Release();
}

void cwv2::detach() {
	clearAll(true);
	Release();
}

bool cwv2::executeScript(LPCWSTR script, executeScriptCompleted handler) {
	if (!view2_3_) return false;
	if (handler) {
		executeScriptCompletedHandler_ = handler;
	}
	else {
		// empty handler 설정
		executeScriptCompletedHandler_ = [](wv2_t sender, LPCWSTR resultObjectAsJson){};
	}
	
	return SUCCEEDED(lastError_ = view2_3_->ExecuteScript(script, this));
}

LPCWSTR cwv2::executeScriptSync(LPCWSTR script) {
	if (!view2_3_) return nullptr;
	executeScriptCompletedHandler_ = nullptr;
	if (executeScriptSyncResult_) {
		free(executeScriptSyncResult_);
		executeScriptSyncResult_ = nullptr;
	}

	HRESULT hr = view2_3_->ExecuteScript(script, this);
	if (FAILED(lastError_ = hr)) {
		return nullptr;
	}

	const ULONGLONG start = ::GetTickCount64();
	const ULONGLONG timeout = 3000;
	while (executeScriptSyncResult_ == nullptr) {
		wait();
		if ((::GetTickCount64() - start) > timeout) {
			break;
		}
	}

	LPCWSTR result = nullptr;
	if (executeScriptSyncResult_) {
		result = _wcsdup(executeScriptSyncResult_);
		free(executeScriptSyncResult_);
		executeScriptSyncResult_ = nullptr;
	}

	return result;
}

LPCWSTR cwv2::getSource() {
	LPCWSTR source = nullptr;
	if (!view2_3_) return source;
	
	LPWSTR uri = nullptr;
	if (SUCCEEDED(lastError_ = view2_3_->get_Source(&uri))) {
		source = _wcsdup(uri);
		CoTaskMemFree(uri);
	}
	return source;
}

bool cwv2::goBack() {
	if (!view2_3_) return false;
	return SUCCEEDED(lastError_ = view2_3_->GoBack());
}

bool cwv2::goForward() {
	if (!view2_3_) return false;
	return SUCCEEDED(lastError_ = view2_3_->GoForward());
}

bool cwv2::navigate(LPCWSTR url) {
	if (!view2_3_) {
		// 웹뷰 획득 이전 단계인 경우라면 uri를 저장하고 획득후에 navigate 한다.
		lastRequest_.isHtmlContent = false;
		lastRequest_.uriOrHtmlContent = url;
		return true;
	}
	return SUCCEEDED(lastError_ = view2_3_->Navigate(url));
}

bool cwv2::navigateToString(LPCWSTR html) {
	if (!view2_3_) {
		// 웹뷰 획득 이전 단계인 경우라면 저장한 후 획득 후 navigate 한다.
		lastRequest_.isHtmlContent = true;
		lastRequest_.uriOrHtmlContent = html ? html : L"";
		return true;
	}
	return SUCCEEDED(lastError_ = view2_3_->NavigateToString(html));
}

bool cwv2::navigateWithWebResource(LPCWSTR uri, LPCWSTR method, BYTE* postData,
	size_t byteSize, LPCWSTR headers) {
	if (!view2_3_) return false;

	// postData 및 headers가 없고, method가 GET이면 navigate로 호출한다.
	if (!postData && !headers && (!method || _wcsicmp(L"GET", method) == 0)) {
		return navigate(uri);
	}
	else {
		ICoreWebView2Environment2* env2 = env_.getEnv2();
		if (!env2) return false;

		// postData 또는 length값이 null(0)이면 모두 null(0)으로 한다.
		if (!postData) byteSize = 0;
		if (byteSize == 0) postData = nullptr;

		CComPtr<IStream> postDataStream = SHCreateMemStream(postData, (UINT)byteSize);
		CComPtr<ICoreWebView2WebResourceRequest> request;
		lastError_ = env2->CreateWebResourceRequest(uri, method, postDataStream,
			headers, &request);
		
		if (FAILED(lastError_)) {
			return false;
		}

		return SUCCEEDED(lastError_ = 
			view2_3_->NavigateWithWebResourceRequest(request));
	}
}

bool cwv2::reload() {
	if (!view2_3_) return false;
	return SUCCEEDED(lastError_ = view2_3_->Reload());
}

bool cwv2::resize(int width, int height) {
	if (!controller_) return false;

	RECT bounds = { 0, };
	bounds.right = bounds.left + width;
	bounds.bottom = bounds.top + height;
	return SUCCEEDED(lastError_ = controller_->put_Bounds(bounds));
}

bool cwv2::setHistoryChangedHandler(historyChanged handler) {
	if (!view2_3_) return false;
	historyChangedHandler_ = handler;
	return true;
}

bool cwv2::setNavigationStartingHandler(navigationStarting handler) {
	if (!view2_3_) return false;
	navigationStartingHandler_ = handler;
	return true;
}

bool cwv2::setNavigationCompletedHandler(navigationCompleted handler) {
	if (!view2_3_) return false;
	navigationCompletedHandler_ = handler;
	return true;
}

bool cwv2::setDomContentLoadedHandler(domContentLoaded handler) {
	if (!view2_3_) return false;
	domContentLoadedHandler_ = handler;
	return true;
}

bool cwv2::setWindowCloseRequestedHandler(windowCloseRequested handler) {
	if (!view2_3_) return false;
	windowCloseRequestedHandler_.bind(handler, this);
	return true;
}

bool cwv2::setIsMutedChangedHandler(isMutedChanged handler) {
	if (!view2_8_) return false;
	isMutedChangedHandler_.bind(handler, this);
	return true;
}

bool cwv2::setIsDocumentPlayingAudioChangedHandler(
	isDocumentPlayingAudioChanged handler) {
	if (!view2_8_) return false;
	isDocumentPlayingAudioChangedHandler_.bind(handler, this);
	return true;
}

bool cwv2::setWebMessageReceivedHandler(webMessageReceived handler) {
	if (!view2_3_) return false;
	webMessageReceivedHandler_ = handler;
	return true;
}

bool cwv2::stop() {
	// webview가 없는 경우에는 stop이 의미가 없기 때문에 성공으로 간주하고, true리턴.
	if (!view2_3_) return true;

	return SUCCEEDED(lastError_ = view2_3_->Stop());
}
	
double cwv2::zoomFactor(const double* newZoomFactor) {
	double curZoomFactor = -1.0;
	if (!controller_) return curZoomFactor;


	if (newZoomFactor) {
		if (SUCCEEDED(controller_->put_ZoomFactor(*newZoomFactor))) {
			curZoomFactor = *newZoomFactor;
		}
	}
	else {
		controller_->get_ZoomFactor(&curZoomFactor);
	}
	return curZoomFactor;
}

wv2bool cwv2::isMuted() {
	wv2bool r = wv2boolNotSupported();
	if (!view2_8_) return r;	
	BOOL value;
	if (SUCCEEDED(r.hr = view2_8_->get_IsMuted(&value))) {
		r.value = value == TRUE;
	}

	return r;
}

wv2bool cwv2::setIsMuted(const bool muted) {
	wv2bool r = wv2boolNotSupported();
	if (!view2_8_) return r;

	r.hr = view2_8_->put_IsMuted(muted ? TRUE : FALSE);
	r.value = SUCCEEDED(r.hr);
	return r;
}

wv2bool cwv2::isDocumentPlayingAudio() {
	wv2bool r = wv2boolNotSupported();
	if (!view2_8_) return r;

	BOOL value;
	if (SUCCEEDED(r.hr = view2_8_->get_IsDocumentPlayingAudio(&value))) {
		r.value = value == TRUE;
	}

	return r;
}

// 웹뷰 초기화가 완료 여부 (초기화가 성공되었음을 의미하지 않음)
bool cwv2::isDone() const {
	return createStatus_ == failed || createStatus_ == completed;
}

cwv2::CreateStatus cwv2::createStatus() const { 
	return createStatus_; 
}

bool cwv2::setVirtualHostNameToFolderMapping(LPCWSTR hostName,
	LPCWSTR folderPath, wv2HostResourceAccessKind accessKind) {
	if (!view2_3_) return false;
	if (!hostName or !folderPath) return false;
	
	lastError_ = view2_3_->SetVirtualHostNameToFolderMapping(hostName,
		folderPath, (COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND)accessKind);

	if (SUCCEEDED(lastError_)) {
		virtualHostName_ = hostName;
		return true;
	}
	else {
		virtualHostName_.clear();
		return false;
	}
}

bool cwv2::postWebMessageAsJson(LPCWSTR messageAsJson) {
	if (!view2_3_ or !messageAsJson) return false;
	lastError_ = view2_3_->PostWebMessageAsJson(messageAsJson);
	return SUCCEEDED(lastError_);
}

bool cwv2::postWebMessageAsString(LPCWSTR messageAsString) {
	if (!view2_3_ or !messageAsString) return false;
	lastError_ = view2_3_->PostWebMessageAsString(messageAsString);
	return SUCCEEDED(lastError_);
}

void cwv2::freeMemory(void* p) {
	if (!p) return;
	free(p);
}

wv2bool cwv2::openTaskManagerWindow() {
	wv2bool r = wv2boolNotSupported();
	if (!view2_8_) return r;
	r.hr = view2_8_->OpenTaskManagerWindow();
	r.value = SUCCEEDED(r.hr);
	return r;
}

EventRegistrationToken emptyEventRegistrationToken() {
	EventRegistrationToken token = { 0, };
	return token;
}

wv2environment* cwv2::getEnvironment() {
	return &env_;
}

wv2bool cwv2::setNewWindowRequestedHandler(newWindowRequested handler) {
	wv2bool r = wv2boolNotSupported();
	if(!view2_3_) return r;
	r.hr = S_OK;
	r.value = true;

	newWindowRequestedHandler_ = handler;

	return r;
}

wv2bool cwv2::setDocumentTitleChangedHandler(documentTitleChanged handler) {
	wv2bool r = wv2boolNotSupported();
	if(!view2_3_) return r;
	r.hr = S_OK;
	r.value = true;

	documentTitleChangedHandler_.bind(handler, this);
	return r;
}

LPCWSTR cwv2::documentTitle() {
	if(!view2_3_) return nullptr;

	LPCWSTR result = nullptr;
	LPWSTR title = nullptr;
	lastError_ = view2_3_->get_DocumentTitle(&title);
	if(SUCCEEDED(lastError_)) {
		result = _wcsdup(title);
		CoTaskMemFree(title);
	}

	return result;
}

wv2bool cwv2::setContentLoadingHandler(contentLoading handler) {
	wv2bool r = wv2boolNotSupported();
	if(!view2_3_) return r;
	r.hr = S_OK;
	r.value = true;

	contentLoadingHandler_.bind(handler, this);
	return r;
}

wv2bool cwv2::setScriptDialogOpeningHandler(scriptDialogOpening handler) {
	wv2bool r = wv2boolNotSupported();
	if(!view2_3_) return r;
	r.hr = S_OK;
	r.value = true;

	scriptDialogOpeningHandler_.bind(handler, this);

	return r;
}

wv2bool cwv2::setDownloadingStartingHandler(downloadStarting handler) {
	wv2bool r = wv2boolNotSupported();
	if (not downloadStartingHandler_.IsSupported()) {
		return r;
	}

	r.hr = S_OK;
	r.value = true;

	downloadStartingHandler_.bind(handler, this);
	return r;
}

wv2bool cwv2::setWebResourceRequestedHandler(webResourceRequested handler) {
	wv2bool r = wv2boolNotSupported();
	if (not webResourceRequestedHandler_.IsSupported()) {
		return r;
	}

	r.hr = S_OK;
	r.value = true;

	webResourceRequestedHandler_.bind(handler, this);
	return r;
}

HRESULT cwv2::addWebResourceRequestedFilter(LPCWSTR uri,
	const wv2webResourceContext resourceContext) {
	if (!view2_3_) return E_NOINTERFACE;
	return view2_3_->AddWebResourceRequestedFilter(uri, 
		(COREWEBVIEW2_WEB_RESOURCE_CONTEXT)resourceContext);
}

HRESULT cwv2::removeWebResourceRequestedFilter(LPCWSTR uri,
	const wv2webResourceContext resourceContext) {
	if (!view2_3_) return E_NOINTERFACE;
	return view2_3_->RemoveWebResourceRequestedFilter(uri, 
		(COREWEBVIEW2_WEB_RESOURCE_CONTEXT)resourceContext);
}