
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	CMFCWebView2 m_webview2;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_editUrl;
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedButtonForward();
	afx_msg void OnBnClickedButtonReload();
};
