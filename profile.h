#pragma once	
#include <atlcomcli.h>
#include "WebView2.h"
#include "wv2.h"

namespace wv2_ {

class profile : public wv2profile {
public:
	profile();
	virtual ~profile();

	void setProfile(CComPtr<ICoreWebView2Profile> profile);
	ICoreWebView2Profile* getProfile();

	void releaseProfile();

	// wv2profile interface ///////////////////////////////////////////////
	LPWSTR getProfileName() override;
	bool getIsInPrivateModeEnabled() override;
	LPWSTR getProfilePath() override;
	LPWSTR getDefaultDownloadFolderPath() override;
	HRESULT setDefaultDownloadFolderPath(LPCWSTR value) override;
	wv2preferredColorScheme getPreferredColorScheme() override;
	HRESULT setPreferredColorScheme(wv2preferredColorScheme value) override;
	// wv2profile interface ///////////////////////////////////////////////
private:
	CComPtr<ICoreWebView2Profile> profile_;
};

}; // namespace wv2