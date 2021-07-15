#pragma once

#include "wv2.h"
#pragma comment(lib,"wv2")

class CMFCWebView2 : public CWnd
{
	DECLARE_DYNAMIC(CMFCWebView2)

public:
	CMFCWebView2();
	virtual ~CMFCWebView2();

	virtual bool Navigate(LPCTSTR uri);
	virtual bool GoBack();
	virtual bool GoForward();
	virtual bool Reload();
	virtual void PreSubclassWindow();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSize(UINT nType, int cx, int cy);	

private:
	wv2* webview2_ = nullptr;
};

