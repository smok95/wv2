#pragma once

#include "wv2.h"
#ifdef _M_IX86	// x86
#pragma comment(lib,"x86/wv2")
#elif _M_X64
#pragma comment(lib,"x64/wv2")
#endif

class CMFCWebView2 : public CStatic
{
	DECLARE_DYNAMIC(CMFCWebView2)

public:
	CMFCWebView2();
	virtual ~CMFCWebView2();

	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

	virtual void PreSubclassWindow();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);

	

private:
	wv2* webview2_ = nullptr;
};


