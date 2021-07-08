
#pragma once
#include "CMFCWebView2.h"

class CexamplemfcDlg : public CDialog
{
public:
	CexamplemfcDlg(CWnd* pParent = nullptr);
	virtual ~CexamplemfcDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMPLE_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCWebView2 m_webview2;
};
