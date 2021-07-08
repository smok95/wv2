// CMFCWebView2.cpp: 구현 파일
//

#include "pch.h"
#include "example_mfc.h"
#include "CMFCWebView2.h"


// CMFCWebView2

IMPLEMENT_DYNAMIC(CMFCWebView2, CStatic)

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


BEGIN_MESSAGE_MAP(CMFCWebView2, CStatic)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CMFCWebView2::PreSubclassWindow() {
	CStatic::PreSubclassWindow();
	if (webview2_ = (wv2*)wv2createSync(nullptr, GetSafeHwnd())) {
		webview2_->navigate(L"https://google.com");
	}
}

BOOL CMFCWebView2::Create(LPCTSTR lpszText, DWORD dwStyle,
	const RECT& rect, CWnd* pParentWnd, UINT nID ) {
	BOOL result = CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
	if (webview2_ = (wv2*)wv2createSync(nullptr, GetSafeHwnd())) {
		webview2_->navigate(L"https://google.com");
	}
	return result;
}

int CMFCWebView2::OnCreate(LPCREATESTRUCT lpcs) {
	if (CStatic::OnCreate(lpcs) == -1) {
		return -1;
	}

	if (webview2_ = (wv2*)wv2createSync(nullptr, GetSafeHwnd())) {
		webview2_->navigate(L"https://google.com");
	}


	return 0;
}

