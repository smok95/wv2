#include <ciso646>
#include <comdef.h>

#include "wv2.h"

#include "cwv2.h"

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
void wv2create(LPCWSTR browserExecutableFolder, HWND parentWindow,
	createCompleted handler, void* userData) {

	cwv2* w = new cwv2(parentWindow, handler, userData);
	w->AddRef();
	theLastError_ = CreateCoreWebView2EnvironmentWithOptions(
		browserExecutableFolder, nullptr, nullptr, w);

	if (FAILED(theLastError_)) {
		w->Release();
		w = nullptr;
		if (handler) {
			handler(nullptr, theLastError_);
		}
	}
}

wv2_t wv2createSync(LPCWSTR browserExecutableFolder, HWND parentWindow) {
	cwv2* handler = new cwv2(parentWindow);
	handler->AddRef();

	theLastError_ = CreateCoreWebView2EnvironmentWithOptions(
		browserExecutableFolder, nullptr, nullptr, handler);

	if (FAILED(theLastError_)) {
		handler->Release();
		handler = nullptr;
		return handler;
	}

	// 초기화가 완료될때 까지 대기
	while (not handler->isDone()) {
		wait();
	}

	if (handler->crateStatus() == cwv2::completed) {
		return handler;
	}
	else {
		handler->Release();
		return nullptr;
	}
}

void wv2destroy(wv2_t* h) {
	if (!h) return;
	((cwv2*)*h)->destroy();
	*h = nullptr;
}

wv2settings* wv2getSettings(wv2_t w) {
	if (!w) return nullptr;
	return ((cwv2*)w)->getSettings();
}

bool wv2setSettings(wv2_t w, const wv2settings* settings) {
	if (!w) return false;
	return ((cwv2*)w)->setSettings(settings);
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

bool wv2reload(wv2_t w) {
	if (!w) return false;
	return ((cwv2*)w)->reload();
}

bool wv2resize(wv2_t w, int width, int height) {
	if (!w) return false;
	return ((cwv2*)w)->resize(width, height);
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

bool wv2setVirtualHostNameToFolderMapping(wv2_t w, LPCWSTR hostName,
	LPCWSTR folderPath, wv2HostResourceAccessKind accessKind) {
	if (!w) return false;
	return ((cwv2*)w)->setVirtualHostNameToFolderMapping(hostName, 
		folderPath, accessKind);
}

HRESULT wv2lastError(wv2_t w) {
	return w ? ((cwv2*)w)->lastError() : theLastError_;
}

LPCWSTR wv2errorMessage(HRESULT hr) {
	_com_error err(hr);
	return _wcsdup(err.ErrorMessage());
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
