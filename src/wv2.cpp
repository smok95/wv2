#include <ciso646>
#include <comdef.h>

#include "wv2.h"

#include "cwv2.h"
#include "cwv2types.h"
#include "cwv2envOpts.h"

using namespace std;
HRESULT theLastError_ = E_NOT_SET;

static void wait();
///////////////////////////////////////////////////////////////////////////////
void wait() {
	MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

///////////////////////////////////////////////////////////////////////////////
bool wv2create(LPCWSTR browserExecutableFolder, LPCWSTR userDataFolder, 
	HWND parentWindow, createCompleted handler, void* userData) {

	return wv2create2(browserExecutableFolder, userDataFolder, nullptr, 
		parentWindow, handler, userData);
}

bool wv2create2(LPCWSTR browserExecutableFolder, LPCWSTR userDataFolder,
	wv2envOpts_t environmentOptions, HWND parentWindow, createCompleted handler, 
	void* userData) {

	cwv2* w = new cwv2(parentWindow, handler, userData);
	w->AddRef();

	ICoreWebView2EnvironmentOptions* pEnvOptions = nullptr;
	if (environmentOptions) {
		pEnvOptions = ((cwv2envOpts*)environmentOptions)->GetCoreWebView2EnvironmentOptions();
	}

	theLastError_ = CreateCoreWebView2EnvironmentWithOptions(
		browserExecutableFolder, userDataFolder, pEnvOptions, w);

	if (FAILED(theLastError_)) {
		w->Release();
		w = nullptr;
		return false;
	}

	return true;
}

#define CWV2(handle) ((cwv2*)handle)

wv2_t wv2createSync(LPCWSTR browserExecutableFolder, LPCWSTR userDataFolder, 
	HWND parentWindow) {
	return wv2createSync2(browserExecutableFolder, userDataFolder, nullptr, 
		parentWindow);
}

wv2_t wv2createSync2(LPCWSTR browserExecutableFolder, LPCWSTR userDataFolder,
	wv2envOpts_t environmentOptions, HWND parentWindow) {

	cwv2* handler = new cwv2(parentWindow);
	handler->AddRef();

	ICoreWebView2EnvironmentOptions* pEnvOptions = nullptr;
	if (environmentOptions) {		
		pEnvOptions = ((cwv2envOpts*)environmentOptions)->GetCoreWebView2EnvironmentOptions();
	}

	theLastError_ = CreateCoreWebView2EnvironmentWithOptions(
		browserExecutableFolder, userDataFolder, pEnvOptions, handler);

	if (FAILED(theLastError_)) {
		handler->Release();
		handler = nullptr;
		return handler;
	}

	// 초기화가 완료될때 까지 대기
	while (not handler->isDone()) {
		wait();
	}

	if (handler->createStatus() == cwv2::completed) {
		return handler;
	}
	else {
		handler->Release();
		return nullptr;
	}
}

wv2environment_t wv2getEnvironment(wv2_t w) {
	if (!w) return nullptr;
	return CWV2(w)->getEnvironment();
}


wv2environment_t wv2getEnv(wv2_t w) {
	return wv2getEnvironment(w);
}

wv2bool wv2environment_setBrowserProcessExitedHandler(wv2environment_t e, 
	browserProcessExited handler) {
	if (!e) return wv2boolInvalidArg();
	return ((cwv2env*)e)->setBrowserProcessExitedHandler(handler);
}

wv2webResourceResponse_t
wv2environment_createWebResourceResponse(wv2environment_t env, IStream* content,
	int32_t statusCode, LPCWSTR reasonPhrase, LPCWSTR headers) {
	if (!env) return nullptr;
	return ((cwv2env*)env)->createWebResourceResponse(content, statusCode, 
		reasonPhrase, headers);
}

void wv2destroy(wv2_t* h) {
	if (!h) return;
	((cwv2*)*h)->destroy();
	*h = nullptr;
}

wv2settings_t wv2getSettings(wv2_t w) {
	if (!w) return nullptr;
	return CWV2(w)->getSettings();
}

void* wv2getUserData(wv2_t w) {
	if (!w) return nullptr;
	return CWV2(w)->getUserData();
}

bool wv2setUserData(wv2_t w, void* userData) {
	if (!w) return false;
	return CWV2(w)->setUserData(userData);
}

bool wv2executeScript(wv2_t w, LPCWSTR script, executeScriptCompleted handler) 
{
	if (!w) return false;
	return CWV2(w)->executeScript(script, handler);
}

LPCWSTR wv2executeScriptSync(wv2_t w, LPCWSTR script) {
	if (!w) return nullptr;
	return CWV2(w)->executeScriptSync(script);                              
}

LPCWSTR wv2getSource(wv2_t w) {
	if (!w) return nullptr;
	return CWV2(w)->getSource();
}

bool wv2goBack(wv2_t w) {
	if (!w) return false;
	return CWV2(w)->goBack();
}

bool wv2goForward(wv2_t w) {
	if (!w) return false;
	return CWV2(w)->goForward();
}

bool wv2navigate(wv2_t w, const wchar_t* url) {
	if (!w) return false;
	return CWV2(w)->navigate(url);
}

bool wv2navigateToString(wv2_t w, const wchar_t* htmlContent) {
	if (!w) return false;
	return CWV2(w)->navigateToString(htmlContent);
}

bool wv2navigateWithWebResource(wv2_t w, LPCWSTR uri,
	LPCWSTR method, BYTE* postData, size_t byteSize, LPCWSTR headers) {
	if (!w) return false;
	return CWV2(w)->navigateWithWebResource(uri, method, postData, byteSize,
		headers);
}

bool wv2reload(wv2_t w) {
	if (!w) return false;
	return CWV2(w)->reload();
}

bool wv2resize(wv2_t w, int width, int height) {
	if (!w) return false;
	return CWV2(w)->resize(width, height);
}

bool wv2postWebMessageAsJson(wv2_t w, LPCWSTR messageAsJson) {
	if (!w or !messageAsJson) return false;
	return CWV2(w)->postWebMessageAsJson(messageAsJson);
}

bool wv2postWebMessageAsString(wv2_t w, LPCWSTR messageAsString) {
	if (!w or !messageAsString) return false;
	return CWV2(w)->postWebMessageAsString(messageAsString);
}

bool wv2setHistoryChangedHandler(wv2_t w, historyChanged handler) {
	if (!w) return false;
	return CWV2(w)->setHistoryChangedHandler(handler);
}

bool wv2setNavigationStartingHandler(wv2_t w, navigationStarting handler) {
	if (!w) return false;
	return CWV2(w)->setNavigationStartingHandler(handler);
}

bool wv2setNavigationCompletedHandler(wv2_t w, navigationCompleted handler) {
	if (!w) return false;
	return CWV2(w)->setNavigationCompletedHandler(handler);
}

bool wv2setDomContentLoadedHandler(wv2_t w, domContentLoaded handler) {
	if (!w) return false;
	return CWV2(w)->setDomContentLoadedHandler(handler);
}

bool wv2setWindowCloseRequestedHandler(wv2_t w, windowCloseRequested handler) {
	if (!w) return false;
	return CWV2(w)->setWindowCloseRequestedHandler(handler);
}

bool wv2setIsMutedChangedHandler(wv2_t w, isMutedChanged handler) {
	if (!w) return false;
	return CWV2(w)->setIsMutedChangedHandler(handler);
}

bool wv2setIsDocumentPlayingAudioChangedHandler(wv2_t w,
	isDocumentPlayingAudioChanged handler) {
	if (!w) return false;
	return CWV2(w)->setIsDocumentPlayingAudioChangedHandler(handler);
}

bool wv2setWebMessageReceivedHandler(wv2_t w, webMessageReceived handler) {
	if (!w) return false;
	return CWV2(w)->setWebMessageReceivedHandler(handler);
}

wv2bool wv2setNewWindowRequestedHandler(wv2_t w, newWindowRequested handler) {	
	if(!w) return wv2boolInvalidArg();
	return CWV2(w)->setNewWindowRequestedHandler(handler);
}

wv2bool wv2setDocumentTitleChangedHandler(wv2_t w, documentTitleChanged handler) {
	if(!w) return wv2boolInvalidArg();
	return CWV2(w)->setDocumentTitleChangedHandler(handler);
}

wv2bool wv2setContentLoadingHandler(wv2_t w, contentLoading handler) {
	if(!w) return wv2boolInvalidArg();
	return CWV2(w)->setContentLoadingHandler(handler);
}

wv2bool wv2setScriptDialogOpningHandler(wv2_t w, scriptDialogOpening handler) {
	if(!w) return wv2boolInvalidArg();
	return CWV2(w)->setScriptDialogOpeningHandler(handler);
}

wv2bool wv2setDownloadStartingHandler(wv2_t w, downloadStarting handler) {
	if (!w) return wv2boolInvalidArg();
	return CWV2(w)->setDownloadingStartingHandler(handler);
}

wv2bool wv2setWebResourceRequestedHandler(wv2_t w, webResourceRequested handler) {
	if (!w) return wv2boolInvalidArg();
	return CWV2(w)->setWebResourceRequestedHandler(handler);
}

wv2bool wv2setAcceleratorKeyPressedHandler(wv2_t w, acceleratorKeyPressed handler) {
	if (!w) return wv2boolInvalidArg();
	return CWV2(w)->setAcceleratorKeyPressedHandler(handler);
}

LPCWSTR wv2documentTitle(wv2_t w) {
	if(!w) return nullptr;
	return CWV2(w)->documentTitle();
}

HRESULT wv2addWebResourceRequestedFilter(wv2_t w,
	LPCWSTR uri, const wv2webResourceContext resourceContext) {
	if (!w || !uri) return E_INVALIDARG;
	return CWV2(w)->addWebResourceRequestedFilter(uri, resourceContext);
}

HRESULT wv2removeWebResourceRequestedFilter(wv2_t w,
	LPCWSTR uri, const wv2webResourceContext resourceContext) {
	if (!w || !uri) return E_INVALIDARG;
	return CWV2(w)->removeWebResourceRequestedFilter(uri, resourceContext);
}

wv2cookieManager_t wv2getCookieManager(wv2_t w) {
	if (!w) return nullptr;
	return CWV2(w)->cookieManager();
}

bool wv2stop(wv2_t w) {
	if (!w) return false;
	return CWV2(w)->stop();
}

void wv2freeMemory(void* p) {
	if (p) free(p);
}

double wv2zoomFactor(wv2_t w, const double* newZoomFactor) {
	if (!w) return -1.0;
	return CWV2(w)->zoomFactor(newZoomFactor);
}

wv2bool wv2isMuted(wv2_t w) {
	if (!w) return wv2boolInvalidArg();
	return CWV2(w)->isMuted();
}

wv2bool wv2setIsMuted(wv2_t w, const bool muted) {
	if (!w) return wv2boolInvalidArg();
	return CWV2(w)->setIsMuted(muted);
}

wv2bool wv2isDocumentPlayingAudio(wv2_t w) {
	if (!w) return wv2boolInvalidArg();
	return CWV2(w)->isDocumentPlayingAudio();
}

bool wv2setVirtualHostNameToFolderMapping(wv2_t w, LPCWSTR hostName,
	LPCWSTR folderPath, wv2HostResourceAccessKind accessKind) {
	if (!w) return false;
	return CWV2(w)->setVirtualHostNameToFolderMapping(hostName, 
		folderPath, accessKind);
}

wv2bool wv2openTaskManagerWindow(wv2_t w) {
	if (!w) return wv2boolInvalidArg();
	return CWV2(w)->openTaskManagerWindow();
}

HRESULT wv2lastError(wv2_t w) {
	return w ? CWV2(w)->lastError() : theLastError_;
}

LPWSTR wv2getAvailableBrowserVersionString(LPCWSTR browserExecutableFolder) {
	LPWSTR versionInfo = nullptr;
	HRESULT hr = GetAvailableCoreWebView2BrowserVersionString(browserExecutableFolder,
		&versionInfo);
	if (SUCCEEDED(hr)) {
		return _wcsdup(versionInfo);
	}
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
LPWSTR
wv2newWindowRequestedEventArgs_uri(wv2newWindowRequestedEventArgs_t args) {
	if(!args) return nullptr;
	return ((wv2newWindowRequestedEventArgs*)args)->uri();
}

bool
wv2newWindowRequestedEventArgs_handled(wv2newWindowRequestedEventArgs_t args) {
	if(!args) return false;
	return ((wv2newWindowRequestedEventArgs*)args)->handled();
}

wv2bool
wv2newWindowRequestedEventArgs_setHandled(wv2newWindowRequestedEventArgs_t args, bool handled) {
	if(!args) return wv2boolInvalidArg();
	return ((wv2newWindowRequestedEventArgs*)args)->setHandled(handled);
}

bool
wv2newWindowRequestedEventArgs_isUserInitiated(wv2newWindowRequestedEventArgs_t args) {
	if(!args) return false;
	return ((wv2newWindowRequestedEventArgs*)args)->isUserInitiated();
}

wv2deferral_t
wv2newWindowRequestedEventArgs_getDeferral(wv2newWindowRequestedEventArgs_t args) {
	if(!args) return nullptr;
	return ((wv2newWindowRequestedEventArgs*)args)->getDeferral();
}

wv2windowFeatures_t
wv2newWindowRequestedEventArgs_windowFeatures(wv2newWindowRequestedEventArgs_t args) {
	if(!args) return nullptr;
	return ((wv2newWindowRequestedEventArgs*)args)->windowFeatures();
}

///////////////////////////////////////////////////////////////////////////////
uint64_t  wv2domContentLoadedEventArgs_navigationId(wv2domContentLoadedEventArgs_t args) {
	if(!args) return 0;
	return ((wv2domContentLoadedEventArgs*)args)->navigationId();
}

///////////////////////////////////////////////////////////////////////////////
bool wv2contentLoadingEventArgs_isErrorPage(wv2contentLoadingEventArgs_t args) {
	if(!args) return false;
	return ((wv2contentLoadingEventArgs*)args)->isErrorPage();
}

uint64_t wv2contentLoadingEventArgs_t_navigationId(wv2contentLoadingEventArgs_t args) {
	if(!args) return 0;
	return ((wv2contentLoadingEventArgs*)args)->navigationId();
}

///////////////////////////////////////////////////////////////////////////////
wv2bool wv2settings_isScriptEnabled(wv2settings_t s) {
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->isScriptEnabled();
}

wv2bool wv2settings_setIsScriptEnabled(wv2settings_t s, bool isScriptEnabled){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->setIsScriptEnabled(isScriptEnabled);
}

wv2bool wv2settings_isWebMessageEnabled(wv2settings_t s){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->isWebMessageEnabled();
}

wv2bool wv2settings_setIsWebMessageEnabled(wv2settings_t s, bool isWebMessageEnabled){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->setIsWebMessageEnabled(isWebMessageEnabled);
}

wv2bool wv2settings_areDefaultScriptDialogsEnabled(wv2settings_t s){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->areDefaultScriptDialogsEnabled();
}

wv2bool wv2settings_setAreDefaultScriptDialogsEnabled(wv2settings_t s, bool areDefaultScriptDialogsEnabled){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->setAreDefaultScriptDialogsEnabled(areDefaultScriptDialogsEnabled);
}

wv2bool wv2settings_isStatusBarEnabled(wv2settings_t s){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->isStatusBarEnabled();
}

wv2bool wv2settings_setIsStatusBarEnabled(wv2settings_t s, bool isStatusBarEnabled){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->setIsStatusBarEnabled(isStatusBarEnabled);
}

wv2bool wv2settings_areDevToolsEnabled(wv2settings_t s){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->areDevToolsEnabled();
}

wv2bool wv2settings_setAreDevToolsEnabled(wv2settings_t s, bool areDevToolsEnabled){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->setAreDevToolsEnabled(areDevToolsEnabled);
}

wv2bool wv2settings_areDefaultContextMenusEnabled(wv2settings_t s){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->areDefaultContextMenusEnabled();
}

wv2bool wv2settings_setAreDefaultContextMenusEnabled(wv2settings_t s, bool enabled){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->setAreDefaultContextMenusEnabled(enabled);
}

wv2bool wv2settings_areHostObjectsAllowed(wv2settings_t s){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->areHostObjectsAllowed();
}

wv2bool wv2settings_setAreHostObjectsAllowed(wv2settings_t s, bool allowed){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->setAreHostObjectsAllowed(allowed);
}

wv2bool wv2settings_isZoomControlEnabled(wv2settings_t s){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->isZoomControlEnabled();
}

wv2bool wv2settings_setIsZoomControlEnabled(wv2settings_t s, bool enabled){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->setIsZoomControlEnabled(enabled);
}

wv2bool wv2settings_isBuiltInErrorPageEnabled(wv2settings_t s){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->isBuiltInErrorPageEnabled();
}

wv2bool wv2settings_setIsBuiltInErrorPageEnabled(wv2settings_t s, bool enabled){
	if(!s) return wv2boolInvalidArg();
	return ((wv2settings*)s)->setIsBuiltInErrorPageEnabled(enabled);
}

LPWSTR wv2settings_userAgent(wv2settings_t s) {
	if(!s) return nullptr;
	return ((wv2settings*)s)->userAgent();
}

HRESULT wv2settings_setUserAgent(wv2settings_t s, LPCWSTR userAgent) {
	if(!s) return E_INVALIDARG;
	return ((wv2settings*)s)->setUserAgent(userAgent);
}

///////////////////////////////////////////////////////////////////////////////
HRESULT wv2deferral_complete(wv2deferral_t d) {
	if(!d) return E_INVALIDARG;
	return ((wv2deferral*)d)->complete();
}

void wv2deleteDeferral(wv2deferral_t* d) {
	if(!d) return;
	wv2deferral* p = (wv2deferral*)*d;
	delete p;
	*d = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
LPWSTR wv2scriptDialogOpeningEventArgs_uri(wv2scriptDialogOpeningEventArgs_t args) {
	if(!args) return nullptr;
	return ((wv2scriptDialogOpeningEventArgs*)args)->uri();
}

wv2scriptDialogKind wv2scriptDialogOpeningEventArgs_kind(wv2scriptDialogOpeningEventArgs_t args) {
	if(!args) return wv2scriptDialogKind_undefined;
	return ((wv2scriptDialogOpeningEventArgs*)args)->kind();
}

LPWSTR wv2scriptDialogOpeningEventArgs_message(wv2scriptDialogOpeningEventArgs_t args) {
	if(!args) return nullptr;
	return ((wv2scriptDialogOpeningEventArgs*)args)->message();
}

HRESULT wv2scriptDialogOpeningEventArgs_accept(wv2scriptDialogOpeningEventArgs_t args) {
	if(!args) return E_INVALIDARG;
	return ((wv2scriptDialogOpeningEventArgs*)args)->accept();
}

LPWSTR wv2scriptDialogOpeningEventArgs_defaultText(wv2scriptDialogOpeningEventArgs_t args) {
	if(!args) return nullptr;
	return ((wv2scriptDialogOpeningEventArgs*)args)->defaultText();
}

LPWSTR wv2scriptDialogOpeningEventArgs_resultText(wv2scriptDialogOpeningEventArgs_t args) {
	if(!args) return nullptr;
	return ((wv2scriptDialogOpeningEventArgs*)args)->resultText();
}

HRESULT wv2scriptDialogOpeningEventArgs_setResultText(wv2scriptDialogOpeningEventArgs_t args, 
	LPCWSTR resultText) {
	if(!args || !resultText) return E_INVALIDARG;
	return ((wv2scriptDialogOpeningEventArgs*)args)->setResultText(resultText);
}

wv2deferral_t wv2scriptDialogOpeningEventArgs_getDeferral(wv2scriptDialogOpeningEventArgs_t args) {
	if(!args) return nullptr;
	return ((wv2scriptDialogOpeningEventArgs*)args)->getDeferral();
}

///////////////////////////////////////////////////////////////////////////////
wv2bool
wv2windowFeatures_hasPosition(wv2windowFeatures_t f) {
	if(!f) return wv2boolInvalidArg();
	return ((wv2windowFeatures*)f)->hasPosition();
}

wv2bool
wv2windowFeatures_hasSize(wv2windowFeatures_t f) {
	if(!f) return wv2boolInvalidArg();
	return ((wv2windowFeatures*)f)->hasSize();
}

uint32_t
wv2windowFeatures_left(wv2windowFeatures_t f) {
	if(!f) return 0;
	return ((wv2windowFeatures*)f)->left();
}

uint32_t
wv2windowFeatures_top(wv2windowFeatures_t f) {
	if(!f) return 0;
	return ((wv2windowFeatures*)f)->top();
}

uint32_t
wv2windowFeatures_height(wv2windowFeatures_t f) {
	if(!f) return 0;
	return ((wv2windowFeatures*)f)->height();
}

uint32_t
wv2windowFeatures_width(wv2windowFeatures_t f) {
	if(!f) return 0;
	return ((wv2windowFeatures*)f)->width();
}

wv2bool
wv2windowFeatures_shouldDisplayMenuBar(wv2windowFeatures_t f) {
	if(!f) return wv2boolInvalidArg();
	return ((wv2windowFeatures*)f)->shouldDisplayMenuBar();
}

wv2bool
wv2windowFeatures_shouldDisplayStatus(wv2windowFeatures_t f) {
	if(!f) return wv2boolInvalidArg();
	return ((wv2windowFeatures*)f)->shouldDisplayStatus();
}

wv2bool
wv2windowFeatures_shouldDisplayToolbar(wv2windowFeatures_t f) {
	if(!f) return wv2boolInvalidArg();
	return ((wv2windowFeatures*)f)->shouldDisplayToolbar();
}

wv2bool
wv2windowFeatures_shouldDisplayScrollBars(wv2windowFeatures_t f) {
	if(!f) return wv2boolInvalidArg();
	return ((wv2windowFeatures*)f)->shouldDisplayScrollBars();
}

///////////////////////////////////////////////////////////////////////////////
bool
wv2downloadStartingEventArgs_cancel(wv2downloadStartingEventArgs_t args) {
	if (!args) return false;
	return ((wv2downloadStartingEventArgs*)args)->cancel();
}

HRESULT
wv2downloadStartingEventArgs_setCancel(wv2downloadStartingEventArgs_t args, bool cancel) {
	if (!args) return E_INVALIDARG;
	return ((wv2downloadStartingEventArgs*)args)->setCancel(cancel);
}

LPWSTR
wv2downloadStartingEventArgs_resultFilePath(wv2downloadStartingEventArgs_t args) {
	if (!args) return nullptr;
	return ((wv2downloadStartingEventArgs*)args)->resultFilePath();
}

HRESULT
wv2downloadStartingEventArgs_setResultFilePath(wv2downloadStartingEventArgs_t args, LPCWSTR resultFilePath) {
	if (!args || !resultFilePath) return E_INVALIDARG;
	return ((wv2downloadStartingEventArgs*)args)->setResultFilePath(resultFilePath);
}

bool
wv2downloadStartingEventArgs_handled(wv2downloadStartingEventArgs_t args) {
	if (!args) return false;
	return ((wv2downloadStartingEventArgs*)args)->handled();
}

HRESULT
wv2downloadStartingEventArgs_setHandled(wv2downloadStartingEventArgs_t args, bool handled) {
	if (!args) return E_INVALIDARG;
	return ((wv2downloadStartingEventArgs*)args)->setHandled(handled);
}

wv2deferral_t
wv2downloadStartingEventArgs_getDeferral(wv2downloadStartingEventArgs_t args) {
	if (!args) return nullptr;
	return ((wv2downloadStartingEventArgs*)args)->getDeferral();
}

///////////////////////////////////////////////////////////////////////////////
#define WRREQ ((wv2webResourceRequest*)handle)

LPWSTR wv2webResourceRequest_uri(wv2webResourceRequest_t handle) {
	if (!handle) return nullptr;
	return WRREQ->uri();
}

HRESULT wv2webResourceRequest_setUri(wv2webResourceRequest_t handle, LPCWSTR uri) {
	if (!handle || !uri) return E_INVALIDARG;
	return WRREQ->setUri(uri);
}

LPWSTR wv2webResourceRequest_method(wv2webResourceRequest_t handle) {
	if (!handle) return nullptr;
	return WRREQ->method();
}

HRESULT wv2webResourceRequest_setMethod(wv2webResourceRequest_t handle, LPCWSTR method) {
	if (!handle || !method) return E_INVALIDARG;
	return WRREQ->setMethod(method);
}

wv2httpRequestHeaders_t wv2webResourceRequest_headers(wv2webResourceRequest_t handle) {
	if (!handle) return nullptr;
	return WRREQ->headers();
}

///////////////////////////////////////////////////////////////////////////////
#define WRRES ((wv2webResourceResponse*)handle)
int32_t
wv2webResourceResponse_statusCode(wv2webResourceResponse_t handle) {
	if (!handle) return -1;
	return WRRES->statusCode();
}

HRESULT
wv2webResourceResponse_setStatusCode(wv2webResourceResponse_t handle, int32_t statusCode) {
	if (!handle) return E_INVALIDARG;
	return WRRES->setStatusCode(statusCode);
}

LPWSTR
wv2webResourceResponse_reasonPhrase(wv2webResourceResponse_t handle) {
	if (!handle) return nullptr;
	return WRRES->reasonPhrase();
}

HRESULT
wv2webResourceResponse_setReasonPhrase(wv2webResourceResponse_t handle, LPCWSTR reasonPhrase) {
	if (!handle || !reasonPhrase) return E_INVALIDARG;
	return WRRES->setReasonPhrase(reasonPhrase);
}
///////////////////////////////////////////////////////////////////////////////
#define WRREQUESTED_ARGS ((wv2webResourceRequestedEventArgs*)args)
wv2webResourceRequest_t
wv2webResourceRequestedEventArgs_request(wv2webResourceRequestedEventArgs_t args) {
	if (!args) return nullptr;
	return WRREQUESTED_ARGS->request();
}

wv2webResourceResponse_t
wv2webResourceRequestedEventArgs_response(wv2webResourceRequestedEventArgs_t args) {
	if (!args) return nullptr;
	return WRREQUESTED_ARGS->response();
}

wv2webResourceContext
wv2webResourceRequestedEventArgs_resourceContext(wv2webResourceRequestedEventArgs_t args) {
	if (!args) return wv2webResourceContext_undefined;
	return WRREQUESTED_ARGS->resourceContext();
}

///////////////////////////////////////////////////////////////////////////////
#define REQ_HDRS ((wv2httpRequestHeaders*)handle)

LPWSTR wv2httpRequestHeaders_getHeader(wv2httpRequestHeaders_t handle, LPCWSTR name) {
	if (!handle || !name) return nullptr;
	return REQ_HDRS->getHeader(name);
}

bool wv2httpRequestHeaders_contains(wv2httpRequestHeaders_t handle, LPCWSTR name) {
	if (!handle || !name) return false;
	return REQ_HDRS->contains(name);
}

HRESULT wv2httpRequestHeaders_setHeader(wv2httpRequestHeaders_t handle, LPCWSTR name, 
	LPCWSTR value) {
	if (!handle || !name) return E_INVALIDARG;
	return REQ_HDRS->setHeader(name, value);
}

HRESULT wv2httpRequestHeaders_removeHeader(wv2httpRequestHeaders_t handle, LPCWSTR name) {
	if (!handle || !name) return E_INVALIDARG;
	return REQ_HDRS->removeHeader(name);
}

///////////////////////////////////////////////////////////////////////////////
#define NC_ARSG ((wv2navigationCompletedEventArgs*)args)
bool wv2navigationCompletedEventArgs_isSuccess(wv2navigationCompletedEventArgs_t args) {
	if (!args) return false;
	return NC_ARSG->isSuccess();
}

wv2webErrorStatus wv2navigationCompletedEventArgs_webErrorStatus(wv2navigationCompletedEventArgs_t args) {
	if (!args) return wv2webErrorStatus_unknown;
	return NC_ARSG->webErrorStatus();
}

uint64_t wv2navigationCompletedEventArgs_navigationId(wv2navigationCompletedEventArgs_t args) {
	if (!args) return 0;
	return NC_ARSG->navigationId();
}

///////////////////////////////////////////////////////////////////////////////
#define AKP_ARGS ((wv2acceleratorKeyPressedEventArgs*)args)

WV2_API wv2KeyEventKind
wv2acceleratorKeyPressedEventArgs_keyEventKind(wv2acceleratorKeyPressedEventArgs_t args) {
	if (!args) return wv2KeyEventKind_undefiend;
	return AKP_ARGS->keyEventKind();
}

WV2_API uint32_t
wv2acceleratorKeyPressedEventArgs_virtualKey(wv2acceleratorKeyPressedEventArgs_t args) {
	if (!args) return 0;
	return AKP_ARGS->virtualKey();
}

WV2_API int32_t
wv2acceleratorKeyPressedEventArgs_keyEventLParam(wv2acceleratorKeyPressedEventArgs_t args) {
	if (!args) return 0;
	return AKP_ARGS->keyEventLParam();
}

WV2_API wv2physicalKeyStatus
wv2acceleratorKeyPressedEventArgs_physicalKeyStatus(wv2acceleratorKeyPressedEventArgs_t args) {
	wv2physicalKeyStatus result = { 0, };
	if (!args) return result;
	result = AKP_ARGS->physicalKeyStatus();
	return result;
}

WV2_API bool
wv2acceleratorKeyPressedEventArgs_handled(wv2acceleratorKeyPressedEventArgs_t args) {
	if (!args) return false;
	return AKP_ARGS->handled();
}

WV2_API HRESULT
wv2acceleratorKeyPressedEventArgs_setHandled(wv2acceleratorKeyPressedEventArgs_t args, bool handled) {
	if (!args) return E_INVALIDARG;
	return AKP_ARGS->setHandled(handled);
}

///////////////////////////////////////////////////////////////////////////////
#define COOKIE ((wv2cookie*)h)

LPWSTR wv2cookie_name(wv2cookie_t h) {
	if (!h) return nullptr;
	return COOKIE->name();
}

LPWSTR wv2cookie_value(wv2cookie_t h) {
	if (!h) return nullptr;
	return COOKIE->value();
}

HRESULT wv2cookie_setValue(wv2cookie_t h, LPWSTR domain) {
	if (!h) return E_INVALIDARG;
	return COOKIE->setValue(domain);
}

LPWSTR wv2cookie_domain(wv2cookie_t h) {
	if (!h) return nullptr;
	return COOKIE->domain();
}

LPWSTR wv2cookie_path(wv2cookie_t h) {
	if (!h) return nullptr;
	return COOKIE->path();
}

double wv2cookie_expires(wv2cookie_t h) {
	if (!h) return 0;
	return COOKIE->expires();
}

HRESULT wv2cookie_setExpires(wv2cookie_t h, double expires) {
	if (!h) return E_INVALIDARG;
	return COOKIE->setExpires(expires);
}

bool wv2cookie_isHttpOnly(wv2cookie_t h) {
	if (!h) return false;
	return COOKIE->isHttpOnly();
}

HRESULT wv2cookie_t_setIsHttpOnly(wv2cookie_t h, bool isHttpOnly) {
	if (!h) return E_INVALIDARG;
	return COOKIE->setIsHttpOnly(isHttpOnly);
}

wv2cookieSameSiteKind wv2cookie_sameSite(wv2cookie_t h) {
	if (!h) return wv2cookieSameSiteKind_undefined;
	return COOKIE->sameSite();
}

HRESULT wv2cookie_setSameSite(wv2cookie_t h, wv2cookieSameSiteKind sameSite) {
	if (!h) return E_INVALIDARG;
	return COOKIE->setSameSite(sameSite);
}

bool wv2cookie_isSecure(wv2cookie_t h) {
	if (!h) return false;
	return COOKIE->isSecure();
}

HRESULT wv2cookie_setIsSecure(wv2cookie_t h, bool isSecure) {
	if (!h) return E_INVALIDARG;
	return COOKIE->setIsSecure(isSecure);
}

bool wv2cookie_isSession(wv2cookie_t h) {
	if (!h) return false;
	return COOKIE->isSession();
}

void wv2cookie_destroy(wv2cookie_t* h) {
	if (!h) return;
	((wv2cookie*)*h)->destroy();
	*h = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
#define CK_LIST ((wv2cookieList*)h)
UINT wv2cookieList_count(wv2cookieList_t h) {
	return CK_LIST->count();
}

wv2cookie_t wv2cookieList_getValueAtIndex(wv2cookieList_t h, UINT index) {
	return CK_LIST->getValueAtIndex(index);
}

///////////////////////////////////////////////////////////////////////////////
#define CM ((wv2cookieManager*)h)

wv2cookie_t wv2cookieManager_createCookie(wv2cookieManager_t h, LPCWSTR name,
	LPCWSTR value, LPCWSTR domain, LPCWSTR path) {
	if (!h) return nullptr;
	return CM->createCookie(name, value, domain, path);
}

wv2cookie_t wv2cookieManager_copyCookie(wv2cookieManager_t h, wv2cookie_t cookieParam) {
	if (!h) return nullptr;
	return CM->copyCookie((wv2cookie*)cookieParam);
}

HRESULT wv2cookieManager_getCookies(wv2cookieManager_t h, LPCWSTR uri,
	getCookiesCompleted handler) {
	if (!h) return E_INVALIDARG;
	return CM->getCookies(uri, handler);
}

HRESULT wv2cookieManager_addOrUpdateCookie(wv2cookieManager_t h, wv2cookie_t cookie) {
	if (!h) return E_INVALIDARG;
	return CM->addOrUpdateCookie((wv2cookie*)cookie);
}

HRESULT wv2cookieManager_deleteCookie(wv2cookieManager_t h, wv2cookie_t cookie) {
	if (!h) return E_INVALIDARG;
	return CM->deleteCookie((wv2cookie*)cookie);
}

HRESULT wv2cookieManager_deleteCookies(wv2cookieManager_t h, LPCWSTR name, LPCWSTR uri) {
	if (!h) return E_INVALIDARG;
	return CM->deleteCookies(name, uri);
}

HRESULT wv2cookieManager_deleteCookiesWithDomainAndPath(wv2cookieManager_t h,
	LPCWSTR name, LPCWSTR domain, LPCWSTR path) {
	if (!h) return E_INVALIDARG;
	return CM->deleteCookiesWithDomainAndPath(name, domain, path);
}

HRESULT wv2cookieManager_deleteAllCookies(wv2cookieManager_t h) {
	if (!h) return E_INVALIDARG;
	return CM->deleteAllCookies();
}