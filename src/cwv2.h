#ifndef _WEBVIEW2_IMPLEMENT_H_
#define _WEBVIEW2_IMPLEMENT_H_

#ifndef OVERRIDE
#define OVERRIDE override
#endif // OVERRIDE

#include <string>
#include <atlcomcli.h>

#include "WebView2.h"
#include "wv2.h"

#include "eventHandler.h"
#include "cwv2env.h"
#include "cwv2settings.h"
#include "cookie/cookieManager.h"

class cwv2 :
	public wv2,
	public ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler,
	public ICoreWebView2CreateCoreWebView2ControllerCompletedHandler,
	public ICoreWebView2ExecuteScriptCompletedHandler,
	public ICoreWebView2NavigationStartingEventHandler,
	public ICoreWebView2NavigationCompletedEventHandler,
	public ICoreWebView2DOMContentLoadedEventHandler,
	public ICoreWebView2HistoryChangedEventHandler,
	public ICoreWebView2PermissionRequestedEventHandler,
	public ICoreWebView2WebMessageReceivedEventHandler,
	public ICoreWebView2NewWindowRequestedEventHandler
{
public:
	friend class WindowCloseRequested;

	enum CreateStatus{
		none,		// ������
		created,	// ����
		completed,	// �غ�Ϸ�
		failed,		// ����	
	};

	struct request {
		bool isHtmlContent = false;
		std::wstring uriOrHtmlContent;
		
		void clear() {
			isHtmlContent = false;
			uriOrHtmlContent.clear();
		}
	};

	cwv2(HWND parentWindow, createCompleted createCompletedHandler =nullptr,
		void* userData =nullptr);
	virtual ~cwv2();

	// ICoreWebView2 interfaces	///////////////////////////////////////////////
	STDMETHODIMP Invoke(HRESULT errorCode, ICoreWebView2Environment *env) OVERRIDE;
	STDMETHODIMP Invoke(HRESULT errorCode, ICoreWebView2Controller* controller) OVERRIDE;
	STDMETHODIMP Invoke(HRESULT errorCode, LPCWSTR resultObjectAsJson) OVERRIDE;
	STDMETHODIMP Invoke(ICoreWebView2 *sender, ICoreWebView2NavigationStartingEventArgs *args) OVERRIDE;
	STDMETHODIMP Invoke(ICoreWebView2 *sender, ICoreWebView2NavigationCompletedEventArgs *args) OVERRIDE;
	STDMETHODIMP Invoke(ICoreWebView2 *sender, ICoreWebView2DOMContentLoadedEventArgs *args) OVERRIDE;
	STDMETHODIMP Invoke(ICoreWebView2 *sender, ICoreWebView2PermissionRequestedEventArgs *args) OVERRIDE;
	STDMETHODIMP Invoke(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) OVERRIDE;
	STDMETHODIMP Invoke(ICoreWebView2 *sender, IUnknown *args) OVERRIDE;
	STDMETHODIMP Invoke(ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args) OVERRIDE;
	STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv) OVERRIDE;
	ULONG STDMETHODCALLTYPE AddRef() OVERRIDE;
	ULONG STDMETHODCALLTYPE Release() OVERRIDE;
	// ICoreWebView2 interfaces	///////////////////////////////////////////////

	// wv2 interface	///////////////////////////////////////////////////////
	wv2settings* getSettings() OVERRIDE;
	
	bool executeScript(LPCWSTR script, executeScriptCompleted handler) OVERRIDE;
	LPCWSTR executeScriptSync(LPCWSTR script) OVERRIDE;
	LPCWSTR getSource() OVERRIDE;
	bool goBack() OVERRIDE;
	bool goForward() OVERRIDE;
	bool navigate(LPCWSTR url) OVERRIDE;
	bool navigateToString(LPCWSTR html) OVERRIDE;
	bool navigateWithWebResource(LPCWSTR uri, LPCWSTR method, BYTE* postData,
		size_t byteSize, LPCWSTR headers) OVERRIDE;
	bool reload() OVERRIDE;
	bool resize(int width, int height) OVERRIDE;

	bool setHistoryChangedHandler(historyChanged handler) OVERRIDE;
	bool setNavigationStartingHandler(navigationStarting handler) OVERRIDE;
	bool setNavigationCompletedHandler(navigationCompleted handler) OVERRIDE;
	bool setDomContentLoadedHandler(domContentLoaded handler) OVERRIDE;
	bool setWindowCloseRequestedHandler(windowCloseRequested handler) OVERRIDE;
	bool setWebMessageReceivedHandler(webMessageReceived handler) OVERRIDE;
	bool stop() OVERRIDE;
	double zoomFactor(const double* newZoomFactor) OVERRIDE;
	
	void destroy() OVERRIDE;
	void detach() OVERRIDE;

	void* getUserData() OVERRIDE;
	bool setUserData(void* userData) OVERRIDE;

	bool setVirtualHostNameToFolderMapping(LPCWSTR hostName, LPCWSTR folderPath, 
		wv2HostResourceAccessKind accessKind) OVERRIDE;

	void freeMemory(void* p) OVERRIDE;

	bool postWebMessageAsJson(LPCWSTR messageAsJson) OVERRIDE;
	bool postWebMessageAsString(LPCWSTR messageAsString) OVERRIDE;
	bool setIsMutedChangedHandler(isMutedChanged handler) OVERRIDE;
	bool setIsDocumentPlayingAudioChangedHandler(
		isDocumentPlayingAudioChanged handler) OVERRIDE;

	wv2bool isMuted() OVERRIDE;
	wv2bool setIsMuted(const bool muted) OVERRIDE;

	wv2bool isDocumentPlayingAudio() OVERRIDE;
	wv2bool openTaskManagerWindow() OVERRIDE;

	wv2environment* getEnvironment() OVERRIDE;
	wv2bool setNewWindowRequestedHandler(newWindowRequested handler) OVERRIDE;
	wv2bool setDocumentTitleChangedHandler(documentTitleChanged handler) OVERRIDE;
	LPCWSTR documentTitle() OVERRIDE;
	wv2bool setContentLoadingHandler(contentLoading handler) OVERRIDE;
	wv2bool setScriptDialogOpeningHandler(scriptDialogOpening handler) OVERRIDE;
	wv2bool setDownloadingStartingHandler(downloadStarting handler) OVERRIDE;
	wv2bool setWebResourceRequestedHandler(webResourceRequested handler) OVERRIDE;

	HRESULT addWebResourceRequestedFilter(LPCWSTR uri,
		const wv2webResourceContext resourceContext) OVERRIDE;
	HRESULT removeWebResourceRequestedFilter(LPCWSTR uri,
		const wv2webResourceContext resourceContext) OVERRIDE;

	wv2cookieManager* cookieManager() OVERRIDE;
	// wv2 interface	///////////////////////////////////////////////////////

	// ���� �ʱ�ȭ�� �Ϸ� ���� (�ʱ�ȭ�� �����Ǿ����� �ǹ����� ����)
	bool isDone() const;
	CreateStatus createStatus() const;

	inline HRESULT lastError() const { return lastError_; }
