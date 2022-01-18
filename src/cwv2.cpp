#include "cwv2.h"

#include <winnt.h>
#include <string>
#include <ciso646>
#include <codecvt>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _M_IX86	// x86
#pragma comment(lib, "webview2/x86/WebView2Loader.dll.lib")	// DLL버전
//#pragma comment(lib, "webview2/x86/WebView2LoaderStatic.lib")	// Static버전
#elif _M_X64
#pragma comment(lib, "webview2/x64/WebView2Loader.dll.lib")	// DLL버전 
//#pragma comment(lib, "webview2/x64/WebView2LoaderStatic.lib")	// Static버전
#elif _M_ARM64
#pragma comment(lib, "webview2/arm64/WebView2Loader.dll.lib")	// DLL버전
//#pragma comment(lib, "webview2/arm64/WebView2LoaderStatic.lib")	// Static버전
#endif

using namespace std;
static void wait();

wv2settings wv2settingsDefault();
EventRegistrationToken emptyEventRegistrationToken();
///////////////////////////////////////////////////////////////////////////////
void wait() {
	MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

///////////////////////////////////////////////////////////////////////////////
cwv2::cwv2(HWND parentWindow,
	createCompleted createCompletedHandler /*=nullptr*/,
	void* userData /*=nullptr*/) :parentWindow_(parentWindow),
	createCompletedHandler_(createCompletedHandler), userData_(userData),
	refCount_(0), createStatus_(none), executeScriptSyncResult_(nullptr),
	settings_(wv2settingsDefault()), executeScriptCompletedHandler_(nullptr),
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
	webMessageReceivedToken_(emptyEventRegistrationToken()) {
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

	if (webview_) {
		windowCloseRequestedHandler_.remove(webview_);
		webview_->remove_WebMessageReceived(webMessageReceivedToken_);
		webview_->remove_PermissionRequested(permissionRequestedToken_);
		webview_->remove_HistoryChanged(historyChangedToken_);
		webview_->remove_DOMContentLoaded(domContentLoadedToken_);
		webview_->remove_NavigationStarting(navigationStartingToken_);
		webview_->remove_NavigationCompleted(navigationCompletedToken_);	
		if (virtualHostName_.length() > 0) {
			webview_->ClearVirtualHostNameToFolderMapping(virtualHostName_.c_str());
		}

		webview_.Release();
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
}

wv2settings* cwv2::getSettings() {
	if (!webview_) return nullptr;

	CComPtr<ICoreWebView2Settings> s;
	lastError_ = webview_->get_Settings(&s);
	if (FAILED(lastError_)) {
		return nullptr;
	}

	BOOL v;
	s->get_AreDefaultContextMenusEnabled(&v);
	settings_.areDefaultContextMenusEnabled = v == TRUE;
	s->get_AreDefaultScriptDialogsEnabled(&v);
	settings_.areDefaultScriptDialogsEnabled = v == TRUE;
	s->get_AreDevToolsEnabled(&v);
	settings_.areDevToolsEnabled= v == TRUE;
	s->get_AreHostObjectsAllowed(&v);
	settings_.areHostObjectsAllowed = v == TRUE;
	s->get_IsBuiltInErrorPageEnabled(&v);
	settings_.isBuiltInErrorPageEnabled = v == TRUE;
	s->get_IsScriptEnabled(&v);
	settings_.isScriptEnabled = v == TRUE;
	s->get_IsStatusBarEnabled(&v);
	settings_.isStatusBarEnabled = v == TRUE;
	s->get_IsWebMessageEnabled(&v);
	settings_.isWebMessageEnabled = v == TRUE;
	s->get_IsZoomControlEnabled(&v);
	settings_.isZoomControlEnabled = v == TRUE;

	return &settings_;
}

bool cwv2::setSettings(const wv2settings* val) {
	if (!webview_ or !val) return false;
	
	CComPtr<ICoreWebView2Settings> s;	
	if (FAILED(lastError_ = webview_->get_Settings(&s))) {
		return false;
	}

	settings_ = *val;
	auto& r = settings_;
	if (FAILED(lastError_ = s->put_AreDefaultContextMenusEnabled(
		r.areDefaultContextMenusEnabled))) {
		return false;
	}

	if (FAILED(lastError_ = s->put_AreDefaultScriptDialogsEnabled(
		r.areDefaultScriptDialogsEnabled))) {
		return false;
	}

	if (FAILED(lastError_ = s->put_AreDevToolsEnabled(r.areDevToolsEnabled))) {
		return false;
	}

	if (FAILED(lastError_ = s->put_AreHostObjectsAllowed(r.areHostObjectsAllowed))) {
		return false;
	}

	if (FAILED(lastError_ = s->put_IsBuiltInErrorPageEnabled(r.isBuiltInErrorPageEnabled))) {
		return false;
	}

	if (FAILED(lastError_ = s->put_IsScriptEnabled(r.isScriptEnabled))) {
		return false;
	}

	if (FAILED(lastError_ = s->put_IsStatusBarEnabled(r.isStatusBarEnabled))) {
		return false;
	}

	if (FAILED(lastError_ = s->put_IsWebMessageEnabled(r.isWebMessageEnabled))) {
		return false;
	}

	if (FAILED(lastError_ = s->put_IsZoomControlEnabled(r.isZoomControlEnabled))) {
		return false;
	}

	return true;
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
		createCompletedHandler_(nullptr, errorCode);
		// 객체 정리
		Release();
	}
	return errorCode;
}

STDMETHODIMP cwv2::Invoke(HRESULT errorCode, ICoreWebView2Environment *env) {
	if (FAILED(errorCode)) {
		return setStatusCreateFail(errorCode);
	}
		
	HRESULT hr = env->CreateCoreWebView2Controller(parentWindow_, this);
	if (FAILED(hr)) {
		lastError_ = hr;
		createStatus_ = failed;
	}
	return hr;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2 *sender, ICoreWebView2PermissionRequestedEventArgs *args) {
	if (sender != webview_) return E_UNEXPECTED;
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
	hr = webview2->QueryInterface(IID_ICoreWebView2_3, (void**)&webview_);
	if (FAILED(hr)) {
		return setStatusCreateFail(hr);
	}
		
	// 부모화면 사이즈에 맞게 변경
	RECT bounds = { 0, };
	GetClientRect(parentWindow_, &bounds);
	controller->put_Bounds(bounds);

	hr = controller->QueryInterface(IID_ICoreWebView2Controller3, (void**)&controller_);
	if (FAILED(hr)) {
		return setStatusCreateFail(hr);
	}
			
	if (webview_ != nullptr) {
		createStatus_ = completed;
		windowCloseRequestedHandler_.add(webview_);
		webview_->add_PermissionRequested(this, &permissionRequestedToken_);
		webview_->add_HistoryChanged(this, &historyChangedToken_);
		webview_->add_DOMContentLoaded(this, &domContentLoadedToken_);
		webview_->add_NavigationStarting(this, &navigationStartingToken_);
		webview_->add_NavigationCompleted(this, &navigationCompletedToken_);
		webview_->add_WebMessageReceived(this, &webMessageReceivedToken_);
	}
	else {
		createStatus_ = failed;
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
		createCompletedHandler_(this, errorCode);
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
	if (sender != webview_) return E_UNEXPECTED;
		
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
	if (sender != webview_) return E_UNEXPECTED;

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
	if (sender != webview_) return E_UNEXPECTED;

	if (webMessageReceivedHandler_) {
		LPWSTR msg = nullptr;
		if (SUCCEEDED(args->TryGetWebMessageAsString(&msg))) {
			webMessageReceivedHandler_(this, msg);
			CoTaskMemFree(msg);
		}
	}
	return S_OK;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2 *sender, ICoreWebView2NavigationCompletedEventArgs *args) {
	if (sender != webview_) return E_UNEXPECTED;
	
	if (navigationCompletedHandler_) {
		navigationCompletedHandler_(this);
	}
	
	return S_OK;
}

STDMETHODIMP cwv2::Invoke(ICoreWebView2 *sender, ICoreWebView2DOMContentLoadedEventArgs *args) {
	if (sender != webview_) return E_UNEXPECTED;

	if (domContentLoadedHandler_) {
		domContentLoadedHandler_(this);
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
	if (!webview_) return false;
	if (handler) {
		executeScriptCompletedHandler_ = handler;
	}
	else {
		// empty handler 설정
		executeScriptCompletedHandler_ = [](wv2_t sender, LPCWSTR resultObjectAsJson){};
	}
	
	return SUCCEEDED(lastError_ = webview_->ExecuteScript(script, this));
}

LPCWSTR cwv2::executeScriptSync(LPCWSTR script) {
	if (!webview_) return nullptr;
	executeScriptCompletedHandler_ = nullptr;
	if (executeScriptSyncResult_) {
		free(executeScriptSyncResult_);
		executeScriptSyncResult_ = nullptr;
	}

	HRESULT hr = webview_->ExecuteScript(script, this);
	if (FAILED(lastError_ = hr)) {
		return nullptr;
	}

	while (executeScriptSyncResult_ != nullptr) {
		wait();
	}

	return executeScriptSyncResult_;
}

LPCWSTR cwv2::getSource() {
	LPCWSTR source = nullptr;
	if (!webview_) return source;
	
	LPWSTR uri = nullptr;
	if (SUCCEEDED(lastError_ = webview_->get_Source(&uri))) {
		source = _wcsdup(uri);
		CoTaskMemFree(uri);
	}
	return source;
}

bool cwv2::goBack() {
	if (!webview_) return false;
	return SUCCEEDED(lastError_ = webview_->GoBack());
}

bool cwv2::goForward() {
	if (!webview_) return false;
	return SUCCEEDED(lastError_ = webview_->GoForward());
}

bool cwv2::navigate(const wchar_t* url) {
	if (!webview_) {
		// 웹뷰 획득 이전 단계인 경우라면 uri를 저장하고 획득후에 navigate 한다.
		lastRequest_.isHtmlContent = false;
		lastRequest_.uriOrHtmlContent = url;
		return true;
	}
	return SUCCEEDED(lastError_ = webview_->Navigate(url));
}

bool cwv2::navigateToString(const wchar_t* html) {
	if (!webview_) {
		// 웹뷰 획득 이전 단계인 경우라면 저장한 후 획득 후 navigate 한다.
		lastRequest_.isHtmlContent = true;
		lastRequest_.uriOrHtmlContent = html ? html : L"";
		return true;
	}
	return SUCCEEDED(lastError_ = webview_->NavigateToString(html));
}

bool cwv2::reload() {
	if (!webview_) return false;
	return SUCCEEDED(lastError_ = webview_->Reload());
}

bool cwv2::resize(int width, int height) {
	if (!controller_) return false;

	RECT bounds = { 0, };
	bounds.right = bounds.left + width;
	bounds.bottom = bounds.top + height;
	return SUCCEEDED(lastError_ = controller_->put_Bounds(bounds));
}

bool cwv2::setHistoryChangedHandler(historyChanged handler) {
	if (!webview_) return false;
	historyChangedHandler_ = handler;
	return true;
}

bool cwv2::setNavigationStartingHandler(navigationStarting handler) {
	if (!webview_) return false;
	navigationStartingHandler_ = handler;
	return true;
}

bool cwv2::setNavigationCompletedHandler(navigationCompleted handler) {
	if (!webview_) return false;
	navigationCompletedHandler_ = handler;
	return true;
}

bool cwv2::setDomContentLoadedHandler(domContentLoaded handler) {
	if (!webview_) return false;
	domContentLoadedHandler_ = handler;
	return true;
}

bool cwv2::setWindowCloseRequestedHandler(windowCloseRequested handler) {
	if (!webview_) return false;
	windowCloseRequestedHandler_.bind(handler, userData_);
	return true;
}

bool cwv2::setWebMessageReceivedHandler(webMessageReceived handler) {
	if (!webview_) return false;
	webMessageReceivedHandler_ = handler;
	return true;
}

bool cwv2::stop() {
	// webview가 없는 경우에는 stop이 의미가 없기 때문에 성공으로 간주하고, true리턴.
	if (!webview_) return true;

	return SUCCEEDED(lastError_ = webview_->Stop());
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

// 웹뷰 초기화가 완료 여부 (초기화가 성공되었음을 의미하지 않음)
bool cwv2::isDone() const {
	return createStatus_ == failed || createStatus_ == completed;
}

cwv2::CreateStatus cwv2::crateStatus() const { 
	return createStatus_; 
}

bool cwv2::setVirtualHostNameToFolderMapping(LPCWSTR hostName,
	LPCWSTR folderPath, wv2HostResourceAccessKind accessKind) {
	if (!webview_) return false;
	if (!hostName or !folderPath) return false;
	
	lastError_ = webview_->SetVirtualHostNameToFolderMapping(hostName,
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
	if (!webview_ or !messageAsJson) return false;
	lastError_ = webview_->PostWebMessageAsJson(messageAsJson);
	return SUCCEEDED(lastError_);
}

bool cwv2::postWebMessageAsString(LPCWSTR messageAsString) {
	if (!webview_ or !messageAsString) return false;
	lastError_ = webview_->PostWebMessageAsString(messageAsString);
	return SUCCEEDED(lastError_);
}

void cwv2::freeMemory(void* p) {
	if (!p) return;
	free(p);
}

wv2settings wv2settingsDefault() {
	wv2settings def = { true, };
	return def;
}

EventRegistrationToken emptyEventRegistrationToken() {
	EventRegistrationToken token = { 0, };
	return token;
}