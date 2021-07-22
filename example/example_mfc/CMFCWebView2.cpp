#include "pch.h"
#include "CMFCWebView2.h"

#include "wv2.h"
#pragma comment(lib,"wv2")


static CStringW gBrowserFolder;

// CMFCWebView2
IMPLEMENT_DYNAMIC(CMFCWebView2, CWnd)

CMFCWebView2::CMFCWebView2()
{
	webview2_ = nullptr;
}

CMFCWebView2::~CMFCWebView2()
{
	if (webview2_) {
		webview2_->destroy();
		webview2_ = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CMFCWebView2, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

bool CMFCWebView2::Initialize(LPCTSTR browserExecutableFolder) {

	if (browserExecutableFolder) {
#if _UNICODE
		gBrowserFolder = browserExecutableFolder;
#else
		gBrowserFolder = CStringW(browserExecutableFolder);
#endif // _UNICODE
	}

	LPWSTR verStr = wv2getAvailableBrowserVersionString(gBrowserFolder);
	if (verStr) {
		wv2freeMemory((void*)verStr);
		return true;
	}
	return false;
}

void CMFCWebView2::PreSubclassWindow() {
	CWnd::PreSubclassWindow();
	webview2_ = (wv2*)wv2createSync(gBrowserFolder, GetSafeHwnd());
}

bool CMFCWebView2::Navigate(LPCTSTR uri) {
	if (!uri || !webview2_) return false;
		
#if _UNICODE
	LPCWSTR wUri = uri;
#else
	LPCWSTR wUri = CStringW(uri);
#endif

	return webview2_->navigate(wUri);
}

int CMFCWebView2::OnCreate(LPCREATESTRUCT lpcs) {
	if (CWnd::OnCreate(lpcs) == -1) {
		return -1;
	}

	webview2_ = (wv2*)wv2createSync(nullptr, GetSafeHwnd());

	return 0;
}

void CMFCWebView2::OnSize(UINT nType, int cx, int cy) {
	CWnd::OnSize(nType, cx, cy);
	if (GetSafeHwnd()) {
		if (webview2_)	webview2_->resize(cx, cy);
	}
}

bool CMFCWebView2::GoBack() {
	return webview2_ ? webview2_->goBack() : false;
}

bool CMFCWebView2::GoForward() {
	return webview2_ ? webview2_->goForward() : false;
}

bool CMFCWebView2::Reload() {
	return webview2_ ? webview2_->reload() : false;
}