private:
	// ��� ���ҽ� ����
	void clearAll(bool detachController = false);
	// �������з� ���� ����
	HRESULT setStatusCreateFail(HRESULT errorCode);

private:
	HWND parentWindow_ = nullptr;
	ULONG refCount_ = 0;
	CComPtr<ICoreWebView2_3> view2_3_;
	CComPtr<ICoreWebView2_4> view2_4_;
	CComPtr<ICoreWebView2_8> view2_8_;
	CComPtr<ICoreWebView2Controller3> controller_;
	cwv2env env_;
	
	request lastRequest_;	// ó������ ���� ������ ��û����
	CreateStatus createStatus_;
	void* userData_;
	LPWSTR executeScriptSyncResult_;
	std::wstring virtualHostName_;
	cwv2settings settings_;

	executeScriptCompleted executeScriptCompletedHandler_;
	createCompleted createCompletedHandler_;
	historyChanged historyChangedHandler_;
	EventRegistrationToken historyChangedToken_;

	navigationCompleted navigationCompletedHandler_;
	EventRegistrationToken navigationCompletedToken_;

	navigationStarting navigationStartingHandler_;
	EventRegistrationToken navigationStartingToken_;

	domContentLoaded domContentLoadedHandler_;
	EventRegistrationToken domContentLoadedToken_;

	WindowCloseRequested windowCloseRequestedHandler_;

	EventRegistrationToken permissionRequestedToken_;

	webMessageReceived webMessageReceivedHandler_;
	EventRegistrationToken webMessageReceivedToken_;

	IsMutedChanged isMutedChangedHandler_;
	IsDocumentPlayingAudioChanged isDocumentPlayingAudioChangedHandler_;

	newWindowRequested newWindowRequestedHandler_{nullptr};
	EventRegistrationToken newWindowRequestedToken_;
	DocumentTitleChanged documentTitleChangedHandler_;
	ContentLoading contentLoadingHandler_;
	ScriptDialogOpening scriptDialogOpeningHandler_;
	DownloadStarting downloadStartingHandler_;
	WebResourceRequested webResourceRequestedHandler_;
	
	HRESULT lastError_;
	bool coInitilized_;
	wv2_::cookieManager cookieManager_;
};

#endif // _WEBVIEW2_IMPLEMENT_H_