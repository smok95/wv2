// CMFCWebView2.cpp: 구현 파일
//

#include "pch.h"
#include "example_mfc.h"
#include "CMFCWebView2.h"


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
END_MESSAGE_MAP()

void CMFCWebView2::PreSubclassWindow() {
	CWnd::PreSubclassWindow();
	if (webview2_ = (wv2*)wv2createSync(nullptr, GetSafeHwnd())) {

		if (wv2settings* settings = webview2_->getSettings()) {
			settings->areDevToolsEnabled = false;
			webview2_->setSettings(settings);
		}

		webview2_->navigate(L"https://google.com");
	}
}

int CMFCWebView2::OnCreate(LPCREATESTRUCT lpcs) {
	if (CWnd::OnCreate(lpcs) == -1) {
		return -1;
	}

	if (webview2_ = (wv2*)wv2createSync(nullptr, GetSafeHwnd())) {
		webview2_->navigate(L"https://google.com");
	}


	return 0;
}

