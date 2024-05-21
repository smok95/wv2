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

wv2env_t wv2getEnv(wv2_t w) {
	if (!w) return nullptr;
	return ((cwv2*)w)->getEnvironment();
}

wv2bool wv2envSetBrowserProcessExitedHandler(wv2env_t e, browserProcessExited handler) {
	if (!e) return wv2boolInvalidArg();
	return ((cwv2env*)e)->setBrowserProcessExitedHandler(handler);
}


void wv2destroy(wv2_t* h) {
	if (!h) return;
	((cwv2*)*h)->destroy();
	*h = nullptr;
}

wv2settings_t wv2getSettings(wv2_t w) {
	if (!w) return nullptr;
	return ((cwv2*)w)->getSettings();
}

void* wv2getUserData(wv2_t w) {
	if (!w) return nullptr;
	return ((cwv2*)w)->getUserData();
}

bool wv2setUserData(wv2_t w, void* userData) {
	if (!w) return false;
	return ((cwv2*)w)->setUserData(userData);
}

bool wv2executeScript(wv2_t w, LPCWSTR script, executeScriptCompleted handler) 
{
	if (!w) return false;
	return ((cwv2*)w)->executeScript(script, handler);
}

LPCWSTR wv2executeScriptSync(wv2_t w, LPCWSTR script) {
	if (!w) return nullptr;
	return ((cwv2*)w)->executeScriptSync(script);                              
}

LPCWSTR wv2getSource(wv2_t w) {
	if (!w) return nullptr;
	return ((cwv2*)w)->getSource();
}

bool wv2goBack(wv2_t w) {
	if (!w) return false;
	return ((cwv2*)w)->goBack();
}

bool wv2goForward(wv2_t w) {
	if (!w) return false;
	return ((cwv2*)w)->goForward();
}

bool wv2navigate(wv2_t w, const wchar_t* url) {
	if (!w) return false;
	return ((cwv2*)w)->navigate(url);
}

bool wv2navigateToString(wv2_t w, const wchar_t* htmlContent) {
	if (!w) return false;
	return ((cwv2*)w)->navigateToString(htmlContent);
}

bool wv2navigateWithWebResource(wv2_t w, LPCWSTR uri,
	LPCWSTR method, BYTE* postData, size_t byteSize, LPCWSTR headers) {
	if (!w) return false;
	return ((cwv2*)w)->navigateWithWebResource(uri, method, postData, byteSize,
		headers);
}

bool wv2reload(wv2_t w) {
	if (!w) return false;
	return ((cwv2*)w)->reload();
}

bool wv2resize(wv2_t w, int width, int height) {
	if (!w) return false;
	return ((cwv2*)w)->resize(width, height);
}

bool wv2postWebMessageAsJson(wv2_t w, LPCWSTR messageAsJson) {
	if (!w or !messageAsJson) return false;
	return ((cwv2*)w)->postWebMessageAsJson(messageAsJson);
}

bool wv2postWebMessageAsString(wv2_t w, LPCWSTR messageAsString) {
	if (!w or !messageAsString) return false;
	return ((cwv2*)w)->postWebMessageAsString(messageAsString);
}

bool wv2setHistoryChangedHandler(wv2_t w, historyChanged handler) {
	if (!w) return false;
	return ((cwv2*)w)->setHistoryChangedHandler(handler);
}

bool wv2setNavigationStartingHandler(wv2_t w, navigationStarting handler) {
	if (!w) return false;
	return ((cwv2*)w)->setNavigationStartingHandler(handler);
}

bool wv2setNavigationCompletedHandler(wv2_t w, navigationCompleted handler) {
	if (!w) return false;
	return ((cwv2*)w)->setNavigationCompletedHandler(handler);
}

bool wv2setDomContentLoadedHandler(wv2_t w, domContentLoaded handler) {
	if (!w) return false;
	return ((cwv2*)w)->setDomContentLoadedHandler(handler);
}

bool wv2setWindowCloseRequestedHandler(wv2_t w, windowCloseRequested handler) {
	if (!w) return false;
	return ((cwv2*)w)->setWindowCloseRequestedHandler(handler);
}

bool wv2setIsMutedChangedHandler(wv2_t w, isMutedChanged handler) {
	if (!w) return false;
	return ((cwv2*)w)->setIsMutedChangedHandler(handler);
}

bool wv2setIsDocumentPlayingAudioChangedHandler(wv2_t w,
	isDocumentPlayingAudioChanged handler) {
	if (!w) return false;
	return ((cwv2*)w)->setIsDocumentPlayingAudioChangedHandler(handler);
}

bool wv2setWebMessageReceivedHandler(wv2_t w, webMessageReceived handler) {
	if (!w) return false;
	return ((cwv2*)w)->setWebMessageReceivedHandler(handler);
}

wv2bool wv2setNewWindowRequestedHandler(wv2_t w, newWindowRequested handler) {	
	if(!w) return wv2boolInvalidArg();
	return ((cwv2*)w)->setNewWindowRequestedHandler(handler);
}

wv2bool wv2setDocumentTitleChangedHandler(wv2_t w, documentTitleChanged handler) {
	if(!w) return wv2boolInvalidArg();
	return ((cwv2*)w)->setDocumentTitleChangedHandler(handler);
}

wv2bool wv2setContentLoadingHandler(wv2_t w, contentLoading handler) {
	if(!w) return wv2boolInvalidArg();
	return ((cwv2*)w)->setContentLoadingHandler(handler);
}

wv2bool wv2setScriptDialogOpningHandler(wv2_t w, scriptDialogOpening handler) {
	if(!w) return wv2boolInvalidArg();
	return ((cwv2*)w)->setScriptDialogOpeningHandler(handler);
}

LPCWSTR wv2documentTitle(wv2_t w) {
	if(!w) return nullptr;
	return ((cwv2*)w)->documentTitle();
}

bool wv2stop(wv2_t w) {
	if (!w) return false;
	return ((cwv2*)w)->stop();
}

void wv2freeMemory(void* p) {
	if (p) free(p);
}

double wv2zoomFactor(wv2_t w, const double* newZoomFactor) {
	if (!w) return -1.0;
	return ((cwv2*)w)->zoomFactor(newZoomFactor);
}

wv2bool wv2isMuted(wv2_t w) {
	if (!w) return wv2boolInvalidArg();
	return ((cwv2*)w)->isMuted();
}

wv2bool wv2setIsMuted(wv2_t w, const bool muted) {
	if (!w) return wv2boolInvalidArg();
	return ((cwv2*)w)->setIsMuted(muted);
}

wv2bool wv2isDocumentPlayingAudio(wv2_t w) {
	if (!w) return wv2boolInvalidArg();
	return ((cwv2*)w)->isDocumentPlayingAudio();
}

bool wv2setVirtualHostNameToFolderMapping(wv2_t w, LPCWSTR hostName,
	LPCWSTR folderPath, wv2HostResourceAccessKind accessKind) {
	if (!w) return false;
	return ((cwv2*)w)->setVirtualHostNameToFolderMapping(hostName, 
		folderPath, accessKind);
}

wv2bool wv2openTaskManagerWindow(wv2_t w) {
	if (!w) return wv2boolInvalidArg();
	return ((cwv2*)w)->openTaskManagerWindow();
}

HRESULT wv2lastError(wv2_t w) {
	return w ? ((cwv2*)w)->lastError() : theLastError_;
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